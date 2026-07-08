# Bar-Ilan University — Advanced Systems Programming (893210)
## Comprehensive Mock Exam 5 (Practice Session)

**Time Allowed:** 3 Hours
**Total Points:** 100

---

### 📝 Question 1: Express, Mongoose MVC & React Lifecycle (40 Points)

#### Scenario
You are required to build a simple **Task Allocation Portal** in JavaScript, consisting of an Express.js backend REST API, a MongoDB/Mongoose database model, and a React frontend page.

The backend schema must enforce:
*   `Task` has a `title` (required, trimmed), `status` (enum: `['Pending', 'Completed']`), and `assignedUser` representing a reference `ObjectId` pointing to a `User` collection.
*   The Express API endpoint `POST /api/tasks` must validate the request payload and save the task in the database.

The frontend React page must:
*   Fetch the tasks list from `/api/tasks` upon component mount using `useEffect` (handling cleanups for slow responses via active state checks).
*   Provide a form submitting new tasks to the backend.

#### Architectural & Grading Constraints
1.  **Immutability:** React frontend state updates must treat variables as read-only, using immutable spreads.
2.  **Mongoose Reference Mapping:** Your schema definition must demonstrate explicit collection linking using `ref`.
3.  **Endpoint Integration:** Write an Express integration test structure verifying that requests containing missing parameters return a `400 Bad Request` status.

---

### 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

Locate and document **exactly 4 distinct bugs** across the C++, Java, MongoDB, and Python socket client code blocks below.

#### Component A: C++ Race Condition (`dataStore.cpp`)
```cpp
#include <vector>
#include <thread>
#include <iostream>

std::vector<int> data_store;

void insert_data(int val) {
    // Spawning 10 concurrent threads running this function
    data_store.push_back(val); // Line 9
}
```

#### Component B: Java Static Context Memory Leak (`utils/TelemetryManager.java`)
```java
package com.biu.telemetry;

import android.content.Context;

public class TelemetryManager {
    // Static reference to context
    private static Context appContext; // Line 7

    public static void init(Context context) {
        appContext = context; // Line 10
    }
}
```

#### Component C: MongoDB Aggregation Projection (`queries/query.js`)
```javascript
// Database holds collection: users { name: "Berger", grade: 85 }
// Question: return only names, suppressing grade and _id:
db.users.find({}, { name: 1, grade: 0, _id: 0 }); // Line 3
```

#### Component D: Python Network Client (`client.py`)
```python
import socket

def send_payload():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 8080))
    # Payload is sent to C++ server
    s.send("Core System Ready") # Line 7
    s.close()
```

---

### 🧠 Question 3: Code Tracing & Exact Output Prediction (20 Points)

#### Section A: POSIX Nested Fork Loops (10 Points)
Trace the process spawning hierarchy and predict the **number of lines printed** by this C++ program:

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 2; i++) {
        if (fork() == 0) {
            fork();
        }
    }
    std::cout << "Node" << std::endl;
    return 0;
}
```

#### Section B: JavaScript Scoping, Closures, & Hoisting (10 Points)
Predict the exact console output sequence of the code segment below:

```javascript
var count = 5;

function runScope() {
    console.log("initial count:", count);
    
    var count = 10;
    
    function inner() {
        count++;
        console.log("inner count:", count);
    }
    
    return inner;
}

const trigger = runScope();
trigger();
console.log("global count:", count);
```

---

### 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

#### Section A: Git Object Store Graph (5 Points)
Complete the hashing layout inside the `.git/objects/` store after running the commands below:

```bash
git init
echo "version 1" > a.txt
git add a.txt
git commit -m "Initial commit"
```
When running these operations:
1.  Git creates a **[1]________** object containing the text "version 1" compressed via `zlib`.
2.  The hash of the commit object is determined by calculating the **[2]________** hash of its contents.
3.  The directory structure inside `.git/objects/` uses the first **[3]________** characters of the hash as the folder name, and the remaining **[4]________** characters as the filename.
4.  The commit object holds a reference pointing to a **[5]________** object that maps `a.txt` to its blob hash.

#### Section B: UDP Lifecycle Connectionless States (5 Points)
Fill in the missing blocks (1-5) representing connectionless UDP flow:
```
      SERVER                          CLIENT
  +------------+                  +------------+
  |  socket()  |                  |  socket()  |
  +------------+                  +------------+
        |                               |
  +----[1]-----+                        |
  |  ________  |                        |
  +------------+                        |
        |                               |
  +----[2]-----+                  +----[4]-----+
  |  ________  | <==============> |  ________  |
  +------------+                  +------------+
        |                               |
  +----[3]-----+                  +----[5]-----+
  |  ________  |                  |  ________  |
  +------------+                  +------------+
```

---

### 📱 Question 5: Mobile Development & Web Security Theory (10 Points)

#### Section A: Android LiveData Threading Boundaries (5 Points)
State the threading constraints when modifying LiveData values. Write a Java code block showing how a background thread safely updates a `MutableLiveData<Integer>` variable.

#### Section B: CSRF Prevention & Token Security (5 Points)
Explain Cross-Site Request Forgery (CSRF). How do anti-CSRF tokens passed inside custom request headers validate requests where standard cookie verification fails?
