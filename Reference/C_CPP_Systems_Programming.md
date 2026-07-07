# C & C++ Advanced Systems Programming — Study Reference Guide

---

## 1. The C++ Compilation, Linking, & Optimization Pipeline

C++ code goes through four distinct compiler phases to become an executable:
1.  **Preprocessing (`g++ -E`)**: Expands macros, resolves `#include` headers, handles conditional structures (`#ifndef`/`#endif`). Outputs `.i` files.
2.  **Compilation (`g++ -S`)**: Translates C++ code into assembly instructions. Outputs `.s` files.
3.  **Assembly (`g++ -c`)**: Converts assembly to machine-code object files. Outputs `.o` files.
4.  **Linking (`g++`)**: Connects object files, resolving references to produce the final executable.

### 1.1 Compiler Optimization Flags & Constructor Elision
*   `-O3`: Enables high-level compiler optimizations, including **Loop Unrolling** (duplicating loop bodies to minimize branch instruction penalties in assembly).
*   `Return Value Optimization (RVO)`: By default, modern compilers elide (omit) copying/moving temporary objects returned from functions to optimize memory.
*   `-fno-elide-constructors`: Disables RVO, forcing the compiler to execute the copy and move constructors explicitly. This is heavily used in exams to trace exact outputs of constructor/destructor logs.

#### Example Tracing:
```cpp
#include <iostream>
class A {
public:
    A() { std::cout << "C"; }
    A(const A&) { std::cout << "Copy"; }
    ~A() { std::cout << "D"; }
};
A func() { A a; return a; }
int main() { A obj = func(); }
```
*   *With RVO (Default)*: Prints `CD` (creates `a` in `func` directly inside the memory address allocated for `obj` in `main`).
*   *With `-fno-elide-constructors`*: Prints `CCopyCopyDDD` (creates `a` in `func`, copies to a temporary return object, destroys `a`, copies temporary to `obj` in `main`, destroys temporary, and finally destroys `obj` at main exit).

### 1.2 Static vs. Dynamic Linking
*   **Static Linking**: Embeds all library code directly inside the executable binary during compilation.
    *   *Pros*: Fast loading; executable runs standalone.
    *   *Cons*: Large binary size; updating the library requires rebuilding the entire application.
*   **Dynamic Linking**: Postpones resolving references until load/runtime. The executable refers to shared libraries (e.g., `.so` on Linux, `.dll` on Windows).
    *   *Pros*: Smaller binary footprint; shared libraries can be updated independently of the executable.
    *   *Cons*: Small runtime loading overhead; depends on external library files being present.

---

## 2. Functions & Lambda Expressions in C++

### 1.1 Function Declarations & Definitions
In C++, separation of declarations (`.h` headers) and definitions (`.cpp` files) is critical to prevent duplicate symbol linker errors.
```cpp
// MathUtils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Declaration tells the compiler the function signature exists
int add(int a, int b);

// Inline functions must be defined in headers to be copied inline by the compiler
inline int multiply(int a, int b) {
    return a * b;
}

// Template functions must also be defined in headers
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

#endif
```

```cpp
// MathUtils.cpp
#include "MathUtils.h"

// Definition provides the actual implementation
int add(int a, int b) {
    return a + b;
}
```

### 1.2 C++ Lambda Expressions
C++ lambdas are anonymous function objects (functors) generated on the fly.
Syntax: `[capture_clause](parameters) mutable exception_specification -> return_type { body }`

#### Capture Clauses:
*   `[]`: Capture nothing.
*   `[x]`: Capture `x` by copy (read-only unless `mutable` is specified).
*   `[&x]`: Capture `x` by reference.
*   `[=]`: Capture all local variables in scope by copy.
*   `[&]`: Capture all local variables in scope by reference.
*   `[this]`: Capture the current class instance pointer.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

void lambda_demonstration() {
    int multiplier = 3;
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Lambda capturing 'multiplier' by value
    std::for_each(numbers.begin(), numbers.end(), [multiplier](int& n) {
        n *= multiplier;
    });

    // Lambda capturing by reference to count items
    int total_sum = 0;
    std::for_each(numbers.begin(), numbers.end(), [&total_sum](int n) {
        total_sum += n;
    });

    // Mutable lambda: allows modifying captured-by-copy values locally
    int counter = 0;
    auto incrementer = [counter]() mutable {
        counter++; // Valid because of mutable; does not affect original counter
        return counter;
    };
}
```

---

## 3. Class Structures, Initializers, & Lifecycle Management

### 2.1 Access Modifiers & Members
*   `private`: Default for classes. Accessible only inside the class and by `friend` declarations.
*   `protected`: Accessible by the class and any derived child classes.
*   `public`: Accessible from any client code holding an instance.

### 2.2 Initializer Lists & Constructor Delegation
*   **Member Initializer List**: Enforces initializing member variables *before* the constructor body runs. Essential for `const` members, references (`&`), or base classes without default constructors.
*   **Constructor Delegation**: Calling another constructor of the same class in the initializer list.

```cpp
#include <string>

class Configuration {
private:
    const std::string mode;
    int& port_reference;
    int timeout;

public:
    // Member Initializer List (mandatory for const mode and reference port_reference)
    Configuration(std::string m, int& port) : mode(std::move(m)), port_reference(port), timeout(3000) {}

    // Delegating Constructor: calls the primary constructor
    explicit Configuration(int& port) : Configuration("Development", port) {}
};
```

### 2.3 Orthodox Canonical Class Form (OCF) — Deep Copy vs. Move Semantics
```cpp
#include <iostream>
#include <utility>

class Buffer {
private:
    size_t size;
    int* data;

public:
    // Default Constructor
    Buffer() : size(0), data(nullptr) {}

    // Parameterized Constructor
    explicit Buffer(size_t s) : size(s), data(new int[s]{0}) {}

    // Destructor
    ~Buffer() {
        delete[] data;
    }

    // Copy Constructor (Deep Copy)
    Buffer(const Buffer& other) : size(other.size), data(other.size ? new int[other.size] : nullptr) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Copy Assignment Operator (Copy and Swap Idiom)
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            int* new_data = other.size ? new int[other.size] : nullptr;
            for (size_t i = 0; i < other.size; ++i) {
                new_data[i] = other.data[i];
            }
            delete[] data;
            data = new_data;
            size = other.size;
        }
        return *this;
    }

    // Move Constructor (Shallow Copy + Resource Nullification)
    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr; // Nullify to prevent double destruction
    }

    // Move Assignment Operator
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data; // Free existing resource
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

---

## 4. Types of Classes & Inheritance Hierarchies

### 3.1 Abstract Base Classes & Pure Virtual Interfaces
*   **Abstract Base Class (ABC)**: Contains at least one **pure virtual function** (`virtual void func() = 0;`). Cannot be instantiated directly.
*   **Interface**: An ABC containing *only* pure virtual functions and a virtual destructor, defining a rigid behavior contract.

```cpp
class ISensor {
public:
    virtual ~ISensor() = default; // Essential for virtual hierarchies
    virtual void configure() = 0; // Pure virtual
    virtual double readData() = 0;
};
```

### 3.2 Polymorphism & The Diamond Problem (Virtual Inheritance)
If `Derived` inherits from two bases that share a common ancestor `Base`, it creates duplicate base instances unless `virtual public` inheritance is used.

```cpp
class Base {
public:
    int value;
    Base() : value(10) {}
};

class Mid1 : virtual public Base {};
class Mid2 : virtual public Base {};

class Final : public Mid1, public Mid2 {
public:
    void printValue() {
        // Without virtual inheritance, "value" is ambiguous (Mid1::value vs Mid2::value)
        std::cout << value << std::endl; 
    }
};
```

---

## 5. Value Categories: Advanced Lvalue/Rvalue Mapping

Every expression has two properties: a type and a value category.
*   **Lvalue**: An expression that has identity and cannot be safely moved (e.g., named variables, references).
*   **Prvalue**: Computations that create temporaries (e.g., math operations `a + b`, literals `10`).
*   **Xvalue**: An expiring object that can be moved (e.g., return value of `std::move(x)`).

```cpp
void process(int& x) { std::cout << "Lvalue ref" << std::endl; }
void process(int&& x) { std::cout << "Rvalue ref" << std::endl; }

int main() {
    int a = 10;
    process(a);            // Prints "Lvalue ref" (a is an lvalue)
    process(20);           // Prints "Rvalue ref" (20 is a prvalue)
    process(std::move(a)); // Prints "Rvalue ref" (std::move casts lvalue to xvalue)
}
```

---

## 6. POSIX Multi-Processing & Multithreading

### 6.1 Processes: `fork()` and `exec()`
`fork()` duplicates the calling process.
`exec()` replaces the process layout entirely.

```cpp
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

void process_spawning() {
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error
        std::cerr << "Fork failed" << std::endl;
    } else if (pid == 0) {
        // Child Process
        std::cout << "Child Process PID: " << getpid() << ", Parent PPID: " << getppid() << std::endl;
        
        // Replace memory space with /bin/ls
        char* args[] = {(char*)"ls", (char*)"-l", nullptr};
        execvp("ls", args);
        
        // This only executes if exec fails
        std::cerr << "Exec failed!" << std::endl;
        exit(1);
    } else {
        // Parent Process: wait for child to complete
        int status;
        waitpid(pid, &status, 0);
        std::cout << "Child exited. Parent resuming." << std::endl;
    }
}
```

### 6.2 POSIX Threads (`pthreads`) & Condition Variables
Condition variables permit threads to sleep until notified of state changes.

```cpp
#include <pthread.h>
#include <vector>

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
std::vector<int> task_queue;

void* producer(void* arg) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&queue_mutex);
        task_queue.push_back(i);
        std::cout << "Produced: " << i << std::endl;
        
        // Signal a waiting consumer thread
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);
        sleep(1);
    }
    return nullptr;
}

void* consumer(void* arg) {
    while (true) {
        pthread_mutex_lock(&queue_mutex);
        
        // Must use WHILE loop to prevent spurious wakeups
        while (task_queue.empty()) {
            pthread_cond_wait(&queue_cond, &queue_mutex); // Unlocks mutex, sleeps, relocks on wakeup
        }
        
        int task = task_queue.back();
        task_queue.pop_back();
        std::cout << "Consumed: " << task << std::endl;
        
        pthread_mutex_unlock(&queue_mutex);
        if (task == 4) break;
    }
    return nullptr;
}
```

### 6.3 Thread-Local Storage (`thread_local`)
The `thread_local` keyword defines a variable that is instantiated once per thread. Each thread has its own separate instance of the variable, avoiding race conditions and the need for mutex locking when tracking thread-specific statistics (e.g., tracking the count of processed requests).

```cpp
#include <iostream>
#include <thread>

void process_request() {
    // Each executing thread holds its own separate copy of request_count
    static thread_local int request_count = 0;
    request_count++;
    std::cout << "Thread " << std::this_thread::get_id() << " request count: " << request_count << std::endl;
}
```

### 6.4 Standard C++ Threads (`std::thread`)
Modern C++ (C++11 and onwards) provides standard concurrency classes. Unlike `pthread_create`, `std::thread` uses standard templates, allowing any callable entity (function, functor, lambda) to run in a separate execution flow.

#### 1. Creating and Launching Threads
When a `std::thread` object is created, it begins executing immediately.
*   **`.join()`**: Blocks the parent thread until the child thread finishes execution.
*   **`.detach()`**: Disconnects the thread object from the actual execution thread, allowing it to run independently in the background.
*   *Warning*: A `std::thread` destructor calls `std::terminate()` if the thread object is still joinable (i.e. has not been joined or detached).

```cpp
#include <iostream>
#include <thread>

void print_message(const std::string& msg, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << msg << std::endl;
    }
}

int main() {
    // Launching a thread with arguments
    std::thread t1(print_message, "Hello from thread", 5);
    
    // Launching a thread via lambda
    std::thread t2([]() {
        std::cout << "Hello from lambda thread" << std::endl;
    });

    t1.join(); // Wait for t1
    t2.join(); // Wait for t2
    return 0;
}
```

#### 2. Passing Arguments by Reference (`std::ref`)
By default, standard C++ thread constructors copy or move all arguments. If a function parameter is a reference (`T&`), passing it directly to the thread constructor will fail to compile. Wrap the reference argument in `std::ref()` to pass it by reference.

```cpp
#include <iostream>
#include <thread>
#include <functional>

void increment(int& val) {
    val++;
}

int main() {
    int num = 10;
    // std::thread t(increment, num); // Compilation Error!
    std::thread t(increment, std::ref(num)); // Correct
    t.join();
    std::cout << "num is: " << num << std::endl; // Prints 11
    return 0;
}
```

#### 3. Thread Synchronization: Mutex and Lock Guards
C++ provides `std::mutex` and RAII-based wrapper structures to automate locking/unlocking and prevent data races:
*   `std::lock_guard<std::mutex> lock(mtx)`: Locks the mutex upon construction and automatically unlocks it when the lock guard goes out of scope (destructor call).
*   `std::unique_lock<std::mutex> lock(mtx)`: Similar to `lock_guard`, but allows manual locking/unlocking, deferring locking, or transfer of ownership (needed for condition variables).

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int shared_counter = 0;

void safe_increment() {
    // Automatically locks mtx here
    std::lock_guard<std::mutex> lock(mtx);
    shared_counter++;
    // mtx is automatically unlocked when lock goes out of scope
}

int main() {
    std::vector<std::thread> pool;
    for (int i = 0; i < 10; ++i) {
        pool.emplace_back(safe_increment);
    }
    for (auto& t : pool) {
        t.join();
    }
    std::cout << "Counter: " << shared_counter << std::endl; // Prints 10
    return 0;
}
```

---

## 7. Sockets Network Architecture

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void tcp_server_blueprint(int port) {
    // 1. Create Socket File Descriptor (TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. Set Port/Address Reuse Options (Crucial for immediate restarts)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    address.sin_port = htons(port);       // Host-to-Network Short conversion
    
    // 3. Bind to specified address & port
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    
    // 4. Start listening (Backlog size 5)
    listen(server_fd, 5);
    
    // 5. Block until client connects
    socklen_t addr_len = sizeof(address);
    int client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    
    // 6. Read/Write payload
    char buffer[1024] = {0};
    read(client_fd, buffer, 1024);
    
    const char* response = "ACK";
    write(client_fd, response, strlen(response));
    
    // 7. Cleanup
    close(client_fd);
    close(server_fd);
}
```

---

## 8. Google Test (gtest) & TDD

Test assertions ensure your design matches exact requirements:
*   `EXPECT_EQ(a, b)`: Checks if `a == b`. Execution continues on failure.
*   `ASSERT_EQ(a, b)`: Checks if `a == b`. Execution halts on failure.
*   `EXPECT_THROW(expr, type)`: Verifies expression throws specific exception type.

```cpp
#include <gtest/gtest.h>

class DatabaseConnection {
public:
    bool isOpen() { return true; }
    int executeQuery(const std::string& query) {
        if (query.empty()) throw std::invalid_argument("Empty Query");
        return 1;
    }
};

// Test Suite Fixture
class DatabaseTest : public ::testing::Test {
protected:
    DatabaseConnection* db;

    void SetUp() override {
        db = new DatabaseConnection();
    }

    void TearDown() override {
        delete db;
    }
};

TEST_F(DatabaseTest, CheckConnectionState) {
    ASSERT_TRUE(db->isOpen());
}

TEST_F(DatabaseTest, QueryErrorValidation) {
    EXPECT_THROW(db->executeQuery(""), std::invalid_argument);
}
```


---

## 9. C++ Regular Expressions & Capturing Groups (`std::regex`)

C++ provides the `<regex>` library to validate input structures, search substrings, and extract pattern subsets.

### 9.1 Match & Search API
*   `std::regex_match`: Returns `true` only if the **entire** input string matches the regular expression pattern.
*   `std::regex_search`: Returns `true` if **any substring** inside the input matches the pattern.
*   `std::smatch` (string-based) or `std::cmatch` (character array-based): Holds the match result collections.

### 9.2 Capturing Groups Mechanism
Capturing groups are defined by enclosing portions of a regular expression pattern in parentheses `(...)`.
*   **Result layout**: When `regex_match` or `regex_search` populates a `std::smatch` object (e.g., `results`), it acts as a collection:
    *   `results[0]`: Contains the **entire matched string**.
    *   `results[1]`: Contains the substring captured by the **first group** `(...)`.
    *   `results[2]`: Contains the substring captured by the **second group** `(...)`, and so on.
    *   `results.size()`: Returns `1 + number_of_capturing_groups`.

```cpp
#include <iostream>
#include <string>
#include <regex>

void parse_sensor_input(const std::string& input) {
    // Regex pattern matching: Word, spaces, and integer.
    // e.g., "temp_sensor 42"
    // Capture groups: Group 1 matches sensor name, Group 2 matches data value.
    std::regex pattern("([a-zA-Z0-9_]+)\\s+(\\d+)");
    std::smatch matches;

    if (std::regex_match(input, matches, pattern)) {
        // matches[0] is the entire match (e.g., "temp_sensor 42")
        std::string sensor_name = matches[1].str(); // Group 1 capture
        std::string sensor_value = matches[2].str(); // Group 2 capture
        
        int val = std::stoi(sensor_value);
        std::cout << "Parsed Sensor: " << sensor_name << " with Value: " << val << std::endl;
    } else {
        std::cout << "Invalid input format!" << std::endl;
    }
}

int main() {
    parse_sensor_input("pressure_valve_3   120"); // Success
    parse_sensor_input("invalid_data");             // Fails
    return 0;
}
```
```
