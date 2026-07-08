# 2023 Semester A, Moed A (תכנות מתקדם 1 תשפ"ג)

## שאלה מספר 1 (20 נק', שני סעיפים, שניהם לגבי הקוד המוצג בעמוד זה)
נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

class Foo {
public:
    Foo() {cout<<"1"<<endl;}
    Foo(const Foo& a){cout<<"2"<<endl;}
    ~Foo() {cout<<"3"<<endl;}
    virtual void x(){cout<<"4"<<endl;}
    virtual void y(){z(); cout<<"5"<<endl;}
    void z(){cout<<"6"<<endl;}
};

class Bar: public Foo {
public:
    Foo a;
    Bar() {cout<<"7"<<endl;}
    Bar(const Bar& b){cout<<"8"<<endl;}
    virtual ~Bar() {cout<<"9"<<endl;}
    void x() {cout<<"10"<<endl;}
    void z() {cout<<"11"<<endl;}
    virtual void i(){cout<<"12"<<endl;}
};

class Caz: public Bar {
public:
    Bar b;
    Caz() {cout<<"13"<<endl;}
    ~Caz() {cout<<"14"<<endl;}
    void x(){cout<<"15"<<endl;}
    virtual void z(){cout<<"16"<<endl;}
    virtual void i(){cout<<"17"<<endl;}
    Bar j(){cout<<"18"<<endl; return b;}
};

int main() {
    Foo *p1= new Bar();
    Bar *p2 = new Caz();
    p1->z();
    p2->z();
    p1->x();
    p2->x();
    p1->y();
    Caz *c = dynamic_cast<Caz*>(p2);
    c->j();
    delete p2;
    delete p1;
}
```
**א.** מלאו את הטבלאות הוירטואליות של המחלקות הנ"ל. חובה לרשום לפני כל מתודה לאיזו מחלקה היא שייכת ע"י הוספת הקידומת `ClassName::` לפני כל מתודה.
**ב.** הקוד מתקמפל ורץ תקין. כתבו כאן את הפלט המדויק.

---

## שאלה מספר 2 (30 נק', שלושה סעיפים, שלושתם לגבי הקוד המוצג בעמוד זה)
נתון הקוד הבא:
```cpp
#include <iostream>
using namespace std;

class Foo {
public:
    virtual void foo1 (int foo) { cout << "Foo::foo1" << endl; };
    virtual void foo2(int foo) { cout << "Foo::foo2" << endl; };
};

class Bar: public Foo {
public:
    virtual void bar1 (int foo) { foo1 (foo); };
    virtual void bar2(int foo) { cout << "Bar::bar2" << endl; };
};

class Caz: public virtual Foo {
public:
    void foo1(int foo) { cout << "Caz::foo1" << endl; };
    virtual void bar2(int foo) { cout << "Caz::bar2" << endl; };
};

class Qux: public virtual Foo {
public:
    void foo2(int foo) { cout << "Qux::foo2" << endl; };
    virtual void bar2(int foo) { cout << "Qux::bar2" << endl; };
};

class Waldo: public Bar, public Caz, public Qux {};
```
**א.** עבור קוד ה-main הבא:
```cpp
int main() {
    Waldo waldo;
    waldo.foo1(32145);
}
```
האם ישנה שגיאת קומפילציה? אם כן, הסבירו כאן את השגיאה ומדוע היא מתרחשת. האם ישנה שגיאת ריצה? אם כן, רשמו כאן את הפלט עד לשגיאה והסבירו מדוע השגיאה מתרחשת. האם הקוד רץ תקין? אם כן, רשמו כאן את הפלט המדויק.

**ב.** עבור קוד ה-main הבא:
```cpp
int main() {
    Waldo waldo;
    waldo.bar1(64187);
}
```
ענו על אותן שאלות כמו בסעיף א' (קומפילציה, ריצה, או פלט תקין).

**ג.** כעת החליפו את המחלקה Waldo בקוד הבא:
```cpp
class Waldo: public Bar, public Caz, public Qux {
public:
    void bar1 (char* x) {cout << "Waldo::bar1" << endl;};
    void bar2(int foo) { cout << "Waldo::bar2" << endl; };
};
```
עבור קוד ה-main הבא, ענו על אותן שאלות כמו קודם:
```cpp
int main() {
    Waldo waldo;
    waldo.bar1(125);
}
```

---

## שאלה מספר 3 (20 נק', שני סעיפים, אין קשר בין הסעיפים)
**א.** תהליך מריץ שני ת'רדים. הת'רד הראשון מריץ את הקוד הבא:
```cpp
for (i=0; i<5; i++) {
    foo=foo+2;
}
```
הת'רד השני מריץ את הקוד הבא:
```cpp
for (i=0; i<5; i++) {
    // make atomic
    foo=foo+3;
    // end make atomic
}
```
נתון כי המשתנה `i` יושב על המחסנית והמשתנה `foo` הינו משאב משותף של הת'רדים, ובהתחלה ערכו אפס. ההערות מסמלות שהקוד ביניהן מתבצע באופן אטומי. מהם הערכים האפשריים של המשתנה `foo` לאחר הרצת התוכנית?.

**ב.** אניה ועליזה בנו קליינט וסרבר בפרוטוקול UDP. עליזה כתבה את השרת ואניה את הקליינט. הקוד שהגישו מכיל 4 בעיות שונות, הסבירו מהן. 
הקוד של השרת והקליינט:
```cpp
// Server (Aliza)
using namespace std;
int main() {
    const int server_port = 5555;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    bind(sock, (struct sockaddr *)&sin, sizeof(sin));
    for (int i = 0; i < 7; i++) {
        struct sockaddr_in from;
        unsigned int from_len = sizeof(struct sockaddr_in);
        char buffer;
        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &from_len);
        cout << "The client sent: " << buffer << endl;
        sendto(sock, buffer, 0, 0, (struct sockaddr *)&sin, sizeof(sin));
    }
    close(sock);
    return 0;
}

// Client (Anya)
using namespace std;
struct sockaddr_in sin;
void f(int sock, char data[]) {
    int data_len = 10;
    int sent_bytes = sendto(sock, data, 5, 0, (struct sockaddr *)&sin, sizeof(sin));
    struct sockaddr_in from;
    unsigned int from_len = sizeof(struct sockaddr_in);
    char buffer;
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &from_len);
    cout << "The server sent: " << buffer << endl;
}
int main() {
    const char *ip_address = "127.0.0.1";
    const int port_no = 5555;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);
    char data[] = "Aliza,";
    f(sock, data);
    char data_addr[] = "Do";
    f(sock, data_addr);
    char data_addr2[] = "you";
    f(sock, data_addr2);
    char data_addr3[] = "wanna";
    f(sock, data_addr3);
    char data_addr4[] = "submit";
    f(sock, data_addr4);
    char data_addr5[] = "the";
    f(sock, data_addr5);
    char data_addr6[] = "program?";
    f(sock, data_addr6);
    close(sock);
    return 0;
}
```

---

## שאלה מספר 4 (30 נק', שלושה סעיפים, חובה לממש ב++C בלבד)
במחשב בו ישנם קבצי וידאו רבים, נהוג לאגד אותם באוספים שונים. אוסף (Collection), מורכב מרשימה של קבצי וידאו ואו אוספים אחרים. ברצוננו להגדיר ספריית וידאו עבור קבצי הוידאו השונים הקיימים במחשב שלנו, עליכם להגדיר את המחלקות הבאות (מותר להוסיף מחלקות נוספות):
* **המחלקה Mp4** עבור קובץ וידאו בפורמט mp4. למחלקה זו יש: שם הקובץ המלא, גודל (int), תאריך יצירה, תאריך שינוי, תג (*char) ואיכות הוידאו (int).
* **המחלקה Avi** עבור קובץ וידאו בפורמט avi. למחלקה זו יש: שם הקובץ המלא, גודל, תאריך יצירה, תאריך שינוי, תג וערך הדחיסה (int).
* **המחלקה Collection** עבור אוסף סרטוני וידאו. למחלקה זו יש: שם האוסף, מערך דינמי שיכול להכיל אובייקטים של קבצי וידאו שונים ו/או אוספים שונים.

אין להשתמש בספריות STL כלל (מותר לכתוב templates בעצמכם במידת הצורך).
**א.** ממשו את כל המחלקות הנדרשות (מספיק תיאור מחלקה: קובץ `.h`, משתנים וחתימות פונקציות).
**ב.** ממשו (קוד מלא) את הדיסטרקטורים של כל המחלקות.
**ג.** עליכם לממש את הפונקציות הבאות:
* `count`: מחזירה את מספר הקבצים הטוטאלי באוסף (בצורה עמוקה).
* `size`: מחזירה את הגודל הטוטאלי של כל הקבצים באוסף (בצורה עמוקה).
* `avg`: מחזירה את איכות הוידאו הממוצעת של כל קבצי ה-mp4 באוסף (בצורה עמוקה).
