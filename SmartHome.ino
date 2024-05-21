// 2 LED
#define GENKAN_LED 4
#define LIVING_SPACE_LED 10

// 人感センサ
#define GENKAN_PIR_SENSOR 5
#define LIVING_SPACE_PIR_SENSOR 3
// 温度センサ
#include <DHT.h> 
#define DHT_PIN 7 
#define DHT_MODEL DHT11 
DHT dht(DHT_PIN, DHT_MODEL); 
// LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Wifi setup
#include <ESP8266WiFi.h>
const char* ssid = "your_wifi_SSID";
const char* password = "your_wifi_password";

// SMTP Client setup
#include <ESP_Mail_Client.h>
#define SMTP_SERVER "smtp.your_email_provider.com" // for example, smtp.gmail.com
#define SMTP_PORT 465 //465 for SSL, 587 for TLS
#define EMAIL_SENDER "your_email@example.com" // the email address where you want to send the alert 
#define EMAIL_SENDER_PASSWORD "your_email_password"
#define EMAIL_RECIPIENT "recipient_email@example.com" // the email address where you want to send the alert

SMTPData smtpData;

int celsius = 0;
int fahrenheit = 0;
bool isHome = false;
bool intruded = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendEmail(const char* subject, const char* message) {
  smtpData.setLogin(SMTP_SERVER, SMTP_PORT, EMAIL_SENDER, EMAIL_SENDER_PASSWORD);
  smtpData.setSender("Home Security", EMAIL_SENDER);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(message, false);
  smtpData.addRecipient(EMAIL_RECIPIENT);

  if (!MailClient.sendMail(smtpData)) {
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  } else {
    Serial.println("Email sent successfully");
  }

  smtpData.empty();
}

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  //Serialの起動
  Serial.begin(9600);
  Serial.write("start\n");
  //LCDの起動
  lcd.init();
  lcd.backlight();
  //人感センサ
  pinMode(GENKAN_PIR_SENSOR, INPUT_PULLUP);
  //pinMode(TOILET_PIR_SENSOR, INPUT_PULLUP);
  //温度センサ
  dht.begin();
  //LED
  pinMode(GENKAN_LED, OUTPUT);
  pinMode(LIVING_SPACE_LED, OUTPUT);

  //Wifi
  setup_wifi();
}

void welcomeMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome home");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(celsius);
  lcd.print(" C, ");
  lcd.print(fahrenheit);
  lcd.print(" F.");
  delay(2000);
  lcd.clear();
}

void breakInAlert(){
  lcd.setCursor(0,0);
  lcd.print("Home got intruded");
  delay(2000);
  lcd.clear();
  sendEmail("Alert: Break-in Detected", "A break-in has been detected at your home.");
}

void loop() {
  celsius = dht.readTemperature();;
  fahrenheit = ((celsius * 9) / 5 + 32);

  int genkan_pir_value = digitalRead(GENKAN_PIR_SENSOR);

  //Owner is back to home, play welcome message
  if (genkan_pir_value == HIGH && isHome == false) {
    digitalWrite(GENKAN_LED, HIGH);
    isHome = true;
    welcomeMessage();
  }
  //Toggle value for genkan LED
  if (genkan_pir_value == HIGH && isHome == true) {
    digitalWrite(GENKAN_LED, HIGH);
  } else if (genkan_pir_value == LOW) {
    digitalWrite(GENKAN_LED, LOW);
  }

  int living_space_pir_value = digitalRead(LIVING_SPACE_PIR_SENSOR);
  if (living_space_pir_value == HIGH) { // Toggle value for living room LED
    digitalWrite(LIVING_SPACE_LED, HIGH);
  } else {
    digitalWrite(LIVING_SPACE_LED, LOW);
  }
  //侵入者の判断
  if(isHome == false && living_space_pir_value == HIGH){ //the owner is not home, but the LED in the living roon reacted. that means the intruder must have gotten in by the windows
    intruded = true;
    breakInAlert();
  }
  if(isHome && !intruded && genkan_pir_value == HIGH && living_space_pir_value == HIGH){ //the owner is at the living room but the LED in the genkan reacted. that means the intruder must have gotten in by the main door 
    intruded = true;
    breakInAlert();
  }
}

