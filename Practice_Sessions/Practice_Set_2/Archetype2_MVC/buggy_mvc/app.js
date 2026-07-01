const express = require('express');
const mongoose = require('mongoose');
const bookingRoutes = require('./routes/Booking');

require('dotenv').config();

mongoose.connect(process.env.CONNECTION_STRING);

const app = express();

// BUG 1: Missing app.use(express.json()) to parse incoming POST/PUT JSON request bodies.
// This causes req.body to be undefined in the controllers.

app.use('/bookings', bookingRoutes);

app.listen(process.env.PORT || 3000);
