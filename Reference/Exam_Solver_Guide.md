# Mitkadem Exam Solver's Reference Guide & Boilerplates

This reference guide contains production-ready code boilerplates and design templates tailored to solve the coding questions commonly found in BIU Advanced Systems Programming (893210) exams.

---

## 1. C++ Systems Programming

### A. CLI Arguments & Environment Variables
```cpp
#include <iostream>
#include <string>
#include <cstdlib> // std::getenv

int main(int argc, char* argv[]) {
    // 1. Parsing command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);

    // 2. Reading environment variables
    const char* env_val = std::getenv("CONNECTION_STRING");
    std::string conn_str = env_val ? std::string(env_val) : "mongodb://localhost:27017/default";

    std::cout << "Starting server on port " << port << " using DB: " << conn_str << std::endl;
    return 0;
}
```

### B. Command Pattern (CLI Invoker)
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

// Receiver
class Document {
public:
    void save() { std::cout << "Saving file..." << std::endl; }
};

// Command Interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

// Concrete Commands
class SaveCommand : public Command {
private:
    Document& doc;
public:
    SaveCommand(Document& d) : doc(d) {}
    void execute() override { doc.save(); }
};

// Invoker & Parser
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
            it->second->execute();
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }
};
```

### C. Observer Pattern (Thread-Safe)
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class Subject {
private:
    std::vector<Observer*> observers;
    mutable std::mutex mtx; // Protects observers list
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

### D. Concurrency & Synchronizers
```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// Thread-Safe Queue (Producer-Consumer)
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
        cv.notify_one(); // Wake up consumer
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !q.empty(); });
        T val = q.front();
        q.pop();
        return val;
    }
};
```

### E. POSIX Networking (TCP Server & UDP Client-Server)

#### 1. TCP Server (POSIX Sockets)
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void runTcpServer(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    while (true) {
        socklen_t len = sizeof(address);
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
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_addr{}, client_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);
    int bytes = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_len);
    buffer[bytes] = '\0';
    
    // Echo back
    sendto(sock, buffer, bytes, 0, (struct sockaddr*)&client_addr, addr_len);
    close(sock);
}

// Client
void sendUdpMessage(const std::string& ip, int port, const std::string& msg) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    sendto(sock, msg.c_str(), msg.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    close(sock);
}
```

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
const Task = require('../models/Task');

// Chained handlers for "/"
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

// Chained handlers for "/:id"
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
app.use(express.json()); // Parses application/json

app.use('/api/tasks', taskRoutes);

mongoose.connect('mongodb://localhost:27017/todo_db')
    .then(() => app.listen(3000, () => console.log('Server running on port 3000')));
```

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
            // Immutable State Update
            setTasks([...tasks, addedTask]);
            setNewText('');
        }
    };

    const handleDeleteTask = async (id) => {
        const res = await fetch(`/api/tasks/${id}`, { method: 'DELETE' });
        if (res.ok) {
            // Immutable Delete
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

---

### C. Vanilla Web Implementation (No React)

#### `index.html`
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Vanilla TODO</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="task-container">
        <form id="task-form">
            <input type="text" id="task-input" required>
            <button type="submit">Add Task</button>
        </form>
        <ul id="task-list" class="task-list"></ul>
    </div>
    <script src="script.js"></script>
</body>
</html>
```

#### `script.js`
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
        li.dataset.id = task._id;
        li.innerHTML = `
            <span>${task.text}</span>
            <button class="delete-btn">Delete</button>
        `;
        
        li.querySelector(".delete-btn").addEventListener("click", async () => {
            const delRes = await fetch(`/api/tasks/${task._id}`, { method: 'DELETE' });
            if (delRes.ok) {
                li.remove();
            }
        });

        taskList.appendChild(li);
    }
});
```

---

## 3. Mobile Development

### A. React Native (Flexbox & Scrolling)
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

#### 2. Repository: `DataRepository.java`
```java
package com.example.mvvm;
import androidx.lifecycle.MutableLiveData;

public class DataRepository {
    // Simulated database query/update
    public void processData(String input, MutableLiveData<String> target) {
        target.setValue("Processed: " + input);
    }
}
```

#### 3. ViewModel: `MainViewModel.java`
```java
package com.example.mvvm;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class MainViewModel extends ViewModel {
    private final MutableLiveData<String> liveResult = new MutableLiveData<>("");
    private final DataRepository repository = new DataRepository();

    public LiveData<String> getResult() {
        return liveResult;
    }

    public void submitData(String input) {
        repository.processData(input, liveResult);
    }
}
```

#### 4. View: `MainActivity.java`
```java
package com.example.mvvm;

import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

public class MainActivity extends AppCompatActivity {
    private MainViewModel viewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText etInput = findViewById(R.id.et_input);
        Button btnSubmit = findViewById(R.id.btn_submit);
        TextView tvDisplay = findViewById(R.id.tv_display);

        // Bind MainViewModel
        viewModel = new ViewModelProvider(this).get(MainViewModel.class);

        // Observe ViewModel live data changes
        viewModel.getResult().observe(this, newText -> {
            tvDisplay.setText(newText);
        });

        // Set click listener to trigger viewmodel actions
        btnSubmit.setOnClickListener(v -> {
            String inputText = etInput.getText().toString();
            viewModel.submitData(inputText);
        });
    }
}
```

---

## 4. Python Network Socket Programming

### A. Persistent TCP Socket Client
```python
import sys
import socket

def run_client():
    if len(sys.argv) < 3:
        print("Usage: python client.py [IP] [PORT]")
        return

    ip = sys.argv[1]
    port = int(sys.argv[2])

    # Establish TCP Socket connection
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((ip, port))
    
    print(f"Connected to remote server at {ip}:{port}")
    try:
        while True:
            msg = input("Send > ")
            if msg.strip().lower() == "exit":
                break
            
            client_socket.sendall((msg + "\n").encode('utf-8'))
            response = client_socket.recv(1024).decode('utf-8')
            print("Received:", response.strip())
    finally:
        client_socket.close()
        print("Disconnected.")

if __name__ == "__main__":
    run_client()
```

### B. Simple Concurrent TCP Socket Server
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
            t = threading.Thread(target=handle_client, args=(client,))
            t.daemon = True
            t.start()
    finally:
        server.close()

if __name__ == "__main__":
    run_server(5000)
```

---

## 5. Java Systems & Concurrency

### A. Java TCP Server (Thread-Per-Client)
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

### B. Java Concurrency & Locks (ReentrantLock)
```java
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class SafeCounter {
    private int count = 0;
    private final Lock lock = new ReentrantLock();

    public void increment() {
        lock.lock();
        try {
            count++;
        } finally {
            lock.unlock(); // Ensure lock release
        }
    }

    public int getCount() {
        lock.lock();
        try {
            return count;
        } finally {
            lock.unlock();
        }
    }
}
```
