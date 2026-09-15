#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <standardloop/util.h>

#define DEFAULT_MAP_SIZE 16
#define DEFAULT_MAP_RESIZE_MULTIPLE 2

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

extern HashMapItem *HashMapItemInit(char *key, Item *item)
{
    if (key == NULL || item == NULL)
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
    hashmap_item->item = item;
    hashmap_item->next = NULL;
    return hashmap_item;
}

extern void HashMapItemFree(HashMapItem *hashmap_item, bool deep)
{
    if (hashmap_item != NULL)
    {
        if (deep)
        {
            free(hashmap_item->key);
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

extern void DEBUGTestDefaultHashFunction()
{
    printf("%lu\n", (unsigned long)defaultHashFunction("one", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("two", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("three", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("four", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("five", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("six", 2));
    printf("%lu\n", (unsigned long)defaultHashFunction("seven", 2));
}

extern HashMap *HashMapInitDefault(void)
{
    return HashMapInit(DEFAULT_MAP_SIZE, DEFAULT_MAP_RESIZE_MULTIPLE, NULL,
                       false);
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

extern HashMap *HashMapInit(u_int32_t initial_capacity, uint8_t resize_multiple,
                            HashFunction *hashFunction, bool force_lowercase)
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
    // printf("[JOSH]: %lu\n", (unsigned long)index);
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

static bool hashMapEntriesInsert(HashMapItem **entries, u_int32_t index,
                                 HashMapItem *entry)
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
    // If duplicate key, update (in future could maybe make this a feature flag
    // for the init function)
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
    return value_obj->item->value;
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

static void HashMapFreeEntries(HashMapItem **entries, u_int32_t size, bool deep,
                               bool entry_values)
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

extern void HashMapFree(void *map)
{
    if (map != NULL)
    {
        if (((HashMap *)map)->entries != NULL)
        {
            HashMapFreeEntries(((HashMap *)map)->entries,
                               ((HashMap *)map)->capacity, true, true);
            ((HashMap *)map)->entries = NULL;
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

extern void HashMapPrint(void *map)
{
    if (map == NULL)
    {
        errno = EINVAL;
        return;
    }
    printf("{");
    u_int32_t entry_count = 0;
    for (u_int32_t i = 0; i < ((HashMap *)map)->capacity; i++)
    {
        HashMapItem *entry = ((HashMap *)map)->entries[i];
        if (entry != NULL)
        {
            HashMapPrintEntry(entry);
            if (entry_count < ((HashMap *)map)->size - 1)
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
            HashMapItem *new_entry =
                HashMapItemInit(iterator->key, iterator->item);
            u_int32_t new_index =
                map->hashFunction(iterator->key, new_capacity);
            bool collision =
                hashMapEntriesInsert(new_entries, new_index, new_entry);
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
//     HashMap *deep_clone = HashMapInit(map->capacity, map->hashFunction,
//     map->force_lowercase); deep_clone->collision_count =
//     map->collision_count; deep_clone->size = map->collision_count; for
//     (u_int32_t i = 0; i < map->capacity; i++)
//     {
//         deep_clone->entries[i] = HashMapItemReplicate(map->entries[i]);
//     }
//     return deep_clone;
// }

extern char *HashMapToString(void *map)
{
    if (map == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    size_t obj_as_string_size = 3; // "{}\0"
    char *obj_as_string = malloc(sizeof(char) * obj_as_string_size);
    if (obj_as_string == NULL)
    {
        return NULL;
    }
    obj_as_string[0] = CURLY_OPEN_CHAR;
    obj_as_string[1] = NULL_CHAR;

    size_t chars_written = 2 - 1;

    u_int64_t entry_count = 0;
    for (u_int64_t i = 0; i < ((HashMap *)map)->capacity; i++)
    {
        HashMapItem *map_entry = ((HashMap *)map)->entries[i];
        bool needs_comma = false;

        while (map_entry != NULL)
        {
            char *entry_key = map_entry->key;
            size_t duplicated_key_size = strlen(entry_key);
            char *duplicated_key = PutQuotesAroundString(entry_key, false);
            duplicated_key_size += 2;

            char *entry_value = ItemToString(map_entry->item);
            size_t entry_value_len = strlen(entry_value);
            if ((map_entry->next == NULL &&
                 entry_count < ((HashMap *)map)->size - 1) ||
                map_entry->next != NULL)
            {
                needs_comma = true;
            }

            obj_as_string_size += duplicated_key_size;
            obj_as_string_size++; // ':'
            obj_as_string_size += entry_value_len;
            obj_as_string_size += needs_comma;

            obj_as_string = realloc(obj_as_string, obj_as_string_size);

            CopyStringCanary(obj_as_string, duplicated_key, chars_written);
            chars_written += duplicated_key_size;
            CopyStringCanary(obj_as_string, ":", chars_written);
            chars_written++;
            CopyStringCanary(obj_as_string, entry_value, chars_written);
            chars_written += entry_value_len;
            if (needs_comma)
            {
                CopyStringCanary(obj_as_string, ",", chars_written);
                chars_written++;
            }
            free(duplicated_key);
            free(entry_value);
            if (map_entry->next == NULL)
            {
                entry_count++;
            }
            needs_comma = false;
            map_entry = map_entry->next;
        }
    }

    obj_as_string[obj_as_string_size - 2] = CURLY_CLOSE_CHAR;
    obj_as_string[obj_as_string_size - 1] = NULL_CHAR;
    return obj_as_string;
}

extern void *HashMapDuplicate(void *map)
{
    if (map == NULL)
    {
        return NULL;
    }
    return NULL;
}
