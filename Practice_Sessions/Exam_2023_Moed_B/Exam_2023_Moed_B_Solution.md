# Past Exam Solution Key — 2023 Semester A, Moed B

---

## שאלה מספר 1 (20 נק')

### א. טבלאות וירטואליות (VTABLE)
*   **VTABLE של Foo:**
    *   `VTABLE_Foo[0]`: `Foo::g`
    *   `VTABLE_Foo[1]`: `Foo::h`
*   **VTABLE של Bar:**
    *   `VTABLE_Bar[0]`: `Bar::g` (override)
    *   `VTABLE_Bar[1]`: `Bar::h` (override)
    *   `VTABLE_Bar[2]`: `Bar::f` (new virtual)
    *   `VTABLE_Bar[3]`: `Bar::i` (new virtual)
*   **VTABLE של Caz:**
    *   `VTABLE_Caz[0]`: `Bar::g` (inherited)
    *   `VTABLE_Caz[1]`: `Caz::h` (override)
    *   `VTABLE_Caz[2]`: `Caz::f` (override)
    *   `VTABLE_Caz[3]`: `Bar::i` (inherited)
*   **VTABLE של Dug:**
    *   `VTABLE_Dug[0]`: `Dug::g` (override)
    *   `VTABLE_Dug[1]`: `Bar::h` (inherited)
    *   `VTABLE_Dug[2]`: `Bar::f` (inherited)
    *   `VTABLE_Dug[3]`: `Bar::i` (inherited)

### ב. פלט הקוד
```
1
8
1
8
1
16
1
8
1
8
1
16
21
15
12
23
15
24
11
2
3
11
2
3
3
18
3
10
3
22
18
3
10
3
10
3
```

---

## שאלה מספר 2 (30 נק')

### סעיף א
*   **קומפילציה/ריצה/פלט:** **רץ תקין**.
*   **הפלט:**
    ```
    1
    2
    1
    1
    3
    ```
*   **הסבר:** 
    *   `f+=d+e` מבצע קודם `d+e` (מפעיל `+` של `Foo`) שמדפיס `1` ומחזיר אובייקט זמני. אז מתבצע `+=` שמדפיס `2`.
    *   `Foo g=f+3` ממיר את `3` ל-`Foo` זמנית באמצעות הבנאי, ואז מבצע `+` שמדפיס `1`.
    *   `d=e+g` מבצע `+` שמדפיס `1` ולאחר מכן השמה `=` שמדפיסה `3`.

### סעיף ב
*   **קומפילציה/ריצה/פלט:** **שגיאת קומפילציה**.
*   **הסבר:** המחלקה `Dug` יורשת מ-`Bar` ומ-`Caz` ירושה לא וירטואלית. שניהם יורשים מ-`Foo`. לכן, בתוך `Dug` ישנם שני מופעים נפרדים של `Foo`. השורה `array = &a;` (השמת כתובת של `Dug` לתוך מצביע `Foo*`) היא שגיאת קומפילציה מכיוון שהקומפיילר אינו יכול להמיר את המצביע בצורה חד-משמעית (ambiguous base class).

### סעיף ג
*   **קומפילציה/ריצה/פלט:** **שגיאת זמן ריצה (Stack Overflow)**.
*   **הסבר:** אופרטור התבנית `operator+` מוגדר כך שהוא מחזיר `f + g`. עבור טיפוס `Foo`, פעולה זו מפעילה שוב את אופרטור התבנית `operator+` באופן רקורסיבי ללא תנאי עצירה. דבר זה מוביל לרקורסיה אינסופית ולקריסת התוכנית בשל הצפת המחסנית בזמן ריצה.

---

## שאלה מספר 3 (20 נק')

### סעיף א (ת'רדים וערכי פלט)
*   **קומפילציה:** הקוד המוצג **לא יתקמפל**.
*   **הסבר:** המשתנה `t` מוגדר בשורה 24 כ-`pthread_t t` (משתנה יחיד ולא מערך). בשורה 27 מתבצעת גישה אליו כמערך (`t[i]`), ובשורה 29 מתבצעת לולאת `for-each` שאינה חוקית על משתנה פשוט.
*   **הערה (אם הקוד היה מתוקן ל-`pthread_t t[2]`):**
    הפלט היחיד האפשרי בזכות השימוש במיוטקס הוא:
    ```
    3 2 
    3 5 
    3 5 
    ```

### סעיף ב (Socket API Flow & Diagrams)
*   **TCP Flow**:
    *   **Server**: `socket()` -> `bind()` -> `listen()` -> `accept()` (חוסם וממתין לחיבור לקוח) -> `read()` / `write()` -> `close()`.
    *   **Client**: `socket()` -> `connect()` (חוסם עד להשלמת לחיצת היד המשולשת) -> `write()` / `read()` -> `close()`.
*   **UDP Flow**:
    *   **Server**: `socket()` -> `bind()` -> `recvfrom()` (חוסם וממתין לקבלת פאקט) -> `sendto()` -> `close()`.
    *   **Client**: `socket()` -> `sendto()` -> `recvfrom()` -> `close()`.

```
      TCP FLOW                                  UDP FLOW
Server          Client                    Server          Client
 socket()        socket()                  socket()        socket()
   |               |                         |               |
 bind()            |                       bind()            |
   |               |                         |               |
 listen()          |                      recvfrom() <--- sendto()
   |               |                      (blocks)        (sends packet)
 accept() <--- connect()                     |               |
(blocks)     (three-way handshake)        sendto()  ---> recvfrom()
   |               |                         |               |
read/write <-> read/write                 close()         close()
   |               |
close()         close()
```

---

## שאלה מספר 4 (30 נק')

### א. תיאור מחלקות וממשקים (`Team.h`, `Coach.h`, `Player.h`)

#### `Coach.h`
```cpp
#ifndef COACH_H
#define COACH_H
#include <string>
#include <iostream>

class Coach {
protected:
    std::string name;
    std::string role;
public:
    Coach(const std::string& n, const std::string& r) : name(n), role(r) {}
    virtual ~Coach() = default;
    virtual void print() const {
        std::cout << "Coach: " << name << ", Role: " << role << std::endl;
    }
};

class TeamCoach : public Coach {
public:
    TeamCoach(const std::string& n) : Coach(n, "Head Coach") {}
};

class AssistantCoach : public Coach {
public:
    AssistantCoach(const std::string& n) : Coach(n, "Assistant Coach") {}
};
#endif
```

#### `Player.h`
```cpp
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>

class Player {
protected:
    std::string name;
    std::string role;
public:
    Player(const std::string& n, const std::string& r) : name(n), role(r) {}
    virtual ~Player() = default;
    virtual double getScore() const = 0;
    virtual Player* clone() const = 0;
    virtual void print() const {
        std::cout << "Player: " << name << ", Role: " << role << ", Score: " << getScore() << std::endl;
    }
};

class Attacker : public Player {
private:
    int goals;
    int kicks;
public:
    Attacker(const std::string& n, int g, int k) : Player(n, "Attacker"), goals(g), kicks(k) {}
    double getScore() const override { return kicks == 0 ? 0.0 : (double)goals / kicks; }
    Player* clone() const override { return new Attacker(*this); }
};

class Protector : public Player {
private:
    int stops;
    int attempts;
public:
    Protector(const std::string& n, int s, int a) : Player(n, "Protector"), stops(s), attempts(a) {}
    double getScore() const override { return attempts == 0 ? 0.0 : (double)stops / attempts; }
    Player* clone() const override { return new Protector(*this); }
};

class Goalkeeper : public Player {
private:
    int saves;
    int totalKicks;
public:
    Goalkeeper(const std::string& n, int s, int tk) : Player(n, "Goalkeeper"), saves(s), totalKicks(tk) {}
    double getScore() const override { return totalKicks == 0 ? 0.0 : (double)saves / totalKicks; }
    Player* clone() const override { return new Goalkeeper(*this); }
};
#endif
```

#### `Team.h`
```cpp
#ifndef TEAM_H
#define TEAM_H
#include "Coach.h"
#include "Player.h"

class Team {
private:
    TeamCoach* headCoach;
    AssistantCoach* assistantCoach;
    Player** players;
    int playerCount;
    int capacity;

    void resize();

public:
    Team(TeamCoach* hc, AssistantCoach* ac);
    ~Team();
    Team(const Team& other);
    Team& operator=(const Team& other);

    void addPlayer(Player* p);
    double getTeamScore() const;
    void printTeam() const;
};
#endif
```

### ב. מימוש הפונקציות (`Team.cpp`)
```cpp
#include "Team.h"

Team::Team(TeamCoach* hc, AssistantCoach* ac) 
    : headCoach(hc), assistantCoach(ac), playerCount(0), capacity(2) {
    players = new Player*[capacity];
}

Team::~Team() {
    delete headCoach;
    delete assistantCoach;
    for (int i = 0; i < playerCount; ++i) {
        delete players[i];
    }
    delete[] players;
}

void Team::resize() {
    capacity *= 2;
    Player** temp = new Player*[capacity];
    for (int i = 0; i < playerCount; ++i) {
        temp[i] = players[i];
    }
    delete[] players;
    players = temp;
}

void Team::addPlayer(Player* p) {
    if (playerCount == capacity) {
        resize();
    }
    players[playerCount++] = p->clone(); // Deep copy via clone
}

double Team::getTeamScore() const {
    if (playerCount == 0) return 0.0;
    double sum = 0;
    for (int i = 0; i < playerCount; ++i) {
        sum += players[i]->getScore();
    }
    return sum / playerCount;
}

void Team::printTeam() const {
    std::cout << "--- Team Details ---" << std::endl;
    if (headCoach) headCoach->print();
    if (assistantCoach) assistantCoach->print();
    std::cout << "Players list:" << std::endl;
    for (int i = 0; i < playerCount; ++i) {
        players[i]->print();
    }
    std::cout << "Overall Team Score: " << getTeamScore() << std::endl;
}
```
