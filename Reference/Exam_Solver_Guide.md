# Mitkadem Exam Solver's Reference Guide & Boilerplates

This reference guide contains production-ready code boilerplates, design patterns, and architecture explanations tailored to solve the programming questions commonly found in BIU Advanced Systems Programming (893210) exams.

---
---

## 1. C++ Systems Programming

### A. CLI Arguments & Environment Variables
In systems programming, reading parameters at startup is a key requirement. This boilerplate demonstrates parsing CLI arguments and reading environment variables.

```cpp
#include <iostream>
#include <string>
#include <cstdlib> // Required for std::getenv

int main(int argc, char* argv[]) {
    // 1. Parsing command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }
    // Convert string parameter to integer
    int port = std::stoi(argv[1]);

    // 2. Reading environment variables
    const char* env_val = std::getenv("CONNECTION_STRING");
    std::string conn_str = env_val ? std::string(env_val) : "mongodb://localhost:27017/default";

    std::cout << "Starting server on port " << port << " using DB: " << conn_str << std::endl;
    return 0;
}
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Standard library `argc`/`argv` parsing and `std::getenv`.
    *   *Option B*: Command line libraries like `getopt` or `boost::program_options`.
*   **Why Option A was chosen**:
    It is standard, lightweight, has no external dependencies, compiles instantly on any environment, and matches the strict C++ standard expected in academic exams.
*   **Alternatives**:
    Using `getopt` is the classic C-style approach but adds syntax complexity. Using Boost or third-party libraries is generally prohibited in exams due to compilation environment constraints.

---
---

### B. Command Pattern (CLI Invoker)
The Command Pattern is used to implement extensibility (Open-Closed Principle). It decouples the object that invokes a command (e.g., CLI parser) from the object that knows how to perform it.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

// Receiver - The object that holds the actual business logic
class Document {
public:
    void save() { std::cout << "Saving file..." << std::endl; }
};

// Command Interface - The common contract for all command actions
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

// Concrete Command - Binds a receiver action to an execute call
class SaveCommand : public Command {
private:
    Document& doc;
public:
    SaveCommand(Document& d) : doc(d) {}
    void execute() override { doc.save(); }
};

// Invoker & Parser - Maps CLI text input to executable commands
class CliParser {
private:
    std::map<std::string, std::shared_ptr<Command>> commands;
public:
    void registerCommand(const std::string& name, std::shared_ptr<Command> cmd) {
        commands[name] = cmd;
    }
    
    void run(const std::string& raw_input) {
        std::stringstream ss(raw_input);
        std::string commandName;
        ss >> commandName;
        
        auto it = commands.find(commandName);
        if (it != commands.end()) {
            it->second->execute(); // Polymorphic invocation
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }
};
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Command Pattern mapping string keys to command interface classes.
    *   *Option B*: Large nested `if-else` or `switch` blocks checking string contents.
*   **Why Option A was chosen**:
    It satisfies the SOLID principles (specifically the **Open-Closed Principle**). Adding a new command (e.g., `DeleteCommand`) does not require editing the `CliParser` class; you simply write a new class and register it at runtime.
*   **Alternatives**:
    Using `if-else` blocks is quick to write but makes the parsing loop highly fragile, hard to test, and tightly coupled to the system receivers.

---
---

### C. Observer Pattern (Thread-Safe)
The Observer Pattern defines a one-to-many dependency between objects so that when one object changes state, all its dependents are notified automatically.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>

// Observer Interface
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

// Subject - The state holder that notifies subscribers
class Subject {
private:
    std::vector<Observer*> observers;
    mutable std::mutex mtx; // Protects observers registration array
public:
    void attach(Observer* obs) {
        std::lock_guard<std::mutex> lock(mtx);
        observers.push_back(obs);
    }
    
    void detach(Observer* obs) {
        std::lock_guard<std::mutex> lock(mtx);
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }
    
    void notify(const std::string& message) const {
        std::lock_guard<std::mutex> lock(mtx);
        for (Observer* obs : observers) {
            if (obs) obs->update(message);
        }
    }
};
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Thread-safe `std::mutex` wrapped observer list.
    *   *Option B*: Simple raw vector pointer arrays without synchronization.
*   **Why Option A was chosen**:
    In systems programming exams, observers are often registered or notified from different threads (e.g., worker socket threads). Using `std::mutex` and `std::lock_guard` prevents race conditions or segmentation faults if one thread attaches/detaches an observer while another triggers notifications.
*   **Alternatives**:
    Unsynchronized lists work fine for single-threaded CLI tasks, but fail instantly under modern asynchronous or thread-pool designs.

---
---

### D. Concurrency & Synchronizers (Producer-Consumer Queue)
This implementation demonstrates how to manage thread coordination using condition variables, preventing CPU resource waste from polling.

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// Thread-Safe Queue (Producer-Consumer Pattern)
template <typename T>
class SafeQueue {
private:
    std::queue<T> q;
    mutable std::mutex mtx;
    std::condition_variable cv;
public:
    void push(T val) {
        std::unique_lock<std::mutex> lock(mtx);
        q.push(val);
        cv.notify_one(); // Wake up one waiting consumer thread
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until queue has items (releases lock while waiting, re-acquires it when woken up)
        cv.wait(lock, [this]() { return !q.empty(); });
        T val = q.front();
        q.pop();
        return val;
    }
};
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Mutex and `std::condition_variable` waiting predicate block.
    *   *Option B*: Spinning in a `while(q.empty()) { std::this_thread::sleep_for(...) }` loop.
*   **Why Option A was chosen**:
    It guarantees zero CPU overhead when the queue is empty. The consumer thread goes to sleep and is woken up instantly by the OS scheduler when the producer pushes data.
*   **Alternatives**:
    Busy-waiting or periodic sleep calls degrade application performance, introduce execution delays, and waste system execution cycles.

---
---

### E. POSIX Sockets (TCP Server & UDP Sockets)

#### 1. TCP Server (POSIX Sockets)
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void runTcpServer(int port) {
    // 1. Create TCP socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    // Set socket options to reuse address/port (prevents 'Address already in use' errors)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections on any interface
    address.sin_port = htons(port);

    // 2. Bind and Listen
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    while (true) {
        socklen_t len = sizeof(address);
        // 3. Block and Accept incoming connections
        int client_fd = accept(server_fd, (struct sockaddr*)&address, &len);
        if (client_fd >= 0) {
            char buffer[1024] = {0};
            read(client_fd, buffer, 1024);
            std::cout << "Received: " << buffer << std::endl;
            const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
            write(client_fd, response, strlen(response));
            close(client_fd);
        }
    }
    close(server_fd);
}
```

#### 2. UDP Client-Server
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// Server
void runUdpServer(int port) {
    // Note the use of SOCK_DGRAM for UDP
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_addr{}, client_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);
    // Receive message and record client address/port inside client_addr
    int bytes = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_len);
    buffer[bytes] = '\0';
    
    // Echo back using client_addr populated by recvfrom
    sendto(sock, buffer, bytes, 0, (struct sockaddr*)&client_addr, addr_len);
    close(sock);
}
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Blocking sockets with `SO_REUSEADDR`.
    *   *Option B*: Non-blocking sockets using `select()`, `poll()`, or `epoll()`.
*   **Why Option A was chosen**:
    Exams typically ask for simple, straightforward network communication. Blocking sockets are easier to write and debug under pressure. Setting `SO_REUSEADDR` is essential to prevent testing failures when restarting servers quickly.
*   **Alternatives**:
    Non-blocking socket models are required for high-concurrency production software, but add significant event-loop boilerplate that is prone to bugs during exams.

---
---
---

## 2. Web MVC Architecture (Node, Express, Mongo, React)

### A. Modular Express Backend + Mongoose REST API

#### `models/Task.js` (Mongoose Model)
```javascript
const mongoose = require('mongoose');

const taskSchema = new mongoose.Schema({
    text: { type: String, required: true },
    completed: { type: Boolean, default: false }
});

module.exports = mongoose.model('Task', taskSchema);
```

#### `routes/task.js` (Modular Router with Chained Verbs)
```javascript
const express = require('express');
const router = express.Router();
const Task = require('../models/Task');

// Route chaining simplifies code by grouping handlers by path
router.route('/')
    .get(async (req, res) => {
        try {
            const tasks = await Task.find();
            res.json(tasks);
        } catch (err) {
            res.status(500).json({ error: err.message });
        }
    })
    .post(async (req, res) => {
        try {
            const newTask = new Task(req.body);
            await newTask.save();
            res.status(201).json(newTask);
        } catch (err) {
            res.status(400).json({ error: err.message });
        }
    });

router.route('/:id')
    .delete(async (req, res) => {
        try {
            const task = await Task.findByIdAndDelete(req.params.id);
            if (!task) return res.status(404).json({ error: "Task not found" });
            res.status(204).send();
        } catch (err) {
            res.status(500).json({ error: err.message });
        }
    });

module.exports = router;
```

#### `app.js` (Main Server)
```javascript
const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');
const taskRoutes = require('./routes/task');

const app = express();

app.use(cors());
app.use(express.json()); // Essential: enables req.body JSON parsing

app.use('/api/tasks', taskRoutes);

mongoose.connect('mongodb://localhost:27017/todo_db')
    .then(() => app.listen(3000, () => console.log('Server running on port 3000')));
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Chained route endpoints (`router.route()`) inside dedicated router modules.
    *   *Option B*: Flat routing directly on `app` (e.g., `app.get('/api/tasks')`) inside a single large `app.js` file.
*   **Why Option A was chosen**:
    It adheres to the MVC architectural separation of concerns. Using separate files for models, controllers/routes, and setup prevents compilation or logic clutter.
*   **Alternatives**:
    Placing all code inside a single `app.js` works for tiny apps but violates the modular architecture patterns expected in modern software exams.

---
---

### B. React Frontend UI (State updates & API requests)

#### `TaskList.jsx`
```jsx
import React, { useState, useEffect } from 'react';
import './TaskList.css';

export default function TaskList() {
    const [tasks, setTasks] = useState([]);
    const [newText, setNewText] = useState('');

    useEffect(() => {
        fetch('/api/tasks')
            .then(res => res.json())
            .then(data => setTasks(data));
    }, []);

    const handleAddTask = async (e) => {
        e.preventDefault();
        if (!newText.trim()) return;

        const res = await fetch('/api/tasks', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ text: newText })
        });

        if (res.ok) {
            const addedTask = await res.json();
            // Immutable State Update - creates a new array reference
            setTasks([...tasks, addedTask]);
            setNewText('');
        }
    };

    const handleDeleteTask = async (id) => {
        const res = await fetch(`/api/tasks/${id}`, { method: 'DELETE' });
        if (res.ok) {
            // Immutable Delete - filters items into a new array
            setTasks(tasks.filter(t => t._id !== id));
        }
    };

    return (
        <div className="task-container">
            <form onSubmit={handleAddTask}>
                <input value={newText} onChange={e => setNewText(e.target.value)} />
                <button type="submit">Add Task</button>
            </form>
            <ul className="task-list">
                {tasks.map(t => (
                    <li key={t._id} className="task-item">
                        <span>{t.text}</span>
                        <button onClick={() => handleDeleteTask(t._id)}>Delete</button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Immutable array operations (`[...tasks, newTask]`, `tasks.filter()`) inside `useState`.
    *   *Option B*: Mutating the existing state directly (e.g. `tasks.push(newTask)` followed by `setTasks(tasks)`).
*   **Why Option A was chosen**:
    React relies on **shallow reference equality** checks to detect state changes. If you mutate the array directly, the array reference (`tasks`) remains the same, and React will not trigger a re-render.
*   **Alternatives**:
    Deep copying objects (`JSON.parse(JSON.stringify(x))`) works but is slow. Functional state updates are the standard way to update state in React.

---
---

### C. Vanilla Web Implementation (No React)
When React is not requested, using Vanilla JS with structured DOM APIs is the standard approach.

```javascript
document.addEventListener("DOMContentLoaded", () => {
    const taskForm = document.getElementById("task-form");
    const taskInput = document.getElementById("task-input");
    const taskList = document.getElementById("task-list");

    // Fetch initial list
    fetch('/api/tasks')
        .then(res => res.json())
        .then(tasks => tasks.forEach(addTaskToDom));

    taskForm.addEventListener("submit", async (e) => {
        e.preventDefault();
        const text = taskInput.value.trim();

        const res = await fetch('/api/tasks', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ text })
        });

        if (res.ok) {
            const newTask = await res.json();
            addTaskToDom(newTask);
            taskInput.value = '';
        }
    });

    function addTaskToDom(task) {
        const li = document.createElement("li");
        li.className = "task-item";
        li.innerHTML = `
            <span>${task.text}</span>
            <button class="delete-btn">Delete</button>
        `;
        
        li.querySelector(".delete-btn").addEventListener("click", async () => {
            const delRes = await fetch(`/api/tasks/${task._id}`, { method: 'DELETE' });
            if (delRes.ok) {
                li.remove(); // Remove element directly from DOM
            }
        });

        taskList.appendChild(li);
    }
});
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Constructing elements using `document.createElement`, inserting HTML, and using query selectors for scope-safe event bindings.
    *   *Option B*: Appending large raw string templates to `innerHTML` and using global event listeners.
*   **Why Option A was chosen**:
    It is secure and prevents performance degradation. Rewriting `innerHTML` on parent containers destroys existing DOM nodes and breaks active event listeners. Creating elements individually keeps event bindings scoped.
*   **Alternatives**:
    Using `document.write` or legacy libraries (like jQuery) is outdated and generally discouraged.

---
---
---

## 3. Mobile Development

### A. React Native (Scroll Optimization)
```javascript
import React, { useState } from 'react';
import { StyleSheet, View, Text, TextInput, TouchableOpacity, FlatList } from 'react-native';

export default function MobileTodo() {
    const [tasks, setTasks] = useState([]);
    const [input, setInput] = useState('');

    const handleAdd = () => {
        if (!input.trim()) return;
        setTasks([...tasks, { id: Date.now().toString(), text: input }]);
        setInput('');
    };

    return (
        <View style={styles.container}>
            <View style={styles.form}>
                <TextInput style={styles.input} value={input} onChangeText={setInput} placeholder="New task..." />
                <TouchableOpacity style={styles.button} onPress={handleAdd}>
                    <Text style={styles.buttonText}>Add</Text>
                </TouchableOpacity>
            </View>
            <FlatList
                data={tasks}
                keyExtractor={item => item.id}
                renderItem={({ item }) => (
                    <View style={styles.taskItem}>
                        <Text style={styles.taskText}>{item.text}</Text>
                    </View>
                )}
            />
        </View>
    );
}

const styles = StyleSheet.create({
    container: { flex: 1, padding: 20, backgroundColor: '#fff' },
    form: { flexDirection: 'row', marginBottom: 20 },
    input: { flex: 1, borderWidth: 1, borderColor: '#ccc', borderRadius: 4, padding: 10 },
    button: { marginLeft: 10, backgroundColor: '#007BFF', justifyContent: 'center', paddingHorizontal: 20, borderRadius: 4 },
    buttonText: { color: 'white', fontWeight: 'bold' },
    taskItem: { padding: 15, borderBottomWidth: 1, borderBottomColor: '#eee' },
    taskText: { fontSize: 16 }
});
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Using `FlatList` to render the list.
    *   *Option B*: Using a `ScrollView` and mapping through arrays using `.map()`.
*   **Why Option A was chosen**:
    `FlatList` optimizes memory and CPU usage by rendering items lazily as they enter the screen viewport. This is crucial for long lists.
*   **Alternatives**:
    `ScrollView` loads all list items into memory immediately on start, which can degrade performance when dealing with large lists.

---
---

### B. Android Java (MVVM Architecture)

#### 1. Layout: `activity_main.xml`
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:padding="16dp">

    <EditText
        android:id="@+id/et_input"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:hint="Enter text" />

    <Button
        android:id="@+id/btn_submit"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Submit" />

    <TextView
        android:id="@+id/tv_display"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:textSize="18sp"
        android:layout_marginTop="20dp" />

</LinearLayout>
```

#### 2. ViewModel & Repository Java bindings
```java
// MainViewModel.java
package com.example.mvvm;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class MainViewModel extends ViewModel {
    private final MutableLiveData<String> liveResult = new MutableLiveData<>("");
    private final DataRepository repository = new DataRepository();

    public LiveData<String> getResult() {
        return liveResult; // Returns read-only LiveData to view
    }

    public void submitData(String input) {
        repository.processData(input, liveResult);
    }
}
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: MVVM architecture with `LiveData` observers.
    *   *Option B*: Handling database logic directly inside `MainActivity`.
*   **Why Option A was chosen**:
    Using MVVM prevents data loss on device rotation (like screen flips). Since the `ViewModel` lives longer than the `Activity`, UI state is preserved automatically.
*   **Alternatives**:
    Writing business logic directly inside activities makes testing hard, creates massive bloated classes (God objects), and causes memory leaks.

---
---
---

## 4. Python Network Socket Programming

### A. Concurrent TCP Socket Server
```python
import socket
import threading

def handle_client(client_socket):
    try:
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
            msg = data.decode('utf-8').strip()
            print("Received:", msg)
            response = f"Echo: {msg}\n"
            client_socket.sendall(response.encode('utf-8'))
    finally:
        client_socket.close()

def run_server(port):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("0.0.0.0", port))
    server.listen(5)
    print(f"Server listening on port {port}...")

    try:
        while True:
            client, addr = server.accept()
            print(f"Accepted connection from {addr}")
            # Spawn a thread per client for concurrency
            t = threading.Thread(target=handle_client, args=(client,))
            t.daemon = True # Allows server exit without waiting for threads
            t.start()
    finally:
        server.close()

if __name__ == "__main__":
    run_server(5000)
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Multi-threaded (`threading.Thread`) server.
    *   *Option B*: Asynchronous programming using `asyncio` or `selectors`.
*   **Why Option A was chosen**:
    Multi-threading is straightforward to implement and debug in exams. Making the threads **daemon** threads ensures they are cleaned up automatically when the main server exits.
*   **Alternatives**:
    `asyncio` is highly efficient but adds syntax complexity (async/await loops) that can be difficult to manage under exam time constraints.

---
---
---

## 5. Java Systems & Concurrency

### A. Java TCP Server (Thread-Per-Client Boilerplate)
```java
import java.io.*;
import java.net.*;

public class JavaTcpServer {
    public static void main(String[] args) {
        int port = 8080;
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Java TCP Server listening on port " + port);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                // Spawn handler thread for concurrency
                new Thread(() -> handleClient(clientSocket)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void handleClient(Socket clientSocket) {
        try (
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)
        ) {
            String line;
            while ((line = in.readLine()) != null) {
                if (line.equalsIgnoreCase("exit")) break;
                out.println("Java Echo: " + line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```

#### Architecture Options & Rationale
*   **Design Options**:
    *   *Option A (Chosen)*: Classic Thread-per-client model.
    *   *Option B*: Non-blocking IO using `java.nio` selectors.
*   **Why Option A was chosen**:
    It is standard, highly readable, and the default paradigm expected in Java systems exams.
*   **Alternatives**:
    Java NIO channels are powerful for high performance, but the syntax is verbose and prone to configuration bugs during exams.
