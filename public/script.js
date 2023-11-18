document.getElementById('programForm').addEventListener('submit', function(e) {
    e.preventDefault();

    const program = document.getElementById('programInput').value;
    const startAddress = document.getElementById('programStartAddress').value;

    fetch('/run', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ program, startAddress })
    })
        .then(response => response.json())
        .then(data => {
            document.getElementById('registers').innerHTML = data.registers;
            document.getElementById('memory').innerHTML = JSON.stringify(data.memory);



        })
        .catch(error => console.error('Error:', error));
});

