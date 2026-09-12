/**
 * @file collections.h
 * @headerfile collections.h <standardloop/collections.h>
 * @brief A C library for data structures such as lists and hashmaps.
 */

#ifndef STANDARDLOOP_COLLECTIONS_H
#define STANDARDLOOP_COLLECTIONS_H
/**
 * @brief Major version of this library.
 */
#define STANDARDLOOP_COLLECTIONS_H_MAJOR_VERSION 0
/**
 * @brief Minor version of this library.
 */
#define STANDARDLOOP_COLLECTIONS_H_MINOR_VERSION 0
/**
 * @brief Patch version of this library.
 */
#define STANDARDLOOP_COLLECTIONS_H_PATCH_VERSION 1
/**
 * @brief Version of this library as a string.
 */
#define STANDARDLOOP_COLLECTIONS_H_VERSION "0.0.1"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ————————— ITEM START —————————
/**
 * @brief A function that will be a part of the Item struct, it determines how
 * the Item will be freed.
 */
typedef void(ItemFreeFunction)(void *);

/**
 * @brief A function that will be a part of the Item struct, it determines how
 * the Item will be printed, mainly for debugging.
 */
typedef void(ItemPrintFunction)(void *);

/**
 * @brief The Item struct, contains a value of any kind, and a function to free
 * the value and print the value.
 */
typedef struct
{
    /** The value itself. */
    void *value;
    /** A function to free the value. */
    ItemFreeFunction *freeFunction;
    /** A function to print the value. */
    ItemPrintFunction *printFunction;
} Item;

/**
 * @brief Prints a string value.
 * @param item The item value to print.
 */
extern void ItemPrintString(void *item);

/**
 * @brief Initializes the Item.
 * @param value The value to put into the Item.
 * @param freeFunction Pointer to a function that can free the value of the
 * Item.
 * @param printFunction Pointer to a function that can print the value of the
 * Item.
 * @return The initialized Item
 */
extern Item *ItemInit(void *value, ItemFreeFunction *freeFunction,
                      ItemPrintFunction *printFunction);

/**
 * @brief Frees an Item
 * @param item The Item to free.
 */
extern void ItemFree(Item *item);

/**
 * @brief Prints an Item
 * @param item The Item to print.
 */
extern void ItemPrint(Item *item);
// ————————— ITEM END —————————

// ————————— LIST START —————————
/**
 * @brief The List struct, contains a header for size, capacity, and resizing
 * multiple and then a List of Items.
 */
typedef struct
{
    /** The current size of the List. */
    u_int32_t size;
    /** The capacity of the List, can be expanded. */
    u_int32_t capacity;
    /** The resizing multiple of the List, if the list is at capacity, what
     * multiple to resize it */
    u_int8_t resize_multiple;
    /** The list of Items. */
    Item **items;
} List;

/**
 * @brief Intializes a List.
 * @param initial_capacity How large for the List to start as.
 * @param resize_multiple What multiple to resize the list after it reaches
 * capacity.
 * @return The initialized List.
 */
extern List *ListInit(u_int32_t initial_capacity, u_int8_t resize_multiple);

/**
 * @brief Intializes a List with sane defaults, see ListInit for more
 * customization.
 * @return The initialized List.
 */
extern List *ListInitDefault(void);
// /**
//  * @brief Fully clones and replicates a list
//  * @param The List to clone.
//  * @return The cloned list.
//  */
// extern List *ListReplicate(List *);

// extern char *ListToString(List *);

/**
 * @brief Add an Item to the front of the List.
 * @param list The List to add an Item to.
 * @param item The Item to add to the List.
 */
extern void ListAddFirst(List *list, Item *item);

/**
 * @brief Add an Item to the last available spot of the List.
 * @param list The List to add an Item to.
 * @param item The Item to add to the List.
 */
extern void ListAddLast(List *list, Item *item);

/**
 * @brief Add an Item to a specific index of the list.
 * @param list The List to add an Item to.
 * @param item The Item to add to the List.
 * @param index The index of the List to place the Item into.
 */
extern void ListAddAtIndex(List *list, Item *item, u_int32_t index);

/**
 * @brief Remove an Item at a specific index.
 * @param list The List to remove an Item from.
 * @param index The index of the List to remove the Item from.
 * @param free Do you want to free the Item as well.
 */
extern void ListRemoveAtIndex(List *list, u_int32_t index, bool free);

/**
 * @brief Remove the first Item from a List.
 * @param list The List to remove an Item from.
 */
extern void ListRemoveFirst(List *list);

/**
 * @brief Remove the last Item from a List.
 * @param list The List to remove an Item from.
 */
extern void ListRemoveLast(List *list);

/**
 * @brief Get an Item from a List at a specific Index.
 * @param list The List to get an Item from.
 * @param index The index at which to get an Item from.
 * @return The Item from the List at a specific Index.
 */
extern Item *ListGetAtIndex(List *list, u_int32_t index);

/**
 * @brief Get the first Item from a List
 * @param list The List to get an Item from.
 * @return The first Item from the specified List.
 */
extern Item *ListGetFirst(List *list);

/**
 * @brief Get and remove an Item from a List
 * @param list The List to get an Item from.
 * @param index The index at which to get an Item from.
 * @return The Item from the List.
 */
extern Item *ListPopAtIndex(List *list, u_int32_t index);

/**
 * @brief Pop the First Item from a List
 * @param list The List to get an Item from.
 * @return The Item from the List.
 */
extern Item *ListPopFirst(List *list);

/**
 * @brief Print the List.
 * @param list The List to print.
 */
extern void ListPrint(List *list);

/**
 * @brief Print info of the List (current size, etc...).
 * @param list The List to print info of.
 */
extern void ListPrintInfo(List *list);

/**
 * @brief Frees the List and all the Items in the List
 * @param list The List to free.
 */
extern void ListFree(List *list);
// ————————— LIST END —————————

// ————————— HASHMAP START —————————
typedef u_int32_t(HashFunction)(char *, u_int32_t);

// typedef enum
// {
//     HASHMAP_COLLISION_OPTION_LINKEDLIST = 0,
//     HASHMAP_COLLISION_OPTION_LIST = 1
// } HashMapCollisionOptions;

/**
 * @brief The HashMap Item struct
 */
typedef struct hashMapItem
{
    /** The key for a hashed value. */
    char *key;
    /** the value hashed to the key. */
    Item *value;
    /** If a collision occurs, the next field - linked list for collisions. */
    struct hashMapItem *next;
} HashMapItem;

/**
 * @brief Initializes as HashMapItem.
 * @param key The string key of HashMapItem.
 * @param value The corresponding value to the hashed key.
 * @return The Initialized HashMapItem.
 */
extern HashMapItem *HashMapItemInit(char *key, Item *value);

/**
 * @brief Frees a HashMapItem, can optional deep free by freeing the Item as
 * well.
 * @param hashmap_item The HashMapItem to free.
 * @param deep If true, will free the Item in the HashMapItem.
 */
extern void HashMapItemFree(HashMapItem *hashmap_item, bool deep);

/**
 * @brief Prints a HashMapItem
 * @param hashmap_item The HashMapItem to print.
 */
extern void HashMapItemPrint(HashMapItem *hashmap_item);

/**
 * @brief The HashMap struct
 */
typedef struct
{
    /** The current size (number of entries) of the HashMap. */
    u_int32_t size;
    /** How large the HashMap can get before it needs to be resized. */
    u_int32_t capacity;
    /** If the HashMap needs to be resized, what should the resizing multiple be
     */
    u_int8_t resize_multiple;
    /** How many collisions have occured.
     */
    u_int32_t collision_count;
    /** The List entries in the HashMap.
     */
    HashMapItem **entries;
    /** The function this HashMap uses to Hash strings.
     */
    HashFunction *hashFunction;
    /** Should only lowercase be considered, for example "THIS" and "this" are
     * both hashed to the same value.
     */
    bool force_lowercase;
    // HashMapCollisionOptions collision_strategy;
} HashMap;

extern HashMapItem *HashMapGet(HashMap *, char *);
extern void HashMapItemPrint(HashMapItem *);

extern HashMap *HashMapInit(u_int32_t, u_int8_t, HashFunction *, bool);
extern HashMap *HashMapInitDefault(void);
extern HashMap *HashMapReplicate(HashMap *);
extern void HashMapFree(HashMap *);
extern void HashMapInsert(HashMap *, HashMapItem *);
extern void HashMapRemove(HashMap *, char *);
extern void HashMapPrint(HashMap *);

extern void DEBUGTestDefaultHashFunction();
// ————————— HASHMAP END —————————

#endif
