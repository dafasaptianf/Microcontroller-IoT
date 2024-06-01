#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h" //Provide the real-time database payload printing info and other helper functions.

// Define network credentials (replace with your actual credentials)
const char* WIFI_SSID = "Username_WiFi" ;
const char* WIFI_PASSWORD = "Password_WiFi";

// Define Firebase project API Key (replace with your actual key)
const char* API_KEY = "API_KEY";

// Define Firebase Realtime Database URL (replace with your actual URL)
const char* DATABASE_URL = "DATABASE_URL_FIREBASE";

// Initialize Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupSuccess = false;

#define PWM 4
#define LED1_PIN 2
bool ledStatus;
int motorStatus;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(PWM, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.print("Connected to....");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // Assigning the project API key
  config.api_key = API_KEY;
  
  //Assign the project URL
  config.database_url = DATABASE_URL;
  
  /// check signup statue
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupSuccess = true;
    }
   else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    
  // Assign the callback function for token generation task
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupSuccess && (millis() - sendDataPrevMillis > 8000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getBool(&fbdo, "/LED"))
    {
      if(fbdo.dataType() == "boolean")
      {
        ledStatus = fbdo.boolData();
        Serial.println("Succesfull READ LED");
        digitalWrite(LED1_PIN, ledStatus);
      }
    }
      else{
      Serial.println("FAILED: " + fbdo.errorReason());
      }
    if (Firebase.RTDB.getInt(&fbdo, "/speed-motor"))
    {
      if(fbdo.dataType() == "int")
      {
        motorStatus = fbdo.intData();
        Serial.println("Succesfull READ motor");
        analogWrite(PWM, motorStatus);
      }
    }
      else{
      Serial.println("FAILED: " + fbdo.errorReason());
      }
  }
}