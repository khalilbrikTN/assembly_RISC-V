const express = require('express');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());

// Define your API endpoints here
// For example, an endpoint to start the simulation
app.post('/simulate', (req, res) => {
    const program = req.body.program;
    const startAddress = req.body.startAddress;

    // Call the C++ function here
    const result = simulateRiscV(program, startAddress);

    res.json({ message: "Simulation completed", output: result });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
