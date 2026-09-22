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

typedef struct complexHashMapItem
{
    /** The key for a hashed value. */
    Item *key;
    /** the item hashed to the key. */
    Item *value;
    /** If a collision occurs, the next field - linked list for collisions. */
    struct complexHashMapItem *next;
} ComplexHashMapItem;

static ComplexHashMapItem *complexHashMapItemInit(Item *key, Item *item);
static void complexHashMapItemFree(ComplexHashMapItem *hashmap_item, bool deep);
static ComplexHashMapItem **complexHashMapEntriesInit(u_int32_t capacity);
static ComplexHashMapItem *
complexHashMapItemDuplicate(ComplexHashMapItem *hashmap_item);

static inline bool isComplexHashMapFull(ComplexHashMap *map);

static void complexHashMapFreeEntryList(ComplexHashMapItem *, bool);
static void complexHashMapFreeEntries(ComplexHashMapItem **, u_int32_t, bool,
                                      bool);
static void complexHashMapPrintEntry(ComplexHashMapItem *);

static bool complexHashMapEntriesInsert(ComplexHashMapItem **, u_int32_t,
                                        ComplexHashMapItem *);

static void complexHashMapResize(ComplexHashMap *map);

extern ComplexHashMapItem *
complexHashMapGetComplexHashMapItem(ComplexHashMap *map, Item *key);

static void complexHashMapItemPrint(ComplexHashMapItem *hashmap_item);

static void complexHashMapItemPrint(ComplexHashMapItem *hashmap_item)
{
    if (hashmap_item != NULL && hashmap_item->value != NULL)
    {
        ItemPrint(hashmap_item->value);
    }
}

static void complexHashMapPrintEntry(ComplexHashMapItem *entry)
{
    if (entry == NULL || entry->value == NULL || entry->key == NULL)
    {
        errno = EINVAL;
        return;
    }
    ComplexHashMapItem *iterator = entry;
    while (iterator != NULL)
    {
        if (iterator->key != NULL && iterator->value != NULL)
        {
            // printf("\"%s\": ", iterator->key);
            ItemPrint(iterator->key);
            printf(": ");
            complexHashMapItemPrint(iterator);
        }
        iterator = iterator->next;
        if (iterator != NULL)
        {
            printf(", ");
        }
    }
}

static ComplexHashMapItem *complexHashMapItemInit(Item *key, Item *value)
{
    if (key == NULL || value == NULL)
    {
        return NULL;
    }
    ComplexHashMapItem *hashmap_item = malloc(sizeof(ComplexHashMapItem));
    if (hashmap_item == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    hashmap_item->key = key;
    hashmap_item->value = value;
    hashmap_item->next = NULL;
    return hashmap_item;
}

static void complexHashMapItemFree(ComplexHashMapItem *hashmap_item, bool deep)
{
    if (hashmap_item != NULL)
    {
        if (deep)
        {
            ItemFree(hashmap_item->key);
            ItemFree(hashmap_item->value);
        }
        free(hashmap_item);
    }
}

static ComplexHashMapItem *
complexHashMapItemDuplicate(ComplexHashMapItem *hashmap_item)
{
    if (hashmap_item == NULL)
    {
        return NULL;
    }

    // create the head first
    ComplexHashMapItem *dupe = complexHashMapItemInit(
        ItemDuplicate(hashmap_item->key), ItemDuplicate(hashmap_item->value));

    ComplexHashMapItem *dupe_itr = dupe;

    ComplexHashMapItem *item_itr = hashmap_item->next;
    while (item_itr != NULL)
    {
        ComplexHashMapItem *collision_dupe = complexHashMapItemInit(
            ItemDuplicate(item_itr->key), ItemDuplicate(item_itr->value));
        dupe_itr->next = collision_dupe;

        dupe_itr = dupe_itr->next;
        item_itr = item_itr->next;
    }
    return dupe;
}

static ComplexHashMapItem **complexHashMapEntriesInit(u_int32_t capacity)
{
    ComplexHashMapItem **entries =
        malloc(sizeof(ComplexHashMapItem *) * capacity);
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

extern ComplexHashMap *ComplexHashMapInit(u_int32_t initial_capacity,
                                          u_int8_t resize_multiple)
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
    ComplexHashMap *map = malloc(sizeof(ComplexHashMap));
    if (map == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    map->resize_multiple = resize_multiple;
    map->size = 0;
    map->collision_count = 0;
    map->capacity = initial_capacity;
    map->entries = complexHashMapEntriesInit(initial_capacity);

    if (map->entries == NULL)
    {
        HashMapFree(map);
        return NULL;
    }

    return map;
}

extern ComplexHashMap *ComplexHashMapInitDefault(void)
{
    return ComplexHashMapInit(DEFAULT_MAP_SIZE, DEFAULT_MAP_RESIZE_MULTIPLE);
}

static inline bool isComplexHashMapFull(ComplexHashMap *map)
{
    return map->capacity == map->size;
}

static bool complexHashMapEntriesInsert(ComplexHashMapItem **entries,
                                        u_int32_t index,
                                        ComplexHashMapItem *entry)
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
    ComplexHashMapItem *collision = entries[index];
    if (collision == NULL)
    {
        entry->next = NULL;
        entries[index] = entry;
        return false;
    }
    // printf("%s -> %s\n", collision->key, entry->key);

    // If duplicate key, update (in future could maybe make this a feature flag
    // for the init function)

    // why do we have this outside the loop and inside the loop?
    if (collision->key != NULL)
    {
        if (ItemEquivalence(collision->key, entry->key))
        {
            entry->next = collision->next;
            collision->next = NULL;
            complexHashMapItemFree(collision, true);
            entries[index] = entry;
            return true;
        }
    }
    ComplexHashMapItem *iterator_prev = collision;
    ComplexHashMapItem *iterator = collision->next;
    while (iterator != NULL)
    {
        if (iterator->key != NULL && entry->key != NULL)
        {
            if (ItemEquivalence(iterator->key, entry->key))
            {
                entry->next = collision->next;
                collision->next = NULL;
                complexHashMapItemFree(collision, true);
                entries[index] = entry;
                return true;
            }
        }
        iterator_prev = iterator;
        iterator = iterator->next;
    }
    entry->next = NULL;
    iterator_prev->next = entry;
    return true;
}

static void complexHashMapFreeEntries(ComplexHashMapItem **entries,
                                      u_int32_t size, bool deep,
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
                    complexHashMapFreeEntryList(entries[i], entry_values);
                    entries[i] = NULL;
                }
            }
        }
        free(entries);
    }
}

static void complexHashMapFreeEntryList(ComplexHashMapItem *entry, bool deep)
{
    if (entry == NULL)
    {
        errno = EINVAL;
        return;
    }
    ComplexHashMapItem *temp = NULL;
    while (entry != NULL)
    {
        temp = entry;
        entry = entry->next;
        if (temp != NULL)
        {
            complexHashMapItemFree(temp, deep);
        }
    }
}

static void complexHashMapResize(ComplexHashMap *map)
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

    ComplexHashMapItem **new_entries = complexHashMapEntriesInit(new_capacity);

    if (new_entries == NULL)
    {
        return;
    }

    for (u_int32_t i = 0; i < map->capacity; i++)
    {
        ComplexHashMapItem *entry = map->entries[i];
        ComplexHashMapItem *iterator = entry;

        while (iterator != NULL)
        {
            ComplexHashMapItem *new_entry =
                complexHashMapItemInit(iterator->key, iterator->value);

            u_int32_t new_index = ItemHash(iterator->key) % new_capacity;

            bool collision =
                complexHashMapEntriesInsert(new_entries, new_index, new_entry);
            if (!collision)
            {
                new_size++;
            }
            else
            {
                new_collision_count++;
            }
            ComplexHashMapItem *temp = iterator;
            iterator = iterator->next;
            complexHashMapItemFree(temp, false);
        }
    }
    complexHashMapFreeEntries(map->entries, map->capacity, false, false);

    map->size = new_size;
    map->collision_count = new_collision_count;
    map->capacity = new_capacity;
    map->entries = new_entries;
}

extern void ComplexHashMapInsert(ComplexHashMap *map, Item *key, Item *value)
{
    if (map == NULL || key == NULL || value == NULL)
    {
        errno = EINVAL;
        return;
    }
    if (isComplexHashMapFull(map))
    {
        complexHashMapResize(map);
    }

    u_int32_t index = ItemHash(key) % map->capacity;
    // printf("[JOSH]: %lu\n", (unsigned long)index);
    assert(index < map->capacity); // TODO
    bool collision = complexHashMapEntriesInsert(
        map->entries, index, complexHashMapItemInit(key, value));
    if (collision)
    {
        map->collision_count++;
    }
    else
    {
        map->size++;
    }
}

extern ComplexHashMapItem *
complexHashMapGetComplexHashMapItem(ComplexHashMap *map, Item *key)
{
    if (map == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    // printf("%s %d\n", key, (int)map->capacity);
    u_int32_t index = ItemHash(key) % map->capacity;
    // printf("%d\n", (int)index);
    ComplexHashMapItem *entry = map->entries[index];
    if (entry == NULL)
    {
        return NULL;
    }
    else if (entry->next ==
             NULL) // no collisions to check this must be the correct value
    {
        return entry;
    }
    ComplexHashMapItem *iterator =
        entry; // todo, learn why I cant do entry->next here
    while (iterator != NULL)
    {
        if (ItemEquivalence(iterator->key,
                            key)) // since we have collisions, we need to check
                                  // if it is exactly the same key
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

extern Item *ComplexHashMapGet(ComplexHashMap *map, Item *key)
{
    if (map == NULL || key == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    ComplexHashMapItem *hashmap_item =
        complexHashMapGetComplexHashMapItem(map, key);
    if (hashmap_item ==
        NULL) // || hashmap_item->value == NULL) // think about this more
    {
        return NULL;
    }
    return hashmap_item->value;
}

extern void *ComplexHashMapDuplicate(void *map)
{
    if (map == NULL)
    {
        return NULL;
    }

    ComplexHashMap *map_ptr = ((ComplexHashMap *)map);

    ComplexHashMap *dupe =
        ComplexHashMapInit(map_ptr->capacity, map_ptr->resize_multiple);

    dupe->size = map_ptr->size;
    dupe->collision_count = map_ptr->collision_count;

    for (u_int64_t i = 0; i < map_ptr->capacity; i++)
    {
        dupe->entries[i] = complexHashMapItemDuplicate(map_ptr->entries[i]);
    }
    return dupe;
}

extern void ComplexHashMapPrint(void *map)
{
    if (map == NULL)
    {
        errno = EINVAL;
        return;
    }
    ComplexHashMap *map_ptr = (ComplexHashMap *)map;
    printf("{");
    u_int32_t entry_count = 0;
    for (u_int32_t i = 0; i < map_ptr->capacity; i++)
    {
        ComplexHashMapItem *entry = map_ptr->entries[i];
        if (entry != NULL)
        {
            complexHashMapPrintEntry(entry);
            if (entry_count < map_ptr->size - 1)
            {
                printf(", ");
            }
            entry_count++;
        }
    }
    printf("}");
}

extern char *ComplexHashMapToString(void *map)
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
    for (u_int64_t i = 0; i < ((ComplexHashMap *)map)->capacity; i++)
    {
        ComplexHashMapItem *map_entry = ((ComplexHashMap *)map)->entries[i];
        bool needs_comma = false;

        while (map_entry != NULL)
        {
            char *entry_key = ItemToString(map_entry->key);
            size_t duplicated_key_size = strlen(entry_key);
            char *duplicated_key =
                strdup(entry_key); // or PutQuotesAroundString?

            // duplicated_key_size += 2;

            char *entry_value = ItemToString(map_entry->value);
            size_t entry_value_len = strlen(entry_value);
            if ((map_entry->next == NULL &&
                 entry_count < ((ComplexHashMap *)map)->size - 1) ||
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

extern void ComplexHashMapFree(void *map)
{
    if (map != NULL)
    {
        ComplexHashMap *map_ptr = (ComplexHashMap *)map;
        if (map_ptr->entries != NULL)
        {
            complexHashMapFreeEntries(map_ptr->entries, map_ptr->capacity, true,
                                      true);
            map_ptr->entries = NULL;
        }
        free(map);
    }
}

static bool complexHashMapItemEquivalence(ComplexHashMapItem *hmi1,
                                          ComplexHashMapItem *hmi2);

static bool complexHashMapItemEquivalence(ComplexHashMapItem *hmi1,
                                          ComplexHashMapItem *hmi2)
{
    // XNOR pointers
    if (hmi1 == NULL && hmi2 == NULL)
    {
        return true;
    }
    else if (hmi1 == NULL || hmi2 == NULL)
    {
        return false;
    }

    ComplexHashMapItem *hmi1_itr = hmi1;
    ComplexHashMapItem *hmi2_itr = hmi2;
    while (hmi1_itr != NULL && hmi2_itr != NULL)
    {
        if (!ItemEquivalence(hmi1_itr->key, hmi2_itr->key) ||
            !ItemEquivalence(hmi1_itr->value, hmi2_itr->value))
        {
            return false;
        }
        // XNOR pointers->next
        if (hmi1_itr->next == NULL && hmi2_itr->next == NULL)
        {
            return true;
        }
        else if (hmi1_itr->next == NULL || hmi2_itr->next == NULL)
        {
            return false;
        }
        // XNOR end
        hmi1_itr = hmi1_itr->next;
        hmi2_itr = hmi2_itr->next;
    }
    return true;
}

extern bool ComplexHashMapEquivalence(void *map1, void *map2)
{
    // XNOR pointers
    if (map1 == NULL && map2 == NULL)
    {
        return true;
    }
    else if (map1 == NULL || map2 == NULL)
    {
        return false;
    }
    // XNOR end

    ComplexHashMap *map1_ptr = (ComplexHashMap *)(map1);
    ComplexHashMap *map2_ptr = (ComplexHashMap *)(map2);

    if (map1_ptr->capacity != map2_ptr->capacity ||
        map1_ptr->collision_count != map2_ptr->collision_count ||
        map1_ptr->resize_multiple != map2_ptr->resize_multiple ||
        map1_ptr->size != map2_ptr->size)
    {
        return false;
    }
    // XNOR entries
    if (map1_ptr->entries == NULL && map2_ptr->entries == NULL)
    {
        return true;
    }
    else if (map1_ptr->entries == NULL || map2_ptr->entries == NULL)
    {
        return false;
    }
    // XNOR end

    // map1_ptr->capacity and map2_ptr->capacity are same
    for (u_int32_t i = 0; i < map1_ptr->capacity; i++)
    {
        if (!complexHashMapItemEquivalence(map1_ptr->entries[i],
                                           map2_ptr->entries[i]))
        {
            return false;
        }
    }
    return true;
}

extern u_int32_t ComplexHashMapHash(void *map)
{
    if (map == NULL)
    {
        return 0;
    }
    return 1;
}
