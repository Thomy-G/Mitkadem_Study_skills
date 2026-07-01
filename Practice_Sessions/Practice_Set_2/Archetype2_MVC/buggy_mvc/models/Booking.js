const mongoose = require('mongoose');

const bookingSchema = new mongoose.Schema({
    className: { type: String, required: true },
    studentName: { type: String, required: true },
    date: { type: Date, default: Date.now }
});

// BUG 2: Exporting the raw schema configuration instead of compiling it into a Model first.
// This causes queries like Booking.find() to throw errors because the exported object has no query methods.
module.exports = bookingSchema; 
