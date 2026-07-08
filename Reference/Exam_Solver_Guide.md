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

#### Detailed Function Breakdown
*   `std::getenv("VARIABLE_NAME")`: Searches the environment list provided by the host environment for a string that matches the name. Returns a pointer to a null-terminated string (`const char*`) on success, or `nullptr` if the variable is not found.
*   `std::stoi(string_var)`: Converts a string representation of a base-10 signed integer into its binary integer value. Throws `std::invalid_argument` if conversion cannot be performed.

#### How to Code This From Scratch
1. Check that `argc` is greater than or equal to `2` before accessing `argv[1]`.
2. Extract `argv[1]` and convert it using `std::stoi()` (or `atoi()`).
3. Query the environment variable using `std::getenv("NAME")`.
4. **Crucial**: Always check if the returned pointer is `nullptr` before wrapping it in `std::string` or using it. Provide a fallback default value.

#### Common Exam Bugs & Gotchas
*   💥 **Null Pointer Crash**: Constructing `std::string s(std::getenv("VAR"))` directly without checking if the environment variable is `nullptr` triggers undefined behavior (usually a Segmentation Fault) if `"VAR"` is unset.
*   💥 **Out-of-Bounds Arguments**: Accessing `argv[1]` before verifying `argc >= 2` causes a crash when running the program without arguments.

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

#### Detailed Function Breakdown
*   `virtual ~Command() = default;`: Ensures that when a derived command is deleted through an interface pointer, the derived destructor is executed (preventing memory leaks).
*   `std::stringstream ss(raw_input)`: Allows parsing string words sequentially using extraction operators (`>>`) just like `std::cin`.
*   `commands.find(commandName)`: Searches the map by key. Returns an iterator to the element on success, or `commands.end()` if the command name is not registered.

#### How to Code This From Scratch
1. Declare an abstract `Command` base class with a pure virtual `execute()` method and a virtual destructor.
2. Implement concrete command classes that override `execute()` and receive a reference to the target **Receiver** class in their constructors.
3. Build the `CliParser` class using a map of `std::string` keys to `std::shared_ptr<Command>` interface objects.
4. Parse user input using `std::stringstream`, lookup the command string in the map, and call `execute()`.

#### Common Exam Bugs & Gotchas
*   💥 **Non-Virtual Interface Destructor**: Forgetting the virtual destructor (`virtual ~Command() = default;`) in the base class. This causes memory leaks since derived destructors are not invoked when deleting base pointers.
*   💥 **Copying Receivers by Value**: Passing the Receiver to the concrete command constructor by value (`Document doc`) instead of by reference (`Document& doc`). This creates a copy of the receiver, leaving the original unchanged.
*   💥 **Dangling Iterators**: Calling `execute()` on `commands[commandName]` directly. If the command does not exist, `operator[]` inserts a default-initialized null pointer into the map, causing a Null Pointer Dereference crash. Use `find()` instead.

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

#### Detailed Function Breakdown
*   `std::lock_guard<std::mutex> lock(mtx)`: A RAII helper that locks the mutex in its constructor and automatically unlocks it when the lock object goes out of scope (e.g. at the end of the function).
*   `std::remove(begin, end, val)`: Rearranges the vector by moving target elements to the end and returns an iterator pointing to the first removed element.
*   `observers.erase(new_end, end)`: Shrinks the vector size to truncate the removed elements.

#### How to Code This From Scratch
1. Declare the `Observer` interface with a virtual `update(...)` function.
2. Inside `Subject`, define a list (vector) of `Observer*` pointers.
3. Protect all list writes/reads (`attach`, `detach`, `notify`) using a `std::mutex`.
4. Iterate over the vector to invoke the update notifications.

#### Common Exam Bugs & Gotchas
*   💥 **Self-Modification Deadlocks**: If an Observer's `update()` method calls `detach(this)` on the Subject, the thread will attempt to acquire `mtx` twice, causing a deadlock.
*   💥 **Missing Mutex Lock on Getters/Notifiers**: Forgetting to lock the mutex in `notify()`. If observers register or unregister from other threads while `notify()` iterates, the vector internal memory will get reallocated, causing a crash.
*   💥 **Dangling Pointers**: If an observer is deleted without calling `detach()`, the subject will hold a dangling pointer, crashing during `notify()`.

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

#### Detailed Function Breakdown
*   `std::unique_lock<std::mutex>`: Unlike `lock_guard`, `unique_lock` can be locked and unlocked manually. This is required by `condition_variable::wait` because it unlocks the mutex when entering sleep and relocks it when waking up.
*   `cv.wait(lock, predicate)`: Blocks the current thread until the condition variable is notified and the predicate returns `true`. This protects against **spurious wakeups** (when a thread wakes up without notification).

#### How to Code This From Scratch
1. Define a template class wrapping a standard `std::queue`.
2. Introduce a `std::mutex` and a `std::condition_variable`.
3. In `push()`, lock the mutex, push the element, and call `cv.notify_one()`.
4. In `pop()`, use `std::unique_lock` (not `lock_guard`) and call `cv.wait()` passing a lambda checks if the queue is not empty (`[this](){ return !q.empty(); }`). Pop and return the element.

#### Common Exam Bugs & Gotchas
*   💥 **Using `std::lock_guard` with Wait**: Compiling `cv.wait()` with `std::lock_guard` fails because the condition variable requires a `unique_lock` signature to manage lock release states.
*   💥 **Spurious Wakeup Vulnerability**: Calling `cv.wait(lock)` without a loop or predicate. A thread might wake up when the queue is still empty, and calling `q.front()` on an empty queue causes undefined behavior.
*   💥 **Missing Notification**: Forgetting to call `cv.notify_one()` in `push()`. Consumer threads will remain asleep forever.

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

#### Detailed Function Breakdown
*   `htons(port)` / `ntohs(val)`: Converts 16-bit integers from host byte order to network byte order (Big Endian) and vice-versa.
*   `setsockopt(..., SO_REUSEADDR, ...)`: Modifies the socket level parameters to permit binding to a port that might be in a temporary `TIME_WAIT` socket state.
*   `accept(...)`: Blocks the executing thread until a TCP handshake from a client is received, returning a new file descriptor (`client_fd`) dedicated to communicating with this client.

#### How to Code This From Scratch
1. Create a socket descriptor (`AF_INET`, `SOCK_STREAM` for TCP or `SOCK_DGRAM` for UDP).
2. Configure `SO_REUSEADDR` to avoid binding lockups.
3. Construct `sockaddr_in` structure. Set family (`AF_INET`), address (`INADDR_ANY`), and port using `htons()`.
4. Bind the socket and call `listen()` (TCP only).
5. For TCP, call `accept()` in a loop. Perform `read()` and `write()`, and close the client socket.
6. For UDP, call `recvfrom()` to read incoming messages and get client addresses, then call `sendto()` to reply.

#### Common Exam Bugs & Gotchas
*   💥 **Missing htons Conversion**: Setting `addr.sin_port = port;` directly without `htons()`. On Little Endian systems (x86/x64), the bytes will be swapped, causing the server to bind to a completely different port.
*   💥 **Leaking Socket Descriptors**: Forgetting to call `close(client_fd)` at the end of the client handling block. This leaks file descriptors. Eventually, the server will exceed the operating system limits and refuse new connections.
*   💥 **Mixing TCP and UDP types**: Passing `SOCK_STREAM` when configuring UDP or `SOCK_DGRAM` for TCP, leading to protocol negotiation failures.

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

#### `routes/task.js` (Modular Router)
```javascript
const express = require('express');
const router = express.Router();
const taskController = require('../controllers/taskController');

// Route chaining delegates requests to controller functions
router.route('/')
    .get(taskController.getTasks)
    .post(taskController.createTask);

router.route('/:id')
    .delete(taskController.deleteTask);

module.exports = router;
```

#### `controllers/taskController.js` (Controllers - Business Logic Callbacks)
```javascript
const Task = require('../models/Task');

// GET /api/tasks
const getTasks = async (req, res) => {
    try {
        const tasks = await Task.find();
        res.status(200).json(tasks);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};

// POST /api/tasks
const createTask = async (req, res) => {
    try {
        const newTask = new Task(req.body);
        await newTask.save();
        res.status(201).json(newTask);
    } catch (err) {
        res.status(400).json({ error: err.message });
    }
};

// DELETE /api/tasks/:id
const deleteTask = async (req, res) => {
    try {
        const task = await Task.findByIdAndDelete(req.params.id);
        if (!task) return res.status(404).json({ error: "Task not found" });
        res.status(204).send(); // 204 No Content
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};

module.exports = { getTasks, createTask, deleteTask };
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

#### Detailed Function Breakdown
*   `express.json()`: Built-in middleware function in Express. It parses incoming requests with JSON payloads and makes the parsed data available under the `req.body` property.
*   `router.route(path)`: Returns an instance of a single route, which can be used to handle HTTP verbs with optional middleware chain configurations.
*   `Task.findByIdAndDelete(id)`: Issues a MongoDB findAndModify remove command. Returns the deleted document.

#### How to Code This From Scratch
1. Configure `app.js`: import Express, Mongoose, CORS, and mount the JSON parser and route modules.
2. In model files, declare schemas using `new mongoose.Schema({...})` and export models using `mongoose.model('Name', schema)`.
3. In router files, instantiate `express.Router()`. Use `.route('/')` to group GET and POST endpoints, and `.route('/:id')` for operations requiring an ID.
4. Extract parameters: use `req.params.id` for URL parameters (e.g. `/:id`) and `req.body` for POST/PUT payloads.
5. Export the router using `module.exports = router` and mount it in `app.js` with `app.use('/prefix', router)`.

#### Common Exam Bugs & Gotchas
*   💥 **Missing `express.json()` parser**: Forgetting to mount `app.use(express.json())`. This causes `req.body` to be `undefined`, which will crash your server when you try to read properties from it.
*   💥 **Missing `await` on Database Operations**: Calling `Task.find()` or `task.save()` without the `await` keyword. Mongoose calls return promises. If you do not await them, the code continues executing, and you will send a pending promise (or empty response) back to the client.
*   💥 **Duplicate Endpoint Registrations**: Declaring two conflicting routes, such as `router.get('/')` followed by another `router.get('/')` or a generic `router.get('/:id')` before a specific one like `router.get('/search')`. Express matches routes in the order they are defined.

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

#### Detailed Function Breakdown
*   `useEffect(callback, deps)`: Runs side effects. If the dependency array `deps` is empty (`[]`), the callback runs exactly once when the component mounts.
*   `setTasks([...tasks, newTask])`: Creates a new array containing all elements of `tasks` plus `newTask`, trigger a shallow reference difference to force a UI re-render.
*   `tasks.filter(...)`: Creates a new array containing only the elements that pass the predicate check.

#### How to Code This From Scratch
1. Declare the component state variables (`useState`) for list arrays and input fields.
2. Use `useEffect` with a `[]` dependency array to fetch initial data on mount.
3. Build forms with `onSubmit={handleSubmit}` and bind inputs to state with `value={text}` and `onChange={e => setText(e.target.value)}`.
4. In event handlers, call `e.preventDefault()` to stop page reloads.
5. Perform `fetch()` calls. Upon receipt of successful responses, update the list state **immutably** using the spread operator (`[...prev, item]`) or `filter()`.

#### Common Exam Bugs & Gotchas
*   💥 **Direct State Mutations**: Mutating arrays directly (e.g. `tasks.push(item)` or `tasks[index] = val`) and calling `setTasks(tasks)`. Since the array reference hasn't changed, React will skip re-rendering.
*   💥 **Infinite Fetch Loops**: Forgetting the dependency array `[]` in `useEffect`. This causes the effect to run on every single render. Since the fetch callback updates state, it triggers a render, which triggers another fetch, causing an infinite loop.
*   💥 **Missing POST headers**: Forgetting to set `'Content-Type': 'application/json'` on fetch POST requests. This causes Express to ignore the body payload, leaving `req.body` empty.

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

#### Detailed Function Breakdown
*   `document.createElement(tagName)`: Creates the specified HTML element.
*   `element.remove()`: Removes the element from the DOM tree.
*   `e.preventDefault()`: Prevents the default browser action for the event (such as reloading the page on form submission).

#### How to Code This From Scratch
1. Wrap all initialization code inside a `DOMContentLoaded` event listener.
2. Bind action handlers directly to DOM elements using `addEventListener`.
3. Retrieve data using `fetch()`. Iterate through data arrays using `.forEach()`.
4. Define a helper function `addItemToDom(data)` that creates container nodes, inserts HTML templates with placeholders, binds nested event listeners, and appends the finished node to the DOM.

#### Common Exam Bugs & Gotchas
*   💥 **Destructive `innerHTML` Concatenation**: Appending elements using `list.innerHTML += newTemplate`. This serializes and parses the entire container DOM tree, which destroys all active event listeners on existing items.
*   💥 **Missing Form Default Prevention**: Forgetting `e.preventDefault()` inside submit handlers. The browser will reload the page immediately, wiping out the application state.

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

#### Detailed Function Breakdown
*   `<FlatList>`: High performance list view wrapper that takes an array (`data`), a key generator (`keyExtractor`), and a renderer callback (`renderItem`).
*   `onChangeText`: Triggered when the text input changes, passing the new text directly to the callback function (unlike web React's `onChange`, which passes an event object).

#### How to Code This From Scratch
1. Import primitive mobile components from `react-native` (`View`, `Text`, `TextInput`, `TouchableOpacity`, `FlatList`, `StyleSheet`).
2. Manage input text state using `useState` and bind it to the `TextInput` using `value` and `onChangeText`.
3. In render sections, wrap all text strings inside `<Text>` tags. Use `<View>` tags for layouts.
4. Pass data arrays to `<FlatList>` and style layout nodes using `StyleSheet.create`.

#### Common Exam Bugs & Gotchas
*   💥 **Unwrapped Text Strings**: Writing raw text strings directly inside `<View>` elements. Unlike web browsers, this causes the mobile app to crash immediately. All strings must be wrapped in `<Text>`.
*   💥 **Missing Key Extractor String Conversion**: Forgetting to convert keys to strings in `keyExtractor`. React Native expects keys to be strings. Passing numbers can cause layout warning flags or rendering issues.

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

#### Detailed Function Breakdown
*   `ViewModelProvider(this).get(MainViewModel.class)`: Returns an instance of the requested `ViewModel` class, ensuring that the same instance is returned as long as the host activity's lifecycle is active.
*   `liveData.observe(lifecycleOwner, observer)`: Registers an observer to receive updates when the value of the `LiveData` changes. The observer is automatically cleaned up when the activity is destroyed.

#### How to Code This From Scratch
1. Design the UI structure in the `XML Layout` file. Make sure all interactive components have unique `@+id` identifiers.
2. In the `Repository` class, write functions that update a `MutableLiveData` reference passed as an argument.
3. In the `ViewModel` class, declare private `MutableLiveData` properties and expose them as read-only `LiveData` using a getter function.
4. In `MainActivity`, instantiate the `ViewModel` using `ViewModelProvider`.
5. Call `.observe()` on the `LiveData` to update the UI when the data changes, and bind click listeners to buttons to pass inputs to the `ViewModel`.

#### Common Exam Bugs & Gotchas
*   💥 **Network / DB Queries on Main Thread**: Performing network calls or database operations directly on the main UI thread. Android will crash the application immediately with a `NetworkOnMainThreadException`.
*   💥 **Updating LiveData from Background Threads**: Calling `liveData.setValue(value)` from a background worker thread. To update LiveData from a background thread, you must call `liveData.postValue(value)` instead.

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
    server.setsockopt(socket.SOL_SOCKET, SO_REUSEADDR, 1)
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

#### Detailed Function Breakdown
*   `socket.socket(socket.AF_INET, socket.SOCK_STREAM)`: Configures a network socket. `AF_INET` sets the IPv4 protocol, and `SOCK_STREAM` sets the TCP transport protocol.
*   `t.daemon = True`: Configures the thread as a background worker. This ensures the thread is terminated automatically when the main server process exits, preventing dangling processes.
*   `client.sendall(data)`: Unlike `send()`, `sendall()` continues sending data until all bytes have been transmitted or an error occurs.

#### How to Code This From Scratch
1. Instantiate a TCP socket using `socket.socket()`.
2. Configure `SO_REUSEADDR` to prevent port binding lockups on restart.
3. Bind the socket to `("0.0.0.0", port)` and call `.listen()`.
4. In a loop, call `.accept()` to get incoming client sockets.
5. Create a new thread using `threading.Thread(target=handler, args=(client,))`, set it as a daemon thread, and start it.

#### Common Exam Bugs & Gotchas
*   💥 **Missing daemon Thread Configurations**: Spawning worker threads without setting `t.daemon = True`. If the main program terminates, these worker threads will remain active, preventing the script from exiting.
*   💥 **Missing Network Stream Decoding**: Attempting to read or write raw socket data without encoding/decoding. Network sockets transmit raw bytes, so you must call `.decode('utf-8')` after receiving data, and `.encode('utf-8')` before sending.

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

#### Detailed Function Breakdown
*   `ServerSocket.accept()`: Listens for a connection to be made to this socket and accepts it. The method blocks until a connection is established.
*   `PrintWriter(outStream, true)`: The second argument `true` enables **auto-flush** mode. This means that calling `println()` will automatically flush the output buffer, sending the data to the client immediately.

#### How to Code This From Scratch
1. Create a `ServerSocket` inside a try-with-resources block to ensure it is closed automatically.
2. In a loop, call `serverSocket.accept()` to get incoming client sockets.
3. Spawn a new thread using `new Thread(() -> handler(socket)).start()`.
4. Wrap socket input and output streams in `BufferedReader` and `PrintWriter` helpers.
5. Read lines using `in.readLine()` until it returns `null` (indicating the client disconnected), and write replies using `out.println()`.

#### Common Exam Bugs & Gotchas
*   💥 **Missing Auto-Flush configuration**: Forgetting to pass the `true` auto-flush flag to `PrintWriter`. If auto-flush is disabled, data will be buffered in memory and won't be sent until the buffer fills up or the socket is closed, leaving the client waiting forever.
*   💥 **Missing Socket Close in finally block**: Forgetting to close the client socket in a `finally` block. If client sockets are not closed, the server will eventually leak resources and refuse new connections.
