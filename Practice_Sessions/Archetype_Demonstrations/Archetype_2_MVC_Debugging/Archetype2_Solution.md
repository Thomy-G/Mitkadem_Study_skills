# Solution: Archetype 2 — Node.js/Express/Mongoose MVC Debugging

Here is the debugging report mapping and fixing the 4 bugs in the `buggy_mvc/` files.

---

### Bug 1: Missing Assignment Operator in `app.js`
- **Location**: `buggy_mvc/app.js` (Line 5)
- **Mechanics**: The statement `const userRoutes require('./routes/User');` is missing the assignment operator (`=`). This results in a JavaScript syntax compilation/parsing error (`SyntaxError: Unexpected identifier`), crashing the server on startup.
- **Fixed Code**:
  ```javascript
  const userRoutes = require('./routes/User');
  ```

### Bug 2: Missing Comma in Mongoose Schema Declaration
- **Location**: `buggy_mvc/models/User.js` (Line 5)
- **Mechanics**: There is a missing comma at the end of the `email` key block, right before the `username` key:
  ```javascript
  email: { type: String, required: true, unique: true }
  username: { type: String, required: true }
  ```
  This is a syntax error in JavaScript object declarations, causing parsing failures on startup.
- **Fixed Code**:
  ```javascript
  email: { type: String, required: true, unique: true },
  username: { type: String, required: true }
  ```

### Bug 3: Route Mapping Collision (Shadowing Route)
- **Location**: `buggy_mvc/routes/User.js` (Lines 7-8)
- **Mechanics**: Two handlers are registered to the same HTTP verb (`POST`) and route path (`/`):
  ```javascript
  router.post('/', userController.createUser);
  router.post('/', userController.updateUser);
  ```
  Express maps paths sequentially. The first handler matches `POST /` and consumes the response, completely shadowing and rendering `updateUser` unreachable.
- **Fixed Code** (RESTful routing convention):
  ```javascript
  router.post('/', userController.createUser);
  router.patch('/:id', userController.updateUser);
  ```

### Bug 4: Un-awaited Async Database Query inside `getUserById`
- **Location**: `buggy_mvc/controllers/User.js` (Line 23)
- **Mechanics**: The database call `User.findById(req.params.id)` is an asynchronous function that returns a Promise. Because it is missing the `await` keyword, `user` contains a Promise object rather than the queried user document. In JavaScript, Promise objects are always truthy, so the check `if (!user)` is bypassed, and the pending Promise is sent to the client instead of the database result.
- **Fixed Code**:
  ```javascript
  const user = await User.findById(req.params.id);
  ```

---

*The fully corrected codebase can be viewed in the [fixed_mvc/](fixed_mvc) directory.*
