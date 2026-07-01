const mongoose = require('mongoose');

// FIXED BUG 2: Added missing comma
const userSchema = new mongoose.Schema({
    email: { type: String, required: true, unique: true },
    username: { type: String, required: true }
});

module.exports = mongoose.model('User', userSchema);
