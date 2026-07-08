# Comprehensive Mock Exam 4 — Official Solution Key

---

## 📝 Question 1: C++ POSIX Sockets & Resource Management (40 Points)

### Section A: Header Declarations (`SocketGuard.h`)
```cpp
#ifndef SOCKET_GUARD_H
#define SOCKET_GUARD_H

class SocketGuard {
private:
    int socket_fd;

public:
    explicit SocketGuard(int fd = -1);
    ~SocketGuard();

    // Disable copy mechanics
    SocketGuard(const SocketGuard&) = delete;
    SocketGuard& operator=(const SocketGuard&) = delete;

    // Enable move semantics
    SocketGuard(SocketGuard&& other) noexcept;
    SocketGuard& operator=(SocketGuard&& other) noexcept;

    int get() const { return socket_fd; }
    void release();
};

#endif // SOCKET_GUARD_H
```

### Section B: Implementation (`SocketGuard.cpp` & Server Functions)

#### `SocketGuard.cpp`
```cpp
#include "SocketGuard.h"
#include <unistd.h>
#include <utility>

SocketGuard::SocketGuard(int fd) : socket_fd(fd) {}

SocketGuard::~SocketGuard() {
    release();
}

void SocketGuard::release() {
    if (socket_fd != -1) {
        close(socket_fd);
        socket_fd = -1;
    }
}

// Move constructor
SocketGuard::SocketGuard(SocketGuard&& other) noexcept : socket_fd(other.socket_fd) {
    other.socket_fd = -1; // Nullify source
}

// Move assignment
SocketGuard& SocketGuard::operator=(SocketGuard&& other) noexcept {
    if (this != &other) {
        release(); // Close existing socket
        socket_fd = other.socket_fd;
        other.socket_fd = -1;
    }
    return *this;
}
```

#### `Server.cpp` (POSIX Sockets)
```cpp
#include "SocketGuard.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

void run_server(int port) {
    int raw_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (raw_fd < 0) return;
    
    // Wrap immediately in our RAII guard
    SocketGuard server_guard(raw_fd);

    int opt = 1;
    setsockopt(server_guard.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_guard.get(), (struct sockaddr*)&address, sizeof(address)) < 0) return;
    if (listen(server_guard.get(), 5) < 0) return;

    socklen_t addr_len = sizeof(address);
    int client_raw_fd = accept(server_guard.get(), (struct sockaddr*)&address, &addr_len);
    if (client_raw_fd >= 0) {
        SocketGuard client_guard(client_raw_fd);
        // Process client communication
    }
    // Both raw sockets are safely closed automatically via destructors
}
```

### Section C: TDD Google Test Suite (`Socket_Test.cpp`)
```cpp
#include <gtest/gtest.h>
#include "SocketGuard.h"

TEST(SocketGuardTest, MoveConstructorNullifiesSource) {
    SocketGuard g1(10);
    SocketGuard g2(std::move(g1));

    EXPECT_EQ(g2.get(), 10);
    EXPECT_EQ(g1.get(), -1); // Verifies source is nullified
}

TEST(SocketGuardTest, MoveAssignmentClosesExisting) {
    SocketGuard g1(20);
    SocketGuard g2(30);

    g2 = std::move(g1);
    EXPECT_EQ(g2.get(), 20);
    EXPECT_EQ(g1.get(), -1);
}
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `AndroidManifest.xml`)
*   **Filename:** `AndroidManifest.xml`
*   **Line / Code Block:** Missing `<uses-permission>` declaration.
*   **Mechanics of Flaw:** To execute any HTTP request or web-socket queries, Android requires explicitly declaring Internet permissions in the manifest file. If missing, Android security manager triggers a runtime `SecurityException` crashing the networking pipeline.
*   **Corrected Statement:** Add the permission tag outside the `<application>` block:
    ```xml
    <uses-permission android:name="android.permission.INTERNET" />
    ```

### Bug 2 (Component B - `adapters/CustomAdapter.java`)
*   **Filename:** `adapters/CustomAdapter.java`
*   **Line / Code Block:** Line 14: `AlertDialog.Builder builder = new AlertDialog.Builder(getContext());`
*   **Mechanics of Flaw:** Builder context mismatch. The adapter was instantiated using `getApplicationContext()`. An application context does not have a window token associated with it, which is required to inflate modal dialog layouts. Spawning a dialog using the application context throws a `WindowManager$BadTokenException`. An **Activity Context** must be passed.
*   **Corrected Statement:** Ensure the activity instance is passed as context to the adapter constructor:
    ```java
    // Initialize inside MainActivity:
    CustomAdapter adapter = new CustomAdapter(MainActivity.this, android.R.layout.simple_list_item_1);
    ```

### Bug 3 (Component C - `controllers/userController.js`)
*   **Filename:** `controllers/userController.js`
*   **Line / Code Block:** Line 8: `const userId = req.params.id;`
*   **Mechanics of Flaw:** Route parameter mismatch. The route was declared as `/api/users/:userId`. This means the parameter is captured on `req.params.userId`. Reading `req.params.id` returns `undefined`, looking up `users[undefined]` and returning a blank response.
*   **Corrected Statement:**
    ```javascript
    const userId = req.params.userId;
    ```

### Bug 4 (Component D - `styles.js`)
*   **Filename:** `styles.js`
*   **Line / Code Block:** Line 11: `height: '60px',`
*   **Mechanics of Flaw:** In React Native styling, size dimensions must be unitless integers (representing density-independent pixels / dp). Passing string units (like `"60px"`) will cause the compiler layout engine to throw an error and crash the app at startup.
*   **Corrected Statement:**
    ```javascript
    height: 60,
    ```

---

## 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

### Section A: C++ RVO Constructor Logs
*   **Predicted Output (with -fno-elide-constructors):**
    ```
    CMoveMoveDDD
    ```
*   **Step-by-step Construction & Destruction Trace:**
    1.  `main` calls `makeContainer()`.
    2.  `makeContainer()` evaluates `Container()`, constructing a temporary object: prints `"C"`.
    3.  `makeContainer()` returns the object. With `-fno-elide-constructors`, returning creates a move-constructed temporary return object: prints `"Move"`.
    4.  The local variable inside `makeContainer()` is destroyed: prints `"D"`.
    5.  Back in `main`, `c` is initialized using the temporary return object via move construction: prints `"Move"`.
    6.  The temporary return object is destroyed: prints `"D"`.
    7.  `main` finishes, and `c` is destroyed: prints `"D"`.
    *   *Total String*: `CMoveMoveDDD`.

### Section B: JavaScript async/await Microtasks Priority
*   **Predicted Output Sequence:**
    ```
    4
    1
    3
    6
    8
    2
    7
    5
    ```
*   **Microtask/Macrotask Step-by-step Tracing:**
    1.  Logs `"4"` (Synchronous).
    2.  `setTimeout(5)` is pushed to the Macrotask queue.
    3.  `first()` is invoked. Logs `"1"`.
    4.  `await second()` suspends `first()`. `second()` runs synchronously and logs `"3"`.
    5.  The remainder of `first()` (logging `"2"`) is queued as a microtask.
    6.  `Promise(6)` constructor body runs synchronously: logs `"6"`. `.then(...)` (logging `"7"`) is queued as a microtask.
    7.  Logs `"8"` (Synchronous).
    8.  Stack clears. Execute Microtask queue:
        - Logs `"2"`.
        - Logs `"7"`.
    9.  Stack clears. Pull from Macrotask queue:
        - Logs `"5"`.

---

## 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

### Section A: Polymorphic Memory Layout & VTABLEs
1.  `1` (Each object instance of a polymorphic class holds exactly one hidden pointer to its class VTABLE, called `vptr`).
2.  `No` (Non-virtual member functions are resolved statically by the compiler at compile-time and are not entered into the VTABLE).
3.  *VTABLE Mappings*:
    *   `VTABLE[0]`: `Base::funcA`
    *   `VTABLE[1]`: `Derived::funcB` (Overrides Base)
    *   `VTABLE[2]`: `Derived::funcD` (New virtual method)

### Section B: Dockerfile Optimization
*   **Slow Cache Explanation:** In the unoptimized Dockerfile, `COPY . .` is placed *before* `RUN npm install`. This means any change to *any* source file voids the Docker build cache for the subsequent layers, forcing the builder to re-download and re-install all `npm` dependencies on every single build.
*   **Optimized Dockerfile:**
```dockerfile
FROM node:18-alpine
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
CMD ["node", "server.js"]
```

---

## 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

### Section A: Android Custom Toast Layouts
*   **Layout Inflation Explanation:** Inflation refers to the process of parsing an XML layout file (which defines view hierarchies statically) and instantiating it into active memory-backed Java View objects at runtime.
*   **Java Custom Toast Snippet:**
```java
// Inflate XML layout using inflater context
LayoutInflater inflater = getLayoutInflater();
View layout = inflater.inflate(R.layout.toast_notification, null);

// Access children views inside layout
TextView text = layout.findViewById(R.id.msg_text);
text.setText("Warning: Sensor Overheat!");

// Instantiate Toast container
Toast toast = new Toast(getApplicationContext());
toast.setDuration(Toast.LENGTH_LONG);
toast.setView(layout); // Inject custom view hierarchy
toast.show();
```

### Section B: Web Cookie Protection Directives
*   **`HttpOnly`**: Blocks client-side Javascript scripts (like `document.cookie`) from reading the cookie value. This mitigates Session Hijacking risks caused by Cross-Site Scripting (XSS) attacks.
*   **`Secure`**: Enforces the browser to transmit the cookie *only* over encrypted connections (HTTPS). This prevents intercepting the session token via Man-in-the-Middle (MITM) attacks on public networks.
*   **`SameSite`**: Restricts the browser from attaching the cookie to cross-site HTTP requests (values: `Strict` or `Lax`). This acts as the primary mitigation directive against Cross-Site Request Forgery (CSRF) vulnerabilities.
