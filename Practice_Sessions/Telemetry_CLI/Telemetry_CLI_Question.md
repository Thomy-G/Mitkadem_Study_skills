# Question: C++ Telemetry CLI System Design (40 Points)

## Description
You are required to implement a real-time Metric Monitoring & Telemetry Command Line Interface (CLI) system in C++. The application processes metric streams, computes summaries on the fly, and enforces warning thresholds. 

The application must read from standard input (`std::cin`) continuously line-by-line and parse each line into tokens. The first token determines the operation. The application must support the following three commands:

1. `track [metric_name] [value]`
   - Records the value of `[metric_name]`. (Values are integers).
   - If the metric name starts with any prefix specified in the environment variable `METRICS_IGNORE_FILTER` (which contains a comma-separated list of prefixes, e.g. `cpu_,sys_`), it must be ignored silently. If `METRICS_IGNORE_FILTER` is empty or missing, do not filter any metrics.
   - If successfully recorded, print: `[metric_name] updated to [value]` to `std::cout`. If ignored, print nothing.
   - If a warning threshold has been set for this metric (see below) and the new value exceeds the threshold limit, print an additional warning line: `WARNING: [metric_name] exceeded [limit]! (current: [value])` to `std::cout`.

2. `average [metric_name]`
   - Computes and prints the average of all recorded values for the given `[metric_name]` to `std::cout` in the format: `[metric_name] average: [average_value]` (as a double/floating-point representation).
   - If no values have been successfully recorded for this metric, print: `[metric_name] has no records`.

3. `threshold [metric_name] [limit]`
   - Sets a warning threshold limit (integer) for the specified `[metric_name]`.
   - Only one threshold can be active per metric at a time. Setting a new threshold overrides the previous one.
   - This command produces no terminal console output.

---

## Architectural & Grading Constraints
To receive full credit (40 points), your solution must adhere to the following software engineering mandates:
1. **Strict Separation of Concerns:** Your solution must completely isolate its data processing and storage logic (e.g., metric storage, threshold checking) from the command-parsing and interaction loop. Describe your file architecture clearly (identifying what goes in `.h` vs `.cpp` files).
2. **Open-Closed Principle (OCP):** Adding new telemetry commands (e.g., `min`, `max`, `clear`) in the future must not require modifying the central event dispatcher or processing loop. Use an appropriate pattern (e.g., the Command Pattern registered in an associative mapping).
3. **Modern Memory Protocols:** Manual allocations (`new`/`delete`) are strictly forbidden. You must ensure automatic scope cleanup and absolute leak prevention by utilizing smart pointers (`std::unique_ptr` or `std::shared_ptr`).
4. **Robustness:** Unrecognized command verbs, missing parameters, or invalid numeric arguments must be ignored silently without throwing exceptions or crashing the execution loop.
