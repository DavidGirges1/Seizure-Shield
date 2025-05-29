//Firebase Connection
/// FireBase connection 
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "esp32@gmail.com"
#define USER_PASSWORD "123456"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCHU39KJMTWUhjcRpT9CJaMOhTzTrHJ8lE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32paula-steam-default-rtdb.europe-west1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;

String uid;
///////////////// END OF FIREBASE 

//Whatsapp API
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "David";
const char* password = "12345678";

String phoneNumber = "+201275313345";
String apiKey = "4499946";

// accalometer 
#include <Wire.h> //
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Create an instance of the ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

/// GSR ///
#include <Wire.h>
#define GSR_PIN 34

/// SD ///
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 23
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* serverName = "http://api.thingspeak.com/update?api_key=";
String apiKey2 = "2Q1SXLGZ3J7S2XNM";


bool codeExecuted = false;

void setup() {

  Serial.begin(115200);

  // WIFI Conenction and What'sAPP
    // Whatsapp 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  // FIREBASE
   /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);

////////////////////////////////////

    /// ACCAOLMETER ///
  // Begin serial communication
  pinMode(2, OUTPUT);
  Serial.begin(115200);

  // Begin I2C communication
  Wire.begin(21, 22); // SDA, SCL

  // Initialize the accelerometer
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }

  /// GSR //// 

  pinMode(GSR_PIN, INPUT);

  /// sd /// 

  sensors.begin();

}



void loop() {
  /// FIREBASE
    if (Firebase.isTokenExpired()){
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  }

  /// Accalometer /// 
  // Get a new sensor event
  sensors_event_t event;
  accel.getEvent(&event);

  // Print the acceleration values
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");

  // Wait for a while before taking the next reading
  delay(500);


  /// GSR ///  Micro Scemends updated
  int gsrValue = analogRead(GSR_PIN);
  float voltage = gsrValue * (5.0/4095.0);
  float GSR = voltage / 0.132;
  Serial.print("GSR Value In micro Scemends: ");
  Serial.println(GSR);
  delay(1000);


  /// SD /// 
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  
  if(tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println("°C");
  }
  delay(1000);


//firebase
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String serverPath = serverName + apiKey2 + "&field1=" + String(GSR) + "&field2=" + String(tempC) + "&field3=" + String(event.acceleration.x) + "&field4=" + String(event.acceleration.y) + "&field5=" + String(event.acceleration.z);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(100);


  // Condition
int counter; 

if (GSR != 0)  {
    if ((tempC > 39.4) || (GSR > 50 || GSR < 5)) {    // GSR and GSM conditions
        Serial.println("Status: Danger"); // print "Status: Danger" when the conditions are met
        Serial.print("--------------------------------------");

        String website = "https://davidelksfam.github.io/epilpesy.github.io/";
        String message = "DANGER! Epilpesy Case Detected, SEE live location and sensors reading from: " + website;

        if (!codeExecuted) {
          sendMessage(message); // send an SMS with the message "Status: Danger";
          codeExecuted = true;

        }
        digitalWrite (2, HIGH);
        delay (1000); 
        digitalWrite (2, LOW);
    } else {
        Serial.println("Status: Safe"); // print "Status: Safe" when the conditions are not met
        Serial.print("--------------------------------------");
    }
}



  // FIREBASE UPDATE READINGSs
  
  //GSR

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "GSR/value", GSR)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
  // TEMP

    if (Firebase.RTDB.setFloat(&fbdo, "temp/value", tempC)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  

  // ACCALOMETER
//x
    if (Firebase.RTDB.setFloat(&fbdo, "AccXX/value", event.acceleration.x)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  //y
    if (Firebase.RTDB.setFloat(&fbdo, "AccYY/value", event.acceleration.y)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  //z
    if (Firebase.RTDB.setFloat(&fbdo, "AccZZ/value", event.acceleration.z)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
}





}

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  

}
