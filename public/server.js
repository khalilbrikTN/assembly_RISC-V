const express = require('express');
const { exec } = require('child_process');
const app = express();
const port = 3000;

app.use(express.static('public')); // Serve static files from 'public' directory
app.use(express.json()); // Parse JSON payloads

app.post('/run', (req, res) => {
    const { program, startAddress } = req.body;

    // Save the program to a file (e.g., program.cpp)
    const fs = require('fs');
    fs.writeFileSync('program.cpp', program);

    // Compile the C++ program
    exec('g++ program.cpp -o program', (error, stdout, stderr) => {
        if (error) {
            return res.status(500).send(`Compilation error: ${stderr}`);
        }

        // Run the compiled program
        exec('./program', (error, stdout, stderr) => {
            if (error) {
                return res.status(500).send(`Runtime error: ${stderr}`);
            }
            res.send(stdout); // Send the output back to the client
        });
    });
});

app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});
