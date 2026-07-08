# Past Exam Solution Key — 2025 Semester B, Moed C

---

## שאלה מספר 1 (35 נק')

### מימוש שרת חישובים ב-++C (POSIX Sockets) ולקוח ב-Python 3

#### C++ TCP Calculation Server (`server.cpp`)
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

void handle_client(int client_fd) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes = read(client_fd, buffer, 1024);
        if (bytes <= 0) break; // Client disconnected
        
        std::string req(buffer);
        std::stringstream ss(req);
        std::string op;
        double a, b;
        
        if (!(ss >> op >> a >> b)) {
            const char* err = "400 Bad Request\n";
            write(client_fd, err, strlen(err));
            continue;
        }

        double result = 0;
        bool valid = true;
        
        if (op == "add") {
            result = a + b;
        } else if (op == "sub") {
            result = a - b;
        } else if (op == "mul") {
            result = a * b;
        } else {
            valid = false;
        }

        if (valid) {
            std::string res = std::to_string(result) + "\n";
            write(client_fd, res.c_str(), res.length());
        } else {
            const char* not_found = "404 Not Found\n";
            write(client_fd, not_found, strlen(not_found));
        }
    }
    close(client_fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int port = std::stoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    while (true) {
        socklen_t len = sizeof(address);
        int client_fd = accept(server_fd, (struct sockaddr*)&address, &len);
        if (client_fd >= 0) {
            handle_client(client_fd); // Handle persistently in main thread for simplicity
        }
    }
    close(server_fd);
    return 0;
}
```

#### Python 3 Persistent Calculator Client (`client.py`)
```python
import sys
import socket

def run_client():
    if len(sys.argv) < 3:
        print("Usage: python client.py [IP] [PORT]")
        return
        
    ip = sys.argv[1]
    port = int(sys.argv[2])

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip, port))
    print("Connected to calculation server. Type commands (e.g., 'add 5 10', 'exit' to quit):")

    try:
        while True:
            cmd = input("> ")
            if cmd.strip().lower() == "exit":
                break
                
            s.send((cmd + "\n").encode("utf-8"))
            response = s.recv(1024).decode("utf-8")
            print("Server response:", response.strip())
    finally:
        s.close()
        print("Connection closed.")

if __name__ == "__main__":
    run_client()
```

---

## שאלה מספר 2 (35 נק')

### אפליקציית Web לחישובים אסינכרוניים (React)

```jsx
import React, { useState } from 'react';
import './Calculator.css';

export default function Calculator() {
    const [num1, setNum1] = useState('');
    const [num2, setNum2] = useState('');
    const [op, setOp] = useState('add');
    const [result, setResult] = useState(null);
    const [history, setHistory] = useState([]);
    const [error, setError] = useState('');

    const handleCalculate = async (e) => {
        e.preventDefault();
        setError('');
        setResult(null);

        // Validation
        const n1 = parseFloat(num1);
        const n2 = parseFloat(num2);
        if (isNaN(n1) || isNaN(n2)) {
            setError('Please enter valid numeric inputs.');
            return;
        }

        try {
            // Simulated REST call (sending computation parameters to server)
            const res = await fetch('/api/calculate', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ op, a: n1, b: n2 })
            });

            if (res.ok) {
                const data = await res.json();
                setResult(data.result);
                // Immutable update of history array
                setHistory(prev => [{ 
                    expression: `${n1} ${op === 'add' ? '+' : op === 'sub' ? '-' : '*'} ${n2}`,
                    result: data.result 
                }, ...prev]);
            } else {
                const errData = await res.json();
                setError(errData.error || 'Calculation failed.');
            }
        } catch (err) {
            setError('Failed to contact calculation server.');
        }
    };

    return (
        <div className="calc-container">
            <h2>Asynchronous Web Calculator</h2>
            <form onSubmit={handleCalculate} className="calc-form">
                <input type="number" value={num1} onChange={e => setNum1(e.target.value)} placeholder="Operand A" />
                <select value={op} onChange={e => setOp(e.target.value)}>
                    <option value="add">+</option>
                    <option value="sub">-</option>
                    <option value="mul">*</option>
                </select>
                <input type="number" value={num2} onChange={e => setNum2(e.target.value)} placeholder="Operand B" />
                <button type="submit">Calculate</button>
            </form>

            {error && <div className="error-alert">{error}</div>}
            {result !== null && <div className="result-alert">Result: {result}</div>}

            <div className="calc-history">
                <h3>Calculation History</h3>
                <ul>
                    {history.map((h, i) => (
                        <li key={i}>{h.expression} = <strong>{h.result}</strong></li>
                    ))}
                </ul>
            </div>
        </div>
    );
}
```

#### `Calculator.css`
```css
.calc-container {
    max-width: 500px;
    margin: 40px auto;
    padding: 24px;
    background-color: #ffffff;
    border-radius: 12px;
    box-shadow: 0 4px 12px rgba(0,0,0,0.1);
    font-family: Arial, sans-serif;
}
.calc-form {
    display: flex;
    gap: 12px;
    margin-bottom: 20px;
}
.calc-form input, .calc-form select, .calc-form button {
    padding: 10px;
    border-radius: 6px;
    border: 1px solid #ccc;
    font-size: 16px;
}
.calc-form button {
    background-color: #007bff;
    color: white;
    cursor: pointer;
    border: none;
}
.error-alert { color: red; margin-bottom: 12px; }
.result-alert { font-size: 18px; font-weight: bold; margin-bottom: 20px; }
.calc-history ul { list-style: none; padding: 0; }
.calc-history li { padding: 8px 0; border-bottom: 1px solid #eee; }
```

---

## שאלה מספר 3 (10 נק')

### זיהוי והסבר של 3 באגים בשרת NodeJS MVC
1.  **באג 1 (`app.js`):** שגיאת סינטקס בשורת ייבוא הראוטים (`const articleRoutes require(...)`). חסר אופרטור ההשמה (`=`) בין שם המשתנה לפונקציית ה-`require`.
2.  **באג 2 (`routes/article.js`):** הגדרת נתיב כפולה. שני הראוטים מוגדרים תחת `GET /`. הראוט השני מיועד להחזרת כתבה לפי ID, ולכן נדרש לשנות את הנתיב שלו ל-`/:id`.
3.  **באג 3 (`models/article.js`):** שגיאת סינטקס בהגדרת סכמת ה-Mongoose. השורה מכילה סוגריים מסולסלים וסוגריים רגילים סוגרים מיותרים באמצע ההגדרה (`title: { });`).

---

## שאלה מספר 4 (20 נק')

### א. הסבר על HTTP Cookies ופעולתם
*   **הסבר:** עוגיות הן צמדי מפתח-ערך הנשלחים על ידי שרתי אינטרנט לדפדפן של הלקוח בכותרת `Set-Cookie`. הדפדפן שומר אותן מקומית, ומצרף אותן אוטומטית לכל בקשת HTTP עתידית המופנית לאותו השרת בכותרת `Cookie`.
*   **שימוש:** עוגיות משמשות לניהול מצב (stateful sessions), התחברות משתמשים (Session ID), ושמירת העדפות לקוח.

### ב. הסבר על JSON Web Tokens (JWT) ופעולתם
*   **הסבר:** JWT הוא תקן פתוח להעברת מידע בצורה בטוחה בין לקוח לשרת כאובייקט JSON חתום קריפטוגרפית. המבנה שלו מורכב משלושה חלקים המופרדים בנקודות: `Header.Payload.Signature`.
    *   **Header**: סוג האסימון ואלגוריתם החתימה.
    *   **Payload**: הטענות (claims) כגון מזהה משתמש ותוקף. המידע אינו מוצפן אלא רק מקודד ב-Base64, לכן אין לשמור מידע רגיש.
    *   **Signature**: החתימה שנוצרת באמצעות מפתח סודי על השרת, ומאשרת שהודעה לא שונתה בדרך.
*   **שימוש:** משמש לאימות ללא מצב (Stateless Authentication) במערכות מבוזרות, ללא צורך לבדוק במסד הנתונים בכל בקשה.
