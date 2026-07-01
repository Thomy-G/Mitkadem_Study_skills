# Solution: Archetype 1 — C++ Architectural Design

This document details the architectural layout, implementation details, and patterns utilized to solve the Archetype 1 question.

---

## 1. Architectural Layout & Separation of Concerns
The implementation is cleanly decoupled into separate translation units:
*   [LoggerSystem.h](LoggerSystem.h) / [LoggerSystem.cpp](LoggerSystem.cpp): Represents the Model layer. Manages recorded log counts, filters out prefix levels via `METRICS_IGNORE_FILTER` values retrieved securely with `std::getenv`, and holds the core logic for error alerts and status resets. It has no concept of input streams.
*   [Command.h](Command.h) / [Command.cpp](Command.cpp): Implements the Command Pattern interface (`Command`) and subclasses (`LogCommand`, `CountCommand`, `ClearCommand`), alongside the execution router (`CommandDispatcher`). This layer validates and processes command arguments.
*   [main.cpp](main.cpp): The main controller. Initializes elements, registers commands, and drives the line-by-line event loop from `std::cin`.

---

## 2. Best Practices & include guards
- **Include Guards**: Every header file implements standard `#ifndef` / `#define` / `#endif` constructs (e.g. `COMMAND_H` and `LOGGERSYSTEM_H`) to prevent duplicate compilation errors.
- **Rule of Zero**: Standard library containers (`std::map`, `std::vector`, `std::string`) are used for memory allocation. No custom copy constructors, assignment operators, or destructors are declared.
- **Smart Pointers**: No raw `new`/`delete` operators are used. Subclassed commands are encapsulated in `std::unique_ptr` using `std::make_unique` to ensure exception safety and automatic lifetime management.
- **Type-Safe Parsing**: Unrecognised commands are silenced via lookup mapping checks in the registry, preventing execution crashes.
