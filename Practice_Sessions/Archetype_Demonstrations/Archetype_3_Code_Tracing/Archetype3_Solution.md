# Solution: Archetype 3 — Code Execution Tracing & Scope Analysis

Here is the step-by-step trace and output explanation for Archetype 3.

---

## Section A: C++ Multiple & Virtual Inheritance Trace

### 1. Construction Order Mechanics
In C++, virtual inheritance guarantees that a shared base class (`Component`) is constructed only once. The sequence of constructor calls is governed by these rules:
1.  **Virtual Base Classes First**: Virtual base classes are initialized first by the most derived constructor (`Laptop`). This ensures `Component` is initialized before any derived intermediate classes.
2.  **Inheritance Declaration Order**: Non-virtual base classes are then initialized in the order they are listed in the class inheritance declaration list: `class Laptop : public Keyboard, public Screen`. Thus, `Keyboard` is initialized, followed by `Screen`.
3.  **Derived Class Last**: The derived class constructor body (`Laptop`) runs last.

### 2. Output
```
Component constructed
Keyboard constructed
Screen constructed
Laptop constructed
```

---

## Section B: JavaScript Event Loop Trace

### 1. Step-by-Step Queue Lifecycle

| Execution Step | Action                    | Call Stack                                        | Microtask Queue                            | Macrotask Queue      | Console Output |
| :------------- | :------------------------ | :------------------------------------------------ | :----------------------------------------- | :------------------- | :------------- |
| **1**          | Synchronous line          | `console.log('Start')`                            | *Empty*                                    | *Empty*              | `Start`        |
| **2**          | `setTimeout` registration | `setTimeout(...)`                                 | *Empty*                                    | `Timeout 1 Callback` |                |
| **3**          | Promise registration      | `Promise.resolve().then(...)`                     | *Empty*                                    | `Timeout 1 Callback` |                |
| **4**          | Call `runAsync()`         | `console.log('Async Start')`                      | `Promise 1 Callback`                       | `Timeout 1 Callback` | `Async Start`  |
| **5**          | `await Promise.resolve()` | Yield control in `runAsync`                       | `Promise 1 Callback`, `Async End Callback` | `Timeout 1 Callback` |                |
| **6**          | Synchronous line          | `console.log('End')`                              | `Promise 1 Callback`, `Async End Callback` | `Timeout 1 Callback` | `End`          |
| **7**          | **Main Line Ends**        | Call Stack clears. Checks Microtask Queue.        | `Promise 1 Callback`, `Async End Callback` | `Timeout 1 Callback` |                |
| **8**          | Dequeue Microtask 1       | Execute `Promise 1` callback. Chains `Promise 2`. | `Async End Callback`, `Promise 2 Callback` | `Timeout 1 Callback` | `Promise 1`    |
| **9**          | Dequeue Microtask 2       | Resume `runAsync` context.                        | `Promise 2 Callback`                       | `Timeout 1 Callback` | `Async End`    |
| **10**         | Dequeue Microtask 3       | Execute `Promise 2` callback.                     | *Empty*                                    | `Timeout 1 Callback` | `Promise 2`    |
| **11**         | **Microtasks Clear**      | Checks Macrotask Queue.                           | *Empty*                                    | `Timeout 1 Callback` |                |
| **12**         | Dequeue Macrotask         | Execute `setTimeout` callback.                    | *Empty*                                    | *Empty*              | `Timeout 1`    |

### 2. Final Terminal Output Sequence
```
Start
Async Start
End
Promise 1
Async End
Promise 2
Timeout 1
```
