#ifndef UI_INDEX_H
#define UI_INDEX_H

const char* html = R"html(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');

        body {
            background-color: #212121;
            color: #e0f7fa;
            font-family: 'Press Start 2P', cursive;
            padding: 20px;
        }

        .fun-mode { animation: pulse 1s infinite; box-shadow: 0px 0px 20px rgba(255, 255, 255, 0.4); }
        @keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }

        .glow-on-hover:hover {
            box-shadow: 0 5px 15px rgba(255, 105, 180, 0.6), 0 5px 15px rgba(255, 255, 0, 0.4);
        }

        .tab a { color: #ff4081 !important; }
        ul.tabs { border-radius: 12px; }
        .tab a.active { background-color: rgba(255, 64, 129, 0.2) !important; color: #ff4081 !important; }
        .indicator { background-color: #ff4081 !important; }

        .control-card {
            background: linear-gradient(145deg, #303030, #484848);
            border-radius: 12px;
            padding: 20px;
            margin-bottom: 20px;
            margin-top: 20px;
            box-shadow: 5px 5px 15px rgba(0, 0, 0, 0.6), -5px -5px 15px rgba(0, 0, 0, 0.3);
        }

        .joint-control {
            margin: inherit;
        }

        .joint-section {align-content: space-evenly;}

        .collection-item {
            border-radius: 12px;
            background-color: #ff40810a !important;!i;!;
            border-bottom: none !important;!i;!;
            margin: 5px !important;!i;!;
        }

        .collection {
            border: none;
        }

        .btn-floating, .btn { background-color: #ff4081; }
        .btn:hover, .btn-floating:hover { background-color: #e91e63; }

        .movement-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            max-width: 300px;
            margin: 20px auto;
        }

        .center-align i {
            color: #ffffff;
        }

        .tooltip:hover::after {
            content: attr(data-tooltip);
            color: white;
            background: #424242;
            padding: 6px 10px;
            position: absolute;
            font-size: 0.8em;
            transform: translateY(-100%);
            white-space: nowrap;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="row">
            <div class="col s12">
                <h4 class="center-align" style="color: #ff4081">Robot Control Madness</h4>

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
                                <div class="collection-item">
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m h')">
                                        <i class="material-icons left">home</i>Home
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m s')">
                                        <i class="material-icons left">search</i>Scan
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m p')">
                                        <i class="material-icons left">pan_tool</i>Pick
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m d')">
                                        <i class="material-icons left">get_app</i>Drop
                                    </button>
                                    <button class="btn waves-effect waves-light" onclick="sendCommand('m w')">
                                        <i class="material-icons left">wave</i>Wave
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
