# Comprehensive Mock Exam 2 — Official Solution Key

---

## 📝 Question 1: C++ Concurrency, Regular Expressions, & TDD (40 Points)

### Section A: Class Declarations & Separation (`LogDispatcher.h`)
```cpp
#ifndef LOG_DISPATCHER_H
#define LOG_DISPATCHER_H

#include <string>
#include <vector>
#include <mutex>
#include <regex>
#include <thread>

class LogDispatcher {
private:
    std::vector<std::string> log_records; // Shared resource
    std::mutex log_mutex;
    std::regex log_regex;

public:
    LogDispatcher();
    
    // Parses and validates log lines using capturing groups
    bool parseAndDispatch(const std::string& input);

    // Thread worker function
    void appendLog(const std::string& level, const std::string& message);

    // Returns a copy of current records (thread-safe)
    std::vector<std::string> getLogs();
};

#endif // LOG_DISPATCHER_H
```

### Section B: Implementation Details (`LogDispatcher.cpp` & `main.cpp`)

#### `LogDispatcher.cpp`
```cpp
#include "LogDispatcher.h"
#include <iostream>
#include <functional>

LogDispatcher::LogDispatcher() : log_regex("LOG \\[ (DEBUG|INFO|WARN|ERROR) \\] \\s+ ([a-zA-Z0-9_\\s]+)") {}

bool LogDispatcher::parseAndDispatch(const std::string& input) {
    std::smatch matches;
    if (std::regex_match(input, matches, log_regex)) {
        std::string level = matches[1].str();
        std::string message = matches[2].str();

        // Launch std::thread passing shared logic and variables
        // Pass shared log records by reference via std::ref() inside helper thread spawn
        std::thread worker(&LogDispatcher::appendLog, this, level, message);
        worker.join(); // Immediate join for testing/synchronous flow demonstration
        return true;
    }
    return false;
}

void LogDispatcher::appendLog(const std::string& level, const std::string& message) {
    // Thread safety using lock_guard
    std::lock_guard<std::mutex> lock(log_mutex);
    std::string formatted = "[" + level + "] " + message;
    log_records.push_back(formatted);
    std::cout << formatted << std::endl;
}

std::vector<std::string> LogDispatcher::getLogs() {
    std::lock_guard<std::mutex> lock(log_mutex);
    return log_records;
}
```

#### `main.cpp`
```cpp
#include "LogDispatcher.h"
#include <iostream>

int main() {
    LogDispatcher dispatcher;
    std::string line;
    while (std::getline(std::cin, line)) {
        dispatcher.parseAndDispatch(line);
    }
    return 0;
}
```

### Section C: TDD Google Test Suite (`LogDispatcher_Test.cpp`)
```cpp
#include <gtest/gtest.h>
#include "LogDispatcher.h"

TEST(LogDispatcherTest, RegexValidExtraction) {
    LogDispatcher dispatcher;
    EXPECT_TRUE(dispatcher.parseAndDispatch("LOG [ DEBUG ]   Core System Ready"));
    EXPECT_TRUE(dispatcher.parseAndDispatch("LOG [ ERROR ] ConnectionLost"));
    EXPECT_FALSE(dispatcher.parseAndDispatch("LOG INVALID [INFO] msg"));
}

TEST(LogDispatcherTest, ThreadSafeAppendVerifiesData) {
    LogDispatcher dispatcher;
    dispatcher.parseAndDispatch("LOG [ INFO ] UserLoggedIn");
    
    auto logs = dispatcher.getLogs();
    ASSERT_EQ(logs.size(), 1);
    EXPECT_EQ(logs[0], "[INFO] UserLoggedIn");
}
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `logger.cpp`)
*   **Filename:** `logger.cpp`
*   **Line / Code Block:** Line 11: `std::thread worker_thread(increment_counter, local_counter);`
*   **Mechanics of Flaw:** `std::thread` constructors copy/move arguments by value by default. Since `increment_counter` takes its parameter by reference (`int&`), passing a raw variable causes compilation failure. It must be wrapped in `std::ref`.
*   **Corrected Statement:**
    ```cpp
    std::thread worker_thread(increment_counter, std::ref(local_counter));
    ```

### Bug 2 (Component B - `components/ItemList.jsx`)
*   **Filename:** `components/ItemList.jsx`
*   **Line / Code Block:** Line 9-10: `items.push('Item C'); setItems(items);`
*   **Mechanics of Flaw:** Directly calling `.push()` mutates the state array in place, keeping the same memory reference. Since the reference is unchanged, React's shallow comparison ignores the update, skipping component re-rendering.
*   **Corrected Statement:** Update state immutably by spreading into a new array:
    ```jsx
    setItems([...items, 'Item C']);
    ```

### Bug 3 (Component C - `App.jsx`)
*   **Filename:** `App.jsx`
*   **Line / Code Block:** Inside `Layout()` function: `<div className="content-pane">`
*   **Mechanics of Flaw:** In `react-router-dom`, nested route components (like `Dashboard`) will not render inside the parent layout unless the parent contains an `<Outlet />` placeholder component.
*   **Corrected Statement:** Import and use `<Outlet />` inside `Layout`:
    ```jsx
    // Add import: import { Outlet, Link } from 'react-router-dom';
    // Inside Layout():
    <div className="content-pane">
        <Outlet />
    </div>
    ```

### Bug 4 (Component A - `logger.cpp`)
*   **Filename:** `logger.cpp`
*   **Line / Code Block:** Local stack-lifetime variable: `int local_counter = 0;` inside `start_worker()`.
*   **Mechanics of Flaw:** If the thread was detached or ran asynchronously, `local_counter` could go out of scope and be destroyed before the thread finishes executing, leading to undefined behavior or memory corruption (dangling references). However, since it is joined immediately (`worker_thread.join()`), the execution remains safe. The actual bug is the missing template type-safety in the compiler dispatching thread configurations (which is solved by `std::ref`).

---

## 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

### Section A: C++ Constructor Elision & Destructors
*   **Predicted Output (with -fno-elide-constructors):**
    ```
    11232333
    ```
*   **Step-by-step Execution Tracing:**
    1.  `main` calls `processTracker()`.
    2.  Inside `processTracker()`, `Tracker temp` is constructed: prints `"1"` (first constructor).
    3.  `return temp;` creates a temporary return object from `temp` via copy constructor: prints `"2"` (first copy constructor).
    4.  `temp` goes out of scope and is destroyed: prints `"3"` (first destructor).
    5.  Back in `main`, `Tracker obj = processTracker();` initializes `obj` from the temporary return object via copy constructor: prints `"2"` (second copy constructor).
    6.  The temporary return object is destroyed: prints `"3"` (second destructor).
    7.  `main` finishes, and `obj` goes out of scope, calling its destructor: prints `"3"` (third destructor).
    *   *Note*: Output string is exactly `123233`. (Some compiler setups might output `1232333` depending on temporary object destruction sequencing).

### Section B: JavaScript Microtask Priority & Callbacks
*   **Predicted Output Sequence:**
    ```
    A
    F
    C
    E
    B
    D
    ```
*   **Event Loop Step-by-step Tracing:**
    1.  Synchronous logs `"A"` first.
    2.  `setTimeout(B)` is pushed to the Macrotask queue.
    3.  `Promise.resolve().then(...)` registers microtasks.
    4.  Synchronous logs `"F"`.
    5.  Stack clears. The Microtask queue executes:
        - Logs `"C"`.
        - Registers `setTimeout(D)` to the Macrotask queue.
        - Resolves downstream `.then(...)`, logging `"E"`.
    6.  Stack clears again. The Macrotask queue executes:
        - Pops first macrotask, logging `"B"`.
        - Pops second macrotask, logging `"D"`.

### Section C: POSIX Process Trees
*   **Number of printed lines:** 4 lines of `"ThreadRun"` (since $2^2 = 4$ processes are spawned).
*   **Process Tree Hierarchy:**
    *   `P` (Parent ID: 1000) forks first loop iteration -> Spawns `C1` (Child 1 ID: 1001).
    *   Second loop iteration: Both `P` and `C1` fork:
        *   `P` spawns `C2` (Child 2 ID: 1002).
        *   `C1` spawns `C3` (Child 3 ID: 1003).
    *   Total: 4 active processes executing std::cout, resulting in 4 lines printed.

---

## 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

### Section A: Low-Level Git Object Mechanics
1.  `zlib`
2.  `space` (separator between type and size)
3.  `null` (or `\0` byte separating header and content)
4.  `tree`
5.  `tree`

### Section B: Garbage Collection Comparison Table

| GC Strategy | How it Identifies Garbage | Pros | Cons / Main Limitation |
| :--- | :--- | :--- | :--- |
| **Reference Counting** | Count of pointer references to object. | Immediate deallocation | Cannot deallocate cyclic references. |
| **Mark & Sweep** | Runs graph search from roots | Resolves cyclic references. | Stops the World (STW) & Fragmented Memory |
| **Generational GC** | Categorizes heap objects by age. | Exploits "most objects die young" | Allocation/promotion overhead. |

---

## 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

### Section A: Android ListViews & Activity Results
*   **ArrayAdapter Purpose:** It acts as a bridge between an `AdapterViews` (such as `ListView`) and the underlying data list. It iterates over the array and instantiates individual view rows (e.g. inflating text widgets) dynamically.
*   **Activity Results Communication Code (Java):**

#### Activity A (Launcher)
```java
private static final int REQUEST_CODE = 101;

// Launching Activity B
Intent intent = new Intent(this, ActivityB.class);
startActivityForResult(intent, REQUEST_CODE);

// Overriding Result Listener
@Override
protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    super.onActivityResult(requestCode, resultCode, data);
    if (requestCode == REQUEST_CODE && resultCode == RESULT_OK && data != null) {
        String resultMessage = data.getStringExtra("message_result");
        // Process message
    }
}
```

#### Activity B (Return Data)
```java
Intent returnIntent = new Intent();
returnIntent.putExtra("message_result", "Success payload from B");
setResult(RESULT_OK, returnIntent);
finish(); // Returns to Activity A
```

### Section B: Pure Node.js Web Server
```javascript
const http = require('http');
const url = require('url');

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    
    if (parsedUrl.pathname === '/api/input' && req.method === 'POST') {
        let body = '';
        
        // Listen to data chunks
        req.on('data', chunk => {
            body += chunk;
        });
        
        // Listen to end of stream
        req.on('end', () => {
            try {
                const payload = JSON.parse(body);
                res.writeHead(201, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ status: "Created", data: payload }));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: "Invalid JSON format" }));
            }
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});

server.listen(3000, () => {
    console.log('Pure Node server running on port 3000');
});
```
