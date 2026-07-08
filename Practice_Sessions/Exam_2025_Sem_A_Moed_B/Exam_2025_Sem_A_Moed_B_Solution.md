# Past Exam Solution Key — 2025 Semester A, Moed B

---

## שאלה מספר 1 (25 נק')

### א. הסבר על HTTP Cookies ופעולתם
*   **הסבר:** עוגיות הן קבצי טקסט קטנים ששרת האינטרנט שולח לדפדפן של המשתמש. הן משמשות לשמירת מידע מזהה על הלקוח כדי להתגבר על היותו של פרוטוקול HTTP חסר-מצב (stateless).
*   **כיצד הן עובדות:**
    1.  הלקוח מבצע בקשת התחברות (למשל עם שם וסיסמה).
    2.  השרת מאמת את הפרטים ושולח תגובה המכילה כותרת `Set-Cookie` (למשל: `Set-Cookie: session_id=12345`).
    3.  הדפדפן שומר את העוגיה באופן מקומי.
    4.  בכל בקשה עתידית לאותו שרת, הדפדפן מצרף אוטומטית את העוגיה בכותרת `Cookie` (למשל: `Cookie: session_id=12345`), ובכך השרת מזהה את הלקוח ללא צורך בהזנת פרטים מחדש.

### ב. מימוש מנגנון התחברות (Login) מבוסס Cookies ב-Express
```javascript
const express = require('express');
const cookieParser = require('cookie-parser');
const path = require('path');

const app = express();
app.use(express.urlencoded({ extended: true }));
app.use(cookieParser("super_secret_key")); // מפתח חתימה לעוגיות חתומות

// 1. POST Login route
app.post('/login', (req, res) => {
    const { username, password } = req.body;
    if (username === "admin" && password === "12345") {
        // יצירת עוגיה חתומה (Signed Cookie) למניעת זיופים
        res.cookie('authenticated', 'true', { 
            signed: true, 
            httpOnly: true, // מונע גישה דרך JS/XSS
            maxAge: 600000 // תוקף ל-10 דקות
        });
        return res.redirect('/dashboard');
    }
    res.status(401).send("Unauthorized: Invalid Credentials");
});

// 2. Protected route using cookie validation
app.get('/dashboard', (req, res) => {
    // שליפת העוגיות החתומות
    if (req.signedCookies.authenticated === 'true') {
        return res.sendFile(path.join(__dirname, 'dashboard.html'));
    }
    res.status(403).send("Forbidden: Please login first");
});

app.listen(3000, () => console.log("Server listening on port 3000"));
```

---

## שאלה מספר 2 (25 נק')

### א. מימוש ממשק כרטיסיות (קוביות) ב-React

#### `CardList.jsx`
```jsx
import React, { useState } from 'react';
import './CardList.css';

export default function CardList() {
    const [cards, setCards] = useState([
        { 
            id: 1, 
            title: "World's Largest Foo", 
            author: "Baz Caz", 
            date: "2024-01-11", 
            category: "Mystery" 
        },
        { 
            id: 2, 
            title: "Unicorn Called Bar in", 
            author: "Grace Lewis", 
            date: "2024-01-10", 
            category: "Nature" 
        },
        { 
            id: 3, 
            title: "Wowwwwww! amazing", 
            author: "aaa bbb", 
            date: "2024-01-12", 
            category: "Guess" 
        }
    ]);

    const addCard = () => {
        const newId = cards.length + 1;
        const newCard = {
            id: newId,
            title: `Dynamic Item #${newId}`,
            author: "System User",
            date: new Date().toISOString().split('T')[0],
            category: "General"
        };
        // Immutable state update
        setCards([...cards, newCard]);
    };

    return (
        <div className="container">
            <button onClick={addCard} className="add-btn">Add</button>
            <div className="card-list">
                {cards.map(card => (
                    <div key={card.id} className="card-box">
                        <h2 className="card-title">{card.title}</h2>
                        <p className="card-meta">{card.author}, {card.date}</p>
                        <p className="card-category">{card.category}</p>
                        <span className="card-icon">☔</span>
                    </div>
                ))}
            </div>
        </div>
    );
}
```

#### `CardList.css`
```css
.container {
    padding: 20px;
    max-width: 400px;
    font-family: system-ui, -apple-system, sans-serif;
}

.add-btn {
    background-color: white;
    color: black;
    border: 2px solid black;
    border-radius: 4px;
    padding: 6px 12px;
    font-size: 16px;
    font-weight: bold;
    cursor: pointer;
    margin-bottom: 20px;
}

.add-btn:hover {
    background-color: #f0f0f0;
}

.card-list {
    display: flex;
    flex-direction: column;
    gap: 16px;
}

.card-box {
    background-color: black;
    color: white;
    border: 2px solid white;
    padding: 20px;
    display: flex;
    flex-direction: column;
    align-items: flex-start;
}

.card-title {
    font-size: 22px;
    font-weight: bold;
    margin: 0 0 12px 0;
}

.card-meta {
    font-size: 16px;
    color: #cccccc;
    margin: 0 0 4px 0;
}

.card-category {
    font-size: 16px;
    color: #bbbbbb;
    margin: 0 0 8px 0;
}

.card-icon {
    font-size: 20px;
    margin: 0;
}
```

#### אפשרות ב': מימוש ב-HTML / CSS / JS ונילה (ללא React)

##### `index.html`
```html
<!DOCTYPE html>
<html lang="he">
<head>
    <meta charset="UTF-8">
    <title>Card List</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="container">
        <button id="add-btn" class="add-btn">Add</button>
        <div id="card-list" class="card-list">
            <!-- קוביות התחלתיות כפי שמוצג בצילום -->
            <div class="card-box">
                <h2 class="card-title">World's Largest Foo</h2>
                <p class="card-meta">Baz Caz, 2024-01-11</p>
                <p class="card-category">Mystery</p>
                <span class="card-icon">☔</span>
            </div>
            <div class="card-box">
                <h2 class="card-title">Unicorn Called Bar in</h2>
                <p class="card-meta">Grace Lewis, 2024-01-10</p>
                <p class="card-category">Nature</p>
                <span class="card-icon">☔</span>
            </div>
            <div class="card-box">
                <h2 class="card-title">Wowwwwww! amazing</h2>
                <p class="card-meta">aaa bbb, 2024-01-12</p>
                <p class="card-category">Guess</p>
                <span class="card-icon">☔</span>
            </div>
        </div>
    </div>
    <script src="script.js"></script>
</body>
</html>
```

##### `script.js`
```javascript
document.addEventListener("DOMContentLoaded", () => {
    const addBtn = document.getElementById("add-btn");
    const cardList = document.getElementById("card-list");
    let cardCount = 3;

    addBtn.addEventListener("click", () => {
        cardCount++;
        
        // יצירת אלמנט קוביה חדש
        const cardBox = document.createElement("div");
        cardBox.className = "card-box";
        
        // יצירת תוכן פנימי עבור הקוביה החדשה
        cardBox.innerHTML = `
            <h2 class="card-title">Dynamic Item #${cardCount}</h2>
            <p class="card-meta">System User, ${new Date().toISOString().split('T')[0]}</p>
            <p class="card-category">General</p>
            <span class="card-icon">☔</span>
        `;
        
        // הוספה דינמית של הקוביה לרשימה
        cardList.appendChild(cardBox);
    });
});
```

##### `style.css`
*עיצוב ה-CSS זהה לזה שמוצג בפתרון ה-React לעיל.*


### ב. דיאגרמת Socket API מלאה של TCP
1.  `bind()`
2.  `listen()`
3.  `accept()`
4.  `connect()`
5.  `close()`

---

## שאלה מספר 3 (20 נק')

### הדגמת פיתוח TDD למערכת המלצת סרטים ב-++C

#### `MovieRecommender_Test.cpp` (Google Test suite)
```cpp
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <map>

// Target Recommender interface class
class MovieRecommender {
private:
    std::map<std::string, std::vector<std::string>> userHistory;
public:
    void addWatchedMovie(const std::string& userId, const std::string& movieId) {
        userHistory[userId].push_back(movieId);
    }
    
    std::vector<std::string> getRecommendations(const std::string& userId) const {
        auto it = userHistory.find(userId);
        if (it == userHistory.end()) return {};
        
        std::vector<std::string> recommendations;
        // Mock recommendation algorithm: recommend similar placeholder entries up to 10
        for (int i = 1; i <= 10; ++i) {
            std::string rec = "Movie_" + std::to_string(i);
            // Ensure we don't recommend a movie already watched
            bool watched = false;
            for (const auto& watchedMovie : it->second) {
                if (watchedMovie == rec) watched = true;
            }
            if (!watched) {
                recommendations.push_back(rec);
            }
        }
        return recommendations;
    }
};

// Test 1: Verification of basic watched movies database entry
TEST(MovieRecommenderTest, AddWatchedVerifyHistory) {
    MovieRecommender recommender;
    recommender.addWatchedMovie("user1", "Movie_A");
    auto recs = recommender.getRecommendations("user1");
    // Ensure returned list is populated
    EXPECT_FALSE(recs.empty());
}

// Test 2: Verify that watched movies are not recommended
TEST(MovieRecommenderTest, RecommendationsDoNotContainWatched) {
    MovieRecommender recommender;
    recommender.addWatchedMovie("user1", "Movie_1");
    auto recs = recommender.getRecommendations("user1");
    
    bool found = false;
    for (const auto& m : recs) {
        if (m == "Movie_1") found = true;
    }
    EXPECT_FALSE(found);
}

// Test 3: Recommendation count limit constraints
TEST(MovieRecommenderTest, RecommendationLimitUpToTen) {
    MovieRecommender recommender;
    recommender.addWatchedMovie("user1", "Movie_99");
    auto recs = recommender.getRecommendations("user1");
    EXPECT_LE(recs.size(), 10);
}
```

---

## שאלה מספר 4 (30 נק')

### א. פלט קוד ה-Fork וה-Wait
*   **הפלט המדויק:**
    ```
    1
    2
    3
    2
    2
    3
    ```
*   **הסבר מעקב הריצה שלב אחרי שלב:**
    1.  התהליך הראשי `P` מתחיל עם `i = 1`. `i % 3 != 0` (1 % 3 != 0) הוא `true`.
    2.  `P` מדפיס `1` (שורה 7) ומקדם את `i` ל-`2`.
    3.  `P` קורא ל-`fork()`. נוצר תהליך בן `C1`.
    4.  `P` (מזהה `foo != 0`) נכנס ל-`wait(NULL)` ונחסם.
    5.  `C1` (מזהה `foo == 0`) מדלג על ה-`if` וממשיך ללולאה הבאה עם `i = 2`.
    6.  `C1` בודק תנאי לולאה: `2 % 3 != 0` -> `true`.
    7.  `C1` מדפיס `2` ומקדם את `i` ל-`3`.
    8.  `C1` קורא ל-`fork()`. נוצר תהליך נכד `G1`.
    9.  `C1` נכנס ל-`wait(NULL)` ונחסם.
    10. `G1` ממשיך ללולאה עם `i = 3`. תנאי הלולאה `3 % 3 != 0` הוא `false`. `G1` יוצא מהלולאה ומסתיים.
    11. `C1` משתחרר מהחסימה ומדפיס את `i` (שהוא `3`).
    12. `C1` בודק תנאי לולאה עם `i = 3` -> `false`. `C1` מסתיים.
    13. `P` משתחרר מחסימת ה-`wait` ומדפיס את `i` (שהוא `2` בזיכרון של `P`).
    14. `P` ממשיך לאיטרציה הבאה עם `i = 2`. תנאי לולאה: `true`.
    15. `P` מדפיס `2` ומקדם את `i` ל-`3`.
    16. `P` קורא ל-`fork()`. נוצר תהליך בן `C2`.
    17. `P` נחסם ב-`wait(NULL)`.
    18. `C2` ממשיך עם `i = 3` -> תנאי לולאה `false`. `C2` מסתיים.
    19. `P` משתחרר מחסימה ומדפיס `3`.
    20. `P` בודק תנאי לולאה עם `i = 3` -> `false`. `P` מסתיים.

### ב. אפליקציית אנדרואיד MVVM עם Repository ו-LiveData (Java)

#### `TextRepository.java`
```java
package com.biu.telemetry;
import androidx.lifecycle.MutableLiveData;

public class TextRepository {
    public void updateText(String input, MutableLiveData<String> liveText) {
        liveText.setValue(input);
    }
}
```

#### `TextViewModel.java`
```java
package com.biu.telemetry;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TextViewModel extends ViewModel {
    private final MutableLiveData<String> currentText = new MutableLiveData<>("");
    private final TextRepository repository = new TextRepository();

    public LiveData<String> getCurrentText() {
        return currentText;
    }

    public void processInput(String input) {
        repository.updateText(input, currentText);
    }
}
```

#### `MainActivity.java`
```java
package com.biu.telemetry;

import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.ViewModelProvider;

public class MainActivity extends AppCompatActivity {
    private TextViewModel viewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText etInput = findViewById(R.id.et_input);
        TextView tvDisplay = findViewById(R.id.tv_display);
        Button btnUpdate = findViewById(R.id.btn_update);

        // Bind ViewModel
        viewModel = new ViewModelProvider(this).get(TextViewModel.class);

        // Observe LiveData updates
        viewModel.getCurrentText().observe(this, newText -> {
            tvDisplay.setText(newText);
        });

        // Click Handler triggers ViewModel action
        btnUpdate.setOnClickListener(v -> {
            String text = etInput.getText().toString();
            viewModel.processInput(text);
        });
    }
}
```
