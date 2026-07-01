# Archetype 3: Code Execution Tracing & Scope Analysis (20 Points)

## Section A: C++ Multiple & Virtual Inheritance (10 Points)
Predict the exact console output of compilation and execution of the following C++ program. Trace the construction and modification order of member properties.

```cpp
#include <iostream>
using namespace std;

class Component {
public:
    Component() { cout << "Component constructed" << endl; }
};

class Keyboard : virtual public Component {
public:
    Keyboard() { cout << "Keyboard constructed" << endl; }
};

class Screen : virtual public Component {
public:
    Screen() { cout << "Screen constructed" << endl; }
};

class Laptop : public Keyboard, public Screen {
public:
    Laptop() { cout << "Laptop constructed" << endl; }
};

int main() {
    Laptop my_laptop;
    return 0;
}
```

---

## Section B: JavaScript Scope, Hoisting, and Promises (10 Points)
Predict the exact print sequence of the following JavaScript program. Describe the state of the Call Stack, Microtask Queue, and Callback/Macrotask Queue at each step.

```javascript
console.log('Start');

setTimeout(() => {
    console.log('Timeout 1');
}, 0);

Promise.resolve().then(() => {
    console.log('Promise 1');
}).then(() => {
    console.log('Promise 2');
});

async function runAsync() {
    console.log('Async Start');
    await Promise.resolve();
    console.log('Async End');
}

runAsync();

console.log('End');
```
