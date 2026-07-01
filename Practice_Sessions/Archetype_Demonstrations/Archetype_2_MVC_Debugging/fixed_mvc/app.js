const express = require('express');
const mongoose = require('mongoose');

// FIXED BUG 1: Added assignment operator (=)
const userRoutes = require('./routes/User'); 

require('dotenv').config();

mongoose.connect(process.env.CONNECTION_STRING);

const app = express();
app.use(express.json());
app.use('/users', userRoutes);

app.listen(process.env.PORT || 3000);
