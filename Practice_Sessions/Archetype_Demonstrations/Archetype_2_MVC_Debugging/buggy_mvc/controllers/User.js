const User = require('../models/User');

exports.createUser = async (req, res) => {
    try {
        const newUser = new User(req.body);
        await newUser.save();
        res.status(201).json(newUser);
    } catch (err) {
        res.status(400).json({ error: err.message });
    }
};

exports.updateUser = async (req, res) => {
    try {
        const updatedUser = await User.findByIdAndUpdate(req.body.id, req.body, { new: true });
        res.json(updatedUser);
    } catch (err) {
        res.status(400).json({ error: err.message });
    }
};

exports.getUserById = async (req, res) => {
    try {
        // BUG 4: Missing await keyword. User.findById() returns a Promise object,
        // which is always truthy, so the "if (!user)" check fails and returns the Promise.
        const user = User.findById(req.params.id); 
        if (!user) {
            return res.status(404).json({ error: 'User not found' });
        }
        res.json(user);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};
