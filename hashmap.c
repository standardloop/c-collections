#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include <standardloop/util.h>

#include "./collections.h"

static void HashMapFreeEntrySingle(HashMapItem *, bool);
static void HashMapFreeEntryList(HashMapItem *, bool);
static void HashMapFreeEntries(HashMapItem **, u_int32_t, bool, bool);
static void HashMapPrintEntry(HashMapItem *);
static u_int32_t defaultHashFunction(char *, u_int32_t);

static bool hashMapEntriesInsert(HashMapItem **, u_int32_t, HashMapItem *);

static HashMapItem **hashMapEntriesInit(u_int32_t);

static inline bool isMapFull(HashMap *);
static void hashMapResize(HashMap *map);

extern HashMapItem *HashMapItemInit(char *key, Item *value)
{
    if (key == NULL || value == NULL)
    {
        return NULL;
    }
    HashMapItem *hashmap_item = malloc(sizeof(HashMapItem));
    if (hashmap_item == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    hashmap_item->key = key;
    hashmap_item->item = value;
    hashmap_item->next = NULL;
    return hashmap_item;
}

extern void HashMapItemFree(HashMapItem *hashmap_item, bool deep)
{
    if (hashmap_item != NULL)
    {
        free(hashmap_item->key);
        if (deep)
        {
            ItemFree(hashmap_item->item);
        }
        free(hashmap_item);
    }
}

extern void HashMapItemPrint(HashMapItem *hashmap_item)
{
    if (hashmap_item != NULL && hashmap_item->item != NULL)
    {
        ItemPrint(hashmap_item->item);
    }
}

// Jenkins's one_at_a_time
static u_int32_t defaultHashFunction(char *key, u_int32_t capacity)
{
    u_int32_t len = strlen(key);
    u_int32_t hash = 0;

    for (u_int32_t i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % capacity;
}

extern HashMap *HashMapInitDefault(void)
{
    return HashMapInit(DEFAULT_MAP_SIZE, DEFAULT_MAP_RESIZE_MULTIPLE, NULL, false);
}

static HashMapItem **hashMapEntriesInit(u_int32_t capacity)
{
    HashMapItem **entries = malloc(sizeof(HashMapItem *) * capacity);
    if (entries == NULL)
    {
        return NULL;
    }

    for (u_int32_t i = 0; i < capacity; i++)
    {
        entries[i] = NULL;
    }

    return entries;
}

extern HashMap *HashMapInit(u_int32_t initial_capacity, uint8_t resize_multiple, HashFunction *hashFunction, bool force_lowercase)
{
    if (initial_capacity == 0)
    {
        errno = EINVAL;
        return NULL;
    }
    if (resize_multiple == 0 || resize_multiple == 1)
    {
        errno = EINVAL;
        return NULL;
    }
    HashMap *map = malloc(sizeof(HashMap));
    if (map == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    map->resize_multiple = resize_multiple;
    map->size = 0;
    map->collision_count = 0;
    map->capacity = initial_capacity;
    map->force_lowercase = force_lowercase;
    map->entries = hashMapEntriesInit(initial_capacity);

    if (map->entries == NULL)
    {
        HashMapFree(map);
        return NULL;
    }

    if (hashFunction == NULL)
    {
        map->hashFunction = defaultHashFunction;
    }
    return map;
}

static inline bool isMapFull(HashMap *map)
{
    return map->capacity == map->size;
}

extern void HashMapInsert(HashMap *map, HashMapItem *entry)
{
    if (map == NULL || entry->key == NULL || entry->item == NULL)
    {
        errno = EINVAL;
        return;
    }
    if (isMapFull(map))
    {
        hashMapResize(map);
    }
    if (map->force_lowercase)
    {
        StringToLower(entry->key);
    }
    u_int32_t index = map->hashFunction(entry->key, map->capacity);
    assert(index < map->capacity); // TODO
    bool collision = hashMapEntriesInsert(map->entries, index, entry);
    if (collision)
    {
        map->collision_count++;
    }
    else
    {
        map->size++;
    }
}

static bool hashMapEntriesInsert(HashMapItem **entries, u_int32_t index, HashMapItem *entry)
{
    // FIXME: may have to use enum for return values
    // collision, no collision, or error
    if (entries == NULL || entry == NULL || entry->key == NULL)
    {
        errno = EINVAL;
        return false;
    }
    // printf("[JOSH]: %s\n", entry->key);
    // fflush(stdout);
    HashMapItem *collision = entries[index];
    if (collision == NULL)
    {
        entry->next = NULL;
        entries[index] = entry;
        return false;
    }
    // printf("%s -> %s\n", collision->key, entry->key);
    // If duplicate key, update (in future could maybe make this a feature flag for the init function)
    if (collision->key != NULL)
    {
        size_t collision_key_len = strlen(collision->key);
        size_t entry_key_len = strlen(entry->key);
        if (collision_key_len == entry_key_len)
        {
            if (strncmp(collision->key, entry->key, entry_key_len) == 0)
            {
                entry->next = collision->next;
                collision->next = NULL;
                HashMapFreeEntrySingle(collision, true);
                entries[index] = entry;
                return true;
            }
        }
    }
    HashMapItem *iterator_prev = collision;
    HashMapItem *iterator = collision->next;
    while (iterator != NULL)
    {
        if (iterator->key != NULL && entry->key != NULL)
        {
            size_t collision_key_len = strlen(collision->key);
            size_t entry_key_len = strlen(entry->key);
            if (collision_key_len == entry_key_len)
            {
                if (strncmp(iterator->key, entry->key, collision_key_len) == 0)
                {
                    iterator_prev->next = entry;
                    entry->next = iterator->next;
                    iterator->next = NULL;
                    HashMapFreeEntrySingle(iterator, true);
                    return true;
                }
            }
        }

        iterator_prev = iterator;
        iterator = iterator->next;
    }
    entry->next = NULL;
    iterator_prev->next = entry;
    return true;
}

extern HashMapItem *HashMapGet(HashMap *map, char *key)
{
    if (map == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    u_int32_t index = map->hashFunction(key, map->capacity);
    HashMapItem *entry = map->entries[index];
    if (entry == NULL)
    {
        return NULL;
    }

    HashMapItem *iterator = entry;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

extern void *HashMapGetValueDirect(HashMap *map, char *key)
{
    if (map == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    HashMapItem *value_obj = HashMapGet(map, key);
    if (value_obj == NULL || value_obj->item == NULL)
    {
        return NULL;
    }
    return value_obj->item;
}

static void HashMapFreeEntryList(HashMapItem *entry, bool deep)
{
    if (entry == NULL)
    {
        errno = EINVAL;
        return;
    }
    HashMapItem *temp = NULL;
    while (entry != NULL)
    {
        temp = entry;
        entry = entry->next;
        if (temp != NULL && temp->key != NULL)
        {
            free(temp->key);
            temp->key = NULL;
        }
        if (temp != NULL)
        {
            HashMapItemFree(temp, deep);
        }
    }
}

static void HashMapFreeEntrySingle(HashMapItem *entry, bool deep)
{
    if (entry == NULL)
    {
        errno = EINVAL;
        return;
    }
    if (entry->key != NULL)
    {
        free(entry->key);
        entry->key = NULL;
    }
    HashMapItemFree(entry, deep);
}

static void HashMapFreeEntries(HashMapItem **entries, u_int32_t size, bool deep, bool entry_values)
{
    if (entries != NULL)
    {
        if (deep)
        {
            for (u_int32_t i = 0; i < size; i++)
            {
                if (entries[i] != NULL)
                {
                    HashMapFreeEntryList(entries[i], entry_values);
                    entries[i] = NULL;
                }
            }
        }
        free(entries);
    }
}

extern void HashMapFree(HashMap *map)
{
    if (map != NULL)
    {
        if (map->entries != NULL)
        {
            HashMapFreeEntries(map->entries, map->capacity, true, true);
            map->entries = NULL;
        }
        free(map);
    }
}

extern void HashMapRemove(HashMap *map, char *key)
{
    if (map == NULL)
    {
        errno = EINVAL;
        return;
    }
    u_int32_t index = map->hashFunction(key, map->capacity);
    HashMapItem *entry = map->entries[index];
    if (entry == NULL)
    {
        return;
    }
    if (entry->next == NULL)
    {
        HashMapFreeEntrySingle(entry, true);
        map->entries[index] = NULL;
        map->size--;
        return;
    }

    if (strcmp(key, entry->key) == 0)
    {
        HashMapItem *temp = entry;
        map->entries[index] = entry->next;
        entry->next = NULL;
        HashMapFreeEntrySingle(temp, true);
        map->collision_count--;
        return;
    }

    HashMapItem *iterator_prev = entry;
    HashMapItem *iterator = entry->next;
    while (iterator != NULL)
    {
        if (strcmp(key, iterator->key) == 0)
        {
            iterator_prev->next = iterator->next;
            map->entries[index] = iterator_prev;
            HashMapFreeEntrySingle(iterator, true);
            map->collision_count--;
            break;
        }
        iterator_prev = iterator_prev->next;
        iterator = iterator->next;
    }
}

extern void HashMapPrint(HashMap *map)
{
    if (map == NULL)
    {
        errno = EINVAL;
        return;
    }
    printf("{");
    u_int32_t entry_count = 0;
    for (u_int32_t i = 0; i < map->capacity; i++)
    {
        HashMapItem *entry = map->entries[i];
        if (entry != NULL)
        {
            HashMapPrintEntry(entry);
            if (entry_count < map->size - 1)
            {
                printf(", ");
            }
            entry_count++;
        }
    }
    printf("}");
}

static void HashMapPrintEntry(HashMapItem *entry)
{
    if (entry == NULL || entry->item == NULL || entry->key == NULL)
    {
        errno = EINVAL;
        return;
    }
    HashMapItem *iterator = entry;
    while (iterator != NULL)
    {
        if (iterator->key != NULL && iterator->item != NULL)
        {
            printf("\"%s\": ", iterator->key);
            HashMapItemPrint(iterator);
        }
        iterator = iterator->next;
        if (iterator != NULL)
        {
            printf(", ");
        }
    }
}

// JOSH
static void hashMapResize(HashMap *map)
{
    // printf("hashMapResize\n");
    // fflush(stdout);
    if (map == NULL)
    {
        errno = EINVAL;
        return;
    }

    u_int32_t new_capacity = map->capacity * DEFAULT_MAP_RESIZE_MULTIPLE;
    u_int32_t new_size = 0;
    u_int32_t new_collision_count = 0;

    HashMapItem **new_entries = hashMapEntriesInit(new_capacity);

    if (new_entries == NULL)
    {
        return;
    }

    for (u_int32_t i = 0; i < map->capacity; i++)
    {
        HashMapItem *entry = map->entries[i];
        HashMapItem *iterator = entry;

        while (iterator != NULL)
        {
            HashMapItem *new_entry = HashMapItemInit(iterator->key, iterator->item);
            u_int32_t new_index = map->hashFunction(iterator->key, new_capacity);
            bool collision = hashMapEntriesInsert(new_entries, new_index, new_entry);
            if (!collision)
            {
                new_size++;
            }
            else
            {
                new_collision_count++;
            }
            HashMapItem *temp = iterator;
            iterator = iterator->next;
            HashMapItemFree(temp, false);
        }
    }
    HashMapFreeEntries(map->entries, map->capacity, false, false);

    map->size = new_size;
    map->collision_count = new_collision_count;
    map->capacity = new_capacity;
    map->entries = new_entries;
}

// // FIXME: not complete
// extern HashMap *HashMapReplicate(HashMap *map)
// {
//     if (map == NULL)
//     {
//         errno = EINVAL;
//         return NULL;
//     }
//     HashMap *deep_clone = HashMapInit(map->capacity, map->hashFunction, map->force_lowercase);
//     deep_clone->collision_count = map->collision_count;
//     deep_clone->size = map->collision_count;
//     for (u_int32_t i = 0; i < map->capacity; i++)
//     {
//         deep_clone->entries[i] = HashMapItemReplicate(map->entries[i]);
//     }
//     return deep_clone;
// }
