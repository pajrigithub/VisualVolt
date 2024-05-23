#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include "Wire.h"
#include "Adafruit_INA219.h"

#define FIREBASE_HOST "https://visualvoltamp-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "yjQwglb6zOZJEDIn3GPhE2ea0kvbbpZxd10nw6NT"
#define WIFI_SSID "KONSIKI LT1"
#define WIFI_PASSWORD "KonsIKI1"

Adafruit_INA219 ina219;

const float calibrationFactor = 0.7586;
float current_mA = 0;
float voltage = 0;
int analogValue;
String data[] = { "data1", "data2", "data3", "data4", "data5", "data6", "data7", "data8", "data9", "data10" };

void baca_arus() {
  current_mA = ina219.getCurrent_mA();
  Serial.println("Nilai Arus: " + String(current_mA) + "mA");
}

void baca_voltase() {
  analogValue = analogRead(17);
  voltage = analogValue / 4096.0 * 3.3 * (1 + 100000.0 / 16000.0) * calibrationFactor;
  Serial.println("Nilai voltage: " + String(voltage) + " V       ||       Nilai analog: " + String(analogValue));
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  ina219.begin();

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  baca_arus();
  baca_voltase();

  for (int i = 0; i < 10; i++) {
    Firebase.setFloat("Voltase/" + data[i], voltage);
  }

  for (int i = 0; i < 10; i++) {
    Firebase.setFloat("Ampere/" + data[i], voltage);
  }


  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  delay(1000);
}
