# JavaScript, Node.js, Express, & MongoDB — Study Reference Guide

---

## 1. Variable Scoping & Hoisting

### 1.1 `var` vs. `let` vs. `const`
*   **`var`**:
    *   **Scope**: Function-scoped. If declared outside any function, it becomes global (bound to the `window` or `global` object).
    *   **Hoisting**: Hoisted to the top of its enclosing function scope and initialized with `undefined`.
    *   **Re-declaration**: Permitted inside the same scope.
*   **`let`**:
    *   **Scope**: Block-scoped (bounded by `{}`).
    *   **Hoisting**: Hoisted to the top of the block, but remains uninitialized in the **Temporal Dead Zone (TDZ)**. Accessing it before declaration throws a `ReferenceError`.
    *   **Re-declaration**: Forbidden in the same scope.
*   **`const`**:
    *   **Scope**: Block-scoped.
    *   **Initialization**: Must be initialized at declaration. Immutable reference (the reference cannot change, but the object contents *can* be modified).
    *   **Temporal Dead Zone**: Same as `let`.

```javascript
function scopeDemonstration() {
    console.log(a); // Prints undefined due to hoisting of var
    // console.log(b); // ReferenceError: Cannot access 'b' before initialization (TDZ)
    
    var a = 10;
    let b = 20;
    
    if (true) {
        var a = 100; // Overwrites outer 'a' because var is function-scoped
        let b = 200; // Shadowed local block variable; does not affect outer 'b'
        console.log("inner a:", a); // 100
        console.log("inner b:", b); // 200
    }
    
    console.log("outer a:", a); // 100
    console.log("outer b:", b); // 20
}
```

---

## 2. Functions & Lexical Arrow Functions (Lambdas)

### 2.1 Function Declarations vs. Function Expressions
*   **Function Declaration**: Hoisted completely to the top of the scope, allowing it to be called *before* its definition.
    ```javascript
    greet(); // Works!
    function greet() { console.log("Hello"); }
    ```
*   **Function Expression**: Not hoisted. Treat as variable assignment.
    ```javascript
    // greetExpr(); // TypeError: greetExpr is not a function
    const greetExpr = function() { console.log("Hello"); };
    ```

### 2.2 Arrow Functions (`this` Binding)
Arrow functions do not bind their own `this`. Instead, they capture the `this` value of the enclosing lexical context.
Regular functions bind `this` dynamically depending on how they are called.

```javascript
const obj = {
    name: "BIU",
    regularFunc: function() {
        console.log("regular:", this.name); // Prints "BIU" because 'this' points to obj
    },
    arrowFunc: () => {
        console.log("arrow:", this.name); // Prints undefined (inherits global scope 'this')
    },
    delayedFunc: function() {
        setTimeout(function() {
            console.log("delayed regular:", this.name); // Prints undefined (callback runs in global context)
        }, 100);
        
        setTimeout(() => {
            console.log("delayed arrow:", this.name); // Prints "BIU" (lexical bind to outer delayedFunc scope)
        }, 100);
    }
};
```

### 2.3 JavaScript Classes, Private Fields (#) & Immutable Freezing
JavaScript supports OOP classes with ES6. Private fields are prefixed with `#` and are inaccessible outside the class body. 
`Object.freeze(obj)` locks down an object, preventing modifications, additions, or deletions of its properties.

```javascript
class Car {
    #fuelCapacity; // Private field declaration
    #currentFuel;

    constructor(name, capacity) {
        this.name = name;
        this.#fuelCapacity = capacity;
        this.#currentFuel = 0;
    }

    fuelCar(amount) {
        const newFuel = this.#currentFuel + amount;
        if (newFuel <= this.#fuelCapacity) {
            this.#currentFuel = newFuel;
        }
    }

    getFuel() {
        return this.#currentFuel;
    }
}

const carInstance = new Car("Toyota", 50);
carInstance.fuelCar(20);
// console.log(carInstance.#currentFuel); // SyntaxError: Private field '#currentFuel' must be declared in an enclosing class

// Freezing objects to make them immutable
const config = { apiEndpoint: "https://api.biu.ac.il", timeout: 5000 };
Object.freeze(config);
config.timeout = 10000; // Does nothing (fails silently in non-strict mode, throws TypeError in strict mode)
console.log(config.timeout); // Prints 5000
```

---

## 3. Asynchronous Execution Models: The JavaScript Event Loop

JavaScript is single-threaded, using an Event Loop to handle asynchronous operations.

```
+-------------------------------------------------+
|                    CALL STACK                   |
+-------------------------------------------------+
                         |
                         v (Asynchronous Web APIs / Node C++)
+-------------------------------------------------+
|                 MICROTASK QUEUE                 | (Promises, process.nextTick)
+-------------------------------------------------+
                         | (Highest Priority: Emptied completely before next step)
                         v
+-------------------------------------------------+
|                 MACROTASK QUEUE                 | (setTimeout, setInterval, I/O)
+-------------------------------------------------+
```

### 3.1 Trace Example
```javascript
console.log("1");

setTimeout(() => {
    console.log("2"); // Macrotask Queue
}, 0);

Promise.resolve().then(() => {
    console.log("3"); // Microtask Queue
});

console.log("4");

// Output Sequence:
// 1
// 4
// 3 (Microtask runs immediately after call stack empties)
// 2 (Macrotask runs on next tick)
```

---

## 4. Node.js & Express RESTful Server Architecture

### 4.1 Server Setup & Routing
Express processes incoming HTTP requests through a list of routes and middleware blocks.

```javascript
const express = require('express');
const app = express();

// Global body-parsing middleware
app.use(express.json());

// Custom logging middleware
app.use((req, res, next) => {
    console.log(`${req.method} request received at ${req.path}`);
    next(); // Must be invoked to proceed to next block
});

// Route parameters vs Query parameters
app.get('/users/:userId/articles', (req, res) => {
    const userId = req.params.userId;        // URL Parameters (e.g. /users/123/articles)
    const limit = req.query.limit || 10;     // Query Strings (e.g. ?limit=5)
    
    res.status(200).json({
        userId,
        limit,
        data: []
    });
});

// Global Error Handling Middleware (must contain exactly 4 parameters)
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(500).json({ error: "Internal Server Error" });
});

app.listen(8080, () => console.log("Server listening on port 8080"));
```

### 4.2 Pure Node.js Native HTTP Server (No Express)
When Express is not allowed, you must use Node's native `http` and `fs` modules to build servers, handle routing, and parse request streams manually:

```javascript
const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const method = req.method;
    const pathName = parsedUrl.pathname;

    // 1. Serving static files manually
    if (pathName === '/' && method === 'GET') {
        fs.readFile(path.join(__dirname, 'index.html'), 'utf8', (err, html) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Error loading page');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(html);
        });
    } 
    // 2. Handling GET API routes manually
    else if (pathName === '/api/data' && method === 'GET') {
        const queryData = parsedUrl.query; // Query params
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ status: "Success", query: queryData }));
    } 
    // 3. Handling POST streams manually (reading chunks)
    else if (pathName === '/api/data' && method === 'POST') {
        let body = '';
        req.on('data', chunk => {
            body += chunk; // Collect buffer chunks
        });
        req.on('end', () => {
            const parsedData = JSON.parse(body);
            res.writeHead(201, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ status: "Created", data: parsedData }));
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Not Found');
    }
});

server.listen(3000, () => console.log('Pure Node server running on port 3000'));
```

---

## 5. MongoDB & Mongoose Modeling

Mongoose is an Object Data Modeling (ODM) library for MongoDB.

```javascript
const mongoose = require('mongoose');

// Define Schema
const productSchema = new mongoose.Schema({
    sku: { 
        type: String, 
        required: true, 
        unique: true, 
        index: true // Speeds up queries based on sku
    },
    name: { 
        type: String, 
        required: [true, 'Product name is mandatory'] 
    },
    price: { 
        type: Number, 
        min: [0, 'Price cannot be negative'] 
    },
    category: {
        type: mongoose.Schema.Types.ObjectId,
        ref: 'Category', // Reference linking to another Collection
        required: true
    }
}, { timestamps: true });

// Export Compiled Model
module.exports = mongoose.model('Product', productSchema);
```

### 5.2 CRUD & Query Optimizations
```javascript
const Product = require('./models/Product');

// Query with relation population
async function getProductWithCategory(productId) {
    return await Product.findById(productId)
        .populate('category') // Automatically resolves Category reference object
        .exec();
}
```

### 5.3 Raw MongoDB Shell Queries
You must know the exact query commands, projection options, and operators inside the MongoDB shell (`mongosh`):

#### 1. Database & Collection Management
*   `show dbs`: List available databases.
*   `use myapp`: Switch context to database `myapp`.
*   `db.createCollection("logs", {capped: true, size: 1000000, max: 100})`: Creates a **Capped Collection** (recycles space; restricted to max 100 documents or 1MB).
*   `db.logs.drop()`: Drops collection.

#### 2. Insert Operations
*   `db.users.insertOne({ name: "Alice", grade: 95 })`
*   `db.users.insertMany([{ name: "Bob", grade: 80 }, { name: "Charlie", grade: 70 }])`

#### 3. Find and Projections
Projections determine which fields are returned (`1` / `true` to include, `0` / `false` to exclude).
*   `db.users.find({ grade: 80 }, { name: 1, _id: 0 })`: Finds users with grade 80, returns *only* their names, suppressing `_id`.

#### 4. Query Comparison & Logical Operators
*   Comparison: `$gt` (greater than), `$gte` (greater or equal), `$lt`, `$lte`, `$ne` (not equal), `$in` (array membership).
    *   `db.users.find({ grade: { $gte: 80 } })`
*   Logical: `$and`, `$or`, `$not`.
    *   `db.users.find({ $and: [{ grade: { $gte: 70 } }, { name: { $ne: "Bob" } }] })`

#### 5. Sorting, Limiting, & Aggregation
*   `db.users.find().sort({ name: 1 })`: Sorts names ascending (`1`) or descending (`-1`).
*   `db.users.find().sort({ grade: -1 }).limit(3)`: Top 3 grades.
*   Aggregations (`$group`, `$avg`, `$sum`, `$min`, `$max`):
    *   `db.users.aggregate([{ $group: { _id: "$year", averageGrade: { $avg: "$grade" } } }])`

#### 6. Indexes
*   `db.users.createIndex({ name: 1 })`: Creates index on `name` ascending to optimize searches.
*   `db.users.find({ name: "Bob" }).explain("executionStats")`: Evaluates index scan vs full collection scan performance.

---

## 6. Web Authentication: Session Cookies vs. JWTs

### 6.1 Session Cookies (Stateful)
*   **How it works**: Client authenticates -> Server creates session in DB/Redis and sends a cookie containing a unique `session_id` to client -> Client browser attaches the cookie automatically to all subsequent requests.
*   **Security features**: Can be configured with `httpOnly` (prevents access via JS/XSS attacks) and `secure` (enforces HTTPS transmission).

### 6.2 JSON Web Tokens (Stateless)
*   **Structure**: Three base64url-encoded sections separated by dots:
    `Header.Payload.Signature`
    *   **Header**: Specifies algorithm (e.g., HS256) and token type.
    *   **Payload**: User details / claims (e.g., `userId`, expiration time). *Warning: This is not encrypted, only base64 encoded. Do not store sensitive details like passwords here.*
    *   **Signature**: Created by signing the header + payload using a secret key on the server.
*   **Verification**: The server verifies token integrity by recalculating the signature. If matching, the payload is trusted without doing a database lookup.
*   **Scaling advantages**: Stateless; multiple distributed microservices can verify tokens without hitting a centralized session database.
