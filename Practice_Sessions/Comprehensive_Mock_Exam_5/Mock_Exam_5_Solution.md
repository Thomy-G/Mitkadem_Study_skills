# Comprehensive Mock Exam 5 — Official Solution Key

---

## 📝 Question 1: Express, Mongoose MVC & React Lifecycle (40 Points)

### Section A: Backend Task Models (`Task.js` & `taskController.js`)

#### `Task.js` (Mongoose Model)
```javascript
const mongoose = require('mongoose');

const taskSchema = new mongoose.Schema({
    title: { 
        type: String, 
        required: true, 
        trim: true 
    },
    status: { 
        type: String, 
        enum: ['Pending', 'Completed'], 
        default: 'Pending' 
    },
    assignedUser: { 
        type: mongoose.Schema.Types.ObjectId, 
        ref: 'User', // Reference mapping
        required: true 
    }
}, { timestamps: true });

module.exports = mongoose.model('Task', taskSchema);
```

#### `taskController.js` (Express Route)
```javascript
const express = require('express');
const router = express.Router();
const Task = require('./Task');

router.post('/api/tasks', async (req, res) => {
    try {
        const { title, assignedUser } = req.body;
        if (!title || !assignedUser) {
            return res.status(400).json({ error: "Missing required parameters" });
        }
        const newTask = new Task({ title, assignedUser });
        await newTask.save();
        res.status(201).json(newTask);
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

module.exports = router;
```

### Section B: Frontend React Hook (`components/TaskDashboard.jsx`)
```jsx
import React, { useState, useEffect } from 'react';

export default function TaskDashboard() {
    const [tasks, setTasks] = useState([]);
    const [title, setTitle] = useState('');

    useEffect(() => {
        let active = true; // Prevents updating state of unmounted component

        fetch('/api/tasks')
            .then(res => res.json())
            .then(data => {
                if (active) {
                    setTasks(data);
                }
            });

        return () => {
            active = false; // Cleanup
        };
    }, []);

    const handleSubmit = async (e) => {
        e.preventDefault();
        const res = await fetch('/api/tasks', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ title, assignedUser: "65a8df24128f1100abcde123" }) // Dummy ID
        });
        if (res.ok) {
            const newTask = await res.json();
            // Immutable State Update
            setTasks(prevTasks => [...prevTasks, newTask]);
            setTitle('');
        }
    };

    return (
        <div>
            <form onSubmit={handleSubmit}>
                <input value={title} onChange={e => setTitle(e.target.value)} />
            </form>
            <ul>
                {tasks.map(t => <li key={t._id}>{t.title} - {t.status}</li>)}
            </ul>
        </div>
    );
}
```

### Section C: Integration Tests (`tests/task.test.js`)
```javascript
const request = require('supertest');
const express = require('express');
const router = require('../taskController');

const app = express();
app.use(express.json());
app.use(router);

describe('POST /api/tasks Validation', () => {
    it('should return 400 Bad Request if title is missing', async () => {
        const res = await request(app)
            .post('/api/tasks')
            .send({ assignedUser: "65a8df24128f1100abcde123" });
            
        expect(res.statusCode).toEqual(400);
        expect(res.body.error).toEqual("Missing required parameters");
    });
});
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `dataStore.cpp`)
*   **Filename:** `dataStore.cpp`
*   **Line / Code Block:** Line 9: `data_store.push_back(val);`
*   **Mechanics of Flaw:** `std::vector` is not thread-safe. Concurrent modifications by multiple threads trigger memory corruption (e.g. data races during allocation expansions). A mutex lock must protect the write boundary.
*   **Corrected Statement:**
    ```cpp
    #include <mutex>
    std::mutex mtx;

    void insert_data(int val) {
        std::lock_guard<std::mutex> lock(mtx);
        data_store.push_back(val);
    }
    ```

### Bug 2 (Component B - `TelemetryManager.java`)
*   **Filename:** `TelemetryManager.java`
*   **Line / Code Block:** Line 7-10: `private static Context appContext;` and `appContext = context;`
*   **Mechanics of Flaw:** Memory Leak. Storing a static reference to an Activity Context prevents it from being garbage collected when destroyed (e.g. on orientation change), causing a memory leak.
*   **Corrected Statement:** Reference the application context to bypass local activity lifetimes:
    ```java
    public static void init(Context context) {
        appContext = context.getApplicationContext();
    }
    ```

### Bug 3 (Component C - `queries/query.js`)
*   **Filename:** `queries/query.js`
*   **Line / Code Block:** Line 3: `db.users.find({}, { name: 1, grade: 0, _id: 0 });`
*   **Mechanics of Flaw:** MongoDB Projection mismatch. In MongoDB queries, you cannot mix inclusions (`1`) and exclusions (`0`) in a single projection document. The only exception is the `_id` field. Trying to mix `name: 1` and `grade: 0` throws a syntax error.
*   **Corrected Statement:**
    ```javascript
    db.users.find({}, { name: 1, _id: 0 }); // Only include name, grade is excluded automatically
    ```

### Bug 4 (Component D - `client.py`)
*   **Filename:** `client.py`
*   **Line / Code Block:** Line 7: `s.send("Core System Ready")`
*   **Mechanics of Flaw:** In Python 3, socket transmission commands require binary buffers (`bytes`), not Unicode string variables. Passing a string throws a `TypeError`.
*   **Corrected Statement:** Encode the string into bytes:
    ```python
    s.send("Core System Ready".encode("utf-8"))
    ```

---

## 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

### Section A: POSIX Nested Fork Loops
*   **Number of printed lines:** 9 lines of `"Node"`.
*   **Step-by-step Process Tree Trace:**
    1.  `i = 0`: Parent `P0` calls `fork()`.
        - Child `C1` is spawned (`fork() == 0`).
        - Since it returned `0`, `C1` calls the nested `fork()`, spawning `C2`.
        - Active processes: `P0` (parent), `C1` (first loop child), `C2` (nested child).
    2.  `i = 1`: All three active processes (`P0`, `C1`, `C2`) execute the loop body:
        - `P0` forks -> spawns `C3`. `C3` (returned `0`) calls nested fork -> spawns `C4`.
        - `C1` forks -> spawns `C5`. `C5` calls nested fork -> spawns `C6`.
        - `C2` forks -> spawns `C7`. `C7` calls nested fork -> spawns `C8`.
    3.  Total active processes at the end of loops: `P0`, `C1`, `C2`, `C3`, `C4`, `C5`, `C6`, `C7`, `C8` (exactly 9 processes).
    4.  All 9 processes print `"Node"`, generating 9 output lines.

### Section B: JavaScript Scoping, Closures, & Hoisting
*   **Predicted Output Sequence:**
    ```
    initial count: undefined
    inner count: 11
    global count: 5
    ```
*   **Hoisting and Execution Scope Trace:**
    1.  `var count` inside `runScope` is hoisted to the top of the function context block.
    2.  `console.log("initial count:", count)` is executed *before* initialization, yielding `undefined` (not `5`, due to shadowing).
    3.  `count` inside `runScope` is set to `10`.
    4.  `inner()` is returned and assigned to `trigger`.
    5.  `trigger()` is called:
        - It references the shadowed `count` in `runScope` (value `10`).
        - `count++` increments it to `11`.
        - Logs `"inner count: 11"`.
    6.  `console.log("global count:", count)` prints the global scope `count`, which remains unaffected: logs `"global count: 5"`.

---

## 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

### Section A: Git Object Store Graph
1.  `blob`
2.  `SHA-1`
3.  `2`
4.  `38`
5.  `tree`

### Section B: UDP Lifecycle Connectionless States
1.  `bind()`
2.  `recvfrom()`
3.  `close()`
4.  `sendto()`
5.  `close()`

---

## 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

### Section A: Android LiveData Threading Boundaries
*   **Threading Constraints:** State mutations of LiveData must occur on the Main thread using `setValue()`. Background threads must post updates using `postValue()`.
*   **Java Background Update Snippet:**
```java
// Background Task Thread
new Thread(() -> {
    int counter = 0;
    while(counter < 10) {
        counter++;
        // Safely update LiveData status asynchronously from worker thread
        liveCounter.postValue(counter);
    }
}).start();
```

### Section B: CSRF Prevention & Token Security
*   **CSRF Definition:** Cross-Site Request Forgery is an exploit where a malicious website triggers a browser to execute unauthorized commands on a target site where the user is currently authenticated (automatically attaching session cookies).
*   **CSRF Token Protection Mechanism:** Standard verification checks cookies, which are automatically sent by the browser. Anti-CSRF verification tokens are random secret strings unique to the user session. The server validates that the Anti-CSRF token is passed inside a custom HTTP header (e.g. `X-CSRF-Token`). Since third-party cross-site scripts cannot access or construct custom headers under Same-Origin Policy (SOP), malicious request attempts are rejected.
