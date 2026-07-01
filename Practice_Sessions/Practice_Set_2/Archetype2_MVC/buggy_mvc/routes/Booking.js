const express = require('express');
const router = express.Router();
const bookingController = require('../controllers/Booking');

router.post('/', bookingController.createBooking);
router.get('/', bookingController.getBookings);

// BUG 3: Invoking the controller function immediately inside the route mapping.
// Since req and res are not defined here, it causes a ReferenceError upon startup.
router.delete('/:id', bookingController.deleteBooking(req, res)); 

module.exports = router;
