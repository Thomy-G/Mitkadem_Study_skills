# Solution: C++ Telemetry CLI System Design

This document details the software engineering patterns, architectural layouts, and robust handling mechanisms used to solve the Telemetry CLI question.

---

## 1. Architectural Layout & Separation of Concerns
To prevent coupling between input parsing and internal logic, we partition the system into three main layers, implemented across separate files:

### A. Model / Service Layer
- **Files**: [TelemetrySystem.h](TelemetrySystem.h) and [TelemetrySystem.cpp](TelemetrySystem.cpp)
- **Responsibility**: Tracks metric history, handles ignore-filters, sets metric thresholds, calculates running averages, and compares values against limits.
- **Independence**: It has no knowledge of command-line input parsing or stdin streaming. It only processes structured arguments (e.g., `std::string` name, `int` value).

### B. Command Pattern Layer
- **Files**: [Command.h](Command.h) and [Command.cpp](Command.cpp)
- **Responsibility**: Implements the Command Pattern.
  - `Command`: The abstract base interface requiring `void execute(const std::vector<std::string>& args, TelemetrySystem& system)`.
  - Concrete Commands (`TrackCommand`, `AverageCommand`, `ThresholdCommand`): Unpack command tokens, validate and convert them robustly, and execute operations on `TelemetrySystem`.
  - If parameter parsing fails (e.g., invalid data types), the command silences the error and aborts execution, satisfying robustness constraints.

### C. Dispatcher / Event Loop
- **File**: [main.cpp](main.cpp)
- **Responsibility**: Houses the main event loop reading from `std::cin` line-by-line, parses the first token as the command verb, and dispatches command arguments via the `CommandDispatcher` registry.

---

## 2. OCP (Open-Closed Principle) Compliance
The **Command Pattern** paired with a lookup registry is the standard way to achieve OCP for string-based CLIs:
- **How it works**: The central dispatcher loop is completely generic. It simply takes the first word, looks it up in the registry map, and executes the corresponding `Command` object.
- **Adding new features**: To support a new command verb (e.g. `min`, `max`, or `clear`), developers do not need to modify the main dispatcher logic or event loop. They only need to subclass `Command` and register the new object instance using `dispatcher.register_command("new_verb", std::make_unique<NewCommand>())`.

---

## 3. Modern Memory Protocols & The Rule of Zero
- **Smart Pointers**: We register commands as `std::unique_ptr<Command>` using `std::make_unique`. There are no raw `new`/`delete` keywords used, meaning all resources are cleaned up automatically upon dispatcher destruction.
- **Rule of Zero**: The classes do not manually allocate, copy, or free raw pointers. We rely entirely on Standard Library containers (`std::map`, `std::vector`, `std::string`) to manage dynamic allocations. Therefore, we do not declare custom copy/move constructors, copy/move assignments, or destructors.
- **Virtual Destructors**: We declare `virtual ~Command() = default;` in the base interface to prevent resource leaks when concrete command objects are deleted via base pointers.

---

## 4. Robustness & Exception Safety
- **Exception-Safe Numeric Conversion**: Converting string tokens to integers using raw `std::stoi` can throw `std::invalid_argument` or `std::out_of_range` exceptions. We wrapped this inside `parse_int()`, catching all exceptions (`catch (...)`) to return a safe boolean status indicator. If conversion fails, the calling command discards the operation silently.
- **Partial Parsing Prevention**: We verify that `idx == str.size()` after calling `std::stoi(str, &idx)`. This prevents strings like `"42abc"` or `"12.5"` from being partially parsed as valid integers.
- **Environment Handling**: Before processing `METRICS_IGNORE_FILTER`, we check if `std::getenv()` returns `nullptr`. If it does, we ignore filtering and avoid segment faults.
- **Division-by-Zero Check**: Before computing averages, we check if the metric collection is empty to prevent undefined mathematical division.
