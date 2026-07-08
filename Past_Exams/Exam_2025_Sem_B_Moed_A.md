# 2025 Semester B, Moed A (תכנות מערכות מתקדם תשפ"ה)

## שאלה מספר 1 (35 נק')
עליכם לבנות שרת אינטרנטי NodeJS בארכיטקטורת MVC, על בסיס שרת ה-gmail שדיברנו עליו במהלך הקורס. ה-API שעליכם לממש:
*   עבור הכתובת `api/labels/`: פעולת GET מחזירה את כל התוויות. פעולת POST יוצרת תווית חדשה.
*   עבור הכתובת `api/labels/:id/`: פעולת GET מחזירה את התווית. פעולת PATCH מעדכנת את התווית. פעולת DELETE מוחקת את התווית.
הקפידו להחזיר סטטוס HTTP מתאים. המידע נשמר בשרת MongoDB (ישירות או בעזרת Mongoose). יש להקפיד על קוד נקי ועל עקרונות SOLID ו-Loose Coupling.

---

## שאלה מספר 2 (35 נק')
בשאלה זו עליכם לכתוב חלק מאפליקציית Web בעזרת React, HTML, CSS, JS (החלק שיעבוד מול השרת של שאלה 1). עליכם לממש את התפריט השמאלי של מסך הראשי הכולל את רשימת התוויות. יש לממש את הקוד כך שכל הפונקציונליות שהוגדרה בשאלה 1 (CRUD של תוויות) ניתנת להפעלה. יש להקפיד על CSS כדי לעצב את התפריט בדומה ל-Gmail, ולחלק את רכיבי ה-React בצורה חכמה.

---

## שאלה מספר 3 (10 נק')
נתון הקוד הבא ב-++C:
```cpp
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main() {
    fork();
    fork();
    fork();
    cout << "foo\n";
}
```
מה הפלט המדויק של הקוד?

---

## שאלה מספר 4 (20 נק', שני סעיפים)
**א. (10 נק')** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;
class A {
public:
    int x;
    A() { x=1; }
};
class B: public A {
public:
    B() { x=2; }
    void fun() { cout << "B " << x; }
};
class C: public A {
public:
    C() { x=3; }
    void fun() { cout << "C" << x; }
};
class D: public B, public C {
public:
    D() { x=4; }
};
int main() {
    B *b = new D();
    b->fun();
    return 0;
}
```
הסבירו שגיאת קומפילציה/ריצה, או כתבו פלט מדויק.

**ב. (10 נק')** כעת מחלקות B ו-C יורשות את A באופן וירטואלי (`virtual public A`). הפונקציות זהות. קוד ה-main שונה ל:
```cpp
int main() {
    D *d = new D();
    d->fun();
    return 0;
}
```
הסבירו שגיאת קומפילציה/ריצה, או כתבו פלט מדויק.
