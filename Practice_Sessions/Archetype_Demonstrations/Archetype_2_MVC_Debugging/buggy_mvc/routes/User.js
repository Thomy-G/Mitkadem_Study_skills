const express = require('express');
const router = express.Router();
const userController = require('../controllers/User');

// BUG 3: Duplicate POST route mappings to the same path.
// The second registration shadows and breaks the user creation operation.
router.post('/', userController.createUser);
router.post('/', userController.updateUser); 

router.get('/:id', userController.getUserById);

module.exports = router;
