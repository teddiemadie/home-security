# home-security

This project is a simple home security system using an ESP8266 microcontroller. It monitors temperature, detects motion in two different areas, and sends email alerts if an intrusion is detected.

## Components

- ESP8266 (NodeMCU)
- 2 LEDs
- 2 PIR motion sensors
- DHT11 temperature and humidity sensor
- I2C LCD display
- WiFi network
- SMTP email service

## Libraries

Ensure you have the following libraries installed in your Arduino IDE:

- `DHT sensor library`
- `LiquidCrystal_I2C`
- `ESP8266WiFi`
- `ESP_Mail_Client`

You can install these libraries via the Arduino Library Manager.

## Pin Configuration

- `GENKAN_LED`: GPIO 4
- `LIVING_SPACE_LED`: GPIO 10
- `GENKAN_PIR_SENSOR`: GPIO 5
- `LIVING_SPACE_PIR_SENSOR`: GPIO 3
- `DHT11 Sensor`: GPIO 7
- `LCD`: I2C address `0x27`, 20x4 characters

## Setup

1. **Circuit Connections:**

   - Connect the LEDs to GPIO 4 and GPIO 10 through resistors.
   - Connect the PIR sensors to GPIO 5 and GPIO 3.
   - Connect the DHT11 sensor to GPIO 7.
   - Connect the I2C LCD display to the I2C pins of the ESP8266.

2. **WiFi Configuration:**

   Update the `ssid` and `password` variables with your WiFi network credentials:
   ```cpp
   const char* ssid = "your_wifi_SSID";
   const char* password = "your_wifi_password";
   ```

3. **SMTP Configuration:**

   Update the email and SMTP server details:
   ```cpp
   #define SMTP_SERVER "smtp.your_email_provider.com"
   #define SMTP_PORT 465
   #define EMAIL_SENDER "your_email@example.com"
   #define EMAIL_SENDER_PASSWORD "your_email_password"
   #define EMAIL_RECIPIENT "recipient_email@example.com"
   ```

## How It Works

1. The system initializes and connects to the WiFi network.
2. It monitors two PIR sensors:
   - One for the entrance (`GENKAN_PIR_SENSOR`)
   - One for the living space (`LIVING_SPACE_PIR_SENSOR`)
3. The DHT11 sensor measures the temperature.
4. The system uses an I2C LCD to display welcome messages and temperature readings.
5. If motion is detected in the entrance area while no one is home, a welcome message is displayed, and the `isHome` state is set to true.
6. If motion is detected in the living space while no one is home, an intrusion alert is triggered, and an email is sent.
7. If both sensors detect motion simultaneously while someone is home, an intrusion alert is triggered, and an email is sent.
