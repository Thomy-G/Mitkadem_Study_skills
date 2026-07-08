# 2025 Semester B, Moed B (תכנות מערכות מתקדם תשפ"ה)

## שאלה מספר 1 (35 נק')
רוצים לבנות מערכת פשוטה לניהול משימות אינטרנטית. המערכת מאפשרת להזין משימה חדשה, לנהל את סדר המשימות הקיימות ולמחוק משימות. עליכם להציג מימוש של כל קוד צד הלקוח (`JS+CSS+HTML` או ב-React) שייצר אפליקציה למערכת ה-Todo. הפונקציונליות כוללת כפתורי Add, Delete, Down, Up שעושים פניות א-סינכרוניות (Fetch) אל השרת לעדכון הנתונים, בנוסף לעדכון ה-UI.

---

## שאלה מספר 2 (35 נק')
בשאלה זו עליכם לבנות שרת אינטרנטי מלא ב-NodeJS בארכיטקטורת MVC, עבור אפליקציית ניהול המשימות משאלה 1. על השרת לתמוך ב-RESTful API לכל פעולות ה-CRUD של משימות ופעולות שינוי סדר. המידע נשמר ב-MongoDB (ישירות או בעזרת Mongoose). יש להקפיד על החזרת סטאטוסי HTTP מתאימים בתצורת JSON, שילוב עקרונות SOLID, ו-Loose Coupling. 

---

## שאלה מספר 3 (10 נק')
אליס ובוב כתבו קליינט וסרבר בפרוטוקול TCP ב-++C. הקוד מכיל 3 באגים שמונעים מהם לתקשר. זהו והסבירו את 3 הבאגים:
*(הערה: קוד השרת משתמש בשגיאה ב-`sin.sin_addr.s_addr = SOCK_STREAM;`. קוד הקליינט מפעיל בטעות `memset(&data_addr, 0, sizeof(data_addr));` כשהפרמטר הוא פוינטר למערך, ושולח הודעה בעזרת האורך השגוי `strlen(sock)` במקום אורך ההודעה)*.

---

## שאלה מספר 4 (20 נק', שני סעיפים)
**א. (10 נק')** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;
class A {
public:
    A() { print(); }
    void print() { cout << "Hello World" << endl; }
};
class B: virtual public A {
public:
    B() { print(); }
};
class C: virtual public A, public B {
public:
    C() { print(); }
};
int main() {
    C c;
    return 0;
}
```
מה הפלט המדויק?

**ב. (10 נק')** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;
class A {
protected: int x;
public:
    A() { x=1; }
    void print() { cout << "X:" << x << endl; }
};
class B: virtual public A {
protected: int x;
public:
    B() { x=2; }
};
class C: virtual public A, public B {
public:
    C() { x=3; }
};
int main() {
    C c;
    c.print();
    return 0;
}
```
מה הפלט המדויק?
