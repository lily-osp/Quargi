#ifndef UI_INDEX_H
#define UI_INDEX_H

const char* htmlContent = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 WEB REMOTE</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');

        :root {
            --primary-dark: #2D1B36;
            --secondary-dark: #1A0F20;
            --accent-purple: #6B2F8A;
            --highlight-purple: #8A3DAD;
            --text-light: #E0D6E4;
            --text-dark: #1A0F20;
            --detect-button: #9C27B0;
            --detect-button-hover: #BA68C8;
            --reset-button: #673AB7;
            --reset-button-hover: #7E57C2;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        html, body {
            font-family: 'Press Start 2P', sans-serif;
            color: var(--text-light);
            margin: 0;
            background-color: var(--secondary-dark);
            min-height: 100vh;
        }

        h1 {
            font-size: clamp(1rem, 2.5vw, 1.4rem);
            color: var(--text-light);
            text-align: center;
            padding: 15px;
            background-color: var(--accent-purple);
            margin: 0;
        }

        h2 {
            color: var(--text-light);
            font-size: clamp(0.7rem, 2vw, 0.9rem);
            margin: 0 0 10px 0;
        }

        .topnav {
            background-color: var(--accent-purple);
            position: sticky;
            top: 0;
            z-index: 100;
        }

        .fun-mode {
            animation: pulse 1s infinite;
            box-shadow: 0px 0px 20px rgba(138, 61, 173, 0.4);
        }

        .container {
            margin: 15px auto;
            max-width: 1280px;
            width: 90%
        }

        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }

        .glow-on-hover:hover {
            box-shadow: 0 5px 15px rgba(107, 47, 138, 0.6), 0 5px 15px rgba(138, 61, 173, 0.4);
        }

        .tab a { color: var(--highlight-purple) !important; }
        ul.tabs { border-radius: 12px; }
        .tab a.active {
            background-color: var(--primary-dark) !important;
            color: var(--highlight-purple) !important;
        }
        .indicator { background-color: var(--highlight-purple) !important; }

        .control-card {
            background: linear-gradient(145deg, var(--primary-dark), var(--secondary-dark));
            border-radius: 12px;
            padding: 20px;
            margin-bottom: 20px;
            margin-top: 20px;
            box-shadow: 5px 5px 15px rgba(0, 0, 0, 0.6), -5px -5px 15px rgba(0, 0, 0, 0.3);
        }

        .joint-control {
            margin: inherit;
        }

        .joint-section {
            align-content: space-evenly;
        }

        .collection-item {
            border-radius: 12px;
            background-color: transparent !important;
            border-bottom: none !important;
            margin: 5px !important;
        }

        .collection {
            border: none;
        }

        .btn-floating, .btn {
            background-color: var(--detect-button);
            border-radius: 8px;
        }

        .btn:hover, .btn-floating:hover {
            background-color: var(--detect-button-hover);
        }

        .movement-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            max-width: 300px;
            margin: 20px auto;
        }

        .center-align i {
            color: var(--text-light);
        }

        .tooltip:hover::after {
            content: attr(data-tooltip);
            color: var(--text-light);
            background: var(--primary-dark);
            padding: 6px 10px;
            position: absolute;
            font-size: 0.8em;
            transform: translateY(-100%);
            white-space: nowrap;
        }

        .button-group {
        display: flex;
        flex-wrap: wrap;
        gap: 10px;
        justify-content: center;
    }

    .movement-btn {
        display: inline-flex;
        align-items: center;
        justify-content: center;
        gap: 8px;
        min-width: 120px;
        padding: 0 20px;
        height: 36px;
    }

    .movement-btn i {
        margin: 0;
        font-size: 20px;
    }

    .movement-btn span {
        font-size: 14px;
    }
    </style>
</head>
<body>
    <div class="topnav">
        <h1>Robot Control Madness</h1>
    </div>
    <div class="container">
        <div class="row">
            <div class="col s12">
                <div class="nav-tabs">
                    <ul class="tabs">
                        <li class="tab col s6"><a class="active" href="#BodyControl">
                            <i class="material-icons left">directions_car</i> Body Control
                        </a></li>
                        <li class="tab col s6"><a href="#ArmControl">
                            <i class="material-icons left">build</i> Arm Control
                        </a></li>
                    </ul>
                </div>

                <div id="BodyControl">
                    <div class="control-card">
                        <h5 class="center-align">Movement Control</h5>
                        <div class="movement-grid">
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('rl')">
                                <i class="material-icons">rotate_left</i>
                            </button>
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('mv')">
                                <i class="material-icons">arrow_upward</i>
                            </button>
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('rr')">
                                <i class="material-icons">rotate_right</i>
                            </button>

                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('lt')">
                                <i class="material-icons">arrow_back</i>
                            </button>
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('st')">
                                <i class="material-icons">stop</i>
                            </button>
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('rt')">
                                <i class="material-icons">arrow_forward</i>
                            </button>

                            <div></div>
                            <button class="btn-floating waves-effect waves-light" onclick="sendCommand('bk')">
                                <i class="material-icons">arrow_downward</i>
                            </button>
                            <div></div>
                        </div>

                        <div class="divider"></div>

                        <div class="speed-control center-align">
                            <h5>Speed Control</h5>
                            <p class="slider-label">Speed: <span id="speedValue">128</span></p>
                            <p class="range-field">
                                <input type="range" min="0" max="255" value="128"
                                    oninput="document.getElementById('speedValue').innerHTML = this.value"
                                    onchange="sendCommand('spd ' + this.value)" />
                            </p>
                        </div>

                        <div class="divider"></div>

                        <div class="center-align">
                            <h5>Obstacle Avoidance</h5>
                            <button class="btn waves-effect waves-light" onclick="sendCommand('oa on')">
                                <i class="material-icons left">security</i>Enable OA
                            </button>
                            <button class="btn waves-effect waves-light" onclick="sendCommand('oa off')">
                                <i class="material-icons left">security</i>Disable OA
                            </button>
                            <button class="btn waves-effect waves-light" onclick="sendCommand('oa nav')">
                                <i class="material-icons left">explore</i>Auto Navigate
                            </button>
                        </div>
                    </div>
                </div>

                <div id="ArmControl">
                    <div class="control-card">
                        <h5 class="center-align">Joint Control</h5>
                        <div class="joint-control">
                            <div class="joint-section">
                                <h6>Base</h6>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('b -')">
                                    <i class="material-icons">arrow_back</i>
                                </button>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('b +')">
                                    <i class="material-icons">arrow_forward</i>
                                </button>
                            </div>

                            <div class="joint-section">
                                <h6>Shoulder</h6>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('s -')">
                                    <i class="material-icons">arrow_downward</i>
                                </button>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('s +')">
                                    <i class="material-icons">arrow_upward</i>
                                </button>
                            </div>

                            <div class="joint-section">
                                <h6>Elbow</h6>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('e -')">
                                    <i class="material-icons">arrow_downward</i>
                                </button>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('e +')">
                                    <i class="material-icons">arrow_upward</i>
                                </button>
                            </div>

                            <div class="joint-section">
                                <h6>Gripper</h6>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('g o')">
                                    <i class="material-icons">pan_tool</i> Open
                                </button>
                                <button class="btn waves-effect waves-light" onclick="sendCommand('g c')">
                                    <i class="material-icons">power_settings_new</i> Close
                                </button>
                            </div>
                        </div>

                        <div class="divider"></div>

                        <div class="center-align">
                            <h5>Pre-defined Movements</h5>
                            <div class="collection">
                                <div class="collection-item button-group">
                                    <button class="btn waves-effect waves-light movement-btn" onclick="sendCommand('m h')">
                                        <i class="material-icons">home</i>
                                        <span>Home</span>
                                    </button>
                                    <button class="btn waves-effect waves-light movement-btn" onclick="sendCommand('m s')">
                                        <i class="material-icons">search</i>
                                        <span>Scan</span>
                                    </button>
                                    <button class="btn waves-effect waves-light movement-btn" onclick="sendCommand('m p')">
                                        <i class="material-icons">pan_tool</i>
                                        <span>Pick</span>
                                    </button>
                                    <button class="btn waves-effect waves-light movement-btn" onclick="sendCommand('m d')">
                                        <i class="material-icons">get_app</i>
                                        <span>Drop</span>
                                    </button>
                                    <button class="btn waves-effect waves-light movement-btn" onclick="sendCommand('m w')">
                                        <i class="material-icons">waving_hand</i>
                                        <span>Wave</span>
                                    </button>
                                </div>
                            </div>
                        </div>

                        <div class="divider"></div>

                        <div class="center-align">
                            <h5>Position Memory</h5>
                            <div class="collection">
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m pos 1')">
                                        <i class="material-icons left">save</i>Save Pos 1
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m save 1')">
                                        <i class="material-icons left">replay</i>Load Pos 1
                                    </button>
                                </div>
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m pos 2')">
                                        <i class="material-icons left">save</i>Save Pos 2
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m save 2')">
                                        <i class="material-icons left">replay</i>Load Pos 2
                                    </button>
                                </div>
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m pos 3')">
                                        <i class="material-icons left">save</i>Save Pos 3
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m save 3')">
                                        <i class="material-icons left">replay</i>Load Pos 3
                                    </button>
                                </div>
                            </div>
                        </div>

                        <div class="divider"></div>

                        <div class="center-align">
                            <h5>Command Recording</h5>
                            <div class="collection">
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('stream')">
                                        <i class="material-icons left">fiber_manual_record</i>Start Recording
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('done')">
                                        <i class="material-icons left">stop</i>Stop Recording
                                    </button>
                                </div>
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('play')">
                                        <i class="material-icons left">play_arrow</i>Play Recording
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('clear')">
                                        <i class="material-icons left">clear</i>Clear Recording
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js"></script>
    <script>
        document.addEventListener('DOMContentLoaded', function() {
            var tabs = document.querySelectorAll('.tabs');
            M.Tabs.init(tabs);
        });

        function sendCommand(cmd) {
            fetch('/command?cmd=' + cmd)
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                    M.toast({html: 'Command sent: ' + cmd, classes: 'rounded green'});
                })
                .catch(error => {
                    console.error('Error:', error);
                    M.toast({html: 'Error sending command', classes: 'rounded red'});
                });
        }
    </script>
</body>
</html>
)html";

#endif // UI_INDEX_H
