# Bar-Ilan University — Advanced Systems Programming (893210)
## Comprehensive Mock Exam 1 (Practice Session)

**Time Allowed:** 3 Hours
**Total Points:** 100

---

### 📝 Question 1: C++ Design, Smart Pointers, & Test-Driven Development (40 Points)

#### Scenario
You are required to build a **Telemetry Subscription Engine** in C++ designed to collect telemetry data events from various system sensors and broadcast them to subscribed handler units. 

The system must read commands from standard input (`std::cin`) continuously and split incoming lines into tokens. The system must support the following verbs:
1. `subscribe [sensor_id] [handler_name]`
   - Registers a new handler named `[handler_name]` to receive updates from sensor `[sensor_id]`.
2. `event [sensor_id] [data_value]`
   - Publishes an update from the sensor containing the integer `[data_value]`. This value should be dispatched to all subscribed handlers.
   - Dispatching to handlers must print the message: `Handler [handler_name] received sensor [sensor_id] data [data_value]` directly to `std::cout`.

#### Architectural & Grading Constraints
To receive full credit (40 Points), your solution must adhere to the following constraints:
1. **Separation of Concerns:** Keep your interactive loop separate from your engine logic. Describe how you would separate code into `.h` and `.cpp` files.
2. **Open-Closed Principle (OCP):** Designing future registration types or commands should not modify the dispatcher logic. Implement the **Observer Pattern**.
3. **No Raw Pointers:** Ensure absolute memory safety by using standard smart pointers (`std::unique_ptr`, `std::shared_ptr`, or `std::weak_ptr`). `new` and `delete` expressions are strictly prohibited.
4. **Test-Driven Development (TDD) Mocking:** Write a complete **Google Test (gtest)** suite that sets up unit tests checking the subscription and event dispatching behaviors.

---

### 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

Below are fragments of a low-level network system with a C++ server (using the POSIX socket API) and a client-side Python script, along with an Express.js backend controller.
There are **exactly 4 distinct bugs** across these components. Locate and document them.

#### Component A: C++ TCP Server (`server.cpp`)
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void start_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Config address struct
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    // Bind socket
    bind(server_fd, (struct sockaddr*)&address, sizeof(address)); // Line 16
    listen(server_fd, 3);
    
    int client_fd = accept(server_fd, nullptr, nullptr);
    char buffer[1024] = {0};
    read(client_fd, buffer, 1024);
    std::cout << "Received: " << buffer << std::endl;
    close(client_fd);
    close(server_fd);
}
```

#### Component B: Python TCP Client (`client.py`)
```python
import socket

def send_payload(port, message):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('127.0.0.1', port))
    client_socket.send(message) # Line 6
    client_socket.close()
```

#### Component C: Express.js Controller Middleware (`controllers/auth.js`)
```javascript
const User = require('../models/User');

exports.checkSubscription = async (req, res, next) => {
    try {
        const user = await User.findById(req.userId);
        if (!user || !user.isSubscribed) {
            return res.status(403).json({ error: "Access denied" });
        }
        // Check complete
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};
```

---

### 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

#### Section A: JavaScript Promises & Scope (10 Points)
Predict the exact console output sequence for the following execution flow:
```javascript
var x = 1;
let y = 2;

setTimeout(() => {
    console.log("timeout x:", x);
}, 0);

const promise = new Promise((resolve) => {
    console.log("promise init");
    resolve(10);
});

promise.then((val) => {
    console.log("promise resolved:", val);
    let y = 20;
});

function scopeTest() {
    console.log("scopeTest x:", x);
    if (true) {
        var x = 100;
    }
}

scopeTest();
console.log("global y:", y);
```

#### Section B: C++ Multiple & Virtual Inheritance (10 Points)
Predict the exact output of this polymorphic layout execution:
```cpp
#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout << "Base constructor" << endl; }
    virtual ~Base() { cout << "Base destructor" << endl; }
};

class Der1 : virtual public Base {
public:
    Der1() { cout << "Der1 constructor" << endl; }
    ~Der1() { cout << "Der1 destructor" << endl; }
};

class Der2 : virtual public Base {
public:
    Der2() { cout << "Der2 constructor" << endl; }
    ~Der2() { cout << "Der2 destructor" << endl; }
};

class Final : public Der1, public Der2 {
public:
    Final() { cout << "Final constructor" << endl; }
    ~Final() { cout << "Final destructor" << endl; }
};

int main() {
    Base* obj = new Final();
    delete obj;
    return 0;
}
```

---

### 🛠️ Question 4: "Fill in the Blanks" / Diagram & Table Completion (10 Points)

#### Section A: Low-Level Git Object Mechanics (5 Points)
A developer runs the following commands in a clean Git workspace:
```bash
$ git init
$ echo "Hello BIU" > a.txt
$ git add a.txt
$ git commit -m "initial commit"
$ echo "Hello Mitkadem" > a.txt
$ git add a.txt
$ git commit -m "second commit"
```
Fill in the table detailing the objects created in `.git/objects/`. Specify their types (`blob`, `tree`, `commit`) and contents.

#### Section B: Socket API Connection Flow (5 Points)
Fill in the blanks (1-5) representing the socket functions in the TCP Connection lifecycle diagram:
```
      SERVER                        CLIENT
  +------------+                +------------+
  |  socket()  |                |  socket()  |
  +------------+                +------------+
        |                             |
  +----[1]-----+                      |
  |  ________  |                      |
  +------------+                      |
        |                             |
  +----[2]-----+                      |
  |  ________  |                      |
  +------------+                      |
        |                             |
  +----[3]-----+                      |
  |  ________  |                      |
  +------------+                      |
        |                             |
        |<==================[4]=======+
        |                   connect() |
  +----[5]-----+                      |
  |  ________  |                      |
  +------------+                      |
```

---

### 📱 Question 5: Android MVVM Layouts & Web Security Theory (10 Points)

#### Section A: Android Layouts & LiveData (5 Points)
Design a basic Android XML configuration with a vertical `LinearLayout` containing a `TextView` (ID: `tv_sensor`) and a `Button` (ID: `btn_refresh`). Then, write a Java ViewModel snippet demonstrating how to expose a `LiveData<String>` representing sensor status updates.

#### Section B: Web Security Theory (5 Points)
Explain the primary differences between **Session Cookies** and **JSON Web Tokens (JWT)** for maintaining authentication state. Specifically details how each mechanisms stores payload data and handles scaling/invalidation.
