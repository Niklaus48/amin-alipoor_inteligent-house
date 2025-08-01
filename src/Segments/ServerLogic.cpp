#include "ServerLogic.h"
#include <Arduino.h>

namespace segments{

    //Constructor
    ServerLogic::ServerLogic(const char* ssid, const char *pass, int32_t channel){
        SSID = ssid;
        Password = pass;
        this->channel = channel;
    } 

    void ServerLogic::sendStaticHtml(const HouseState& state){
        String html = R"rawliteral(<!DOCTYPE html>
            <html lang="en">
            <head>
                <script src="https://cdn.jsdelivr.net/npm/sweetalert2@11"></script>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Amin Alipoor</title>
            <style>
                * {
                    margin: 0;
                    padding: 0;
                    box-sizing: border-box;
                }

                body {
                    font-family: Arial, sans-serif;
                    background-color: #f0f2f5;
                    padding: 20px;
                }

                .container {
                    max-width: 1200px;
                    margin: 0 auto;
                }

                .header {
                    text-align: center;
                    margin-bottom: 30px;
                    color: #333;
                }

                .clock-container {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            border-radius: 15px;
            margin-bottom: 20px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.2);
        }

        .clock {
            font-size: 2.5em;
            font-weight: bold;
            margin-bottom: 5px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }

        .date-info {
            font-size: 1.1em;
            opacity: 0.9;
            margin-bottom: 10px;
        }

        .uptime {
            font-size: 0.9em;
            opacity: 0.8;
        }

                .clock {
                    font-size: 2em;
                    font-weight: bold;
                    color: #2c3e50;
                    margin-bottom: 10px;
                }

                .dashboard {
                    display: grid;
                    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
                    gap: 20px;
                }

                .card {
                    background: white;
                    border-radius: 10px;
                    padding: 20px;
                    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
                    border-left: 4px solid #3498db;
                }

                .card h3 {
                    color: #2c3e50;
                    margin-bottom: 15px;
                    font-size: 1.2em;
                }

                .sensor-value {
                    font-size: 1.5em;
                    font-weight: bold;
                    margin: 10px 0;
                }

                .temperature { color: #e74c3c; }
                .humidity { color: #3498db; }

                .status {
                    display: inline-block;
                    padding: 5px 15px;
                    border-radius: 20px;
                    font-weight: bold;
                    margin: 5px 0;
                }

                .status.on, .status.open, .status.active, .status.detected {
                    background-color: #2ecc71;
                    color: white;
                }

                .status.off, .status.closed, .status.inactive, .status.not-detected {
                    background-color: #e74c3c;
                    color: white;
                }

                .control-btn {
                    background-color: #3498db;
                    color: white;
                    border: none;
                    padding: 10px 20px;
                    border-radius: 5px;
                    cursor: pointer;
                    font-size: 1em;
                    margin: 5px;
                    transition: background-color 0.3s;
                }

                .control-btn:hover {
                    background-color: #2980b9;
                }

                .control-btn:active {
                    transform: scale(0.98);
                }

                .control-btn.danger {
                    background-color: #e74c3c;
                }

                .control-btn.danger:hover {
                    background-color: #c0392b;
                }

                .control-btn.success {
                    background-color: #27ae60;
                }

                .control-btn.success:hover {
                    background-color: #229954;
                }

                .controls {
                    margin-top: 15px;
                }

                .last-update {
                    text-align: center;
                    color: #7f8c8d;
                    margin-top: 20px;
                    font-size: 0.9em;
                }

                @media (max-width: 768px) {
                    .dashboard {
                        grid-template-columns: 1fr;
                    }

                    .clock {
                        font-size: 1.5em;
                    }
                }

                .auto-mode {
            background-color: #f39c12;
            color: white;
        }

        .auto-mode:hover {
            background-color: #e67e22;
        }

        .auto-indicator {
            display: inline-block;
            padding: 3px 8px;
            background-color: #f39c12;
            color: white;
            border-radius: 10px;
            font-size: 0.8em;
            margin-left: 10px;
        }
            </style>
        </head>
        <body>
            <div class="container">
                <div class="header">
            <h1>Amin Alipoor Dashboard</h1>
            <div class="clock-container">
                <div class="clock" id="clock"></div>
            </div>
        </div>

                <div class="dashboard">
                    <!-- Temperature & Humidity Card -->
                    <div class="card">
                        <h3>🌡وضعیت آب و هوا</h3>
                        <div class="sensor-value temperature">
                            دما : <span id="temperature">$temp$</span>°C
                        </div>
                        <div class="sensor-value humidity">
                            رطوبت : <span id="humidity">$hum$</span>%
                        </div>
                    </div>

                    <!-- Fan Control Card -->
                    <div class="card">
            <h3>💨 تنظیمات پنکه</h3>
            <div>
                وضعیت : <span class="status" id="fan-status">$fan$</span>
                <span class="auto-indicator" id="auto-indicator" style="display: none;">AUTO</span>
            </div>
            <div>
                حالت خودکار : <span class="status" id="auto-fan-status">$auto-fan$</span>
            </div>
            <div class="controls">
                <button class="control-btn success" onclick="SendAction('fan/on')">روشن کردن</button>
                <button class="control-btn danger" onclick="SendAction('fan/off')">خاموش کردن</button>
                <button class="control-btn auto-mode" onclick="SendAction('fan/auto')">تغییر وضعیت خودکار</button>
            </div>
            <div style="margin-top: 10px; font-size: 0.9em; color: #666;">
                آستانه ی دما: <span id="auto-threshold">$max-temp$</span>
            </div>
        </div>

                    <!-- Lamp Control Card -->
                    <div class="card">
                        <h3>تنظیمات لامپ</h3>
                        <div>
                            وضعیت: <span class="status" id="lamp-status">$lamp$</span>
                        </div>
                        <div>
                            حالت خودکار : <span class="status" id="auto-light-status">$auto-light$</span>
                        </div>
                        <div class="controls">
                            <button class="control-btn success" onclick="SendAction('lamp/on')">روشن کردن</button>
                            <button class="control-btn danger" onclick="SendAction('lamp/off')">خاموش کردن</button>
                            <button class="control-btn auto-mode" onclick="SendAction('lamp/auto')">تغییر وضعیت خودکار</button>
                        </div>
                    </div>

                    <!-- Door Control Card -->
                    <div class="card">
                        <h3>🚪تنظیمات در</h3>
                        <div>
                            وضعیت در: <span class="status" id="door-status">$door$</span>
                        </div>
                        <div>
                            محافظ در: <span class="status" id="door-protector-status">$door-protect$</span>
                        </div>
                        <div class="controls">
                            <button class="control-btn success" onclick="SendAction('door/open')">باز کردن در</button>
                            <button class="control-btn danger" onclick="SendAction('door/close')">بستن در</button>
                            <button class="control-btn" onclick="SendAction('door/protect')">تغییر وضعیت محافظ</button>
                        </div>
                    </div>

                    <!-- Presence Sensor Card -->
                    <div class="card">
                        <h3>👤سنسور حضور</h3>
                        <div>
                        وضعیت: <span class="status" id="presence-status">$precense$</span>
                        </div>
                    </div>
                </div>
            </div>

            <script>
                const ESP32_IP = 'localhost:8180';

                // Fetch sensor data from ESP32
                async function SendAction(action) {
                    try {
                        const response = await fetch(`http://${ESP32_IP}/${action}`);
                        const data = await response.json();

                        ApplyJsonData(data);

                    } catch (error) {
                        console.error('Error fetching sensor data:', error);
                        document.getElementById('last-update').textContent = 'Connection Error';
                    }
                }

                function ApplyJsonData(data){         
                    // Update temperature and humidity
                    document.getElementById('temperature').textContent = data.temp || '--';
                    document.getElementById('auto-threshold').textContent = data.tempThreshold || '--';
                    document.getElementById('humidity').textContent = data.hum || '--';
                    document.getElementById('door-status').textContent = data.isMainDoorOpen ? 'باز' : 'بسته';
                    document.getElementById('door-protector-status').textContent = data.IsDoorPretectorActive ? 'فعال' : 'غیرفعال';
                    document.getElementById('fan-status').textContent = data.IsFanActive ? 'روشن' : 'خاموش';
                    document.getElementById('auto-fan-status').textContent = data.IsAutoFanActive ? 'فعال' : 'غیرفعال';
                    document.getElementById('lamp-status').textContent = data.IsLightOn ? 'روشن' : 'خاموش';
                    if (data.IsLightOn == false){
                        if (data.IsAutoLightActive){
                            if (data.IsAnyPrecense){
                                document.getElementById('lamp-status').textContent = 'روشن'
                            }
                            else if (data.min >= 16){
                                document.getElementById('lamp-status').textContent = 'روشن'
                            }
                            else{
                                document.getElementById('lamp-status').textContent = 'خاموش'
                            }
                        }
                    }
                    document.getElementById('auto-light-status').textContent = data.IsAutoLightActive ? 'فعال' : 'غیرفعال';
                    document.getElementById('presence-status').textContent = data.IsAnyPrecense ? 'دارای حضور' : 'بدون حضور';

                    let min = String(data.min + 100).slice(-2).padStart(2, '0');
                    let sec = String(data.sec).slice(-2).padStart(2, '0');

                    document.getElementById('clock').textContent = min + ':' + sec;

                    if (!Swal.isVisible()){
                        if (data.temp >= 70){
                            Swal.fire({
                            title: 'هشدار',
                            text: 'دما بیش از حد مجاز احتمال آتش سوزی',
                            icon: 'warning',
                            confirmButtonText: 'تایید'
                            });
                        }
                    }

                    if (!Swal.isVisible()){
                        if (data.IsDoorPretectorActive && data.isMainDoorOpen){
                            Swal.fire({
                            title: 'هشدار',
                            text: 'هشدار سرقت',
                            icon: 'warning',
                            confirmButtonText: 'تایید'
                            });
                        }
                    }
                }

                async function FetchData(){
                    const response = await fetch(`http://${ESP32_IP}/fetch`);
                    const data = await response.json();

                    ApplyJsonData(data);
                }

                FetchData();
                setInterval(FetchData, 1000);
            </script>
        </body>
        </html>)rawliteral";

        server.send(200,"text/html",html);
    }

    void ServerLogic::Init(){
        WiFi.begin(SSID, Password, channel);
        Serial.println("Attempt to connect to the wifi with SSId: ");
        Serial.print(SSID);
        Serial.print(" and Password: ");
        Serial.print(Password);
        Serial.println();

        Serial.print("Connecting");
        while (WiFi.status() != WL_CONNECTED){
            delay(100);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Successfully Connected to wifi!");

        //config routes here
        server.on("/", [this]() {
            this->sendStaticHtml(HouseState::Instance);
        });

        server.on("/fetch", [this]() {
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });

        server.on("/fan/on", [this]() {
            HouseState::Instance.IsFanActive = true;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/fan/off", [this]() {
            HouseState::Instance.IsFanActive = false;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/fan/auto", [this]() {
            HouseState::Instance.IsAutoFanActive = !HouseState::Instance.IsAutoFanActive;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });

        server.on("/lamp/on", [this]() {
            HouseState::Instance.IsLightOn = true;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/lamp/off", [this]() {
            HouseState::Instance.IsLightOn = false;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/lamp/auto", [this]() {
            HouseState::Instance.IsAutoLightActive = !HouseState::Instance.IsAutoLightActive;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });


        server.on("/door/open", [this]() {
            HouseState::Instance.IsMainDoorOpen = true;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/door/close", [this]() {
            HouseState::Instance.IsMainDoorOpen = false;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });
        server.on("/door/protect", [this]() {
            HouseState::Instance.IsDoorPretectorActive = !HouseState::Instance.IsDoorPretectorActive;
            HouseState::Instance.IsMainDoorOpen = false;
            NotifyObservers(HouseState::Instance);
            server.send(200, "application/json", HouseState::Instance.ToJson());
        });


        server.begin();
        Serial.println("HTTP server started");
        Serial.println("Please open this url to access server => http://localhost:8180");
    }

    void ServerLogic::Update(){
        server.handleClient();
    }
}

