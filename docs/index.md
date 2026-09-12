# API Reference

## Classes

| Name                          | Description                                                                                                                                             |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`Item`](#item)               | The [Item](#item) struct, contains a value of any kind, and a function to free the value and print the value.                                           |
| [`List`](#list)               | The [List](#list) struct, contains a header for size, capacity, and resizing multiple and then a [List](#list) of Items.                                |
| [`HashMap`](#hashmap)         | The [HashMap](#hashmap) struct, contains a header for size, capacity, and resizing multiple. Has a list of entries and a pointer to a hashing function. |
| [`HashMapItem`](#hashmapitem) | The [HashMap](#hashmap)[Item](#item) struct. Contains the key, the value, and a next field for collisions.                                              |

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

A Function that takes key and a capacity and hashes the key to return an index.

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
HashMapItem * HashMapItemInit(char * key, Item * value)
```

Initializes as [HashMapItem](#hashmapitem).

#### Returns

The Initialized [HashMapItem](#hashmapitem).

#### Parameters

| Parameter | Type              | Description                                    |
| --------- | ----------------- | ---------------------------------------------- |
| `key`     | `char *`          | The string key of [HashMapItem](#hashmapitem). |
| `value`   | [`Item`](#item) * | The corresponding value to the hashed key.     |

---

### HashMapItemFree

```cpp
void HashMapItemFree(HashMapItem * hashmap_item, bool deep)
```

Frees a [HashMapItem](#hashmapitem), can optional deep free by freeing the [Item](#item) as well.

#### Parameters

| Parameter      | Type                            | Description                                                              |
| -------------- | ------------------------------- | ------------------------------------------------------------------------ |
| `hashmap_item` | [`HashMapItem`](#hashmapitem) * | The [HashMapItem](#hashmapitem) to free.                                 |
| `deep`         | `bool`                          | If true, will free the [Item](#item) in the [HashMapItem](#hashmapitem). |

---

### HashMapItemPrint

```cpp
void HashMapItemPrint(HashMapItem * hashmap_item)
```

Prints a [HashMapItem](#hashmapitem).

#### Parameters

| Parameter      | Type                            | Description                               |
| -------------- | ------------------------------- | ----------------------------------------- |
| `hashmap_item` | [`HashMapItem`](#hashmapitem) * | The [HashMapItem](#hashmapitem) to print. |

---

### HashMapGet

```cpp
HashMapItem * HashMapGet(HashMap * map, char * key)
```

Get a [HashMapItem](#hashmapitem) based on a key.

#### Returns

The [HashMapItem](#hashmapitem) from the [HashMap](#hashmap) (if found).

#### Parameters

| Parameter | Type                    | Description                           |
| --------- | ----------------------- | ------------------------------------- |
| `map`     | [`HashMap`](#hashmap) * | The [HashMap](#hashmap) to look into. |
| `key`     | `char *`                | the key to use to look up the item.   |

---

### HashMapInit

```cpp
HashMap * HashMapInit(u_int32_t initial_capacity, u_int8_t resize_multiple, HashFunction * hashFunction, bool force_lowercase)
```

Initialize a fresh [HashMap](#hashmap).

#### Returns

The initialized [HashMap](#hashmap).

#### Parameters

| Parameter          | Type                              | Description                                                                |
| ------------------ | --------------------------------- | -------------------------------------------------------------------------- |
| `initial_capacity` | `u_int32_t`                       | The starting capacity of the [HashMap](#hashmap).                          |
| `resize_multiple`  | `u_int8_t`                        | What resizing multiple to use when the [HashMap](#hashmap) is at capacity. |
| `hashFunction`     | [`HashFunction`](#hashfunction) * | A pointer to a function for hashing string to an index.                    |
| `force_lowercase`  | `bool`                            | Do we want to force lowercase conversion for all keys.                     |

---

### HashMapInitDefault

```cpp
HashMap * HashMapInitDefault(void)
```

Initialize a fresh [HashMap](#hashmap) with sane defaults, see HashMapInit for more customization.

#### Returns

The initialized [HashMap](#hashmap).

---

### HashMapFree

```cpp
void HashMapFree(HashMap * map)
```

Frees a [HashMap](#hashmap).

#### Parameters

| Parameter | Type                    | Description                                   |
| --------- | ----------------------- | --------------------------------------------- |
| `map`     | [`HashMap`](#hashmap) * | The [HashMap](#hashmap) that should be freed. |

---

### HashMapInsert

```cpp
void HashMapInsert(HashMap * map, HashMapItem * entry)
```

Insert a [HashMapItem](#hashmapitem) into a [HashMap](#hashmap).

#### Parameters

| Parameter | Type                            | Description                                                                        |
| --------- | ------------------------------- | ---------------------------------------------------------------------------------- |
| `map`     | [`HashMap`](#hashmap) *         | The [HashMap](#hashmap) that should be inserted into.                              |
| `entry`   | [`HashMapItem`](#hashmapitem) * | The [HashMapItem](#hashmapitem) entry to be inserted into the [HashMap](#hashmap). |

---

### HashMapRemove

```cpp
void HashMapRemove(HashMap * map, char * key)
```

Remove a [HashMapItem](#hashmapitem) from a [HashMap](#hashmap).

#### Parameters

| Parameter | Type                    | Description                                                                        |
| --------- | ----------------------- | ---------------------------------------------------------------------------------- |
| `map`     | [`HashMap`](#hashmap) * | The [HashMap](#hashmap) that will have a [HashMapItem](#hashmapitem) removed from. |
| `key`     | `char *`                | The key of the [HashMapItem](#hashmapitem) to be removed.                          |

---

### HashMapPrint

```cpp
void HashMapPrint(HashMap * map)
```

Print a [HashMap](#hashmap).

#### Parameters

| Parameter | Type                    | Description                       |
| --------- | ----------------------- | --------------------------------- |
| `map`     | [`HashMap`](#hashmap) * | The [HashMap](#hashmap) to print. |

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
#include <standardloop/collections.h>
```

```cpp
struct HashMap
```

The [HashMap](#hashmap) struct, contains a header for size, capacity, and resizing multiple. Has a list of entries and a pointer to a hashing function.

### Public Attributes

| Return                            | Name                                    | Description                                                                                           |
| --------------------------------- | --------------------------------------- | ----------------------------------------------------------------------------------------------------- |
| `u_int32_t`                       | [`size`](#size-1)                       | The current size (number of entries) of the [HashMap](#hashmap).                                      |
| `u_int32_t`                       | [`capacity`](#capacity-1)               | How large the [HashMap](#hashmap) can get before it needs to be resized.                              |
| `u_int8_t`                        | [`resize_multiple`](#resize_multiple-1) | If the [HashMap](#hashmap) needs to be resized, what should the resizing multiple be                  |
| `u_int32_t`                       | [`collision_count`](#collision_count)   | How many collisions have occured.                                                                     |
| [`HashMapItem`](#hashmapitem) **  | [`entries`](#entries)                   | The [List](#list) entries in the [HashMap](#hashmap).                                                 |
| [`HashFunction`](#hashfunction) * | [`hashFunction`](#hashfunction-1)       | The function this [HashMap](#hashmap) uses to Hash strings.                                           |
| `bool`                            | [`force_lowercase`](#force_lowercase)   | Should only lowercase be considered, for example "THIS" and "this" are both hashed to the same value. |

---

#### size

```cpp
u_int32_t size
```

The current size (number of entries) of the [HashMap](#hashmap).

---

#### capacity

```cpp
u_int32_t capacity
```

How large the [HashMap](#hashmap) can get before it needs to be resized.

---

#### resize_multiple

```cpp
u_int8_t resize_multiple
```

If the [HashMap](#hashmap) needs to be resized, what should the resizing multiple be

---

#### collision_count

```cpp
u_int32_t collision_count
```

How many collisions have occured.

---

#### entries

```cpp
HashMapItem ** entries
```

Type: [`HashMapItem`](#hashmapitem) **

The [List](#list) entries in the [HashMap](#hashmap).

---

#### hashFunction

```cpp
HashFunction * hashFunction
```

Type: [`HashFunction`](#hashfunction) *

The function this [HashMap](#hashmap) uses to Hash strings.

---

#### force_lowercase

```cpp
bool force_lowercase
```

Should only lowercase be considered, for example "THIS" and "this" are both hashed to the same value.

## HashMapItem

```cpp
#include <standardloop/collections.h>
```

```cpp
struct HashMapItem
```

The [HashMap](#hashmap)[Item](#item) struct. Contains the key, the value, and a next field for collisions.

### Public Attributes

| Return                 | Name                | Description                                                         |
| ---------------------- | ------------------- | ------------------------------------------------------------------- |
| `char *`               | [`key`](#key)       | The key for a hashed value.                                         |
| [`Item`](#item) *      | [`value`](#value-1) | the value hashed to the key.                                        |
| `struct hashMapItem *` | [`next`](#next)     | If a collision occurs, the next field - linked list for collisions. |

---

#### key

```cpp
char * key
```

The key for a hashed value.

---

#### value

```cpp
Item * value
```

Type: [`Item`](#item) *

the value hashed to the key.

---

#### next

```cpp
struct hashMapItem * next
```

If a collision occurs, the next field - linked list for collisions.
