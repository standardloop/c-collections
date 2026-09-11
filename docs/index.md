# API Reference

## Classes

| Name                          | Description |
| ----------------------------- | ----------- |
| [`Item`](#item)               |             |
| [`List`](#list)               |             |
| [`HashMap`](#hashmap)         |             |
| [`HashMapItem`](#hashmapitem) |             |

## Macros

---

### STANDARDLOOP_COLLECTIONS_H_MAJOR_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_MAJOR_VERSION 0
```

---

### STANDARDLOOP_COLLECTIONS_H_MINOR_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_MINOR_VERSION 0
```

---

### STANDARDLOOP_COLLECTIONS_H_PATCH_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_PATCH_VERSION 1
```

---

### STANDARDLOOP_COLLECTIONS_H_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_VERSION "0.0.1"
```

---

### DEFAULT_LIST_SIZE

```cpp
#define DEFAULT_LIST_SIZE 16
```

---

### DEFAULT_LIST_RESIZE_MULTIPLE

```cpp
#define DEFAULT_LIST_RESIZE_MULTIPLE 2
```

---

### DEFAULT_MAP_SIZE

```cpp
#define DEFAULT_MAP_SIZE 16
```

---

### DEFAULT_MAP_RESIZE_MULTIPLE

```cpp
#define DEFAULT_MAP_RESIZE_MULTIPLE 2
```

## Enumerations

---

### HashMapCollisionOptions

```cpp
enum HashMapCollisionOptions
```

| Value                                 | Description |
| ------------------------------------- | ----------- |
| `HASHMAP_COLLISION_OPTION_LINKEDLIST` |             |
| `HASHMAP_COLLISION_OPTION_LIST`       |             |

## Typedefs

---

### ItemFreeFunction

```cpp
using ItemFreeFunction = void
```

---

### ItemPrintFunction

```cpp
using ItemPrintFunction = void
```

---

### HashFunction

```cpp
using HashFunction = u_int32_t
```

## Functions

---

### ItemPrintString

```cpp
void ItemPrintString(void *)
```

---

### ItemInit

```cpp
Item * ItemInit(void *, ItemFreeFunction *, ItemPrintFunction *)
```

---

### ItemFree

```cpp
void ItemFree(Item *)
```

---

### ItemPrint

```cpp
void ItemPrint(Item *)
```

---

### ListInit

```cpp
List * ListInit(u_int32_t, u_int8_t)
```

---

### ListInitDefault

```cpp
List * ListInitDefault(void)
```

---

### ListReplicate

```cpp
List * ListReplicate(List *)
```

---

### ListToString

```cpp
char * ListToString(List *)
```

---

### ListAddFirst

```cpp
void ListAddFirst(List *, Item *)
```

---

### ListAddLast

```cpp
void ListAddLast(List *, Item *)
```

---

### ListAddAtIndex

```cpp
void ListAddAtIndex(List *, Item *, u_int32_t)
```

---

### ListRemoveAtIndex

```cpp
void ListRemoveAtIndex(List *, u_int32_t, bool)
```

---

### ListRemoveFirst

```cpp
void ListRemoveFirst(List *)
```

---

### ListRemoveLast

```cpp
void ListRemoveLast(List *)
```

---

### ListGetAtIndex

```cpp
Item * ListGetAtIndex(List *, u_int32_t)
```

---

### ListGetFirst

```cpp
Item * ListGetFirst(List *)
```

---

### ListPopAtIndex

```cpp
Item * ListPopAtIndex(List *, u_int32_t)
```

---

### ListPopFirst

```cpp
Item * ListPopFirst(List *)
```

---

### ListPrint

```cpp
void ListPrint(List *)
```

---

### ListPrintInfo

```cpp
void ListPrintInfo(List *)
```

---

### ListFree

```cpp
void ListFree(List *)
```

---

### HashMapItemInit

```cpp
HashMapItem * HashMapItemInit(char *, Item *)
```

---

### HashMapItemFree

```cpp
void HashMapItemFree(HashMapItem *, bool)
```

---

### HashMapItemPrint

```cpp
void HashMapItemPrint(HashMapItem *)
```

---

### HashMapGet

```cpp
HashMapItem * HashMapGet(HashMap *, char *)
```

---

### HashMapInit

```cpp
HashMap * HashMapInit(u_int32_t, u_int8_t, HashFunction *, bool)
```

---

### HashMapInitDefault

```cpp
HashMap * HashMapInitDefault(void)
```

---

### HashMapReplicate

```cpp
HashMap * HashMapReplicate(HashMap *)
```

---

### HashMapFree

```cpp
void HashMapFree(HashMap *)
```

---

### HashMapInsert

```cpp
void HashMapInsert(HashMap *, HashMapItem *)
```

---

### HashMapRemove

```cpp
void HashMapRemove(HashMap *, char *)
```

---

### HashMapPrint

```cpp
void HashMapPrint(HashMap *)
```

---

### DEBUGTestDefaultHashFunction

```cpp
void DEBUGTestDefaultHashFunction()
```

## Item

```cpp
struct Item
```

### Public Attributes

| Return                | Name                              | Description |
| --------------------- | --------------------------------- | ----------- |
| `void *`              | [`value`](#value)                 |             |
| `ItemFreeFunction *`  | [`freeFunction`](#freefunction)   |             |
| `ItemPrintFunction *` | [`printFunction`](#printfunction) |             |

---

#### value

```cpp
void * value
```

---

#### freeFunction

```cpp
ItemFreeFunction * freeFunction
```

---

#### printFunction

```cpp
ItemPrintFunction * printFunction
```

## List

```cpp
struct List
```

### Public Attributes

| Return      | Name                                  | Description |
| ----------- | ------------------------------------- | ----------- |
| `u_int32_t` | [`size`](#size)                       |             |
| `u_int32_t` | [`capacity`](#capacity)               |             |
| `u_int8_t`  | [`resize_multiple`](#resize_multiple) |             |
| `Item **`   | [`items`](#items)                     |             |

---

#### size

```cpp
u_int32_t size
```

---

#### capacity

```cpp
u_int32_t capacity
```

---

#### resize_multiple

```cpp
u_int8_t resize_multiple
```

---

#### items

```cpp
Item ** items
```

## HashMap

```cpp
struct HashMap
```

### Public Attributes

| Return                    | Name                                        | Description |
| ------------------------- | ------------------------------------------- | ----------- |
| `u_int32_t`               | [`size`](#size-1)                           |             |
| `u_int32_t`               | [`capacity`](#capacity-1)                   |             |
| `u_int8_t`                | [`resize_multiple`](#resize_multiple-1)     |             |
| `u_int32_t`               | [`collision_count`](#collision_count)       |             |
| `HashMapItem **`          | [`entries`](#entries)                       |             |
| `HashFunction *`          | [`hashFunction`](#hashfunction-1)           |             |
| `bool`                    | [`force_lowercase`](#force_lowercase)       |             |
| `HashMapCollisionOptions` | [`collision_strategy`](#collision_strategy) |             |

---

#### size

```cpp
u_int32_t size
```

---

#### capacity

```cpp
u_int32_t capacity
```

---

#### resize_multiple

```cpp
u_int8_t resize_multiple
```

---

#### collision_count

```cpp
u_int32_t collision_count
```

---

#### entries

```cpp
HashMapItem ** entries
```

---

#### hashFunction

```cpp
HashFunction * hashFunction
```

---

#### force_lowercase

```cpp
bool force_lowercase
```

---

#### collision_strategy

```cpp
HashMapCollisionOptions collision_strategy
```

## HashMapItem

```cpp
struct HashMapItem
```

### Public Attributes

| Return                 | Name              | Description |
| ---------------------- | ----------------- | ----------- |
| `char *`               | [`key`](#key)     |             |
| `Item *`               | [`item`](#item-1) |             |
| `struct hashMapItem *` | [`next`](#next)   |             |

---

#### key

```cpp
char * key
```

---

#### item

```cpp
Item * item
```

---

#### next

```cpp
struct hashMapItem * next
```

Generated by [Moxygen](https://0state.com/moxygen)
