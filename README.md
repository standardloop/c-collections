# c-collections

[https://github.com/standardloop/c-collections](https://github.com/standardloop/c-collections)

[![GitHub Release](https://img.shields.io/github/v/release/standardloop/c-collections?sort=semver)](https://github.com/standardloop/c-collections/releases) ![Platform: macOS](https://img.shields.io/badge/platform-macOS-000000?style=flat&logo=apple&logoColor=white) ![C Version](https://img.shields.io/badge/C_Standard-C17-00599C?logo=c&logoColor=white)

## About

This repo contains my reusable datastructures all written in C released as a `dylib`

## Testing

This repo uses https://github.com/standardloop/c-testing to aid in testing.

## Available Tasks

<!-- TASKS_START -->

```sh
task: Available tasks for this project:
* default
* release
* dependencies
* test
* sanitize
* test:build-sanitize
* test:build
* test:run
* test:run-sanitize
* clean
* fmt
* docs
* test:leaks            Run macOS leaks on the test program.
```

<!-- TASKS_END -->

## Docs

Docs are generated a first generated with `doxygen` as `xml` and then `moxygen` reads the xml to output markdown in the `docs/` folder.

This repo uses reusable tasks from https://github.com/standardloop/taskfiles/

## Precommit

This repo uses [prek](https://github.com/j178/prek).

Please run `prek install`.
