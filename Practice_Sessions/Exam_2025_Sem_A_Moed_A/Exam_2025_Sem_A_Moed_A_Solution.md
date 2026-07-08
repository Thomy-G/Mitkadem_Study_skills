# Past Exam Solution Key — 2025 Semester A, Moed A

---

## שאלה מספר 1 (25 נק')
השלמות הקוד המדויקות עבור כל אחד מ-12 המלבנים:

1. **מלבן מספר 1 (בקובץ `app.js`):**
   ```javascript
   const express = require('express');
   ```
2. **מלבן מספר 2 (בקובץ `app.js`):**
   ```javascript
   const app = express();
   ```
3. **מלבן מספר 3 (בקובץ `routes/article.js`):**
   ```javascript
   const express = require('express');
   var router = express.Router();
   ```
4. **מלבן מספר 4 (בקובץ `routes/article.js`):**
   ```javascript
   .get(articleController.getArticles)
   ```
5. **מלבן מספר 5 (בקובץ `routes/article.js`):**
   ```javascript
   .post(articleController.createArticle);
   ```
6. **מלבן מספר 6 (בקובץ `routes/article.js`):**
   ```javascript
   route('/:id')
   ```
7. **מלבן מספר 7 (בקובץ `routes/article.js`):**
   ```javascript
   .get(articleController.getArticle)
   ```
8. **מלבן מספר 8 (בקובץ `routes/article.js`):**
   ```javascript
   .put(articleController.updateArticle)
   ```
9. **מלבן מספר 9 (בקובץ `routes/article.js`):**
   ```javascript
   .delete(articleController.deleteArticle);
   ```
10. **מלבן מספר 10 (בקובץ `controllers/article.js`):**
    ```javascript
    const { title, published } = req.body;
    const article = await articleService.createArticle(title, published);
    res.status(201).json(article);
    ```
11. **מלבן מספר 11 (בקובץ `services/article.js`):**
    ```javascript
    article.title = title;
    await article.save();
    ```
12. **מלבן מספר 12 (בקובץ `models/article.js`):**
    ```javascript
    const Article = new mongoose.Schema({
    ```

---

## שאלה מספר 2 (25 נק')

### א. אנדרואיד XML Layout
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    android:padding="16dp">

    <Button
        android:id="@+id/btn_foo"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Foo" />

    <EditText
        android:id="@+id/et_bar"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:hint="Bar" />

    <Button
        android:id="@+id/btn_foobar"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="FOOBAR" />

</LinearLayout>
```

### ב. עיצוב CSS וצבעי כפתורים
*   **צבע כפתור Foo (שמאלי):** **אדום** (Red)
*   **צבע כפתור Bar (אמצעי):** **ירוק** (Green)
*   **צבע כפתור Foobar (ימני):** **כחול** (Blue)
*   **הסבר מפורט:**
    1.  הכפתור הראשון `Foo` מתאים רק לסלקטור הכללי `button` שקובע צבע רקע אדום.
    2.  הכפתור השני `Bar` מתאים ל-`button`, ל-`.caz` ול-`button.caz`. רמת הספציפיות (Specificity) של `button.caz` היא `0,0,1,1` (אלמנט + קלאס) שהיא גבוהה יותר מזו של `.caz` (`0,0,1,0`) או `button` (`0,0,0,1`), ולכן צבע הרקע הוא ירוק.
    3.  הכפתור השלישי `Foobar` בעל מזהה ייחודי `id="bar"`. סלקטור ה-ID (`#bar`) הוא בעל ספציפיות גבוהה מאוד (`0,1,0,0`), הגוברת על סלקטורים של קלאס ואלמנט, ולכן צבעו כחול.

---

## שאלה מספר 3 (20 נק')

### טבלת קבצי ה-Objects ב-Git
לאחר הוספת הקבצים ויצירת הקומיטים, נוצרים 7 אובייקטים בתוך `.git/objects`:

| סוג האובייקט | תוכן / תפקיד האובייקט |
| :--- | :--- |
| **Blob 1** | מכיל את המחרוזת "bar" (משותף ל-`foo.txt` ול-`a.txt` כיוון שהתוכן שלהם זהה). |
| **Blob 2** | מכיל את המחרוזת "cazzzzz" (התוכן של `caz.txt`). |
| **Tree 1** | מייצג את ספריית השורש בקומיט הראשון: ממפה את `foo.txt` ל-**Blob 1**. |
| **Tree 2** | מייצג את תיקיית המשנה `mydir`: ממפה את `a.txt` ל-**Blob 1** ואת `caz.txt` ל-**Blob 2**. |
| **Tree 3** | מייצג את ספריית השורש החדשה בקומיט השני: ממפה את `foo.txt` ל-**Blob 1** ואת `mydir` ל-**Tree 2**. |
| **Commit 1** | מייצג את הקומיט הראשון: מצביע על **Tree 1** ואינו מכיל מצביע להורה. |
| **Commit 2** | מייצג את הקומיט השני: מצביע על **Tree 3** ומכיל מצביע הורה ל-**Commit 1**. |

---

## שאלה מספר 4 (30 נק')

### א. פלט קוד ה-JavaScript
*   **הפלט המדויק:**
    ```
    8
    1
    2
    3
    5
    9[object Object]++Alice
    1
    2
    10[object Promise]
    3
    5
    42
    ```
*   **הסבר מעקב ריצה:**
    1.  הפונקציה `bar()` מופעלת ומדפיסה `8`.
    2.  מתבצע `await foo()`. הפונקציה `foo` מופעלת ומדפיסה `1` ו-`2` (בתוך ה-Promise constructor שרץ סינכרונית). היא רושמת טיימר של 2 שניות ומשהה את `bar`.
    3.  לאחר 2 שניות, הטיימר מתעורר, מדפיס `3`, מבצע `resolve` ומדפיס `5`.
    4.  ה-Promise נפתר, `bar` מתעוררת ומדפיסה `9[object Object]++Alice`.
    5.  מתבצעת השמה `f2 = foo()`. `foo` מופעלת ומדפיסה `1` ו-`2` (שוב רושמת טיימר חדש). המשתנה `f2` מכיל אובייקט Promise לא פתור.
    6.  מבוצעת הדפסה של `10` ומחרוזת ייצוג הפרומיס: `10[object Promise]`.
    7.  מתבצע `await f2`. `bar` מושהית שוב וממתינה לטיימר השני.
    8.  לאחר עוד 2 שניות, הטיימר השני מתעורר, מדפיס `3` ו-`5`.
    9.  `bar` מתעוררת, שולפת את הערך `age` (שהוא `42`) ומדפיסה אותו.

### ב. עקרון ה-OCP ב-SOLID (קוד ++C והסברים)
עקרון ה-Open/Closed קובע כי ישויות תוכנה צריכות להיות **פתוחות להרחבה, אך סגורות לשינוי**.

```cpp
#include <iostream>
#include <vector>
#include <memory>

// Abstract interface - closed for modification, open for extension
class IFilterRule {
public:
    virtual ~IFilterRule() = default;
    virtual bool isMatch(int value) const = 0;
};

// Target processor closed for modifications
class NumberProcessor {
public:
    std::vector<int> filter(const std::vector<int>& numbers, const IFilterRule& rule) {
        std::vector<int> results;
        for (int num : numbers) {
            if (rule.isMatch(num)) {
                results.push_back(num);
            }
        }
        return results;
    }
};

// Concrete extensions (open for new rules)
class EvenFilter : public IFilterRule {
public:
    bool isMatch(int value) const override {
        return value % 2 == 0;
    }
};

class GreaterThanFilter : public IFilterRule {
private:
    int limit;
public:
    GreaterThanFilter(int lim) : limit(lim) {}
    bool isMatch(int value) const override {
        return value > limit;
    }
};

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    NumberProcessor processor;

    // Filter even numbers
    EvenFilter even;
    auto evens = processor.filter(data, even);

    // Filter values > 5
    GreaterThanFilter gt5(5);
    auto large = processor.filter(data, gt5);

    return 0;
}
```
*   **הסבר:** 
    *   המחלקה `NumberProcessor` סגורה לשינויים (Closed for Modification). אין צורך לגעת בקוד שלה כדי להוסיף סוגי פילטרים חדשים בעתיד.
    *   המערכת פתוחה להרחבות (Open for Extension) על ידי ירושה מראש הממשק `IFilterRule` (למשל, יצירת פילטרים חדשים כמו `OddFilter` או `NegativeFilter`).
