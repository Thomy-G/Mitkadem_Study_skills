# Past Exam Solution Key — 2025 Semester B, Moed A

---

## שאלה מספר 1 (35 נק')

### מימוש שרת Express/Mongoose Labels API

#### `Label.js` (Mongoose Model)
```javascript
const mongoose = require('mongoose');

const labelSchema = new mongoose.Schema({
    name: { type: String, required: true, unique: true },
    color: { type: String, default: '#808080' }
});

module.exports = mongoose.model('Label', labelSchema);
```

#### `labelController.js` (Express Router)
```javascript
const express = require('express');
const router = express.Router();
const Label = require('./Label');

// 1. GET all labels
router.get('/api/labels', async (req, res) => {
    try {
        const labels = await Label.find();
        res.status(200).json(labels);
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

// 2. POST create new label
router.post('/api/labels', async (req, res) => {
    try {
        const { name, color } = req.body;
        if (!name) return res.status(400).json({ error: "Name is required" });
        const label = new Label({ name, color });
        await label.save();
        res.status(201).json(label);
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

// 3. GET label by ID
router.get('/api/labels/:id', async (req, res) => {
    try {
        const label = await Label.findById(req.params.id);
        if (!label) return res.status(404).json({ error: "Label not found" });
        res.status(200).json(label);
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

// 4. PATCH update label
router.patch('/api/labels/:id', async (req, res) => {
    try {
        const label = await Label.findByIdAndUpdate(req.params.id, req.body, { new: true });
        if (!label) return res.status(404).json({ error: "Label not found" });
        res.status(200).json(label);
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

// 5. DELETE label
router.delete('/api/labels/:id', async (req, res) => {
    try {
        const label = await Label.findByIdAndDelete(req.params.id);
        if (!label) return res.status(404).json({ error: "Label not found" });
        res.status(204).send(); // 204 No Content
    } catch (err) {
        res.status(500).json({ error: "Server Error" });
    }
});

module.exports = router;
```

---

## שאלה מספר 2 (35 נק')

### מימוש תפריט התוויות ב-React ועיצוב ב-CSS

#### `LabelsSidebar.jsx` (React)
```jsx
import React, { useState, useEffect } from 'react';
import './LabelsSidebar.css';

export default function LabelsSidebar() {
    const [labels, setLabels] = useState([]);
    const [newLabelName, setNewLabelName] = useState('');

    // Fetch on mount
    useEffect(() => {
        fetchLabels();
    }, []);

    const fetchLabels = async () => {
        const res = await fetch('/api/labels');
        if (res.ok) {
            const data = await res.ok ? await res.json() : [];
            setLabels(data);
        }
    };

    const handleCreate = async (e) => {
        e.preventDefault();
        if (!newLabelName.trim()) return;
        const res = await fetch('/api/labels', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name: newLabelName })
        });
        if (res.ok) {
            const newLabel = await res.json();
            setLabels([...labels, newLabel]);
            setNewLabelName('');
        }
    };

    const handleDelete = async (id) => {
        const res = await fetch(`/api/labels/${id}`, { method: 'DELETE' });
        if (res.status === 204) {
            setLabels(labels.filter(l => l._id !== id));
        }
    };

    return (
        <div className="gmail-sidebar">
            <h3 className="sidebar-title">Labels</h3>
            <form onSubmit={handleCreate} className="label-form">
                <input 
                    placeholder="New label..." 
                    value={newLabelName}
                    onChange={e => setNewLabelName(e.target.value)} 
                />
                <button type="submit">Add</button>
            </form>
            <ul className="labels-list">
                {labels.map(l => (
                    <li key={l._id} className="label-item">
                        <span className="label-badge" style={{ backgroundColor: l.color }} />
                        <span className="label-name">{l.name}</span>
                        <button onClick={() => handleDelete(l._id)} className="delete-btn">×</button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
```

#### `LabelsSidebar.css` (CSS)
```css
.gmail-sidebar {
    width: 250px;
    background-color: #f6f8fc;
    font-family: 'Google Sans', Roboto, sans-serif;
    padding: 16px;
    border-radius: 16px;
}
.sidebar-title {
    color: #444746;
    font-size: 14px;
    margin-bottom: 12px;
}
.label-form {
    display: flex;
    gap: 8px;
    margin-bottom: 12px;
}
.label-form input {
    flex: 1;
    border: 1px solid #c4c7c5;
    border-radius: 8px;
    padding: 6px 12px;
}
.labels-list {
    list-style: none;
    padding: 0;
    margin: 0;
}
.label-item {
    display: flex;
    align-items: center;
    padding: 8px 12px;
    border-radius: 20px;
    cursor: pointer;
}
.label-item:hover {
    background-color: #e9eef6;
}
.label-badge {
    width: 12px;
    height: 12px;
    border-radius: 50%;
    margin-right: 12px;
}
.label-name {
    flex: 1;
    font-size: 14px;
    color: #1f1f1f;
}
.delete-btn {
    background: none;
    border: none;
    color: #444746;
    font-size: 18px;
    cursor: pointer;
}
```

---

## שאלה מספר 3 (10 נק')

### פלט קוד ה-Fork ב-++C
*   **הפלט המדויק:**
    ```
    foo
    foo
    foo
    foo
    foo
    foo
    foo
    foo
    ```
*   **הסבר:** 
    *   התוכנית מפעילה 3 קריאות `fork()` רצופות.
    *   כל קריאת `fork()` מכפילה את מספר התהליכים הפעילים.
    *   לאחר ה-`fork` הראשון יש 2 תהליכים. לאחר ה-`fork` השני יש 4 תהליכים. לאחר ה-`fork` השלישי יש 8 תהליכים.
    *   כל 8 התהליכים ממשיכים להדפסת ה-`cout` בשורה 14, ולכן המילה `"foo"` מודפסת בדיוק 8 פעמים.

---

## שאלה מספר 4 (20 נק')

### סעיף א
*   **ניתוח קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** הבנאי של `D` מנסה לבצע את ההשמה `x=4;`. המחלקה `D` יורשת מ-`B` ומ-`C` ירושה לא וירטואלית, ושניהם יורשים מ-`A` (המכילה את המשתנה `x`). כתוצאה מכך, בתוך `D` ישנם שני מופעים נפרדים של `x` (אחד מכל מסלול ירושה). הפניה ל-`x` היא דו-משמעית (ambiguous), והקומפיילר נכשל בשורה `x=4;`.

### סעיף ב
*   **ניתוח קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** ירושה וירטואלית פותרת את בעיית הדו-משמעות של המשתנה `x` (קיים רק מופע אחד משותף של `A::x` בתוך `D`), ולכן הבנאי של `D` מתקמפל בהצלחה. עם זאת, ב-`main` מתבצעת הקריאה `d->fun();`. היות ושתי מחלקות האב `B` ו-`C` מגדירות מתודה לא וירטואלית בשם `fun()`, הקריאה דרך המצביע ל-`D` אינה חד-משמעית, והקומפילציה נכשלת (ambiguous member lookup).
