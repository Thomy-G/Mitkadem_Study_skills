# Android Development & Java MVVM Architecture — Study Reference Guide

---

## 1. Android XML Layout Architecture

Android layouts define the UI structure. The most common container is `LinearLayout`, which arranges child components sequentially.

### 1.1 Core Layout Configurations
*   `orientation`: Determines alignment (`vertical` or `horizontal`).
*   `layout_width` & `layout_height`:
    *   `match_parent`: Expands to fill all available parent space.
    *   `wrap_content`: Shrinks to fit the content size inside.
*   `layout_weight`: Distributes remaining parent space proportionally among sibling views.

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:padding="16dp">

    <!-- View stretched using weight -->
    <TextView
        android:id="@+id/tv_sensor_output"
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="1"
        android:text="System Telemetry Data"
        android:textSize="20sp" />

    <EditText
        android:id="@+id/et_input"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:hint="Type payload name..." />

    <Button
        android:id="@+id/btn_action"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Publish Event" />

</LinearLayout>
```

---

## 2. The Android MVVM Architectural Pattern

MVVM separates data management from the UI to make code modular and testable.

```
+--------------------+      observes      +---------------------+
|    View (Activity) |<===================|      ViewModel      |
+--------------------+                    +---------------------+
          |                                          |
          v triggers action                          v queries data
+---------------------------------------------------------------+
|                        Repository                             |
+---------------------------------------------------------------+
        |                                       |
        v                                       v
+------------------+                    +------------------+
| Local DB (Room)  |                    | Network API      |
+------------------+                    +------------------+
```

### 2.1 The Repository Layer
Abstracts data access from local databases (Room) or remote APIs.
```java
// SensorRepository.java
package com.example.biu_app;

import androidx.lifecycle.MutableLiveData;

public class SensorRepository {
    private static SensorRepository instance;

    public static synchronized SensorRepository getInstance() {
        if (instance == null) {
            instance = new SensorRepository();
        }
        return instance;
    }

    public void fetchLatestSensorData(MutableLiveData<String> targetData) {
        // Simulating background thread fetch
        new Thread(() -> {
            try {
                Thread.sleep(1000);
                targetData.postValue("Sensor Status: Active (42C)"); // postValue used from background thread
            } catch (InterruptedException e) {
                targetData.postValue("Fetch failed");
            }
        }).start();
    }
}
```

### 2.2 The ViewModel & LiveData Layer
Manages UI data in a lifecycle-aware way.
*   **LiveData**: Read-only wrapper observed by the UI thread.
*   **MutableLiveData**: Subclass of LiveData that exposes values updates via `.setValue()` (Main UI thread) and `.postValue()` (Worker threads).

```java
// SensorViewModel.java
package com.example.biu_app;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class SensorViewModel extends ViewModel {
    private final MutableLiveData<String> mSensorData = new MutableLiveData<>();
    private final SensorRepository repository;

    public SensorViewModel() {
        repository = SensorRepository.getInstance();
        mSensorData.setValue("Status: Idle"); // Main thread assignment
    }

    // Expose as read-only LiveData to prevent Views from changing internal data
    public LiveData<String> getSensorData() {
        return mSensorData;
    }

    public void refreshData() {
        mSensorData.setValue("Status: Refreshing...");
        repository.fetchLatestSensorData(mSensorData);
    }
}
```

### 2.3 The View (Activity / Observation Setup)
```java
// MainActivity.java
package com.example.biu_app;

import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

public class MainActivity extends AppCompatActivity {
    private SensorViewModel viewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tvOutput = findViewById(R.id.tv_sensor_output);
        Button btnRefresh = findViewById(R.id.btn_action);

        // Instantiate ViewModel safely via factory to preserve state across rotation
        viewModel = new ViewModelProvider(this).get(SensorViewModel.class);

        // Observe changes to the LiveData stream
        viewModel.getSensorData().observe(this, newStatus -> {
            tvOutput.setText(newStatus);
        });

        // Trigger action on click
        btnRefresh.setOnClickListener(v -> viewModel.refreshData());
    }
}
```

---

## 3. The Activity Lifecycle

The operating system manages the lifecycle of Android apps through a sequence of callback states.

```
       +--------------+
       |  onCreate()  |
       +--------------+
              |
       +--------------+
       |  onStart()   |
       +--------------+
              |
       +--------------+
       |  onResume()  | <=== App is running and has focus
       +--------------+
              |
       +--------------+
       |  onPause()   | <=== App is partially obscured (e.g., popup dialog)
       +--------------+
              |
       +--------------+
       |   onStop()   | <=== App is completely hidden
       +--------------+
              |
       +--------------+
       |  onDestroy() | <=== App is destroyed (called on finish() or configuration changes)
       +--------------+
```

### 3.1 Lifecycle Callback Details
*   **`onCreate()`**: Triggers when the system first creates the Activity. Performs initial setups (inflating XML layouts, retrieving View references, instantiating ViewModels).
*   **`onStart()`**: Makes the Activity visible to the user but not yet interactive.
*   **`onResume()`**: Makes the Activity interactive and brings it to the foreground. This is where you register sensors or resume animations.
*   **`onPause()`**: Called when the Activity is partially obscured by another component (e.g., a modal dialog or split-screen mode). Save temporary state or pause animations here.
*   **`onStop()`**: Called when the Activity is completely hidden from view. CPU-intensive operations should be paused here.
*   **`onDestroy()`**: Called before the Activity is destroyed. Release all remaining resources to prevent memory leaks.
*   **Configuration Changes (e.g., screen rotation)**: Destroys the active instance, running `onPause() -> onStop() -> onDestroy()`, and creates a new instance using `onCreate() -> onStart() -> onResume()`. **ViewModels survive this cycle**, which is why they are used to persist UI state.

---

## 4. ListViews & ArrayAdapters

For displaying simple text lists dynamically, Android uses `ListView` coupled with an `ArrayAdapter` to bind lists or arrays of strings to view elements:

```java
import android.app.Activity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import java.util.ArrayList;

public class ListActivity extends Activity {
    private ListView listView;
    private ArrayAdapter<String> adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list); // Needs a <ListView id="@+id/list_view" ... /> in layout

        listView = findViewById(R.id.list_view);

        ArrayList<String> dataList = new ArrayList<>();
        dataList.add("Telemetry Item 1");
        dataList.add("Telemetry Item 2");

        // Simple List Item Layout from android framework resource list_item_1
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, dataList);
        listView.setAdapter(adapter);

        // Click listener on list elements
        listView.setOnItemClickListener((parent, view, position, id) -> {
            String selectedItem = adapter.getItem(position);
            // Process click
        });
    }
}
```

---

## 5. Intents & Multi-Activity Communication

Intents are messaging objects used to request actions from other app components (like launching other Activities).

### 5.1 Explicit vs. Implicit Intents
*   **Explicit Intent**: Names the exact target class to be launched. Typically used inside your own application.
    ```java
    Intent intent = new Intent(MainActivity.this, SecondActivity.class);
    intent.putExtra("key_string", "Message to SecondActivity");
    startActivity(intent);
    ```
*   **Implicit Intent**: Declares a general action to perform, allowing the operating system to find matching registered apps (e.g., opening a web page or dialer).
    ```java
    Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.biu.ac.il"));
    startActivity(intent);
    ```

### 5.2 Launching Activities for Results
If Activity A needs a return payload from Activity B:
1.  **Activity A** launches Activity B using a request code:
    ```java
    private static final int REQ_CODE = 100;
    
    Intent intent = new Intent(this, SecondActivity.class);
    startActivityForResult(intent, REQ_CODE);
    
    // Override listener
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQ_CODE && resultCode == RESULT_OK && data != null) {
            String result = data.getStringExtra("result_key");
            // Process return result
        }
    }
    ```
2.  **Activity B** sets the result package and finishes execution:
    ```java
    Intent returnIntent = new Intent();
    returnIntent.putExtra("result_key", "Returned data from B");
    setResult(RESULT_OK, returnIntent);
    finish(); // Closes Activity B, returning control to Activity A
    ```

---

## 6. Toasts & Custom Layout Inflation

Toasts display lightweight popups that disappear after a timeout:

*   **Standard Toast**:
    ```java
    Toast.makeText(getApplicationContext(), "Status Updated!", Toast.LENGTH_SHORT).show();
    ```
*   **Custom Layout Inflation Toast**:
    Inflates a custom XML layout (e.g., `res/layout/toast_custom.xml`) to display rich custom alerts:
    ```java
    // Inflate XML layout
    View customView = getLayoutInflater().inflate(R.layout.toast_custom, null);
    
    // Configure components inside layout
    TextView text = customView.findViewById(R.id.toast_text_view);
    text.setText("Rich Custom Notification");
    
    // Instantiate and show Toast
    Toast toast = new Toast(getApplicationContext());
    toast.setDuration(Toast.LENGTH_SHORT);
    toast.setView(customView); // Sets custom view
    toast.show();
    ```

---

## 7. Garbage Collection & Memory Management (JVM)

Managed languages (like Java) automate memory deallocation using a **Garbage Collector (GC)**.

### 7.1 Garbage Collection Strategies
1.  **Reference Counting**:
    *   *Mechanism*: Tracks the number of references to each object. Deallocates memory when reference count drops to `0`.
    *   *Pros*: Simple and immediate.
    *   *Cons*: Extra space/time overhead; **cannot resolve cyclic references** (e.g., Object A references B, and B references A; their counts never reach `0` even if isolated from the program).
2.  **Mark & Sweep**:
    *   *Mark phase*: Starts from root references (stack, registers, static objects) and traverses the active object graph, marking visited elements.
    *   *Sweep phase*: Scans the entire heap, deallocates any unmarked elements, and resets flags.
    *   *Cons*: **Stop-The-World (STW)** pause (all program threads must halt during sweep); leads to memory fragmentation.
    *   *Compacted Mark-Sweep*: Relocates surviving objects consecutively after the sweep phase. This eliminates fragmentation and enables fast sequential memory allocation, at the cost of processing time.

### 7.2 Generational Garbage Collection
Most objects die young. The JVM separates the heap into different regions based on object age:
1.  **Young Generation**:
    *   Divided into three zones: **Eden**, **$S_0$ (Survivor 0)**, and **$S_1$ (Survivor 1)**.
    *   All new objects are allocated in **Eden**.
    *   When Eden is full, a **Minor GC** triggers: referenced objects are copied to $S_0$, and Eden is cleared.
    *   On the next Minor GC, survivors from Eden and $S_0$ are moved to $S_1$. The survivor spaces alternate on subsequent collections.
    *   Each survivor object has an **age counter** incremented per collection.
2.  **Old Generation**:
    *   Holds long-lived objects. When an object's age counter exceeds a certain threshold (e.g., 15), it is promoted from the Young Generation to the Old Generation.
    *   **Major GC** collections run on the Old Generation (less frequent, but take longer).
3.  **Permanent Generation (Metaspace)**:
    *   Stores class definitions, method metadata, static variables, and constant pools.

---

## 8. JVM & Python Bytecode Compilation Architectures

### 8.1 The JVM JIT Compiler
Java code is compiled by `javac` into intermediate **bytecode** (`.class` files) rather than native machine code.
*   At runtime, the JVM loads and interprets the bytecode.
*   **Just-In-Time (JIT) Compilation**: To optimize execution speed, a dedicated JVM compiler thread identifies "hotspot" methods (frequently executed blocks) and compiles them directly into native machine code at runtime.
*   **Levels of Compilation**:
    1.  *Interpreted*: Code starts running interpreted (no compilation overhead).
    2.  *Tiered/Short Compilation*: Frequent methods compiled with simple optimizations.
    3.  *Optimized Compilation*: Extremely hot methods are compiled with full, heavy optimizations.

### 8.2 Python Implementations: CPython vs. Jython
Python is a language specification, and its behavior varies by runtime implementation:
*   **CPython**: The standard C-based implementation. Compiles Python source (`.py`) into bytecode (`.pyc` files) which is interpreted at runtime by the CPython Virtual Machine (PVM).
*   **Jython**: A Java-based implementation of Python. It compiles Python source code directly into **Java Bytecode**, allowing Python programs to run natively inside any JVM and interact directly with Java libraries.
