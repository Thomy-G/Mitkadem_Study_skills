# 2025 Semester A, Moed A (תכנות מערכות מתקדם תשפ"ה)

## שאלה מספר 1 (25 נק')
לפניכם קטעי קוד מתוך תוכנית ב NodeJS, אשר מהווה שרת אינטרנטי מבוסס MVC החושף RESTful API ומאפשר לבצע פעולות CRUD מול מסד נתונים. במספר מקומות בקוד נמחק קוד, ובמקום יש מלבן עם מספר. השלימו את הקוד החסר עבור כל אחד מ-12 המלבנים.

### קובץ: `app.js`
```javascript
// [מלבן מספר 1]
const bodyParser = require('body-parser');
const cors = require('cors');
const mongoose = require('mongoose');
const articles = require('./routes/article');

require('custom-env').env(process.env.NODE_ENV, './config');

mongoose.connect(process.env.CONNECTION_STRING,
    { useNewUrlParser: true,
      useUnifiedTopology: true });

// [מלבן מספר 2]
app.use(cors());
app.use(bodyParser.urlencoded({extended : true}));
app.use(express.json());

app.use('/articles', articles);

app.listen(process.env.PORT);
```

### קובץ: `routes/article.js`
```javascript
// [מלבן מספר 3]
const articleController = require('../controllers/article');

router.route('/')
// [מלבן מספר 4]
// [מלבן מספר 5]

router.
// [מלבן מספר 6]
// [מלבן מספר 7]
// [מלבן מספר 8]
// [מלבן מספר 9]

module.exports = router;
```

### קובץ: `controllers/article.js`
```javascript
const articleService = require('../services/article');

const createArticle = async (req, res) => {
    // [מלבן מספר 10]
};

const getArticles = async (req, res) => {
    res.json(await articleService.getArticles());
};

const getArticle = async (req, res) => {
    const article = await articleService.getArticleById(req.params.id);
    if (!article) {
        return res.status(404).json({ errors: ['Article not found'] });
    }
    res.json(article);
};

// ...

module.exports = { createArticle, getArticles, getArticle, updateArticle, deleteArticle };
```

### קובץ: `services/article.js`
```javascript
const Article = require('../models/article');

const createArticle = async (title, published) => {
    const article = new Article({ title : title });
    if (published) article.published = published;
    return await article.save();
};

const getArticleById = async (id) => { return await Article.findById(id); };

const getArticles = async () => { return await Article.find({}); };

const updateArticle = async (id, title) => {
    const article = await getArticleById(id);
    if (!article) return null;

    // [מלבן מספר 11]
    return article;
};

const deleteArticle = async (id) => {
    const article = await getArticleById(id);
    if (!article) return null;

    await article.remove();
    return article;
};

module.exports = { createArticle, getArticleById, getArticles, updateArticle, deleteArticle };
```

### קובץ: `models/article.js`
```javascript
const mongoose = require('mongoose');

// [מלבן מספר 12]
    title : {
        type: String,
        required: true
    },
    published: {
        type: Date,
        default: Date.now
    }
});

module.exports = mongoose.model('Article', Article);
```

---

## שאלה מספר 2 (25 נק', שני סעיפים)
**א. (15 נק')** כיתבו את קוד ה-XML Layout באנדרואיד שייתן מסך בעל הפריסה הבאה: כפתור Foo, תיבת טקסט לעריכה (עם רמז "Bar"), וכפתור FOOBAR תחתיו.
**ב. (10 נק')** נתון הקוד הבא:
```html
<html>
<head>
<style>
button { margin: 10px; background-color: #FF0000; color:#FFFFFF; }
#bar { background-color: #0000FF; }
.caz { background-color: #000000; }
button.caz { background-color: #00FF00; }
button.bar { background-color: #FFFFFF; }
</style>
</head>
<body>
<button>Foo</button>
<button class="caz">Bar</button>
<button class="caz" id="bar">Foobar</button>
</body>
</html>
```
במידה והקוד רץ תקין, מה יהיה צבע הרקע (במילים, למשל: "אדום", ולא בקוד צבע) של השמאלי ביותר, האמצעי והימני ביותר? אם יש שגיאה, הסבירו אותה.

---

## שאלה מספר 3 (20 נק')
אליס יצרה תיקייה חדשה בשם `internals` ובתוכה קובץ בשם `foo.txt` המכיל את המחרוזת "bar". היא הריצה בטרמינל את הפקודה `git init`, ולאחר מכן `git add foo.txt` ואז `git commit -m "First commit"`. 
כעת, המשיכה ויצרה תיקייה בשם `mydir` בתוך `internals`, ובתוכה שני קבצים: `a.txt` (המכיל "bar") ו-`caz.txt` (המכיל "cazzzzz"). אליס עשתה `git add` לשני הקבצים ולאחר מכן `git commit`. 
השלימו טבלה המפרטת בדיוק אילו קבצים (commits, trees, blobs) קיימים כעת בתיקיית `.git/objects`, ומה התוכן של כל אחד מהם.

---

## שאלה מספר 4 (30 נק', שני סעיפים)
**א. (10 נק')** נתון הקוד הבא ב-JavaScript:
```javascript
async function foo() {
    console.log(1)
    return new Promise((resolve, reject) => {
        console.log(2)
        setTimeout(() => {
            console.log(3)
            resolve({ name: "Alice", age: 42 })
            console.log("5")
        }, 2000)
    });
}
async function bar() {
    console.log("8")
    try {
        const f = await foo()
        console.log("9" + f + "+" + "+" + f.name)
    } catch (error) { console.error("Error: ", error) }
    const f2 = foo();
    console.log("10" + f2)
    const y = (await f2).age
    console.log(y)
}
bar()
```
האם הקוד רץ תקין? אם כן, כתבו את הפלט המדויק. אם יש שגיאת ריצה, הסבירו אותה.

**ב. (20 נק')** כתבו קוד ב-++C (תקני ומלא) המדגים את העקרון מאחורי האות O (Open/Closed Principle) בעקרונות ה-SOLID. הוסיפו הסברים מתאימים לקוד.
