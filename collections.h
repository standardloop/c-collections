#ifndef STANDARDLOOP_LIST_H
#define STANDARDLOOP_LIST_H

#define STANDARDLOOP_LIST_H_MAJOR_VERSION 0
#define STANDARDLOOP_LIST_H_MINOR_VERSION 0
#define STANDARDLOOP_LIST_H_PATCH_VERSION 0
#define STANDARDLOOP_LIST_H_VERSION "0.0.0"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ————————— ITEM START —————————
typedef void(ItemFreeFunction)(void *);
typedef void(ItemPrintFunction)(void *);

typedef struct
{
    void *value;
    ItemFreeFunction *freeFunction;
    ItemPrintFunction *printFunction;
} Item;

extern void ItemPrintString(void *);

extern Item *ItemInit(void *, ItemFreeFunction *, ItemPrintFunction *);
extern void ItemFree(Item *);

extern void ItemPrint(Item *);
// ————————— ITEM END —————————

// ————————— LIST START —————————
#define DEFAULT_LIST_SIZE 16
#define DEFAULT_LIST_RESIZE_MULTIPLE 2

typedef struct
{
    u_int32_t size;
    u_int32_t capacity;
    u_int8_t resize_multiple;
    Item **items;
} List;

extern List *ListInit(u_int32_t, u_int8_t);
extern List *ListInitDefault(void);
extern List *ListReplicate(List *);

extern char *ListToString(List *);
extern void ListAddFirst(List *, Item *);
extern void ListAddLast(List *, Item *);
extern void ListAdd(List *, Item *, u_int32_t);

extern void ListRemove(List *, u_int32_t);
extern void ListRemoveFirst(List *);
extern void ListRemoveLast(List *);

extern Item *ListGetAtIndex(List *, u_int32_t);

extern void ListPrint(List *);
extern void ListPrintInfo(List *);
extern void ListFree(List *);
// ————————— LIST END —————————

// ————————— HASHMAP START —————————
#define DEFAULT_MAP_SIZE 16
#define DEFAULT_MAP_RESIZE_MULTIPLE 2

typedef u_int32_t(HashFunction)(char *, u_int32_t);

typedef enum
{
    HASHMAP_COLLISION_OPTION_LINKEDLIST = 0,
    HASHMAP_COLLISION_OPTION_LIST = 1
} HashMapCollisionOptions;

typedef struct hashMapItem
{
    char *key;
    Item *item;
    struct hashMapItem *next;
} HashMapItem;

extern HashMapItem *HashMapItemInit(char *, Item *);
extern void HashMapItemFree(HashMapItem *, bool);
extern void HashMapItemPrint(HashMapItem *);

typedef struct
{
    u_int32_t size;
    u_int32_t capacity;
    u_int8_t resize_multiple;
    u_int32_t collision_count;
    HashMapItem **entries;
    HashFunction *hashFunction;
    bool force_lowercase;
    HashMapCollisionOptions collision_strategy;
} HashMap;

extern HashMapItem *HashMapGet(HashMap *, char *);
;
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
