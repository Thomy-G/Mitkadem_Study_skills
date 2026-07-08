# Past Exam Solution Key — 2025 Semester B, Moed B

---

## שאלה מספר 1 (35 נק')

### מימוש קוד צד לקוח של מערכת ה-Todo (React)

#### `TodoApp.jsx`
```jsx
import React, { useState, useEffect } from 'react';
import './TodoApp.css';

export default function TodoApp() {
    const [tasks, setTasks] = useState([]);
    const [newTaskText, setNewTaskText] = useState('');

    useEffect(() => {
        fetchTasks();
    }, []);

    const fetchTasks = async () => {
        const res = await fetch('/api/tasks');
        if (res.ok) {
            const data = await res.json();
            setTasks(data);
        }
    };

    const handleAddTask = async (e) => {
        e.preventDefault();
        if (!newTaskText.trim()) return;
        const res = await fetch('/api/tasks', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ text: newTaskText })
        });
        if (res.ok) {
            const task = await res.json();
            setTasks([...tasks, task]);
            setNewTaskText('');
        }
    };

    const handleDelete = async (id) => {
        const res = await fetch(`/api/tasks/${id}`, { method: 'DELETE' });
        if (res.ok) {
            setTasks(tasks.filter(t => t._id !== id));
        }
    };

    const handleMove = async (index, direction) => {
        const targetIndex = direction === 'up' ? index - 1 : index + 1;
        if (targetIndex < 0 || targetIndex >= tasks.length) return;

        const updatedTasks = [...tasks];
        // Swap locally
        const temp = updatedTasks[index];
        updatedTasks[index] = updatedTasks[targetIndex];
        updatedTasks[targetIndex] = temp;

        const res = await fetch('/api/tasks/reorder', {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ 
                taskId1: tasks[index]._id, 
                taskId2: tasks[targetIndex]._id 
            })
        });

        if (res.ok) {
            setTasks(updatedTasks);
        }
    };

    return (
        <div className="todo-container">
            <h2>Task Manager</h2>
            <form onSubmit={handleAddTask}>
                <input value={newTaskText} onChange={e => setNewTaskText(e.target.value)} />
                <button type="submit">Add Task</button>
            </form>
            <ul>
                {tasks.map((t, index) => (
                    <li key={t._id}>
                        <span>{t.text}</span>
                        <button onClick={() => handleMove(index, 'up')} disabled={index === 0}>▲</button>
                        <button onClick={() => handleMove(index, 'down')} disabled={index === tasks.length - 1}>▼</button>
                        <button onClick={() => handleDelete(t._id)} className="delete-btn">Delete</button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
```

---

## שאלה מספר 2 (35 נק')

### מימוש שרת Express/Mongoose Todo REST API

#### `Task.js` (Mongoose Model)
```javascript
const mongoose = require('mongoose');

const taskSchema = new mongoose.Schema({
    text: { type: String, required: true },
    order: { type: Number, default: 0 }
});

module.exports = mongoose.model('Task', taskSchema);
```

#### `server.js` (Express REST API)
```javascript
const express = require('express');
const mongoose = require('mongoose');
const Task = require('./Task');

const app = express();
app.use(express.json());

// 1. GET all tasks sorted by order
app.get('/api/tasks', async (req, res) => {
    try {
        const tasks = await Task.find().sort({ order: 1 });
        res.status(200).json(tasks);
    } catch (err) {
        res.status(500).json({ error: "Database Error" });
    }
});

// 2. POST create task
app.post('/api/tasks', async (req, res) => {
    try {
        const count = await Task.countDocuments();
        const task = new Task({ text: req.body.text, order: count });
        await task.save();
        res.status(201).json(task);
    } catch (err) {
        res.status(500).json({ error: "Database Error" });
    }
});

// 3. DELETE task
app.delete('/api/tasks/:id', async (req, res) => {
    try {
        const task = await Task.findByIdAndDelete(req.params.id);
        if (!task) return res.status(404).json({ error: "Task not found" });
        res.status(200).json({ status: "Success" });
    } catch (err) {
        res.status(500).json({ error: "Database Error" });
    }
});

// 4. PUT reorder tasks (Swap orders)
app.put('/api/tasks/reorder', async (req, res) => {
    try {
        const { taskId1, taskId2 } = req.body;
        const task1 = await Task.findById(taskId1);
        const task2 = await Task.findById(taskId2);

        if (!task1 || !task2) return res.status(404).json({ error: "Tasks not found" });

        const tempOrder = task1.order;
        task1.order = task2.order;
        task2.order = tempOrder;

        await task1.save();
        await task2.save();

        res.status(200).json({ status: "Success" });
    } catch (err) {
        res.status(500).json({ error: "Reorder Failed" });
    }
});

mongoose.connect('mongodb://localhost:27017/todo')
    .then(() => app.listen(3000, () => console.log("Todo server listening on port 3000")));
```

---

## שאלה מספר 3 (10 נק')

### זיהוי והסבר של 3 באגים בתקשורת TCP (C++)
1.  **שרת - הגדרת כתובת שגויה:** השרת מגדיר `sin.sin_addr.s_addr = SOCK_STREAM;`. הקבוע `SOCK_STREAM` מייצג את סוג הסוקט ולא כתובת IP. השרת צריך להאזין לכל הממשקים בעזרת `INADDR_ANY`.
2.  **לקוח - איפוס מצביע במקום זיכרון:** הלקוח מפעיל `memset(&data_addr, 0, sizeof(data_addr));` כאשר `data_addr` הוא ככל הנראה פוינטר (`char*`), ולכן פעולה זו מאפסת רק את גודל המצביע (4 או 8 בתים) ולא את כל המערך המיועד.
3.  **לקוח - אורך הודעה שגוי ב-send:** הלקוח שולח הודעה בעזרת `strlen(sock)`. משתנה הסוקט הוא מספר שלם (`int`) ולא מחרוזת, ולכן הפעלת `strlen` עליו היא שגיאת הידור חמורה (או גורמת לגישת זיכרון לא חוקית). יש להעביר את אורך ההודעה המיועדת לשליחה.

---

## שאלה מספר 4 (20 נק')

### סעיף א
*   **הפלט המדויק:**
    ```
    Hello World
    Hello World
    Hello World
    ```
*   **הסבר:** 
    *   האובייקט `c` נוצר מטיפוס `C`.
    *   סדר הבנייה: ראשית נבנה האב הוירטואלי `A` פעם אחת (מדפיס `"Hello World"`).
    *   לאחר מכן נבנה `B` (מדפיס `"Hello World"` כיוון שהוא יורש את פונקציית `print` מ-`A`).
    *   לבסוף מופעל בנאי המחלקה `C` (מדפיס `"Hello World"`).

### סעיף ב
*   **הפלט המדויק:**
    ```
    X:1
    ```
*   **הסבר (עקרון הדומיננטיות בירושה וירטואלית):**
    *   במחלקה `C`, ההשמה `x=3` מתבצעת בבנאי. מכיוון ש-`B` יורש מ-`A` באופן וירטואלי ומגדיר משתנה `x` משלו, משתנה זה (`B::x`) **מכסה ומציף (dominates)** את המשתנה `A::x` עבור המחלקה `C`.
    *   לכן, `x=3` בבנאי של `C` מעדכן את `B::x` ל-`3`. המשתנה `A::x` נשאר ללא שינוי בערכו המקורי `1`.
    *   הקריאה `c.print()` מפעילה את הפונקציה `print()` השייכת ל-`A`. בתוך scope הפונקציה הזו, הפניה ל-`x` מגיעה ישירות מטווח הראייה של `A`, כלומר `A::x`, ולכן מודפס הערך `1`.
    ```
