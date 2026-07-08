# 2023 Semester A, Moed B (תכנות מתקדם 1 תשפ"ג)

## שאלה מספר 1 (20 נק', שני סעיפים, שניהם לגבי הקוד המוצג בשאלה זו)
נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

class Foo {
public:
    Foo() {cout<<"1"<<endl;}
    Foo(const Foo& a){cout<<"2"<<endl;}
    ~Foo() {cout<<"3"<<endl;}
    Foo f(){cout<<"4"<<endl; return *this;}
    virtual void g(){h(); cout<<"5"<<endl;}
    virtual void h(){i(); cout<<"6"<<endl;}
    void i(){cout<<"7"<<endl;}
};

class Bar: public Foo {
public:
    Bar() {cout<<"8"<<endl;}
    Bar(const Bar& b){cout<<"9"<<endl;}
    virtual ~Bar() {cout<<"10"<<endl;}
    virtual Foo f(){cout<<"11"<<endl; return *this;}
    virtual void g(){k(); cout<<"12"<<endl;}
    virtual void h(){i(); cout<<"13"<<endl;}
    virtual void i(){cout<<"14"<<endl;}
    void k() {cout<<"15"<<endl;}
};

class Caz: public Bar {
    Foo a;
public:
    Caz() {cout<<"16"<<endl;}
    Caz(const Caz& c){cout<<"17"<<endl;}
    virtual ~Caz() {cout<<"18"<<endl;}
    virtual Foo f(){g(); cout<<"19"<<endl; return *this;}
    virtual void h(){i(); cout<<"20"<<endl;}
};

class Dug: public Bar {
    Caz c;
public:
    Dug(){cout<<"21"<<endl;}
    ~Dug(){cout<<"22"<<endl;}
    void g(){cout<<"23"<<endl;}
    void j(){cout<<"24"<<endl; f();}
};

int main() {
    Foo *a = new Bar();
    Foo *b = new Caz();
    Foo *c = new Dug();
    a->g();
    c->g();
    Bar* e = dynamic_cast<Bar*>(b);
    if(e)
        e->k();
    Dug* d = dynamic_cast<Dug*>(c);
    if(d)
        d->j();
    a->f();
    delete a;
    delete e;
    delete ((Dug*)c);
}
```
**א.** מלאו את הטבלאות הוירטואליות (VTABLE) של המחלקות הנ"ל. חובה לרשום לאיזו מחלקה היא שייכת ע"י הוספת הקידומת `ClassName::`.
**ב.** הקוד מתקמפל ורץ תקין. כתבו כאן את הפלט המדויק.

---

## שאלה מספר 2 (30 נק', שלושה סעיפים, אין קשר בין הסעיפים)
**א.** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

class Foo {
    int x;
public:
    Foo(int a): x(a) {}
    const Foo operator+(const Foo &b) const {
        cout << "1" << endl;
        return Foo(x + b.x);
    }
    Foo& operator+=(const Foo &b) {
        cout << "2" << endl;
        x+=b.x;
        return *this;
    }
    Foo& operator=(const Foo &b) {
        cout << "3" << endl;
        x=b.x;
        return *this;
    }
};

int main() {
    int a = 1;
    int b = 2;
    int c = 3;
    c+=a+b;
    b=a+c;
    Foo d(1), e(2), f(3);
    f+=d+e;
    Foo g=f+3;
    d=e+g;
}
```
האם ישנה שגיאת קומפילציה? שגיאת ריצה? האם הקוד רץ תקין ואם כן מה הפלט?.

**ב.** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

class Foo {
public:
    void print() {}
};
class Bar: public Foo {
public:
    void print() const { cout << "Bar" << endl; }
};
class Caz: public Foo {
public:
    void print() const { cout << "Caz" << endl; }
};
class Dug: public Bar, public Caz {
public:
    void print() const { Caz::print(); }
};

int main() {
    Dug a;
    Bar b;
    Caz c;
    Foo *array;
    array = &a;
    array = &b;
    array = &c;
    for (int i=0; i<3; i++)
        array[i]->print();
}
```
האם ישנה שגיאת קומפילציה/ריצה או מה הפלט המדויק?.

**ג.** נתון הקוד הבא:
```cpp
#include<iostream>
using namespace std;

class Foo {
    int x;
public:
    Foo(int y=42): x(y) {}
    int getX() const { return x; }
};

ostream& operator<<(ostream& out, const Foo& obj) {
    return out << obj.getX() << endl;
}

template<class T>
T operator+(const T& f, const T& g){
    cout << f;
    cout << g;
    return f + g;
}

int main() {
    Foo a(1), b(2);
    cout << a+b;
    return 0;
}
```
האם ישנה שגיאת קומפילציה/ריצה או מה הפלט המדויק?.

---

## שאלה מספר 3 (20 נק', שני סעיפים, אין קשר בין הסעיפים)
**א.** נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

pthread_mutex_t foo;
int a = 1;
int b = 2;

void* bar(void *caz) {
    pthread_mutex_lock(&foo);
    if (a < b)
        a += b;
    else
        b += a;
    cout << a << " " << b << " " << endl;
    pthread_mutex_unlock(&foo);
    int c = 2*a + 2*b;
    return (void*)c;
}

int main() {
    pthread_t t;
    pthread_mutex_init(&foo, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_create(&t[i], NULL, &bar, NULL);
    }
    for (pthread_t tt : t)
        pthread_join(tt, NULL);
    cout << a << " " << b << " " << endl;
    return 0;
}
```
הקוד מתקמפל ורץ. מהם כל הפלטים האפשריים של הרצת הקוד?.

**ב.** תארו את flow הפעולות של client ו-server בתקשורת tcp ובתקשורת udp בהתאם ל-Socket API. יש לצרף הסבר מה עושה כל פעולה ולהסביר היכן מחכים למשהו, חובה להמחיש בעזרת דיאגרמות.

---

## שאלה מספר 4 (30 נק', שני סעיפים, חובה לממש ב++C בלבד)
עליכם לממש מערכת לניהול קבוצת כדורגל.
אתם נדרשים ליצור מחלקה בשם `Team` (קבוצה). לקבוצה יש:
* שני מאמנים: מאמן ראשי (`TeamCoach`) ומאמן עוזר (`AssistantCoach`), שניהם יורשים מ-`Coach`. לכל מאמן יש שם ותפקיד.
* מספר שחקנים (`Player`). שחקנים יכולים להיות מסוגים שונים: תוקף (`Attacker`), מגן (`Protector`) ושוער (`Goalkeeper`).
* לכל שחקן יש שם, תפקיד ורמת איכות (score) שמחושבת בצורה הבאה:
  - רמת האיכות של התוקף: מספר גולים חלקי מספר הבעיטות.
  - רמת האיכות של המגן: מספר התקפות שעצר חלקי מספר התקפות שהשתתף.
  - רמת האיכות של השוער: מספר כדורים שעצר חלקי מספר כל הכדורים שנבעטו אליו.
* יש לממש פונקציית הדפסה של שחקן עם הפרמטרים המתאימים.
* רמת האיכות של הקבוצה מחושבת כממוצע איכויות של שחקניה.
* פונקציית הדפסה שמדפיסה את כל הפרטים של הקבוצה (מאמנים ושחקנים).
* מחלקת "קבוצה" מאפשרת שכפול של שחקן, ניהול פרמטרים של שחקן.

יש להקפיד על תכנון מונחה עצמים ומימוש יעיל תוך מינימום קוד כפול.
**א.** ממשו את כל המחלקות הנדרשות (מספיק לרשום תיאור מחלקה קובץ `.h` - שם מחלקה, ירושה, משתנים, חתימות פונקציות).
**ב.** עליכם לממש את הפונקציות של המחלקות (קובץ `.cpp`).
