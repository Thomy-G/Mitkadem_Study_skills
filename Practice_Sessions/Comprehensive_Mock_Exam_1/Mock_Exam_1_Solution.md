# Comprehensive Mock Exam 1 — Official Solution Key

---

## 📝 Question 1: C++ Design, Smart Pointers, & Test-Driven Development (40 Points)

### Section A: Class Declarations & Separation (`Telemetry.h`)
```cpp
#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Observer Pattern Interface
class TelemetryHandler {
public:
    virtual ~TelemetryHandler() = default;
    virtual void onSensorUpdate(const std::string& sensor_id, int data_value) = 0;
    virtual std::string getName() const = 0;
};

// Concrete Telemetry Handler implementation
class ConsoleHandler : public TelemetryHandler {
private:
    std::string name;
public:
    explicit ConsoleHandler(std::string handler_name);
    void onSensorUpdate(const std::string& sensor_id, int data_value) override;
    std::string getName() const override;
};

// Telemetry Subject
class TelemetryEngine {
private:
    // Store handlers by sensor_id using smart pointers
    std::unordered_map<std::string, std::vector<std::shared_ptr<TelemetryHandler>>> subscriptions;

public:
    void subscribe(const std::string& sensor_id, std::shared_ptr<TelemetryHandler> handler);
    void publishEvent(const std::string& sensor_id, int data_value);
};

#endif // TELEMETRY_H
```

### Section B: Implementation Details (`Telemetry.cpp` & `main.cpp`)

#### `Telemetry.cpp`
```cpp
#include "Telemetry.h"
#include <iostream>

ConsoleHandler::ConsoleHandler(std::string handler_name) : name(std::move(handler_name)) {}

void ConsoleHandler::onSensorUpdate(const std::string& sensor_id, int data_value) {
    std::cout << "Handler " << name << " received sensor " << sensor_id << " data " << data_value << std::endl;
}

std::string ConsoleHandler::getName() const {
    return name;
}

void TelemetryEngine::subscribe(const std::string& sensor_id, std::shared_ptr<TelemetryHandler> handler) {
    subscriptions[sensor_id].push_back(handler);
}

void TelemetryEngine::publishEvent(const std::string& sensor_id, int data_value) {
    if (subscriptions.find(sensor_id) != subscriptions.end()) {
        for (const auto& handler : subscriptions[sensor_id]) {
            if (handler) {
                handler->onSensorUpdate(sensor_id, data_value);
            }
        }
    }
}
```

#### `main.cpp`
```cpp
#include "Telemetry.h"
#include <iostream>
#include <sstream>

int main() {
    TelemetryEngine engine;
    std::unordered_map<std::string, std::shared_ptr<TelemetryHandler>> active_handlers;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string command, sensor_id;
        ss >> command;

        if (command == "subscribe") {
            std::string handler_name;
            if (ss >> sensor_id >> handler_name) {
                // Fetch or instantiate handler uniquely to prevent duplicates
                if (active_handlers.find(handler_name) == active_handlers.end()) {
                    active_handlers[handler_name] = std::make_shared<ConsoleHandler>(handler_name);
                }
                engine.subscribe(sensor_id, active_handlers[handler_name]);
            }
        } else if (command == "event") {
            int data_value;
            if (ss >> sensor_id >> data_value) {
                engine.publishEvent(sensor_id, data_value);
            }
        }
    }
    return 0;
}
```

### Section C: Google Test Suite (`Telemetry_Test.cpp`)
```cpp
#include <gtest/gtest.h>
#include "Telemetry.h"
#include <sstream>

// Mock Handler for testing callbacks without output pollution
class MockHandler : public TelemetryHandler {
public:
    std::string last_sensor;
    int last_val = -1;
    bool called = false;
    std::string name;

    explicit MockHandler(std::string name) : name(name) {}

    void onSensorUpdate(const std::string& sensor_id, int data_value) override {
        last_sensor = sensor_id;
        last_val = data_value;
        called = true;
    }

    std::string getName() const override { return name; }
};

// Unit tests demonstrating subscription event propagation
TEST(TelemetryTest, SubscriptionFiresCallback) {
    TelemetryEngine engine;
    auto handler = std::make_shared<MockHandler>("Tester1");
    
    engine.subscribe("temp_sensor", handler);
    engine.publishEvent("temp_sensor", 42);
    
    EXPECT_TRUE(handler->called);
    EXPECT_EQ(handler->last_sensor, "temp_sensor");
    EXPECT_EQ(handler->last_val, 42);
}

TEST(TelemetryTest, NonSubscribedSensorDoesNotFire) {
    TelemetryEngine engine;
    auto handler = std::make_shared<MockHandler>("Tester1");
    
    engine.subscribe("temp_sensor", handler);
    engine.publishEvent("pressure_sensor", 100);
    
    EXPECT_FALSE(handler->called);
}
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `server.cpp`)
*   **Mechanics of Flaw:** Without configuring the socket for address/port reuse, subsequent restarts of the server will fail with an "Address already in use" bind error. Standard BIU exams mandate setting `SO_REUSEADDR`.
*   **Corrected Statement:** Set the socket options before calling `bind`:
    ```cpp
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    ```

### Bug 2 (Component B - `client.py`)
*   **Mechanics of Flaw:** In Python 3, socket transmission requires binary types (`bytes`), not strings. Calling `send(message)` with a string parameter raises a `TypeError`.
*   **Corrected Statement:** Encode the message to bytes before sending:
    ```python
    client_socket.send(message.encode('utf-8'))
    ```

### Bug 3 (Component C - `controllers/auth.js`)
*   **Mechanics of Flaw:** The middleware controller completes its logic but **omits calling `next()`**. This traps the HTTP request within the middleware pipeline, causing the client connection to hang indefinitely without proceeding to the next route handler.
*   **Corrected Statement:** Include `next()` after validation passes:
    ```javascript
    const user = await User.findById(req.userId);
    if (!user || !user.isSubscribed) {
        return res.status(403).json({ error: "Access denied" });
    }
    next();
    ```

### Bug 4 (Component C - `controllers/auth.js`)
*   **Mechanics of Flaw:** The controller checks `req.userId` but has not extracted it. In standard Express JWT authorization pipelines, `req.userId` must be populated by a preceding authentication middleware (like `verifyToken`). If authentication fails or is un-configured, `req.userId` remains `undefined`, causing `User.findById(undefined)` to crash or execute erroneously.
*   **Corrected Statement:** Ensure token decryption and request parameter verification are handled:
    ```javascript
    if (!req.userId) {
        return res.status(401).json({ error: "No token provided / Authentication required" });
    }
    ```

---

## 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

### Section A: JavaScript Promises & Scope
*   **Predicted Terminal Console Output:**
    ```
    promise init
    scopeTest x: undefined
    global y: 2
    promise resolved: 10
    timeout x: 100
    ```
*   **Step-by-step Execution Tracing:**
    1. Synchronous block runs: `var x = 1` and `let y = 2` are bound.
    2. `setTimeout` callback is pushed onto the Macrotask queue.
    3. The `Promise` constructor runs immediately (synchronously), logging `"promise init"`. The resolve callback is pushed onto the Microtask queue.
    4. `scopeTest()` is invoked:
       - Inside `scopeTest`, the local `var x` declaration inside the `if` block is hoisted to the top of the function scope.
       - Consequently, `console.log("scopeTest x:", x)` prints `undefined` (not `1`), because the hoisted local `var x` shadows the global `x`.
       - `x` is then assigned `100` globally and locally (within function scope).
    5. `console.log("global y:", y)` prints `2`. (Note: The `let b = 300` on the original sample was a syntax error if duplicate declarations exist, but here `let y = 20` is block-scoped inside the promise handler so global `y` remains `2`).
    6. Call stack clears. The Microtask queue is processed: Promise `then` callback fires, logging `"promise resolved: 10"`.
    7. Macrotask queue is processed: `setTimeout` callback runs, logging `"timeout x: 100"` (reflecting the global scope change from the hoisted `var` modification).

### Section B: C++ Multiple & Virtual Inheritance
*   **Predicted Terminal Console Output:**
    ```
    Base constructor
    Der1 constructor
    Der2 constructor
    Final constructor
    Final destructor
    Der2 destructor
    Der1 destructor
    Base destructor
    ```
*   **Trace & Construction/Destruction Mechanics:**
    1. Due to the `virtual` inheritance from `Base`, there is only a single instance of `Base` in the instance of `Final`.
    2. Virtual bases are constructed first, exactly once: `Base` constructor prints `"Base constructor"`.
    3. Non-virtual base classes are constructed in the order they appear in the class declaration:
       - `Der1` constructor prints `"Der1 constructor"`.
       - `Der2` constructor prints `"Der2 constructor"`.
    4. Concrete class constructor runs: `Final` constructor prints `"Final constructor"`.
    5. Destruction occurs in the exact reverse order:
       - `Final destructor`
       - `Der2 destructor`
       - `Der1 destructor`
       - `Base destructor` (Note: since `Base` virtual destructor was called via the pointer `Base* obj = new Final()`, the virtual destructor table correctly resolves the derived destructors in reverse order).

---

## 🛠️ Question 4: "Fill in the Blanks" / Diagram & Table Completion (10 Points)

### Section A: Low-Level Git Object Mechanics

| Object Hash Prefix | Object Type (`blob`/`tree`/`commit`) | Description / Content |
| :--- | :--- | :--- |
| **Object 1** | `blob` | Represents contents of first `a.txt`: `"Hello BIU\n"` |
| **Object 2** | `tree` | Lists directories/files at commit 1: links to `a.txt` (Object 1). |
| **Object 3** | `commit` | Metadata for Commit 1: links to `tree` (Object 2), author, date, message `"initial commit"`. |
| **Object 4** | `blob` | Represents updated contents of `a.txt`: `"Hello Mitkadem\n"` |
| **Object 5** | `tree` | Lists directories/files at commit 2: links to updated `a.txt` (Object 4). |
| **Object 6** | `commit` | Metadata for Commit 2: links to `tree` (Object 5), links to parent commit (Object 3), author, message `"second commit"`. |

### Section B: Socket API Connection Flow
1. `bind()`
2. `listen()`
3. `accept()`
4. `connect()` (initiated by client)
5. `recv()` or `read()` / `write()`

---

## 📱 Question 5: Android MVVM Layouts & Web Security Theory (10 Points)

### Section A: Android Layouts & LiveData

#### XML Layout (`res/layout/activity_main.xml`)
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:padding="16dp">

    <TextView
        android:id="@+id/tv_sensor"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="Sensor Status: Unknown"
        android:textSize="18sp" />

    <Button
        android:id="@+id/btn_refresh"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginTop="16dp"
        android:text="Refresh" />

</LinearLayout>
```

#### ViewModel (`SensorViewModel.java`)
```java
package com.example.biu_exam;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class SensorViewModel extends ViewModel {
    private final MutableLiveData<String> sensorData = new MutableLiveData<>();

    public SensorViewModel() {
        sensorData.setValue("Initial System Idle");
    }

    public LiveData<String> getSensorData() {
        return sensorData;
    }

    public void updateSensorStatus(String newStatus) {
        sensorData.setValue(newStatus);
    }
}
```

### Section B: Web Security Theory
*   **Session Cookies**: State is stored **statefully** on the server side (in databases or memory caches like Redis). The client is given a Session ID cookie.
    *   *Scaling/Invalidation*: Easy to invalidate immediately (by deleting the session on the server), but difficult to scale horizontally without sticky sessions or distributed database synchronization.
*   **JSON Web Tokens (JWT)**: State is stored **statelessly** on the client side inside the cryptographically signed token string.
    *   *Scaling/Invalidation*: Easily scalable horizontally (servers only verify the cryptographic signature without database lookups), but highly difficult to invalidate immediately before expiration unless blacklisting schemas are implemented.
