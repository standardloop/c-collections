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

// ITEM START
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
// ITEM END


// LIST BEGIN
#define DEFAULT_LIST_SIZE 16
#define DEFAULT_LIST_RESIZE_MULTIPLE 2

typedef struct
{
    u_int32_t size;
    u_int32_t capacity;
    Item **items;
} List;

extern List *ListInit(u_int32_t);
extern List *DefaultListInit(void);
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
// LIST END

#endif
