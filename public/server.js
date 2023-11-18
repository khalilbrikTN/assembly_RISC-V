const express = require('express');
const { exec } = require('child_process');
const toml = require("toml");
const app = express();
const port = 3000;

app.use(express.static('./')); // Serve static files from 'public' directory
app.use(express.json()); // Parse JSON payloads

const fs = require('fs');
const path = require('path');

function parseCSVToMatrix(filePath) {
    // Read the file synchronously (for simplicity)
    const fileContents = fs.readFileSync(filePath, 'utf8');

    // Split the contents by new line to get rows
    const rows = fileContents.split('\n');

    // Split each row by comma and create the matrix
    const matrix = rows.map(row => row.split(','));

    return matrix;
}

function parseTOML(filePath) {
    // Read the file synchronously (for simplicity)
    const fileContents = fs.readFileSync(filePath, 'utf8');

    return toml.parse(fileContents);
}

// Example usage
const registers = parseCSVToMatrix("../registers.csv");
console.log(registers);


const memory = parseTOML("../memory.csv");
console.log(memory);

app.post('/run', (req, res) => {
    const { program, startAddress } = req.body;
    const fs = require('fs');

    // Save the program to a file
    fs.writeFileSync('../program1.txt', program);

    // Function to promisify exec
    function execPromise(command) {
        return new Promise((resolve, reject) => {
            exec(command, (error, stdout, stderr) => {
                if (error) {
                    reject({stdout, stderr});
                } else {
                    resolve(stdout);
                }
            });
        });
    }

    // Compile and run the program
    execPromise('g++ ../main.cpp -o ../main')
        .then(() => execPromise('cd ../ && ./main'))
        .catch(({stdout, stderr}) => res.status(500).send(`${stdout}\nError1: ${stderr}`))
        .then((output) => {
            res.json({
                output: output,
                registers: registers,
                memory: memory
            });
        })
        .catch(({stdout, stderr}) => res.status(500).send(`${stdout}\nError2: ${stderr}`));
});


app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});


