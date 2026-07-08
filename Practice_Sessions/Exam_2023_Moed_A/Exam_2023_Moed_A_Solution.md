# Past Exam Solution Key — 2023 Semester A, Moed A

---

## שאלה מספר 1 (20 נק')

### א. טבלאות וירטואליות (VTABLE)
*   **VTABLE של Foo:**
    *   `VTABLE_Foo[0]`: `Foo::x`
    *   `VTABLE_Foo[1]`: `Foo::y`
*   **VTABLE של Bar:**
    *   `VTABLE_Bar[0]`: `Bar::x` (override)
    *   `VTABLE_Bar[1]`: `Foo::y` (inherited)
    *   `VTABLE_Bar[2]`: `Bar::i` (new virtual)
*   **VTABLE של Caz:**
    *   `VTABLE_Caz[0]`: `Caz::x` (override)
    *   `VTABLE_Caz[1]`: `Foo::y` (inherited)
    *   `VTABLE_Caz[2]`: `Caz::i` (override)
    *   `VTABLE_Caz[3]`: `Caz::z` (new virtual)

### ב. פלט הקוד
```
1
1
7

1
1
7
1
1
7
13

6

11

10

15

6
5
18
1
1
8
9
3
3

14
9
3
3
9
3
3
3
```

#### הסבר מעקב ריצה מפורט:

1. **יצירת האובייקט `p1` (`Foo *p1 = new Bar();`):**
   * הקומפיילר מקצה זיכרון ומבצע את הבנאי של `Bar`.
   * תחילה מופעל בנאי מחלקת הבסיס `Foo`: מדפיס `1`.
   * לאחר מכן מאותחלים חברי המחלקה `Bar`. ל-`Bar` יש משתנה חבר `Foo a`, ולכן מופעל הבנאי הדיפולטיבי של `Foo` עבור `a`: מדפיס `1`.
   * בבנאי של `Bar` אין אתחול מפורש של הבסיס `Foo` או הממבר `a` ברשימת האתחול, ולכן הקומפיילר קורא לבנאים הדיפולטיביים שלהם.
   * לבסוף מופעל גוף הבנאי של `Bar`: מדפיס `7`.
   * **פלט מצטבר:** `1`, `1`, `7`.

2. **יצירת האובייקט `p2` (`Bar *p2 = new Caz();`):**
   * מופעל הבנאי של `Caz`.
   * תחילה נבנה האב `Bar` (שבו נבנה תחילה האב `Foo` -> מדפיס `1`, מאותחל הממבר `Foo a` -> מדפיס `1`, וגוף הבנאי של `Bar` -> מדפיס `7`).
   * לאחר מכן מאותחלים חברי המחלקה `Caz`. ל-`Caz` יש משתנה חבר `Bar b`. אתחול `b` מפעיל את הבנאי הדיפולטיבי של `Bar` (הבונה תחילה את הבסיס `Foo` -> מדפיס `1`, את הממבר `Foo a` -> מדפיס `1`, וגוף הבנאי של `Bar` -> מדפיס `7`).
   * לבסוף מופעל גוף הבנאי של `Caz`: מדפיס `13`.
   * **פלט מצטבר:** `1`, `1`, `7`, `1`, `1`, `7`, `13`.

3. **הקריאה `p1->z();`:**
   * המצביע `p1` הוא מטיפוס `Foo*`. הפונקציה `z()` אינה וירטואלית ב-`Foo`.
   * לכן מתבצע הידור סטטי (Static Binding) לפונקציה `Foo::z()`: מדפיסה `6`.

4. **הקריאה `p2->z();`:**
   * המצביע `p2` הוא מטיפוס `Bar*`. הפונקציה `z()` אינה וירטואלית ב-`Bar` (אף על פי ש-`Caz` מגדירה אותה כוירטואלית, ההגדרה של `Bar::z` היא לא וירטואלית באב).
   * לכן מתבצע הידור סטטי לפונקציה `Bar::z()`: מדפיסה `11`.

5. **הקריאה `p1->x();`:**
   * הפונקציה `x()` מוגדרת כוירטואלית ב-`Foo`. מתבצע הידור דינמי (Dynamic Binding).
   * היות והאובייקט בפועל הוא `Bar`, מופעלת הפונקציה הדורסת `Bar::x()`: מדפיסה `10`.

6. **הקריאה `p2->x();`:**
   * הפונקציה `x()` היא וירטואלית. האובייקט בפועל הוא `Caz`.
   * מופעלת הפונקציה הדורסת `Caz::x()`: מדפיסה `15`.

7. **הקריאה `p1->y();`:**
   * הפונקציה `y()` היא וירטואלית ב-`Foo` ואינה דרוסה ב-`Bar`.
   * לכן מופעלת `Foo::y()`. בתוכה נקראת הפונקציה `z()`. מכיוון ש-`z()` אינה וירטואלית ב-`Foo`, מתבצע הידור סטטי ל-`Foo::z()` המדפיסה `6`.
   * לאחר מכן גוף `Foo::y()` מדפיס `5`.
   * **פלט מצטבר:** `6`, `5`.

8. **הקריאה `c->j();`:**
   * מבוצעת המרה והפעלת המתודה `j()` של `Caz`:
     * גוף הפונקציה מדפיס `18`.
     * הפונקציה מחזירה את הממבר `b` (מטיפוס `Bar`) **על ידי ערך (by value)**.
     * החזרה בערך מפעילה את בנאי ההעתקה המותאם של `Bar` (`Bar(const Bar&)`).
     * בנאי ההעתקה המוגדר של `Bar` לא קורא במפורש לבנאי העתקה של אב או של הממבר, ולכן הקומפיילר מפעיל עבורם בנאי דיפולטיבי: הבסיס `Foo` נבנה דיפולטיבית -> מדפיס `1`, והממבר `Foo a` נבנה דיפולטיבית -> מדפיס `1`. לאחר מכן גוף בנאי ההעתקה של `Bar` מדפיס `8`.
     * האובייקט הזמני המוחזר מהפונקציה נהרס מיד בסיום הביטוי:
       * מופעל הדיסטרקטור הוירטואלי של `Bar`: מדפיס `9`.
       * הממבר `a` שלו נהרס: מדפיס `3`.
       * חלק האב `Foo` נהרס: מדפיס `3`.
     * **פלט מצטבר:** `18`, `1`, `1`, `8`, `9`, `3`, `3`.

9. **הקריאה `delete p2;`:**
   * `p2` מצביע ל-`Caz` וטיפוסו `Bar*`. הדיסטרקטור של `Bar` הוא וירטואלי, ולכן מתבצע הידור דינמי לדיסטרקטור של `Caz` (`~Caz`):
     * גוף `Caz::~Caz()` מופעל: מדפיס `14`.
     * הממבר `Bar b` של `Caz` נהרס (מפעיל `Bar::~Bar` -> מדפיס `9`, הממבר `Foo a` שלו נהרס -> מדפיס `3`, והאב `Foo` שלו נהרס -> מדפיס `3`).
     * חלק האב `Bar` של `Caz` נהרס (מפעיל `Bar::~Bar` -> מדפיס `9`, הממבר `Foo a` שלו נהרס -> מדפיס `3`, והאב `Foo` שלו נהרס -> מדפיס `3`).
     * **פלט מצטבר:** `14`, `9`, `3`, `3`, `9`, `3`, `3`.

10. **הקריאה `delete p1;`:**
    * `p1` מצביע ל-`Bar` וטיפוסו `Foo*`.
    * **אבל** הדיסטרקטור של `Foo` אינו וירטואלי!
    * לכן מתבצע הידור סטטי לדיסטרקטור `Foo::~Foo()` בלבד: מדפיס `3`. הדיסטרקטור של `Bar` והחברים שלו אינם נקראים כלל (שגיאת זליגת זיכרון קלאסית).

---

## שאלה מספר 2 (30 נק')

### סעיף א
*   **קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** המחלקה `Waldo` יורשת מ-`Bar` (ירושה רגילה ולא וירטואלית מ-`Foo`) ומ-`Caz` ו-`Qux` (ירושה וירטואלית מ-`Foo`). כתוצאה מכך, נוצרים שני מופעים נפרדים של מחלקת הבסיס `Foo` בתוך `Waldo`. הקריאה `waldo.foo1(...)` היא דו-משמעית (ambiguous) מכיוון שלקומפיילר יש שני נתיבי ירושה שונים ל-`Foo` שאינם מתלכדים, ולכן הוא לא יודע איזה מופע להפעיל.

### סעיף ב
*   **קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** הקריאה `waldo.bar1(...)` מנסה להפעיל את הפונקציה המוגדרת ב-`Bar`. בתוך `Bar::bar1`, מתבצעת קריאה ל-`foo1(foo)`. מכיוון ש-`foo1` היא פונקציה וירטואלית וסוג האובייקט בזמן ריצה הוא `Waldo`, מתבצע הידור דינמי. הקומפיילר מזהה שהמרת המצביע `this` (שהוא מטיפוס `Waldo*`) לטיפוס `Foo*` היא דו-משמעית באובייקט `Waldo` (בגלל כפל מופעי `Foo`), ולכן הקוד לא מתקמפל.

### סעיף ג
*   **קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** במחלקה `Waldo` החדשה הוגדרה פונקציה `void bar1(char* x)`. ב-C++, הגדרת פונקציה במחלקה נגזרת **דורסת ומסתירה** (hides) את כל הפונקציות בעלות אותו השם במחלקות הבסיס, ללא קשר לחתימת הפרמטרים שלהן. לכן, הפונקציה `Bar::bar1(int)` מוסתרת. הקריאה `waldo.bar1(125)` מנסה להתאים את הפרמטר `125` (מטיפוס `int`) לפונקציה היחידה הגלויה `bar1(char*)`, המרה זו מ-`int` ל-`char*` אינה חוקית וההידור נכשל.

---

## שאלה מספר 3 (20 נק')

### סעיף א (ת'רדים וערכי foo)
*   **הערכים האפשריים:** `10`, `13`, `16`, `19`, `22`, `25`.
*   **הסבר:** 
    *   הת'רד השני מבצע את הפעולה `foo = foo + 3` בצורה אטומית (סך הכל מוסיף 15 ב-5 איטרציות).
    *   הת'רד הראשון מבצע את הפעולה `foo = foo + 2` בצורה **לא אטומית** (קריאה, הוספה, כתיבה).
    *   בשל חוסר האטומיות של הת'רד הראשון, הוא יכול לקרוא ערך ישן, להיעצר, לאפשר לת'רד השני לבצע מספר איטרציות אטומיות, ואז לכתוב את הערך הישן פלוס 2 ובכך "לדרוס" ולאבד את העדכונים של הת'רד השני.
    *   כל דריסה כזו גורמת לאיבוד של כפולות של 3 מהסכום המקסימלי (25). מספר הדריסות יכול לנוע בין 0 (תוצאה 25) ל-5 (תוצאה 10).

### סעיף ב (4 בעיות ב-UDP)
1.  **שימוש ב-TCP במקום UDP:** שני הצדדים יוצרים סוקט עם הפרמטר `SOCK_STREAM` (המתאים ל-TCP) במקום `SOCK_DGRAM` (המתאים ל-UDP).
2.  **שליחת הודעה ריקה:** השרת מבצע `sendto` עם גודל buffer של `0`, מה ששולח פאקטים ריקים ללא תוכן.
3.  **שליחת תגובה לעצמו:** השרת שולח את התגובה לכתובת `sin` (שהיא כתובת השרת עצמו) במקום לכתובת `from` שהתקבלה מהלקוח ב-`recvfrom`.
4.  **שגיאת טיפוסים ב-recvfrom:** המשתנה `buffer` מוגדר כ-`char` בודד, אך מועבר ל-`recvfrom` כמשתנה ישיר ולא ככתובת (`&buffer` או מצביע), מה שגורם לשגיאת קומפילציה חמורה.

---

## שאלה מספר 4 (30 נק')

### א. תיאור מחלקות וממשקים (`Mp4.h`, `Avi.h`, `Collection.h`)

#### `VideoElement.h`
```cpp
#ifndef VIDEO_ELEMENT_H
#define VIDEO_ELEMENT_H

class VideoElement {
public:
    virtual ~VideoElement() = default;
    virtual int count() const = 0;
    virtual int size() const = 0;
    virtual double totalQuality(int& mp4Count) const = 0;
};

#endif
```

#### `Mp4.h`
```cpp
#ifndef MP4_H
#define MP4_H

#include "VideoElement.h"

class Mp4 : public VideoElement {
private:
    char* filename;
    int filesize;
    char* createDate;
    char* modifyDate;
    char* tag;
    int videoQuality;

public:
    Mp4(const char* name, int size, const char* created, const char* modified, const char* t, int quality);
    ~Mp4() override;

    int count() const override { return 1; }
    int size() const override { return filesize; }
    double totalQuality(int& mp4Count) const override {
        mp4Count++;
        return videoQuality;
    }
};

#endif
```

#### `Avi.h`
```cpp
#ifndef AVI_H
#define AVI_H

#include "VideoElement.h"

class Avi : public VideoElement {
private:
    char* filename;
    int filesize;
    char* createDate;
    char* modifyDate;
    char* tag;
    int compression;

public:
    Avi(const char* name, int size, const char* created, const char* modified, const char* t, int comp);
    ~Avi() override;

    int count() const override { return 1; }
    int size() const override { return filesize; }
    double totalQuality(int& mp4Count) const override { return 0; }
};

#endif
```

#### `Collection.h`
```cpp
#ifndef COLLECTION_H
#define COLLECTION_H

#include "VideoElement.h"

class Collection : public VideoElement {
private:
    char* name;
    VideoElement** elements;
    int capacity;
    int currentSize;

    void resize();

public:
    Collection(const char* colName);
    ~Collection() override;

    void add(VideoElement* elem);
    int count() const override;
    int size() const override;
    double totalQuality(int& mp4Count) const override;
    double avg() const;
};

#endif
```

### ב. מימוש הדיסטרקטורים (`.cpp`)

#### `Mp4.cpp` (Destructor only)
```cpp
#include "Mp4.h"

// Helper copy function omitted for brevity
Mp4::~Mp4() {
    delete[] filename;
    delete[] createDate;
    delete[] modifyDate;
    delete[] tag;
}
```

#### `Avi.cpp` (Destructor only)
```cpp
#include "Avi.h"

Avi::~Avi() {
    delete[] filename;
    delete[] createDate;
    delete[] modifyDate;
    delete[] tag;
}
```

#### `Collection.cpp` (Destructor & Functions)
```cpp
#include "Collection.h"

Collection::~Collection() {
    delete[] name;
    for (int i = 0; i < currentSize; ++i) {
        delete elements[i]; // Deep delete
    }
    delete[] elements;
}
```

### ג. מימוש הפונקציות (count, size, avg)

```cpp
#include "Collection.h"

int Collection::count() const {
    int total = 0;
    for (int i = 0; i < currentSize; ++i) {
        total += elements[i]->count();
    }
    return total;
}

int Collection::size() const {
    int totalBytes = 0;
    for (int i = 0; i < currentSize; ++i) {
        totalBytes += elements[i]->size();
    }
    return totalBytes;
}

double Collection::totalQuality(int& mp4Count) const {
    double sum = 0;
    for (int i = 0; i < currentSize; ++i) {
        sum += elements[i]->totalQuality(mp4Count);
    }
    return sum;
}

double Collection::avg() const {
    int mp4Count = 0;
    double totalQ = totalQuality(mp4Count);
    if (mp4Count == 0) return 0.0;
    return totalQ / mp4Count;
}
```
