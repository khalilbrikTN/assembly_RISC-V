document.getElementById('programForm').addEventListener('submit', function(event) {
    event.preventDefault();

    var programInput = document.getElementById('programInput').value;
    var programStartAddress = document.getElementById('programStartAddress').value;

    // TODO: Replace this with actual API call
    simulateProgram(programInput, programStartAddress);
});

function simulateProgram(program, startAddress) {
    // Mock simulation output
    var output = "Simulating program...\nStart Address: " + startAddress + "\nProgram:\n" + program;

    // Display output in the simulationOutput div
    document.getElementById('simulationOutput').textContent = output;

    // In actual implementation, make an AJAX request to your API server
    // and handle the response here.
}
