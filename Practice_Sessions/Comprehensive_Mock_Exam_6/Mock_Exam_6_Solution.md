# Comprehensive Mock Exam 6 — Official Solution Key

---

## 📝 Question 1: C++ Templates, Functors, & polymorphic structures (40 Points)

### Section A: Header Declarations (`DataFilter.h`)
```cpp
#ifndef DATA_FILTER_H
#define DATA_FILTER_H

#include <vector>
#include <memory>
#include <algorithm>

template <typename T>
class DataFilter {
private:
    std::vector<std::shared_ptr<T>> items;

public:
    DataFilter() = default;

    void add(std::shared_ptr<T> item) {
        items.push_back(item);
    }

    // Accepts a functor predicate and filters items
    template <typename Predicate>
    std::vector<std::shared_ptr<T>> filter(Predicate pred) const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& item : items) {
            if (item && pred(*item)) { // Invoke functor
                result.push_back(item);
            }
        }
        return result;
    }
};

#endif // DATA_FILTER_H
```

### Section B: Implementation (`DataFilter.cpp`)
All template definitions are included in the header above as required for template class compilations in C++.

### Section C: TDD Google Test Suite (`Filter_Test.cpp`)
```cpp
#include <gtest/gtest.h>
#include "DataFilter.h"

// Sample Data Class
struct Product {
    std::string name;
    double price;
};

// Functor Predicate
struct PriceUnderLimit {
    double limit;
    PriceUnderLimit(double lim) : limit(lim) {}

    bool operator()(const Product& p) const {
        return p.price < limit;
    }
};

TEST(DataFilterTest, FilterProductsByPriceLimit) {
    DataFilter<Product> filterEngine;
    filterEngine.add(std::make_shared<Product>(Product{"Keyboard", 45.0}));
    filterEngine.add(std::make_shared<Product>(Product{"Monitor", 250.0}));
    filterEngine.add(std::make_shared<Product>(Product{"Mouse", 20.0}));

    // Filter using functor
    PriceUnderLimit predicate(50.0);
    auto results = filterEngine.filter(predicate);

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0]->name, "Keyboard");
    EXPECT_EQ(results[1]->name, "Mouse");
}
```

---

## 🔍 Question 2: The "Find the Bugs" Debugging Challenge (20 Points)

### Bug 1 (Component A - `index.js`)
*   **Filename:** `index.js`
*   **Line / Code Block:** Entire file.
*   **Mechanics of Flaw:** React Native navigation requires that all stack navigators are wrapped inside a root `<NavigationContainer>` component. Omitting this wrapper prevents navigation context initialization, causing the app layout constructor to crash.
*   **Corrected Statement:** Import `NavigationContainer` and wrap the navigator:
    ```jsx
    import { NavigationContainer } from '@react-navigation/native';
    // Inside index.js or App entry:
    export default function App() {
        return (
            <NavigationContainer>
                <AppNavigation />
            </NavigationContainer>
        );
    }
    ```

### Bug 2 (Component B - `server.py`)
*   **Filename:** `server.py`
*   **Line / Code Block:** Line 12: `print("Received: " + data)`
*   **Mechanics of Flaw:** In Python 3, binary buffers received over socket (`bytes`) cannot be implicitly concatenated with string objects (`str`). Attempting to add `data` directly to a string throws a `TypeError`.
*   **Corrected Statement:** Decode the byte buffer before printing:
    ```python
    print("Received: " + data.decode("utf-8"))
    ```

### Bug 3 (Component C - `models/Game.js`)
*   **Filename:** `models/Game.js`
*   **Line / Code Block:** Line 7: `capped: { max: 100 }`
*   **Mechanics of Flaw:** In MongoDB/Mongoose, defining a Capped Collection *must* include a explicit size allocation constraint (in bytes). Passing only the `max` document limit without specifying a `size` parameter is invalid and will fail database validation schemas.
*   **Corrected Statement:**
    ```javascript
    capped: { size: 1024 * 1024, max: 100 } // Allocate 1MB size limit
    ```

### Bug 4 (Component D - `launcher.cpp`)
*   **Filename:** `launcher.cpp`
*   **Line / Code Block:** Line 11: `std::thread t(updateState, current_state);`
*   **Mechanics of Flaw:** `std::thread` constructor passes arguments by value. Since `updateState` expects a reference (`std::string&`), the compiler rejects the mismatch. To pass the reference correctly, the argument must be wrapped in `std::ref()`.
*   **Corrected Statement:**
    ```cpp
    std::thread t(updateState, std::ref(current_state));
    ```

---

## 🧠 Question 3: Code Trace & Exact Output Prediction (20 Points)

### Section A: C++ Multiple Inheritance Construction Order
*   **Predicted Output:**
    ```
    BAG~G~A~B
    ```
*   **Step-by-step Construction & Destruction Trace:**
    1.  `Gamma` inherits from `Beta` and `Alpha` in that order (`class Gamma : public Beta, public Alpha`).
    2.  Base classes are constructed in the order they are listed in the inheritance declaration list (not the order in constructor initialization list):
        - `Beta` constructed first: prints `"B"`.
        - `Alpha` constructed second: prints `"A"`.
    3.  `Gamma` constructor runs: prints `"G"`.
        - Output of construction: `BAG`.
    4.  Destructors execute in the exact reverse order of construction:
        - `Gamma` destructor runs: prints `"~G"`.
        - `Alpha` destructor runs: prints `"~A"`.
        - `Beta` destructor runs: prints `"~B"`.
        - Output of destruction: `~G~A~B`.
    *   *Total String*: `BAG~G~A~B`.

### Section B: JavaScript Promises & microtask loops
*   **Predicted Output Sequence:**
    ```
    start
    promise body
    end
    then 1: resolved val
    then 2: next val
    ```
*   **Execution Loop Step-by-step Tracing:**
    1.  Logs `"start"` (Synchronous).
    2.  Instantiates `new Promise`. The callback executor runs synchronously immediately: logs `"promise body"`, resolving to `"resolved val"`.
    3.  `.then` chain registrations run. The first `.then(...)` is pushed to the Microtask queue.
    4.  Logs `"end"` (Synchronous).
    5.  Stack clears. The Microtask queue executes:
        - Pulls first task: logs `"then 1: resolved val"`, returns `"next val"`.
        - Pushes second `.then(...)` to the Microtask queue.
    6.  Microtask queue executes subsequent task:
        - Logs `"then 2: next val"`.

---

## 🛠️ Question 4: "Fill in the Blanks" / Tables & Diagrams (10 Points)

### Section A: VTABLE Layout Memory Grid
1.  `X::v1()`
2.  `Y::v1()` (Overrides X)
3.  `Y::v2()` (New virtual method)

### Section B: Docker Multi-Stage Build layers
1.  `FROM`
2.  `COPY`
3.  small
4.  `scratch` (or `alpine`)
5.  attack

---

## 📱 Question 5: Mobile & Web Concurrency (10 Points)

### Section A: Android Activity Lifecycle Callbacks
1.  **First Launch**: `onCreate() -> onStart() -> onResume()`
2.  **Press Home**: `onPause() -> onStop()`
3.  **App Resume**: `onRestart() -> onStart() -> onResume()`
4.  **Press Back**: `onPause() -> onStop() -> onDestroy()`

### Section B: Native Node.js Stream Buffering
*   **Stream events explanation:**
    *   `data`: Fires periodically whenever a buffer chunk of request body payload is received.
    *   `end`: Fires once the client finishes sending all request stream data.
*   **Chunk reading purpose:** HTTP requests can contain massive payloads (e.g. file uploads or large JSON trees). Buffering chunks prevents server memory crashes by streaming data in segments, allowing parsing and validation to process chunks asynchronously without blocking the execution event loop thread.
