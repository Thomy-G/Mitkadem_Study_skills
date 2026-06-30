Here are two highly accurate Markdown files that model the exact structure, technical layout, and common pitfalls found in your past university exams.

You can upload these directly to your agent's knowledge base or feed them into its system prompt so it knows how to generate identical test problems.

---

### File 1: `Exam_2026_Moed_A_Sample.md`

```markdown
# Bar-Ilan University — Advanced Systems Programming (893210)
## Restructured Reconstruction: Semester A, Moed Alef (2026)

---

### Question 1: C++ CLI System Design (40 Points)

#### Description
You are required to implement a Command Line Interface (CLI) system in C++ designed to manage text streams, enforce processing transformations, and persist payloads into separate file blocks. 

The application must read from standard input (`std::cin`) continuously and split incoming lines into tokens. The first token determines the operation. The application must support the following three commands:

1. `rev [file name] [text]`
   - Creates a new file named `[file name]`.
   - The file contents must be the provided `[text]` completely reversed (e.g., input `abc` becomes `cba`).
   - The target directory destination path where files are written must be dynamically loaded from an environment variable named `TEXT_PROCESSING_DIR`. If the variable is missing, fallback to the current local directory (`.`).
   - This command produces no terminal console output.

2. `double [file name] [text]`
   - Creates a new file named `[file name]`.
   - The file contents must be the provided `[text]` with every individual character duplicated inline (e.g., input `abc` becomes `aabbcc`).
   - Saved to the directory defined by `TEXT_PROCESSING_DIR`.
   - This command produces no terminal console output.

3. `display [file name]`
   - Reads the specified file from `TEXT_PROCESSING_DIR` and prints its exact contents directly to the standard output (`std::cout`).

#### Architectural & Grading Constraints
To receive full credit, you must adhere to the following software engineering design mandates:
* **Strict Separation of Concerns:** Your solution must completely isolate its processing logic from the main interaction loop. You must explicitly design an abstract interface base class or separate class structures. Describe your implementation file architecture clearly (`.h` vs `.cpp` distribution frameworks).
* **Open-Closed Principle (OCP):** The addition of future string-transformation verbs must not require modifying your central command dispatcher block or processing loop. Use an appropriate design pattern (e.g., the Command Pattern paired with an associative tracking map).
* **Modern Memory Protocols:** Manual allocations (`new`/`delete`) are forbidden. You must ensure automatic scope cleanup and absolute leak prevention by utilizing smart pointers (`std::unique_ptr` or `std::shared_ptr`).
* **Robustness:** Unrecognized command verbs or missing parameter lines must be ignored silently without throwing exceptions or crashing the engine process loop.

---

### Question 2: Full-Stack MERN Architecture & Debugging (20 Points)

The following code belongs to an Express.js backend web service using Mongoose to manage an article publishing workflow. The service is broken up across standard MVC design layers. 

Analyze the files below. Identify and document **exactly 4 distinct compilation or runtime bugs**. For each bug, specify the filename, explain the underlying mechanics of the breakage, and provide the exact corrected code statement.

#### `app.js`
```javascript
const express = require('express');
const articleRoutes require('./routes/article'); // Line 2

require('custom-env').env(process.env.NODE_ENV, './config');

mongoose.connect(process.env.CONNECTION_STRING, { 
    useNewUrlParser: true, 
    useUnifiedTopology: true 
}); // Line 7

var app = express();
app.use(express.json());
app.use('/articles', articleRoutes);

app.listen(process.env.PORT);

```

#### `routes/article.js`

```javascript
const express = require('express');
var router = express.Router();
const articleController = require('../controllers/article');

router.get('/', articleController.getAllArticles);
router.get('/', articleController.getArticleById); // Line 6

module.exports = router;

```

#### `models/article.js`

```javascript
const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const articleScheme = new mongoose.Schema({ title: { }); type: String, required: true } }); // Line 4

module.exports = mongoose.model('Article', articleScheme);

```

---

### Question 3: Code Execution Tracing Sheets (20 Points)

#### Section A: JavaScript Hoisting and Scope Transitions

Predict the exact runtime outcome of compiling and executing the following JavaScript snippet. If it crashes, name the explicit error variant and explain why. If it runs cleanly, track the terminal logs step-by-step.

```javascript
var a = 10;
let b = 100;
foo();

function foo() {
    console.log('a:', a);
    if (true) {
        var a = 20;
    }
    let b = 200;
}

console.log('b:', b);
let b = 300;

```

#### Section B: C++ Multiple & Virtual Inheritance

Predict the exact console output of the following polymorphic layout initialization. Trace out the explicit structural modification rules enforced by the `virtual` keyword.

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    int x;
    Base() { x = 1; }
};

class Intermediate1 : virtual public Base {
public:
    Intermediate1() { x = 2; }
};

class Intermediate2 : virtual public Base {
public:
    Intermediate2() { x = 3; }
};

class Descendant : public Intermediate1, public Intermediate2 {
public:
    Descendant() { x = 4; }
};

int main() {
    Descendant d;
    cout << d.x << endl;
    return 0;
}

```

```

---

### File 2: `Exam_2025_Moed_B_Systems_Focus.md`

```markdown
# Bar-Ilan University — Advanced Systems Programming (893210)
## Targeted Reconstruction: Semester B, Moed Bet (2025)

---

### Question 1: POSIX Systems Programming & Process Trees (20 Points)

Analyze the following Linux systems application utilizing the standard C/C++ library headers `unistd.h` and `sys/wait.h`.

Assume that the operating system allocates process identification metrics (`pid`) in a strictly sequential linear format starting at parent execution wrapper ID `1000`. Disregard adjacent system threads.

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    cout << "Init PID: " << getpid() << " PPID: " << getppid() << endl;
    
    int pid = fork();
    
    if (pid == 0) {
        cout << "Child Alpha PID: " << getpid() << " PPID: " << getppid() << endl;
        execlp("/bin/echo", "echo", "Alpha Finished", NULL);
        cout << "Alpha Terminal Assert" << endl;
    } 
    else if (pid > 0) {
        wait(NULL);
        cout << "Parent Resumed tracking child: " << pid << endl;
    }
    
    return 0;
}

```

#### Tasks

1. Draw or map out the explicit process tree diagram generated by this code block execution.
2. Provide the exact console terminal trace logs produced from initialization to exit termination.
3. Explain the behavioral mechanics of the `execlp` system call wrapper on line 13. Will the string statement on line 14 (`"Alpha Terminal Assert"`) print? Why or why not?

---

### Question 2: Infrastructure Optimization & Git Database Internals (20 Points)

#### Section A: DevOps & Optimization Patterns

Analyze this unoptimized `Dockerfile` built for an advanced containerized React/Node web configuration:

```dockerfile
FROM node:alpine
COPY . /app
WORKDIR /app
RUN npm install
EXPOSE 8080
CMD ["node", "server.js"]

```

1. Identify the structural architectural design flaw present in this configuration regarding build times and dependency handling.
2. Explain how the Docker engine handles **Layer Caching** and describe the consequences of editing a single client file under this configuration.
3. Rewrite the optimized production-grade version of this `Dockerfile`.

#### Section B: Low-Level Git Object Mechanics

A developer runs the following sequence of commands inside an empty directory:

```bash
$ git init
$echo "advanced systems engineering" > manifest.txt$ git add mainfest.txt
$ git commit -m "initial core snapshot"

```

Describe exactly how the internal Git content-addressable filesystem changes inside the hidden `.git/objects/` tree directory. Specify the number of objects generated, their explicit internal object type tags (`blob`, `tree`, `commit`), and what metadata parameters are isolated within each distinct block wrapper.

```

```
