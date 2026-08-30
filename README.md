# c-collections

[https://github.com/standardloop/c-collections](https://github.com/standardloop/c-collections)

[![GitHub Release](https://img.shields.io/github/v/release/standardloop/c-collections?sort=semver)](https://github.com/standardloop/c-collections/releases) ![Platform: macOS](https://img.shields.io/badge/platform-macOS-000000?style=flat&logo=apple&logoColor=white) ![C Version](https://img.shields.io/badge/C_Standard-C17-00599C?logo=c&logoColor=white)

## About

This repo contains my reusable datastructures all written in C released as a `dylib`

### Item

An `Item` is a wrapper around a `void` pointer.

It looks like this

```C
typedef struct
{
    void *value;
    ItemFreeFunction *freeFunction;
    ItemPrintFunction *printFunction;
} Item;
```

This `Item` is the foundation of these collections present.

### List

WIP

### Hashmap

WIP
