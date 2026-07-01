# Archetype 1: C++ Architectural Design (40 Points)

## Description
You are required to implement a command-line Event Logging System in C++ that processes log streams, counts log frequencies, and raises alerts. 

The application reads from standard input (`std::cin`) continuously line-by-line. Each line contains a command verb followed by parameters. The application must support three commands:

1. `log [level] [message]`
   - Records an event with the specified `[level]` (e.g., `INFO`, `WARNING`, `ERROR`). The `[message]` can consist of multiple words.
   - If the log level matches any filter prefix specified in the environment variable `LOGGER_IGNORE_FILTER` (which contains a comma-separated list of prefixes, e.g., `DBG_,INF_`), it must be ignored silently.
   - If successfully recorded, print: `[[level]]: [message]` to `std::cout`.
   - If the log level is exactly `ERROR`, print an additional alert line: `CRITICAL ALERT: [message]` to `std::cout`.

2. `count [level]`
   - Prints the count of all successfully recorded logs of the given `[level]` to `std::cout` in the format: `[level] count: [number]`.
   - If no logs have been recorded for this level, print: `[level] count: 0`.

3. `clear`
   - Resets all logged statistics and threshold metrics.
   - This command produces no terminal console output.

---

## Architectural & Grading Constraints
To receive full credit, your solution must adhere to the following software engineering mandates:
1. **Header & Implementation Separation**: Isolate all classes cleanly. You must write separate `.h` header files containing declarations and `.cpp` implementation files containing definitions.
2. **Open-Closed Principle (OCP)**: Future logger commands (e.g., `dump`, `status`, `exit`) should be addable without modifying the central dispatcher loop. Use the **Command Pattern** with a command dispatcher registry.
3. **Memory Safety & OCF**: Manual allocations (`new`/`delete`) are strictly forbidden. Use smart pointers (`std::unique_ptr` or `std::shared_ptr`).
4. **Header guards**: Implement standard `#ifndef` include guards in all header files.
5. **Robustness**: Unrecognized command verbs or missing parameter lines must be ignored silently.
