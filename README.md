# Onion-Spoilage-Monitoring-System
An IoT-based onion spoilage monitoring system that uses
VOC sensing, temperature and humidity monitoring, and
a trend-based Onion Spoilage Risk Index (OSRI) to detect
early spoilage conditions in onion storage environments.

The system integrates ESP32, MQ-135, DHT22, AWS IoT Core,
MQTT communication, and Node-RED visualization.
## Features

- Real-time VOC monitoring
- Temperature and humidity sensing
- OSRI calculation
- AWS IoT cloud integration
- MQTT communication
- Node-RED dashboard
- Trend-based spoilage analysis
- Ventilation recommendation
## Hardware Used

- ESP32
- MQ-135 Gas Sensor
- DHT22 Sensor
- Wi-Fi Network
## Software Used

- Arduino IDE
- AWS IoT Core
- MQTT
- Node-RED
## System Architecture
<img width="975" height="817" alt="image" src="https://github.com/user-attachments/assets/076d77b8-e95e-45fe-b372-fe878741d6c8" />
### Results

The system successfully monitors onion storage conditions in real time and provides spoilage risk estimation using the OSRI model.

### Dashboard Output

<img width="768" height="1366" alt="Screenshot 2026-03-09 170930" src="https://github.com/user-attachments/assets/8fdc3ea8-4a91-410f-aa49-0d22a3f92db7" />
<img width="768" height="1366" alt="Screenshot 2026-03-09 171009" src="https://github.com/user-attachments/assets/9b631440-7c04-4956-b988-e97363c88b79" />



### VOC Trend Analysis

<img width="573" height="417" alt="Screenshot 2026-04-29 235652" src="https://github.com/user-attachments/assets/b9cae9a0-e6ee-4104-9db0-875ae24ef440" />


### OSRI Trend Analysis

<img width="1366" height="768" alt="Screenshot 2026-03-10 123938" src="https://github.com/user-attachments/assets/258b13ec-ada9-471e-9f08-fc1b6e33c7bd" />


### Key Outcomes

- Real-time monitoring of VOC, temperature, and humidity
- Cloud-based data storage using AWS IoT Core
- MQTT communication between ESP32 and AWS
- Interactive Node-RED dashboard
- Trend-based spoilage risk assessment
- Early warning generation for potential spoilage




