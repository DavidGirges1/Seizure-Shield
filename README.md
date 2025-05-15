# Seizure-Shield

**Seizure-Shield** is a wearable IoT device designed to assist individuals experiencing generalized epileptic seizures. The system combines real-time seizure detection with automated emergency response capabilities.

## 🚨 Key Features

- **Real-time Seizure Detection**
- **Audible Alarm**  
  Emits a loud sound to alert nearby individuals.
- **Automated Emergency Messaging**  
  Sends WhatsApp messages to emergency contacts, including:
  - Real-time GPS location
  - Link to live sensor data
- **Web-based Monitoring Dashboard**  
  Displays live sensor data through visual charts.

---

## 🔧 Technologies Used

### 🛠️ Hardware Components
- ESP32 microcontroller board
- Galvanic Skin Response (GSR) sensor
- Accelerometer sensor
- Temperature sensor
- GPS module

### 💻 Software & Platforms
- **C Programming** – for ESP32 firmware
- **WhatsApp API** – for automated alert messages with real-time links
- **ThingSpeak** – for live sensor data visualization via web charts
- **Web App** – built using HTML and Bootstrap to display:
  - Sensor readings
  - Victim’s GPS location

---

## 📄 Additional Resources

For detailed insights on the testing methodology and scientific background, refer to:

- [`poster.pdf`](./poster.pdf)
- [`portfolio.pdf`](./portfolio.pdf)
