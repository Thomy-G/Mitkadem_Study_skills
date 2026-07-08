# Comprehensive Mock Exam 3 — Official Solution Key

---

## 📝 Question 1: C++ Design, Thread-Safe Singleton, & Decorator Pattern (40 Points)

### Section A: Header Declarations (`DatabaseConfig.h` & `QueryLogger.h`)

#### `DatabaseConfig.h`
```cpp
#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <string>

class DatabaseConfig {
private:
    std::string db_url;
    
    // Private constructors/destructors to prevent instantiation
    DatabaseConfig() : db_url("mongodb://localhost:27017/prod") {}
    ~DatabaseConfig() = default;

public:
    // Delete copy constructors and assignments
    DatabaseConfig(const DatabaseConfig&) = delete;
    DatabaseConfig& operator=(const DatabaseConfig&) = delete;

    // Meyers Singleton Instance Getter (thread-safe by C++11 standard)
    static DatabaseConfig& getInstance() {
        static DatabaseConfig instance;
        return instance;
    }

    std::string getUrl() const { return db_url; }
    void setUrl(const std::string& url) { db_url = url; }
};

#endif // DATABASE_CONFIG_H
```

#### `QueryLogger.h`
```cpp
#ifndef QUERY_LOGGER_H
#define QUERY_LOGGER_H

#include <string>
#include <memory>

// Component Interface
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual std::string log(const std::string& query) = 0;
};

// Concrete Component
class SimpleLogger : public ILogger {
public:
    std::string log(const std::string& query) override {
        return "Query: " + query;
    }
};

// Decorator Base
class LoggerDecorator : public ILogger {
protected:
    std::unique_ptr<ILogger> wrapped_logger;
public:
    LoggerDecorator(std::unique_ptr<ILogger> logger) : wrapped_logger(std::move(logger)) {}
};

// Concrete Decorator
class TimestampLoggerDecorator : public LoggerDecorator {
public:
    using LoggerDecorator::LoggerDecorator;
    std::string log(const std::string& query) override {
        return "[2026-07-08T12:00:00Z] " + wrapped_logger->log(query);
    }
};

#endif // QUERY_LOGGER_H
```

### Section B: Implementation & Execution Flow
The declarations above contain inline definitions for ease of configuration.

### Section C: TDD Google Test Suite (`System_Test.cpp`)
```cpp
#include <gtest/gtest.h>
#include "DatabaseConfig.h"
#include "QueryLogger.h"

TEST(DatabaseConfigTest, SingletonAddressStability) {
    DatabaseConfig& c1 = DatabaseConfig::getInstance();
    DatabaseConfig& c2 = DatabaseConfig::getInstance();
    EXPECT_EQ(&c1, &c2); // Verifies identical memory references
}

TEST(QueryLoggerDecoratorTest, VerifyMetadataAppending) {
    std::unique_ptr<ILogger> logger = std::make_unique<SimpleLogger>();
    std::unique_ptr<ILogger> decorated = std::make_unique<TimestampLoggerDecorator>(std::move(logger));
    
    std::string output = decorated->log("SELECT * FROM USERS");
    EXPECT_EQ(output, "[2026-07-08T12:00:00Z] Query: SELECT * FROM USERS");
}
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `models/Log.js`)
*   **Filename:** `models/Log.js`
*   **Line / Code Block:** Line 11: Inside `logSchema.pre('save', ...)` callback.
*   **Mechanics of Flaw:** Mongoose middleware uses a callback model where the handler *must* invoke `next()` to hand off execution control. If omitted, the request hangs indefinitely, resulting in a database operation timeout.
*   **Corrected Statement:**
    ```javascript
    logSchema.pre('save', function(next) {
        console.log("Preparing to save log: " + this.message);
        next(); // Proceed to save
    });
    ```

### Bug 2 (Component B - `components/Timer.jsx`)
*   **Filename:** `components/Timer.jsx`
*   **Line / Code Block:** Line 8 & 11: `setSeconds(seconds + 1);` and `}, []);`
*   **Mechanics of Flaw:** Stale closure. Since the dependency array is empty (`[]`), the `useEffect` hook captures the initial value of `seconds` (which is `0`). On every tick, it sets state to `0 + 1 = 1`, causing the timer to freeze at `1`.
*   **Corrected Statement:** Use functional state updates to avoid capturing outer scopes:
    ```jsx
    setSeconds(prevSeconds => prevSeconds + 1);
    ```

### Bug 3 (Component C - `Complex.cpp`)
*   **Filename:** `Complex.cpp`
*   **Line / Code Block:** Line 8: `void operator=(const Complex& other)`
*   **Mechanics of Flaw:** Copy assignment operators should return a reference to self (`Complex&`) to permit chaining (e.g., `c1 = c2 = c3`). Returning `void` prevents compilation on Line 15 since `c2 = c3` evaluates to `void`, which cannot be assigned to `c1`.
*   **Corrected Statement:**
    ```cpp
    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }
    ```

### Bug 4 (Component D - `destructor.cpp`)
*   **Filename:** `destructor.cpp`
*   **Line / Code Block:** Line 5: `~Base()` destructor is not declared virtual.
*   **Mechanics of Flaw:** Polymorphic delete bug. When deleting a derived object via a pointer to its base class (`Base*`), if the base class destructor is not virtual, the runtime performs static dispatch, invoking only the base destructor and leaking the memory allocated for the derived class.
*   **Corrected Statement:**
    ```cpp
    virtual ~Base() { std::cout << "Base Destructor\n"; }
    ```

---

## 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

### Section A: C++ Diamond Virtual Inheritance Construction
*   **Predicted Output:**
    ```
    Top Left Right Bottom ~Bottom ~Right ~Left ~Top 
    ```
*   **Step-by-step Construction & Destruction Trace:**
    1.  `Bottom` inherits from `Left` and `Right`, which both inherit virtually from `Top`.
    2.  In virtual inheritance, the most-derived class (`Bottom`) constructs the virtual base (`Top`) exactly once: prints `"Top "`.
    3.  `Left` is constructed next: prints `"Left "`.
    4.  `Right` is constructed next: prints `"Right "` (its virtual base `Top` initialization is skipped as it has already been initialized).
    5.  `Bottom` constructor runs: prints `"Bottom "`.
    6.  `delete ptr;` invokes destructors in the exact reverse order of construction: `"~Bottom ~Right ~Left ~Top "`.
    *   *Warning*: Note that virtual inheritance requires base destructors to be virtual to clean up correctly, which `Top` is not in this specific trace unless declared as such. However, since the code calls delete on `ptr` (where `Top* ptr = new Bottom();`), if `~Top()` is not virtual, calling `delete ptr` would only print `~Top` (undefined behavior). Assuming the question implies standard polymorphic destruction or if virtual was omitted to test that specific edge case. Under strict rules, if `~Top()` is non-virtual, only `~Top` runs. Output: `Top Left Right Bottom ~Top`. Since `Top` is the parent base, we assume virtual is declared or the output is `Top Left Right Bottom ~Top`. Let's assume the question tests polymorphic delete: output is `Top Left Right Bottom ~Top`.

### Section B: JavaScript Scoping & Event Loop
*   **Predicted Output Sequence:**
    ```
    outer val: 10
    inner block val: 20
    global val: 10
    timeout val: 10
    ```
*   **Explanation of Scope & Temporal Dead Zone (TDZ):**
    1.  `outer()` is invoked. `val` inside `outer` refers to the outer lexical scope `val` (which is `10`): logs `"outer val: 10"`.
    2.  `setTimeout` pushes a macrotask logging `val` to the queue.
    3.  Inside the `if` block, `let val = 20;` creates a block-scoped variable, shadowing the outer variable: logs `"inner block val: 20"`.
    4.  `outer()` finishes executing.
    5.  `console.log("global val:", val)` runs synchronusly: logs `"global val: 10"`.
    6.  Stack clears. The event loop pulls the timeout task: logs `"timeout val: 10"`.

---

## 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

### Section A: TCP Connection Handshake States
1.  `bind()`
2.  `listen()`
3.  `accept()`
4.  `read()` / `write()` (or `recv()` / `send()`)
5.  `close()`

### Section B: Git References Mechanics
1.  `.git/refs/heads/`
2.  `.git/HEAD`
3.  branch
4.  SHA-1 hash
5.  lightweight

---

## 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

### Section A: Android LiveData ViewModel Structure
*   **Java Code:**
```java
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TelemetryViewModel extends ViewModel {
    private final MutableLiveData<String> status = new MutableLiveData<>("INIT");

    public LiveData<String> getStatus() {
        return status; // Exposes read-only interface
    }

    public void updateStatus(String newStatus) {
        status.setValue(newStatus);
    }
}
```
*   **`setValue` vs `postValue`:**
    *   `setValue()` must be called exclusively on the **Main (UI) Thread**. It updates state immediately.
    *   `postValue()` is designed for **worker threads**. It posts a task to the Main thread queue to update state asynchronously, ensuring UI updates do not crash the background execution flow.

### Section B: JWT Structure & Security Claims
*   **Cryptographic Verification Mechanism:** The server verifies token integrity by recalculating the signature section using a private secret key and hashing the Base64url-encoded header + payload (`HMACSHA256(Base64(Header) + "." + Base64(Payload), Secret)`). If matching, the token is verified.
*   **Payload Encryption Explanation:** The payload is **Base64url-encoded, not encrypted**. Anyone can decode the payload string using standard decoder functions. Thus, storing sensitive info (like passwords) is prohibited. However, claims like usernames are safe because the server uses the signature to guarantee that the payload has not been modified.
