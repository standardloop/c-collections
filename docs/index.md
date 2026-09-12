# API Reference

## Classes

| Name                          | Description                                                                                                              |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| [`Item`](#item)               | The [Item](#item) struct, contains a value of any kind, and a function to free the value and print the value.            |
| [`List`](#list)               | The [List](#list) struct, contains a header for size, capacity, and resizing multiple and then a [List](#list) of Items. |
| [`HashMap`](#hashmap)         |                                                                                                                          |
| [`HashMapItem`](#hashmapitem) |                                                                                                                          |

## Macros

---

### STANDARDLOOP_COLLECTIONS_H_MAJOR_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_MAJOR_VERSION 0
```

Major version of this library.

---

### STANDARDLOOP_COLLECTIONS_H_MINOR_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_MINOR_VERSION 0
```

Minor version of this library.

---

### STANDARDLOOP_COLLECTIONS_H_PATCH_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_PATCH_VERSION 1
```

Patch version of this library.

---

### STANDARDLOOP_COLLECTIONS_H_VERSION

```cpp
#define STANDARDLOOP_COLLECTIONS_H_VERSION "0.0.1"
```

Version of this library as a string.

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

A function that will be a part of the [Item](#item) struct, it determines how the [Item](#item) will be freed.

---

### ItemPrintFunction

```cpp
using ItemPrintFunction = void
```

A function that will be a part of the [Item](#item) struct, it determines how the [Item](#item) will be printed, mainly for debugging.

---

### HashFunction

```cpp
using HashFunction = u_int32_t
```

## Functions

---

### ItemPrintString

```cpp
void ItemPrintString(void * item)
```

Prints a string value.

#### Parameters

| Parameter | Type     | Description              |
| --------- | -------- | ------------------------ |
| `item`    | `void *` | The item value to print. |

---

### ItemInit

```cpp
Item * ItemInit(void * value, ItemFreeFunction * freeFunction, ItemPrintFunction * printFunction)
```

Initializes the [Item](#item).

#### Returns

The initialized [Item](#item)

#### Parameters

| Parameter       | Type                                        | Description                                                          |
| --------------- | ------------------------------------------- | -------------------------------------------------------------------- |
| `value`         | `void *`                                    | The value to put into the [Item](#item).                             |
| `freeFunction`  | [`ItemFreeFunction`](#itemfreefunction) *   | Pointer to a function that can free the value of the [Item](#item).  |
| `printFunction` | [`ItemPrintFunction`](#itemprintfunction) * | Pointer to a function that can print the value of the [Item](#item). |

---

### ItemFree

```cpp
void ItemFree(Item * item)
```

Frees an [Item](#item).

#### Parameters

| Parameter | Type              | Description                |
| --------- | ----------------- | -------------------------- |
| `item`    | [`Item`](#item) * | The [Item](#item) to free. |

---

### ItemPrint

```cpp
void ItemPrint(Item * item)
```

Prints an [Item](#item).

#### Parameters

| Parameter | Type              | Description                 |
| --------- | ----------------- | --------------------------- |
| `item`    | [`Item`](#item) * | The [Item](#item) to print. |

---

### ListInit

```cpp
List * ListInit(u_int32_t initial_capacity, u_int8_t resize_multiple)
```

Intializes a [List](#list).

#### Returns

The initialized [List](#list).

#### Parameters

| Parameter          | Type        | Description                                                 |
| ------------------ | ----------- | ----------------------------------------------------------- |
| `initial_capacity` | `u_int32_t` | How large for the [List](#list) to start as.                |
| `resize_multiple`  | `u_int8_t`  | What multiple to resize the list after it reaches capacity. |

---

### ListInitDefault

```cpp
List * ListInitDefault(void)
```

Intializes a [List](#list) with sane defaults, see ListInit for more customization.

#### Returns

The initialized [List](#list).

---

### ListAddFirst

```cpp
void ListAddFirst(List * list, Item * item)
```

Add an [Item](#item) to the front of the [List](#list).

#### Parameters

| Parameter | Type              | Description                                    |
| --------- | ----------------- | ---------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to add an [Item](#item) to.  |
| `item`    | [`Item`](#item) * | The [Item](#item) to add to the [List](#list). |

---

### ListAddLast

```cpp
void ListAddLast(List * list, Item * item)
```

Add an [Item](#item) to the last available spot of the [List](#list).

#### Parameters

| Parameter | Type              | Description                                    |
| --------- | ----------------- | ---------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to add an [Item](#item) to.  |
| `item`    | [`Item`](#item) * | The [Item](#item) to add to the [List](#list). |

---

### ListAddAtIndex

```cpp
void ListAddAtIndex(List * list, Item * item, u_int32_t index)
```

Add an [Item](#item) to a specific index of the list.

#### Parameters

| Parameter | Type              | Description                                                     |
| --------- | ----------------- | --------------------------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to add an [Item](#item) to.                   |
| `item`    | [`Item`](#item) * | The [Item](#item) to add to the [List](#list).                  |
| `index`   | `u_int32_t`       | The index of the [List](#list) to place the [Item](#item) into. |

---

### ListRemoveAtIndex

```cpp
void ListRemoveAtIndex(List * list, u_int32_t index, bool free)
```

Remove an [Item](#item) at a specific index.

#### Parameters

| Parameter | Type              | Description                                                      |
| --------- | ----------------- | ---------------------------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to remove an [Item](#item) from.               |
| `index`   | `u_int32_t`       | The index of the [List](#list) to remove the [Item](#item) from. |
| `free`    | `bool`            | Do you want to free the [Item](#item) as well.                   |

---

### ListRemoveFirst

```cpp
void ListRemoveFirst(List * list)
```

Remove the first [Item](#item) from a [List](#list).

#### Parameters

| Parameter | Type              | Description                                        |
| --------- | ----------------- | -------------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to remove an [Item](#item) from. |

---

### ListRemoveLast

```cpp
void ListRemoveLast(List * list)
```

Remove the last [Item](#item) from a [List](#list).

#### Parameters

| Parameter | Type              | Description                                        |
| --------- | ----------------- | -------------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to remove an [Item](#item) from. |

---

### ListGetAtIndex

```cpp
Item * ListGetAtIndex(List * list, u_int32_t index)
```

Get an [Item](#item) from a [List](#list) at a specific Index.

#### Returns

The [Item](#item) from the [List](#list) at a specific Index.

#### Parameters

| Parameter | Type              | Description                                      |
| --------- | ----------------- | ------------------------------------------------ |
| `list`    | [`List`](#list) * | The [List](#list) to get an [Item](#item) from.  |
| `index`   | `u_int32_t`       | The index at which to get an [Item](#item) from. |

---

### ListGetFirst

```cpp
Item * ListGetFirst(List * list)
```

Get the first [Item](#item) from a [List](#list).

#### Returns

The first [Item](#item) from the specified [List](#list).

#### Parameters

| Parameter | Type              | Description                                     |
| --------- | ----------------- | ----------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to get an [Item](#item) from. |

---

### ListPopAtIndex

```cpp
Item * ListPopAtIndex(List * list, u_int32_t index)
```

Get and remove an [Item](#item) from a [List](#list).

#### Returns

The [Item](#item) from the [List](#list).

#### Parameters

| Parameter | Type              | Description                                      |
| --------- | ----------------- | ------------------------------------------------ |
| `list`    | [`List`](#list) * | The [List](#list) to get an [Item](#item) from.  |
| `index`   | `u_int32_t`       | The index at which to get an [Item](#item) from. |

---

### ListPopFirst

```cpp
Item * ListPopFirst(List * list)
```

Pop the First [Item](#item) from a [List](#list).

#### Returns

The [Item](#item) from the [List](#list).

#### Parameters

| Parameter | Type              | Description                                     |
| --------- | ----------------- | ----------------------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to get an [Item](#item) from. |

---

### ListPrint

```cpp
void ListPrint(List * list)
```

Print the [List](#list).

#### Parameters

| Parameter | Type              | Description                 |
| --------- | ----------------- | --------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to print. |

---

### ListPrintInfo

```cpp
void ListPrintInfo(List * list)
```

Print info of the [List](#list) (current size, etc...).

#### Parameters

| Parameter | Type              | Description                         |
| --------- | ----------------- | ----------------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to print info of. |

---

### ListFree

```cpp
void ListFree(List * list)
```

Frees the [List](#list) and all the Items in the [List](#list).

#### Parameters

| Parameter | Type              | Description                |
| --------- | ----------------- | -------------------------- |
| `list`    | [`List`](#list) * | The [List](#list) to free. |

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
#include <standardloop/collections.h>
```

```cpp
struct Item
```

The [Item](#item) struct, contains a value of any kind, and a function to free the value and print the value.

### Public Attributes

| Return                                      | Name                              | Description                    |
| ------------------------------------------- | --------------------------------- | ------------------------------ |
| `void *`                                    | [`value`](#value)                 | The value itself.              |
| [`ItemFreeFunction`](#itemfreefunction) *   | [`freeFunction`](#freefunction)   | A function to free the value.  |
| [`ItemPrintFunction`](#itemprintfunction) * | [`printFunction`](#printfunction) | A function to print the value. |

---

#### value

```cpp
void * value
```

The value itself.

---

#### freeFunction

```cpp
ItemFreeFunction * freeFunction
```

Type: [`ItemFreeFunction`](#itemfreefunction) *

A function to free the value.

---

#### printFunction

```cpp
ItemPrintFunction * printFunction
```

Type: [`ItemPrintFunction`](#itemprintfunction) *

A function to print the value.

## List

```cpp
#include <standardloop/collections.h>
```

```cpp
struct List
```

The [List](#list) struct, contains a header for size, capacity, and resizing multiple and then a [List](#list) of Items.

### Public Attributes

| Return             | Name                                  | Description                                                                                        |
| ------------------ | ------------------------------------- | -------------------------------------------------------------------------------------------------- |
| `u_int32_t`        | [`size`](#size)                       | The current size of the [List](#list).                                                             |
| `u_int32_t`        | [`capacity`](#capacity)               | The capacity of the [List](#list), can be expanded.                                                |
| `u_int8_t`         | [`resize_multiple`](#resize_multiple) | The resizing multiple of the [List](#list), if the list is at capacity, what multiple to resize it |
| [`Item`](#item) ** | [`items`](#items)                     | The list of Items.                                                                                 |

---

#### size

```cpp
u_int32_t size
```

The current size of the [List](#list).

---

#### capacity

```cpp
u_int32_t capacity
```

The capacity of the [List](#list), can be expanded.

---

#### resize_multiple

```cpp
u_int8_t resize_multiple
```

The resizing multiple of the [List](#list), if the list is at capacity, what multiple to resize it

---

#### items

```cpp
Item ** items
```

Type: [`Item`](#item) **

The list of Items.

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
| [`Item`](#item) *      | [`item`](#item-1) |             |
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

Type: [`Item`](#item) *

---

#### next

```cpp
struct hashMapItem * next
```
