# Practice Set 2: Exam Simulation Questions

This practice set contains 4 exam-style questions modeled after the Bar-Ilan University Advanced Systems Programming (893210) curriculum.

---

## 🏛️ Question 1: C++ Architectural Design (40 Points)
You are required to implement a command-line Command Stream Processor in C++ that processes integers, manages a FIFO buffer, and filters data inputs.

The application reads continuously from standard input (`std::cin`) line-by-line. The first token determines the operation. The application must support the following three commands:

1. `push [value]`
   - Adds the integer `[value]` to the end of a shared FIFO buffer.
   - If a filter threshold is active (see `filter` below) and `[value]` is strictly less than the filter threshold, it must be ignored silently.
   - If successfully added, print: `Value [value] pushed` to `std::cout`.
   
2. `pop`
   - Removes and retrieves the oldest integer from the FIFO buffer.
   - If successfully retrieved, print: `Value [value] popped` to `std::cout`.
   - If the buffer is empty, print: `Buffer empty` to `std::cout`.

3. `filter [min_val]`
   - Sets a minimum value threshold filter (integer).
   - This command produces no output.
   - The initial threshold filter must be read from an environment variable `DEFAULT_MIN_FILTER` upon system start. If the variable is missing or cannot be parsed as an integer, no filter is active initially.

### Architectural & Grading Constraints:
* **Separation of Concerns**: Isolate class declarations (`.h` files with include guards) from class implementations (`.cpp` files).
* **Open-Closed Principle (OCP)**: Adding new commands (e.g. `clear`, `size`, `peek`) in the future must not require modifying the central event dispatcher loop. Use the **Command Pattern** with a command registry mapping.
* **Modern Memory Management**: Manual allocations (`new`/`delete`) are strictly forbidden. Use smart pointers (`std::unique_ptr` or `std::shared_ptr`).
* **Robustness**: Unrecognized command verbs or invalid parameters must be ignored silently.

*Starter files are created under [Archetype1_CPP/](Archetype1_CPP/). Implement your solution across those files.*

---

## 💻 Question 2: Node.js/Express/Mongoose MVC Debugging (20 Points)
The Express.js backend application under [Archetype2_MVC/buggy_mvc/](Archetype2_MVC/buggy_mvc/) is designed to manage a stateless class booking system. However, the application is broken.

Analyze the codebase under [buggy_mvc/](Archetype2_MVC/buggy_mvc/). Identify and document **exactly 4 distinct compilation or runtime bugs**. For each bug:
1. Specify the filename and line number (or code snippet).
2. Explain the underlying mechanics of why it breaks the application.
3. Provide the exact corrected code statement.

---

## 🔍 Question 3: Code Execution Tracing & Scope Analysis (20 Points)

### Section A: C++ Polymorphism & Virtual Destructors (10 Points)
Predict the exact console output of compiling and executing the following C++ program. Explain the resource cleanup sequence and point out if there are any memory leaks or undefined behaviors.

```cpp
#include <iostream>
using namespace std;

class Device {
public:
    Device() { cout << "Device initialized" << endl; }
    ~Device() { cout << "Device destroyed" << endl; }
};

class Smartphone : public Device {
private:
    int* battery_level;
public:
    Smartphone() { 
        battery_level = new int(100); 
        cout << "Smartphone initialized" << endl; 
    }
    ~Smartphone() { 
        delete battery_level; 
        cout << "Smartphone destroyed" << endl; 
    }
};

int main() {
    Device* dev = new Smartphone();
    delete dev;
    return 0;
}
```

### Section B: JavaScript Closure Scoping & Loops (10 Points)
Predict the exact console output and timing sequence when executing the following JavaScript snippet. Explain the difference between `var` and `let` scope bindings inside loop structures in relation to closures and the event loop.

```javascript
for (var i = 1; i <= 3; i++) {
    setTimeout(function() {
        console.log('var index:', i);
    }, i * 1000);
}

for (let j = 1; j <= 3; j++) {
    setTimeout(function() {
        console.log('let index:', j);
    }, j * 1000);
}
```

---

## 🛠️ Question 4: DevOps, Docker & Git Low-Level Internals (20 Points)

### Section A: Docker Multi-Stage Builds (10 Points)
Analyze the following unoptimized `Dockerfile` for a compiled Go API server:

```dockerfile
FROM golang:1.20
WORKDIR /app
COPY . .
RUN go build -o main .
EXPOSE 8080
CMD ["./main"]
```

1. Explain why this image is unoptimized for a production environment (mentioning security, layer footprint, and image size).
2. Rewrite this configuration utilizing **Multi-Stage Builds** to produce a secure, lightweight final container image.

### Section B: Git Internal Reference Files (10 Points)
A developer runs the following commands inside an active Git repository on branch `main`:

```bash
$ git checkout -b feature-auth
$ echo "auth logic" > auth.txt
$ git add auth.txt
$ git commit -m "add authentication"
$ git tag -a v1.0 -m "stable release"
```

Explain exactly how the Git metadata tracking files inside `.git/refs/` change:
1. Describe the changes, paths, and contents of the files under `.git/refs/heads/`.
2. Describe the changes, paths, and contents of the files under `.git/refs/tags/`.
3. Explain the relationship between the ref file contents and Git commit objects.
