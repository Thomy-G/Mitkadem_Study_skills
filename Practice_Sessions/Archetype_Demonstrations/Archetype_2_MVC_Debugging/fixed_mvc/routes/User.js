const express = require('express');
const router = express.Router();
const userController = require('../controllers/User');

// FIXED BUG 3: Standardized routing to RESTful verbs to avoid collision
router.post('/', userController.createUser);
router.patch('/:id', userController.updateUser); 

router.get('/:id', userController.getUserById);

module.exports = router;
