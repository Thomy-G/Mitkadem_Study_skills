Here is another highly realistic, structured mock exam paper modeled after the Bar-Ilan University curriculum guidelines, past tests, and the `סיכום יאיר` master study notes.

This can be copy-pasted directly into your Antigravity agent's simulation vault as a third reference point.

---

### File 3: `Exam_2025_Moed_Alef_Full_Spec.md`

```markdown
# Bar-Ilan University — Advanced Systems Programming (893210)
## Targeted Reconstruction: Semester B, Moed Alef (2025)

---

### Question 1: Full-Stack MVC Labels Engine (35 Points)

#### Description
You are tasked with developing a backend internet microservice using Node.js, Express, and Mongoose that implements a core sub-segment of a stateless Gmail-style workflow management infrastructure. The server must manage data models representing user-defined classification objects known as **Labels** (used to categorize and organize message payloads).

The service must strictly adhere to the RESTful API uniform interface specification, communicating exclusively via JSON data payloads. You must implement the following server routing table mappings completely:

1. `GET /api/labels`
   - Retrieves a full structural collection of all active Label documents stored in the database.
   - Returns an HTTP status code of `200 OK` paired with the JSON array payload.

2. `POST /api/labels`
   - Generates a brand-new label instance utilizing parameters passed inside the incoming request body JSON (`req.body`).
   - Returns an HTTP status code of `201 Created` along with the newly initialized document object.

3. `GET /api/labels/:id`
   - Locates a specific label matching the unique parameter ID.
   - If the label exists, returns `200 OK` with the matching resource. If no matching ID is found, terminates the query chain immediately and returns an explicit `404 Not Found` JSON error status block.

4. `PATCH /api/labels/:id`
   - Modifies the data fields of an existing label matching the parameterized ID.
   - Returns `200 OK` alongside the newly updated resource properties.

5. `DELETE /api/labels/:id`
   - Removes the specified label resource permanently from the persistent MongoDB instance.
   - Returns a successful stateless HTTP validation status code of `204 No Content`.

#### Architectural & Grading Constraints
To pass the strict automated grading parameters, your codebase structure must showcase explicit division across decoupled architectural file layers:
* **`app.js` (The Infrastructure Master Class):** Houses global configurations, mounts imported routing modules, and establishes connections to the MongoDB server using connection strings pulled securely from environment variables.
* **`.env` (Environment Configurations):** A dedicated configuration layout storing critical variables like `CONNECTION_STRING` and target operational `PORT` mappings. Your code must prevent hard-coded environment leakage.
* **Routes Layer (`routes/label.js`):** Extracts raw parameter lines and maps operations cleanly across distinct HTTP verbs.
* **Controllers Layer (`controllers/label.js`):** Intercepts request payloads, handles parameter validation loops, calls underlying services asynchronously, and sends correct HTTP status status headers.
* **Models Layer (`models/label.js`):** Outlines data schemas cleanly using Mongoose Schema constraints.

---

### Question 2: Sockets API & Concurrent Systems Design (35 Points)

#### Description
Write a complete production-grade C++ server application using low-level Unix Sockets API syscalls that acts as a multithreaded network service processing incoming socket streams.

The server must bind to local port `8080` and run an infinite listening loop. Every time an external client connection is negotiated via `accept()`, the master server process thread must **not** pause or handle it inline. Instead, it must handle concurrency using one of these two operational modes (describe or code both clearly):

* **Operational Variant A (Process-per-Client):** Spawns an isolated child process via `fork()` to handle stream operations completely independently. The parent process must immediately cycle back to listen for subsequent connections. You must explicitly prevent the accumulation of resource-draining "Zombie Processes" by correctly implementing termination synchronization mechanics (`wait` or `waitpid`).
* **Operational Variant B (Thread-Pool/Pthreads Execution):** Details an architecture where client socket descriptions are handed off to a thread processing routine. 

#### Structural & Operational Mandates
* **Port Lock Prevention:** You must include explicit socket configuration setup descriptors (`setsockopt` paired with the `SO_REUSEADDR` token flag) immediately following the primary `socket()` allocation call. This ensures that if the server process terminates abruptly, it can override kernel `TIME_WAIT` blocks and re-bind to port `8080` instantly upon rebooting.
* **Graceful Lifecycle Management:** Ensure that every file descriptor allocation (socket handles) is closed properly when its respective stream processing sequence reaches completion to prevent system resource leaks.

---

### Question 3: Structural Design Patterns Validation (10 Points)

A systems engineer attempts to apply the **Proxy Design Pattern** in C++ to add lazy-initialization logic to a memory-expensive graphics object class layout. Analyze their proposed implementation framework code below:

```cpp
#include <iostream>

class VectorGraphic {
public:
    void render() { std::cout << "Processing complex vector canvas render loops...\n"; }
};

class GraphicProxy {
private:
    VectorGraphic* realSubject = nullptr;
public:
    ~GraphicProxy() { delete realSubject; }
    
    void render() {
        if (realSubject == nullptr) {
            realSubject = new VectorGraphic();
        }
        realSubject->render();
    }
};

int main() {
    GraphicProxy proxy;
    proxy.render();
    return 0;
}

```

Evaluate this structural design against strict design pattern tenets and object-oriented principles. Identify **exactly two architectural or structural flaws** that prevent this code from being a valid, resilient production-grade Proxy pattern. Explain the mechanism behind each flaw and provide the exact refactoring adjustments required to resolve them.

---

### Question 4: Advanced Code Tracing Sheets (20 Points)

#### Section A: Single-Threaded JavaScript Promises & Event-Loop Priority

Predict the exact console output log printing sequence produced when the JavaScript V8 compilation engine executes the following asynchronous sequence block.

Clearly outline how the engine schedules tasks across the **Call Stack**, the high-priority **Microtask Queue** (Promises/`.then()`), and the macro **Task/Callback Queue** (`setTimeout`).

```javascript
console.log('1: Initialization Engine');

setTimeout(() => {
    console.log('2: Macro Task Triggered');
}, 0);

Promise.resolve()
    .then(() => {
        console.log('3: Micro Task Alpha');
    })
    .then(() => {
        console.log('4: Micro Task Beta');
    });

async function executeAsync() {
    console.log('5: Step Inside Async Line');
    await Promise.resolve();
    console.log('6: Resuming Post-Await Context');
}

executeAsync();
console.log('7: Final Line Reached');

```

#### Section B: C++ Pre/Post Operator Overloading Mechanics

Trace the operations of the memory registries step-by-step and provide the exact final console line print output.

```cpp
#include <iostream>

class Metric {
public:
    int score;
    Metric(int s) : score(s) {}

    // Operator 1
    Metric& operator++() {
        score += 2;
        return *this;
    }

    // Operator 2
    Metric operator++(int) {
        Metric originalState = *this;
        score += 2;
        return originalState;
    }
};

int main() {
    Metric m1(10);
    Metric m2 = ++m1;
    Metric m3 = m1++;

    std::cout << m1.score << " " << m2.score << " " << m3.score << std::endl;
    return 0;
}

```

```

```
