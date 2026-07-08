# Bar-Ilan University — Advanced Systems Programming (893210)
## Comprehensive Mock Exam 4 (Practice Session)

**Time Allowed:** 3 Hours
**Total Points:** 100

---

### 📝 Question 1: C++ POSIX Sockets & Resource Management (40 Points)

#### Scenario
You are required to build a simple **Socket Listener Engine** in C++. The engine sets up a local TCP socket server using the POSIX sockets API.

To ensure safety and prevent resource leaks, you must implement an RAII wrapper class called **SocketGuard** that binds a socket file descriptor and guarantees it will be closed automatically via its destructor when the guard goes out of scope.

#### Architectural & Grading Constraints
1.  **File Separation:** Keep declaration (`.h`) and implementation (`.cpp`) files separate.
2.  **RAII Resource Ownership:** The `SocketGuard` class must manage the lifetime of the socket file descriptor (`int fd`). It must disable copying (copy constructor & copy assignment) to prevent double-closing issues, but it should support ownership transfers via **move semantics** (move constructor & move assignment).
3.  **POSIX Socket Setup:** Implement a server function `void run_server(int port)` that creates a TCP socket (`AF_INET`, `SOCK_STREAM`), sets options to reuse address/ports, binds, listens, and accepts a single incoming connection.
4.  **TDD Google Test Suite:** Write unit tests using **Google Test** verifying:
    *   `SocketGuard` correctly moves ownership and nullifies the source object descriptor.
    *   The socket descriptor is closed correctly on destruction (mocking or checking active descriptor validity).

---

### 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

Locate and document **exactly 4 distinct bugs** across the Android Manifest, Android adapter, Express route handler, and React Native style code segments below.

#### Component A: Android Manifest (`AndroidManifest.xml`)
```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.biu.telemetry">

    <application
        android:allowBackup="true"
        android:label="Telemetry App"
        android:theme="@style/Theme.AppCompat">
        
        <activity android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

    <!-- App performs network fetches to an Express server -->
</manifest>
```

#### Component B: Android Dialog Adapter (`adapters/CustomAdapter.java`)
```java
package com.biu.telemetry;

import android.app.AlertDialog;
import android.content.Context;
import android.view.View;
import android.widget.ArrayAdapter;

public class CustomAdapter extends ArrayAdapter<String> {
    public CustomAdapter(Context context, int resource) {
        super(context, resource);
    }

    public void showDeleteConfirm() {
        // App crashed here:
        AlertDialog.Builder builder = new AlertDialog.Builder(getContext()); // Line 14
        builder.setTitle("Confirm Delete")
               .setMessage("Are you sure?")
               .show();
    }
}
// Note: CustomAdapter was initialized in MainActivity via:
// CustomAdapter adapter = new CustomAdapter(getApplicationContext(), android.R.layout.simple_list_item_1);
```

#### Component C: ExpressJS API Controller (`controllers/userController.js`)
```javascript
const express = require('express');
const router = express.Router();

const users = { "123": { name: "Berger" } };

// Retrieve user by ID
router.get('/api/users/:userId', (req, res) => {
    const userId = req.params.id; // Line 8
    const user = users[userId];
    res.status(200).json(user);
});

module.exports = router;
```

#### Component D: React Native Layout StyleSheet (`styles.js`)
```javascript
import { StyleSheet } from 'react-native';

export const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: '#fff',
        alignItems: 'center'
    },
    headerBox: {
        width: '100%',
        height: '60px', // Line 11
        paddingTop: 10
    }
});
```

---

### 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

#### Section A: C++ RVO Constructor Logs (10 Points)
Trace the execution and predict the exact console output of this C++ program if compiled with the flag:
`g++ -fno-elide-constructors main.cpp`

```cpp
#include <iostream>
using namespace std;

class Container {
public:
    Container() { cout << "C"; }
    Container(const Container&) { cout << "Copy"; }
    Container(Container&&) noexcept { cout << "Move"; }
    ~Container() { cout << "D"; }
};

Container makeContainer() {
    return Container();
}

int main() {
    Container c = makeContainer();
    return 0;
}
```

#### Section B: JavaScript async/await Microtasks Priority (10 Points)
Predict the exact console output sequence of the asynchronous operations script below:

```javascript
async function first() {
    console.log(1);
    await second();
    console.log(2);
}

async function second() {
    console.log(3);
}

console.log(4);

setTimeout(() => {
    console.log(5);
}, 0);

first();

new Promise((resolve) => {
    console.log(6);
    resolve();
}).then(() => {
    console.log(7);
});

console.log(8);
```

---

### 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

#### Section A: Polymorphic Memory Layout & VTABLEs (5 Points)
Trace the virtual table structures of the classes defined below. Draw or fill in the VTABLE offset mappings:

```cpp
class Base {
public:
    virtual void funcA() { std::cout << "Base A\n"; }
    virtual void funcB() { std::cout << "Base B\n"; }
    void funcC() {} // Non-virtual
};

class Derived : public Base {
public:
    void funcB() override { std::cout << "Derived B\n"; }
    virtual void funcD() { std::cout << "Derived D\n"; }
};
```
1.  How many `vptr` pointers exist inside an instantiated object of class `Derived`?
2.  Does `funcC()` have an entry in the VTABLE?
3.  Write the class names responsible for the functions executing at each offset of `Derived`'s VTABLE:
    *   `VTABLE[0]`: **________**::funcA()
    *   `VTABLE[1]`: **________**::funcB()
    *   `VTABLE[2]`: **________**::funcD()

#### Section B: Dockerfile Optimization (5 Points)
Explain why the Dockerfile layout in **Unoptimized** leads to slow build caches, and write the **Optimized** version by altering the instruction order:

```dockerfile
# UNOPTIMIZED
FROM node:18-alpine
WORKDIR /app
COPY . .
RUN npm install
CMD ["node", "server.js"]
```
```dockerfile
# OPTIMIZED (Write below)
```

---

### 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

#### Section A: Android Custom Toast Layouts (5 Points)
Explain the mechanism of layout inflation in Android development. Write a Java code snippet showing how to inflate a custom XML layout named `toast_notification.xml` (containing a `TextView` with ID `msg_text`) and bind it to a `Toast` object.

#### Section B: Web Cookie Protection Directives (5 Points)
Explain how the `HttpOnly`, `Secure`, and `SameSite` flags configured on HTTP session cookies protect against specific client-side security vulnerabilities.
