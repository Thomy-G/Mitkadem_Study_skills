const mongoose = require('mongoose');

// BUG 2: Missing comma between schema fields
const userSchema = new mongoose.Schema({
    email: { type: String, required: true, unique: true }
    username: { type: String, required: true }
});

module.exports = mongoose.model('User', userSchema);
