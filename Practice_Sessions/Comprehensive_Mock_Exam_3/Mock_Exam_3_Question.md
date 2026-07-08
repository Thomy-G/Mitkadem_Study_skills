# Bar-Ilan University — Advanced Systems Programming (893210)
## Comprehensive Mock Exam 3 (Practice Session)

**Time Allowed:** 3 Hours
**Total Points:** 100

---

### 📝 Question 1: C++ Design, Thread-Safe Singleton, & Decorator Pattern (40 Points)

#### Scenario
You are required to build a thread-safe system configuration manager in C++ representing a **DatabaseConfig** block, which must be implemented using the **Meyers Singleton** pattern. 

In addition, you need to implement a **QueryLogger** system that processes query logs. Use the **Decorator Pattern** to dynamically extend the QueryLogger so it adds customizable metadata (such as timestamps or user tags) before writing logs to `std::cout`.

#### Architectural & Grading Constraints
1.  **Meyers Singleton:** The `DatabaseConfig` class must guarantee thread-safe lazy initialization automatically using C++ static local variables. It must prevent direct instantiations, copies, or assignments.
2.  **Decorator Pattern:** The logger interface must declare a virtual `void log(const std::string& query)` function. Create a concrete `SimpleLogger` and a decorator class `TimestampLoggerDecorator` that pre-appends a timestamp to the log query.
3.  **Smart Pointer Boundaries:** Do not use raw pointers. The decorator must wrap the underlying logger instance using a smart pointer (`std::unique_ptr` or `std::shared_ptr`).
4.  **Google Test (gtest) Suite:** Write unit tests verifying:
    *   The singleton instance returns the same memory address across calls.
    *   The `TimestampLoggerDecorator` correctly pre-appends text to the logged query output.

---

### 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

Locate and document **exactly 4 distinct bugs** across the Mongoose model, React component, and C++ operator overloading code segments below.

#### Component A: Mongoose Model (`models/Log.js`)
```javascript
const mongoose = require('mongoose');

const logSchema = new mongoose.Schema({
    message: { type: String, required: true },
    timestamp: { type: Date, default: Date.now }
});

// Middleware to log before saving
logSchema.pre('save', function(next) {
    console.log("Preparing to save log: " + this.message);
    // Line 11: Processing complete
});

module.exports = mongoose.model('Log', logSchema);
```

#### Component B: React Stale Closure (`components/Timer.jsx`)
```jsx
import React, { useState, useEffect } from 'react';

export default function Timer() {
    const [seconds, setSeconds] = useState(0);

    useEffect(() => {
        const interval = setInterval(() => {
            setSeconds(seconds + 1); // Line 8
        }, 1000);
        return () => clearInterval(interval);
    }, []); // Line 11

    return <h1>Seconds: {seconds}</h1>;
}
```

#### Component C: C++ Operator Overloading (`Complex.cpp`)
```cpp
#include <iostream>

class Complex {
public:
    double real, imag;
    Complex(double r, double i) : real(r), imag(i) {}

    // Overloading copy assignment operator
    void operator=(const Complex& other) { // Line 8
        real = other.real;
        imag = other.imag;
    }
};

void run_assignment() {
    Complex c1(1, 1), c2(2, 2), c3(3, 3);
    c1 = c2 = c3; // Line 15
}
```

#### Component D: C++ Polymorphic Destruction (`destructor.cpp`)
```cpp
#include <iostream>

class Base {
public:
    Base() { std::cout << "Base Constructor\n"; }
    ~Base() { std::cout << "Base Destructor\n"; } // Line 5
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived Constructor\n"; }
    ~Derived() { std::cout << "Derived Destructor\n"; }
};

int main() {
    Base* obj = new Derived();
    delete obj; // Line 15
    return 0;
}
```

---

### 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

#### Section A: C++ Diamond Virtual Inheritance Construction (10 Points)
Predict the exact console output of this virtual inheritance initialization hierarchy:

```cpp
#include <iostream>
using namespace std;

class Top {
public:
    Top() { cout << "Top "; }
    ~Top() { cout << "~Top "; }
};

class Left : virtual public Top {
public:
    Left() { cout << "Left "; }
    ~Left() { cout << "~Left "; }
};

class Right : virtual public Top {
public:
    Right() { cout << "Right "; }
    ~Right() { cout << "~Right "; }
};

class Bottom : public Left, public Right {
public:
    Bottom() { cout << "Bottom "; }
    ~Bottom() { cout << "~Bottom "; }
};

int main() {
    Top* ptr = new Bottom();
    delete ptr;
    return 0;
}
```

#### Section B: JavaScript Scoping & Event Loop (10 Points)
Predict the exact console output of the execution flow below. Explain the behavior of the Temporal Dead Zone (TDZ).

```javascript
let val = 10;

function outer() {
    console.log("outer val:", val);
    
    setTimeout(() => {
        console.log("timeout val:", val);
    }, 0);

    if (true) {
        let val = 20;
        console.log("inner block val:", val);
    }
}

outer();
console.log("global val:", val);
```

---

### 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

#### Section A: TCP Connection Handshake States (5 Points)
Complete the Socket API flow by writing the correct socket function calls (1-5):
```
      SERVER                          CLIENT
  +------------+
  |  socket()  |
  +------------+
        |
  +----[1]-----+
  |  ________  |
  +------------+
        |
  +----[2]-----+
  |  ________  |
  +------------+
        |
  +----[3]-----+
  |  ________  | <================== connect()
  +------------+                     [Client]
        |
  +----[4]-----+
  |  ________  | <====== Read/Write data
  +------------+
        |
  +----[5]-----+
  |  ________  |
  +------------+
```

#### Section B: Git References Mechanics (5 Points)
Complete the blanks (1-5) explaining Git branch pointer storage:
*   In Git, branches are simply pointers stored as files in the directory **[1]________**.
*   The active branch is referenced inside the file named **[2]________** in the root `.git` folder.
*   This file contains a path pointer in the format: `ref: refs/heads/` followed by the **[3]________** name.
*   Each file inside `refs/heads/` contains exactly 40 characters representing the **[4]________** of the latest commit.
*   When a new commit is created, Git updates this reference file with the new commit hash, demonstrating that branches in Git are **[5]________** (weightless) pointer references.

---

### 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

#### Section A: Android LiveData ViewModel Structure (5 Points)
Write a Java class `TelemetryViewModel` that extends `ViewModel` and implements:
1.  A private `MutableLiveData<String>` variable containing system status.
2.  A public `LiveData<String>` getter method that enforces read-only encapsulation for the View layer.
3.  Explain the difference between `setValue()` and `postValue()` when updating this LiveData object.

#### Section B: JWT Structure & Security Claims (5 Points)
Describe the cryptographic mechanism used to verify JSON Web Tokens (JWT). Why is it safe to store non-sensitive user identity claims (like usernames) in the token payload without encryption?
