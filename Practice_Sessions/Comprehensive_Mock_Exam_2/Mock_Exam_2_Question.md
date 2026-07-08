# Bar-Ilan University — Advanced Systems Programming (893210)
## Comprehensive Mock Exam 2 (Practice Session)

**Time Allowed:** 3 Hours
**Total Points:** 100

---

### 📝 Question 1: C++ Concurrency, Regular Expressions, & TDD (40 Points)

#### Scenario
You are required to build a **Multi-threaded Log Dispatcher Engine** in C++. The engine reads log commands from standard input (`std::cin`), parses them using regular expressions, and dispatches them asynchronously to worker threads.

The engine must support the following string command pattern parsed via **regular expressions**:
`LOG \[ (DEBUG|INFO|WARN|ERROR) \] \s+ ([a-zA-Z0-9_\s]+)`
*   *Group 1 (Level)*: Captures the log severity level (`DEBUG`, `INFO`, `WARN`, or `ERROR`).
*   *Group 2 (Message)*: Captures the log message text.

When a valid log command is received, the dispatcher must spawn a **standard C++ thread (`std::thread`)** to handle it. The handling thread must:
1.  Safely append the message to a central log vector shared across threads.
2.  Print the message to `std::cout` in the format: `[Level] Message` (e.g., `[DEBUG] Core System Ready`).
3.  Synchronization must be enforced using a `std::mutex` protected by a `std::lock_guard` to prevent race conditions on `std::cout` and the shared vector.

#### Architectural & Grading Constraints
1.  **Separation of Concerns:** Split your architecture into clean declarations (`.h` header) and implementations (`.cpp` files).
2.  **Standard Thread Mechanics:** Use `std::thread` and ensure all spawned threads are properly joined before program exit to prevent termination crashes.
3.  **Ref Argument Passing:** Pass the shared log vector by reference to your thread worker function. You must explicitly demonstrate the use of `std::ref()` in your thread initialization.
4.  **TDD Google Test Suite:** Write unit tests using the **Google Test** framework verifying:
    *   Correct regex extraction of severity levels and messages.
    *   Thread-safe data insertion to the shared log vector.

---

### 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

Locate and document **exactly 4 distinct bugs** across the C++, React Native, and React Web Router code snippets below.

#### Component A: C++ Thread Setup (`logger.cpp`)
```cpp
#include <iostream>
#include <thread>
#include <vector>

void increment_counter(int& counter) {
    counter++;
}

void start_worker() {
    int local_counter = 0;
    // Launch worker thread
    std::thread worker_thread(increment_counter, local_counter); // Line 11
    worker_thread.join();
}
```

#### Component B: React Native Component (`components/ItemList.jsx`)
```jsx
import React, { useState } from 'react';
import { View, Text, FlatList, Button } from 'react-native';

export default function ItemList() {
    const [items, setItems] = useState(['Item A', 'Item B']);

    const addItem = () => {
        items.push('Item C'); // Line 9
        setItems(items);      // Line 10
    };

    return (
        <View style={{ flex: 1, padding: 20 }}>
            <FlatList
                data={items}
                keyExtractor={(item, index) => index.toString()}
                renderItem={({ item }) => <Text>{item}</Text>}
            />
            <Button title="Add" onPress={addItem} />
        </View>
    );
}
```

#### Component C: Web React Layout Routing (`App.jsx`)
```jsx
import React from 'react';
import { BrowserRouter, Routes, Route, Link } from 'react-router-dom';

function Layout() {
    return (
        <div>
            <header>BIU Portal Navigation</header>
            <nav>
                <Link to="/">Home</Link> | <Link to="/dashboard">Dashboard</Link>
            </nav>
            <div className="content-pane">
                {/* Nested child views should render here */}
            </div>
        </div>
    );
}

function Dashboard() { return <h1>Dashboard Screen</h1>; }

export default function App() {
    return (
        <BrowserRouter>
            <Routes>
                <Route path="/" element={<Layout />}>
                    <Route path="dashboard" element={<Dashboard />} />
                </Route>
            </Routes>
        </BrowserRouter>
    );
}
```

---

### 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

#### Section A: C++ Constructor Elision & Destructors (10 Points)
Trace the execution and predict the exact console output of the code block below if compiled with the C++ flag:
`g++ -fno-elide-constructors main.cpp`

```cpp
#include <iostream>
using namespace std;

class Tracker {
public:
    Tracker() { cout << "1"; }
    Tracker(const Tracker&) { cout << "2"; }
    ~Tracker() { cout << "3"; }
};

Tracker processTracker() {
    Tracker temp;
    return temp;
}

int main() {
    Tracker obj = processTracker();
    return 0;
}
```

#### Section B: JavaScript Microtask Priority & Callbacks (5 Points)
Predict the exact console output sequence for this asynchronous JavaScript execution:
```javascript
console.log("A");

setTimeout(() => {
    console.log("B");
}, 0);

Promise.resolve().then(() => {
    console.log("C");
    setTimeout(() => {
        console.log("D");
    }, 0);
}).then(() => {
    console.log("E");
});

console.log("F");
```

#### Section C: POSIX Process Trees (5 Points)
Draw the process tree hierarchy and predict the number of output lines printed from this program execution (Assume `fork()` succeeds):
```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 2; i++) {
        fork();
    }
    std::cout << "ThreadRun" << std::endl;
    return 0;
}
```

---

### 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

#### Section A: Low-Level Git Object Mechanics (5 Points)
Complete the blanks (1-5) describing Git filesystem behavior:
*   Git compresses object files using the **[1]________** compression library.
*   The raw header of a Blob object before hashing consists of the type tag, a **[2]________**, and a **[3]________** byte.
*   A **[4]________** object associates blobs and sub-trees with their filenames and permissions.
*   A Commit object contains a pointer to a single root **[5]________** object, metadata, and optional pointers to parent commits.

#### Section B: Garbage Collection Comparison Table (5 Points)
Complete the missing columns in the Garbage Collection strategy comparison table:

| GC Strategy | How it Identifies Garbage | Pros | Cons / Main Limitation |
| :--- | :--- | :--- | :--- |
| **Reference Counting** | [1]____________________ | Immediate deallocation | [2]____________________ |
| **Mark & Sweep** | Runs graph search from roots | [3]____________________ | Stops the World (STW) & Fragmented Memory |
| **Generational GC** | [4]____________________ | Exploits "most objects die young" | [5]____________________ |

---

### 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

#### Section A: Android ListViews & Activity Results (5 Points)
1.  Explain the purpose of `ArrayAdapter` in binding data to Android `ListView` widgets.
2.  Write a Java snippet showing how Activity A launches Activity B requesting a result (`startActivityForResult`), and how Activity B returns a custom string parameter (`message_result`) and closes itself.

#### Section B: Pure Node.js Web Server (5 Points)
Write a pure Node.js native HTTP server (`http.createServer`) without any Express package imports. The server must:
*   Listen on port `3000`.
*   Handle `POST` requests at path `/api/input`.
*   Read the request body streams manually (collecting chunks via data events) and send back the parsed JSON payload with a `201 Created` status code.
