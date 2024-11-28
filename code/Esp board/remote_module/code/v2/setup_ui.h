// Configuration Page UI
const char SETUP_UI[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
    <title>Device Setup</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            max-width: 500px;
            margin: 0 auto;
            padding: 20px;
            background-color: #121212;
            color: #e0e0e0;
        }
        h1, h2 {
            text-align: center;
        }
        h1 {
            color: #ffb400;
        }
        input, select {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            box-sizing: border-box;
            border: 1px solid #333;
            background-color: #1e1e1e;
            color: #e0e0e0;
            border-radius: 5px;
        }
        input:focus, select:focus {
            outline: none;
            border-color: #ffb400;
            box-shadow: 0 0 5px #ffb400;
        }
        .section {
            border: 1px solid #333;
            border-radius: 10px;
            padding: 15px;
            margin-bottom: 15px;
            background-color: #1e1e1e;
        }
        .submit-btn {
            background-color: #ff5722;
            color: white;
            border: none;
            cursor: pointer;
            padding: 12px;
            font-size: 1.1em;
            border-radius: 5px;
            width: -webkit-fill-available;
            transition: transform 0.1s ease, background-color 0.3s ease;
        }
        .submit-btn:hover {
            background-color: #e64a19;
            transform: scale(1.05);
        }
        .submit-btn:active {
            transform: scale(0.95);
        }
        #adafruitFields {
            margin-top: 15px;
        }
    </style>
</head>
<body>
    <h1>Device Setup</h1>
    <form action="/setup" method="POST">
        <label for="ssid">WiFi SSID:</label>
        <input type="text" id="ssid" name="ssid" required>
        <label for="password">WiFi Password:</label>
        <input type="password" id="password" name="password" required>
        <label for="mdns">mDNS Name:</label>
        <input type="text" id="mdns" name="mdns" required>
        <button type="submit" class="submit-btn">Save & Reboot</button>
    </form>
</body>
</html>
)rawliteral";
