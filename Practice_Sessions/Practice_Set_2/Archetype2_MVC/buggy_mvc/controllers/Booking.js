const Booking = require('../models/Booking');

exports.createBooking = async (req, res) => {
    try {
        const newBooking = new Booking(req.body);
        await newBooking.save();
        
        // BUG 4: Setting the status code but not sending any response.
        // This causes requests to hang indefinitely until a timeout.
        res.status(201); 
    } catch (err) {
        res.status(400).json({ error: err.message });
    }
};

exports.getBookings = async (req, res) => {
    try {
        const bookings = await Booking.find();
        res.json(bookings);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};

exports.deleteBooking = async (req, res) => {
    try {
        await Booking.findByIdAndDelete(req.params.id);
        res.status(204).send();
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
};
