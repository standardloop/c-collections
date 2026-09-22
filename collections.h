/**
 * @file collections.h
 * @headerfile collections.h <standardloop/collections.h>
 * @brief A C library for data structures such as lists and hashmaps.
 */

#ifndef STANDARDLOOP_COLLECTIONS_H
#define STANDARDLOOP_COLLECTIONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ————————— ITEM START —————————
/**
 * @brief A function that will be a part of the Item struct, it determines how
 * the Item will be freed.
 */
typedef void(ItemValueFreeFunction)(void *);
/**
 * @brief Frees the List and all the Items in the List
 * @param list The List to free.
 */
extern void ListFree(void *list); // void * to fit ItemFreeFunction definition
/**
 * @brief Frees a HashMap
 * @param map The HashMap that should be freed.
 */
extern void HashMapFree(void *map);

/**
 * @brief A function that will be a part of the Item struct, it determines how
 * the Item will be printed, mainly for debugging.
 */
typedef void(ItemValuePrintFunction)(void *);
extern void DefaultPrint(void *v);
extern void PrintString(void *value);
extern void PrintInt(void *value);
extern void ListPrint(void *list); // void * to fit ItemPrintFunction definition
extern void HashMapPrint(void *map);
extern void LinkedListPrint(void *list);
extern void ComplexHashMapPrint(void *map);

/**
 * @brief A function that will be a part of the Item struct, it determines how
 * the Item can be converted to a string.
 */
typedef char *(ItemValueToStringFunction)(void *);
extern char *DefaultToString(void *v);
extern char *StringToString(void *s);
extern char *IntToString(void *num);
extern char *ListToString(void *list);
extern char *HashMapToString(void *map);
extern char *LinkedListToString(void *list);
extern char *ComplexHashMapToString(void *map);

typedef void *(ItemValueReplicateFunction)(void *);
extern void *DefaultDuplicate(void *v);
extern void *DuplicateInt(void *original);
extern void *DuplicateString(void *original);
extern void *ListDuplicate(void *list);
extern void *HashMapDuplicate(void *map);
extern void *LinkedListDuplicate(void *list);

typedef bool(ItemValueEquivalenceFunction)(void *, void *);
extern bool StringEquivalence(void *s1, void *s2);
extern bool IntEquivalence(void *int1, void *int2);
extern bool ListEquivalence(void *list1, void *list2);
extern bool HashMapEquivalence(void *map1, void *map2);
extern bool ComplexHashMapEquivalence(void *map1, void *map2);

typedef u_int32_t(ItemValueHashFunction)(void *);
extern u_int32_t StringHash(void *str);
extern u_int32_t IntHash(void *integer);
extern u_int32_t ListHash(void *list);
extern u_int32_t HashMapHash(void *map);
extern u_int32_t ComplexHashMapHash(void *map);

typedef struct
{
    /** A function to free the value. */
    ItemValueFreeFunction *freeFunction;
    /** A function to print the value. */
    ItemValuePrintFunction *printFunction;
    /** A function to turn the value. */
    ItemValueToStringFunction *toStringFunction;
    /** A function deep replicate a value of an item. */
    ItemValueReplicateFunction *duplicateFunction;
    /** A function to hash the structure */
    ItemValueHashFunction *hashFunction;
    /** A function to check for  equivalency*/
    ItemValueEquivalenceFunction *equivalenceFunction;
} ItemValueOperations;

extern ItemValueOperations ItemValueStringOperations;
extern ItemValueOperations ItemValueIntOperations;
extern ItemValueOperations ItemValueListOperations;
extern ItemValueOperations ItemValueHashMapOperations;
extern ItemValueOperations ItemValueComplexHashMapOperations;
extern ItemValueOperations ItemValueLinkedListOperations;

extern const char SL_COLLECTIONS_TYPE_STRING;
extern const char SL_COLLECTIONS_TYPE_INT;
extern const char SL_COLLECTIONS_TYPE_LIST;
extern const char SL_COLLECTIONS_TYPE_HASHMAP;
extern const char SL_COLLECTIONS_TYPE_COMPLEX_HASHMAP;
extern const char SL_COLLECTIONS_TYPE_LINKED_LIST;

/**
 * @brief The Item struct, contains a value of any kind, and a function to free
 * the value and print the value.
 */
typedef struct
{
    /** The value itself. */
    void *value;
    /** What type is the value, by using const char memory addresses. */
    const void *type_id;
    /** Common Operations to be performed on the value. */
    ItemValueOperations *value_ops;
} Item;

/**
 * @brief Initializes the Item.
 * @param value The value to put into the Item.
 * @param value_ops A struct containing operations that can be performed on the
 * value of the Item.
 * @return The initialized Item
 */
extern Item *ItemInit(void *value, ItemValueOperations *value_ops);

extern Item *ItemInitV2(void *value, const void *type_id,
                        ItemValueOperations *value_ops);

#define ITEM_TYPES_MATCH(item1, item2) ((item1)->type_id == (item2)->type_id)

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

/**
 * @brief Converts and Item to a String, uses the ItemToStringFunction.
 * @param item The Item to convert to string.
 * @return a string representation of the item.
 */
extern char *ItemToString(Item *item);

extern Item *ItemDuplicate(Item *item);

extern u_int32_t ItemHash(Item *item);

extern bool ItemEquivalence(Item *item1, Item *item2);

/// @cond INTERNAL
extern void TestItem();
/// @endcond

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
 * @brief Initializes a List.
 * @param initial_capacity How large for the List to start as.
 * @param resize_multiple What multiple to resize the list after it reaches
 * capacity.
 * @return The initialized List.
 */
extern List *ListInit(u_int32_t initial_capacity, u_int8_t resize_multiple);

/**
 * @brief Initializes a List with sane defaults, see ListInit for more
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

// TODO, reconsider this if having stack and queue
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
 * @brief Print info of the List (current size, etc...).
 * @param list The List to print info of.
 */
extern void ListPrintInfo(List *list);

/// @cond INTERNAL
extern void TestList();
/// @endcond
// ————————— LIST END —————————

// ————————— HASHMAP START —————————
/**
 * @brief A Function that takes key and a capacity and hashes the key to return
 * an index.
 */
typedef u_int32_t(HashFunction)(char *, u_int32_t);

/**
 * @brief The HashMap Item struct. Contains the key, the value, and a next field
 * for collisions
 */
typedef struct hashMapItem
{
    /** The key for a hashed value. */
    char *key;
    /** the item hashed to the key. */
    Item *item;
    /** If a collision occurs, the next field - linked list for collisions. */
    struct hashMapItem *next;
} HashMapItem;

/**
 * @brief Initializes as HashMapItem.
 * @param key The string key of HashMapItem.
 * @param item The corresponding value to the hashed key.
 * @return The Initialized HashMapItem.
 */
extern HashMapItem *HashMapItemInit(char *key, Item *item);

extern HashMapItem *HashMapItemDuplicate(HashMapItem *hashmap_item);

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
 * @brief The HashMap struct, contains a header for size, capacity, and resizing
 * multiple. Has a list of entries and a pointer to a hashing function.
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
} HashMap;

/**
 * @brief Get a HashMapItem based on a key.
 * @param map The HashMap to look into.
 * @param key the key to use to look up the item.
 * @return The HashMapItem from the HashMap (if found).
 */
extern HashMapItem *HashMapGet(HashMap *map, char *key);

/**
 * @brief Initialize a fresh HashMap
 * @param initial_capacity The starting capacity of the HashMap.
 * @param resize_multiple What resizing multiple to use when the HashMap is at
 * capacity.
 * @param hashFunction A pointer to a function for hashing string to an index.
 * @param force_lowercase Do we want to force lowercase conversion for all keys.
 * @return The initialized HashMap.
 */
extern HashMap *HashMapInit(u_int32_t initial_capacity,
                            u_int8_t resize_multiple,
                            HashFunction *hashFunction, bool force_lowercase);

/**
 * @brief Initialize a fresh HashMap with sane defaults, see HashMapInit for
 * more customization.
 * @return The initialized HashMap.
 */
extern HashMap *HashMapInitDefault(void);

/**
 * @brief Insert a HashMapItem into a HashMap
 * @param map The HashMap that should be inserted into.
 * @param entry The HashMapItem entry to be inserted into the HashMap.
 */
extern void HashMapInsert(HashMap *map, HashMapItem *entry);

/**
 * @brief Remove a HashMapItem from a HashMap
 * @param map The HashMap that will have a HashMapItem removed from.
 * @param key The key of the HashMapItem to be removed.
 */
extern void HashMapRemove(HashMap *map, char *key);

/**
 * @brief Get a value directly from a lookup instead of receiving HashMapItem
 * then unwrapping to Item, then unwarapping to value
 * @param map The HashMap to look into.
 * @param key The lookup key.
 * @return The direct void * value.
 */
extern void *HashMapGetValueDirect(HashMap *map, char *key);

/// @cond INTERNAL
extern void TestHashMap();
extern void DEBUGTestDefaultHashFunction();
/// @endcond
// ————————— HASHMAP END —————————

// ————————— QUEUE START —————————
// ————————— QUEUE END —————————

// ————————— STACK START —————————
// ————————— STACK END —————————

// ————————— LINKEDLIST START —————————
typedef struct linkedListNode
{
    struct linkedListNode *next;
    Item *item;
} LinkedListNode;

typedef struct
{
    LinkedListNode *head;
    u_int32_t size;
} LinkedList;

extern LinkedList *LinkedListInit();
extern bool IsLinkedListEmpty(LinkedList *list);
extern void LinkedListPrint(void *list);
extern void LinkedListAddToFront(LinkedList *list, Item *item);
extern void LinkedListAddToBack(LinkedList *list, Item *item);
extern void LinkedListAddAtIndex(LinkedList *list, Item *item, u_int64_t index);
extern void LinkedListFree(void *list);
extern void LinkedListDeleteAtIndex(LinkedList *list, u_int64_t index);
extern void LinkedListDeleteBack(LinkedList *list);
extern void LinkedListDeleteFront(LinkedList *list);

/// @cond INTERNAL
extern void TestLinkedList();
/// @endcond
// ————————— LINKEDLIST END —————————

// ————————— COMPLEX HASHMAP START —————————
typedef struct complexHashMapItem ComplexHashMapItem;

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
    ComplexHashMapItem **entries;
} ComplexHashMap;

extern ComplexHashMap *ComplexHashMapInit(u_int32_t initial_capacity,
                                          u_int8_t resize_multiple);

extern ComplexHashMap *ComplexHashMapInitDefault(void);

extern void ComplexHashMapFree(void *map);

extern Item *ComplexHashMapGet(ComplexHashMap *map, Item *key);

extern void ComplexHashMapInsert(ComplexHashMap *map, Item *key, Item *value);

extern void ComplexHashMapRemove(ComplexHashMap *map, Item *key);

extern void *ComplexHashMapDuplicate(void *map);

/// @cond INTERNAL
extern void TestComplexHashMap();
/// @endcond
// ————————— COMPLEX HASHMAP END —————————

#endif
