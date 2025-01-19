#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>MiniMacro Config</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f4f4f9;
      color: #333;
    }

    .container {
      max-width: 500px;
      margin: 20px auto;
      padding: 20px;
      background: #ffffff;
      border-radius: 8px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    }

    h1 {
      text-align: center;
      margin-bottom: 20px;
    }

    form {
      display: flex;
      flex-direction: column;
      gap: 20px;
    }

    .section {
      border: 1px solid #ccc;
      border-radius: 8px;
      padding: 15px;
      background-color: #f9f9f9;
    }

    .section h2 {
      margin-top: 0;
      text-transform: capitalize;
      font-size: 1.2em;
    }

    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
    }

    textarea {
      width: calc(100% - 20px); /* Account for padding */
      margin: 5px 10px;
      padding: 10px;
      border: 1px solid #ccc;
      border-radius: 4px;
      resize: vertical;
      font-size: 14px;
      box-sizing: border-box;
    }

    button {
      padding: 12px 20px;
      border: none;
      border-radius: 4px;
      font-size: 16px;
      cursor: pointer;
      transition: background-color 0.3s;
    }

    button[type="button"] {
      width: 100%;
      margin-top: 10px;
    }

    #save-macros {
      background-color: #264653;
      color: white;
    }

    #save-macros:hover {
      background-color: #1d3557;
    }

    #factory-reset {
      background-color: #e63946;
      color: white;
      margin-top: 20px;
    }

    #factory-reset:hover {
      background-color: #c1121f;
    }

    .button-container {
      padding: 0 10px;
    }

    .Red {
      color: #e63946;
    }

    .Green {
      color: #2a9d8f;
    }

    .Yellow {
      color: #f4a261;
    }

    .Blue {
      color: #457b9d;
    }

    .Black {
      color: #000000;
    }

    .White {
      color: #7d7d7d;
    }
  </style>
</head>

<body onload="pageLoad()">
  <div class="container">
    <h1>Macro Config</h1>
    <form action="/save-macros" method="post" id='mainConfigForm' accept-charset='utf-8'>
      <!-- Red Section -->
      <div class="section">
        <h2 class="Red">Red</h2>
        <label for="red-click">Click</label>
        <textarea id="red-click" name="red-click" rows="2" placeholder="Enter macro for Red Click"></textarea>

        <label for="red-dblclick">Double Click</label>
        <textarea id="red-dblclick" name="red-dblclick" rows="2"
          placeholder="Enter macro for Red Double Click"></textarea>
      </div>

      <!-- Green Section -->
      <div class="section">
        <h2 class="Green">Green</h2>
        <label for="green-click">Click</label>
        <textarea id="green-click" name="green-click" rows="2" placeholder="Enter macro for Green Click"></textarea>

        <label for="green-dblclick">Double Click</label>
        <textarea id="green-dblclick" name="green-dblclick" rows="2"
          placeholder="Enter macro for Green Double Click"></textarea>
      </div>

      <!-- Yellow Section -->
      <div class="section">
        <h2 class="Yellow">Yellow</h2>
        <label for="yellow-click">Click</label>
        <textarea id="yellow-click" name="yellow-click" rows="2" placeholder="Enter macro for Yellow Click"></textarea>

        <label for="yellow-dblclick">Double Click</label>
        <textarea id="yellow-dblclick" name="yellow-dblclick" rows="2"
          placeholder="Enter macro for Yellow Double Click"></textarea>
      </div>

      <!-- Blue Section -->
      <div class="section">
        <h2 class="Blue">Blue</h2>
        <label for="blue-click">Click</label>
        <textarea id="blue-click" name="blue-click" rows="2" placeholder="Enter macro for Blue Click"></textarea>

        <label for="blue-dblclick">Double Click</label>
        <textarea id="blue-dblclick" name="blue-dblclick" rows="2"
          placeholder="Enter macro for Blue Double Click"></textarea>
      </div>

      <!-- Black Section -->
      <div class="section">
        <h2 class="Black">Black</h2>
        <label for="black-click">Click</label>
        <textarea id="black-click" name="black-click" rows="2" placeholder="Enter macro for Black Click"></textarea>

        <label for="black-dblclick">Double Click</label>
        <textarea id="black-dblclick" name="black-dblclick" rows="2"
          placeholder="Enter macro for Black Double Click"></textarea>
      </div>

      <!-- White Section -->
      <div class="section">
        <h2 class="White">White</h2>
        <label for="white-click">Click</label>
        <textarea id="white-click" name="white-click" rows="2" placeholder="Enter macro for White Click"></textarea>

        <label for="white-dblclick">Double Click</label>
        <textarea id="white-dblclick" name="white-dblclick" rows="2"
          placeholder="Enter macro for White Double Click"></textarea>
      </div>

      <div class="button-container">
        <button type="button" id="save-macros" onclick="onClickSave()">Save Macros</button>
        <button type="button" id="factory-reset" onclick="onClickFactoryReset()">Erase Everything</button>
      </div>
    </form>
  </div>

  <script>
    function pageLoad() {
      fetch("/get-macros")
        .then(response => response.json())
        .then(data => {
          document.getElementById("red-click").value = data.RED.single;
          document.getElementById("red-dblclick").value = data.RED.double;
          document.getElementById("green-click").value = data.GREEN.single;
          document.getElementById("green-dblclick").value = data.GREEN.double;
          document.getElementById("yellow-click").value = data.YELLOW.single;
          document.getElementById("yellow-dblclick").value = data.YELLOW.double;
          document.getElementById("blue-click").value = data.BLUE.single;
          document.getElementById("blue-dblclick").value = data.BLUE.double;
          document.getElementById("black-click").value = data.BLACK.single;
          document.getElementById("black-dblclick").value = data.BLACK.double;
          document.getElementById("white-click").value = data.WHITE.single;
          document.getElementById("white-dblclick").value = data.WHITE.double;
        })
        .catch(error => console.error('Error fetching data:', error));
    }
    function onClickSave() {
      const form = document.getElementById('mainConfigForm');
      const formData = new FormData(form);
      const payload = {
        RED: { single: formData.get('red-click'), double: formData.get('red-dblclick') },
        GREEN: { single: formData.get('green-click'), double: formData.get('green-dblclick') },
        YELLOW: { single: formData.get('yellow-click'), double: formData.get('yellow-dblclick') },
        BLUE: { single: formData.get('blue-click'), double: formData.get('blue-dblclick') },
        BLACK: { single: formData.get('black-click'), double: formData.get('black-dblclick') },
        WHITE: { single: formData.get('white-click'), double: formData.get('white-dblclick') }
      };

      console.log(JSON.stringify(payload, null, 2));
      fetch('/save-macros', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload)
      })
        .then(response => {
          if (response.ok) {
            alert('Macros saved successfully!');
            // The server will restart the ESP after saving
          } else {
            alert('Error saving macros');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          alert('Error saving macros');
        });
    }
    function onClickFactoryReset() {
      if (confirm('WARNING: This will erase all your macros and reset to factory defaults. Are you sure?')) {
        fetch('/factory-reset')
          .then(response => {
            if (response.ok) {
              alert('Device is resetting to factory defaults...');
              // The server will restart the ESP after reset
            } else {
              alert('Error resetting device');
            }
          })
          .catch(error => {
            console.error('Error:', error);
            alert('Error resetting device');
          });
      }
    }
  </script>
</body>

</html>
)rawliteral";
