
#include <Arduino.h>


#define BLYNK_TEMPLATE_ID "TMPL3ZU8B3lQC"
#define BLYNK_TEMPLATE_NAME "New Template"
#define BLYNK_AUTH_TOKEN "IEEtzeTuvPXBYmQcNEVMwCOW4x5uDzAW"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHTPIN 15
#define DHTTYPE DHT22
#define SOIL_PIN 34
#define RELAY_PIN 5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

bool manualMode = false;
int manualState = 0;

float tempThreshold = 50.0;
int soilThreshold = 40;

// Manual pump control
BLYNK_WRITE(V0) {
    manualState = param.asInt();
    manualMode = true;

    if (manualState == 1) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Manual Pump ON");
    } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("Manual Pump OFF");
    }
}

// Switch back to auto
BLYNK_WRITE(V4) {
    if(param.asInt() == 1){
        manualMode = false;
        Serial.println("AUTO MODE ENABLED");
    }
}

void sendSensorData() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilRaw = analogRead(SOIL_PIN);

    if(isnan(temp) || isnan(humidity)){
        Serial.println("DHT Error");
        return;
    }

    int soilPercent = map(soilRaw, 4095, 0, 0, 100);
    soilPercent = constrain(soilPercent,0,100);

    Blynk.virtualWrite(V1,temp);
    Blynk.virtualWrite(V2,humidity);
    Blynk.virtualWrite(V3,soilPercent);

    Serial.println("------ SENSOR DATA ------");
    Serial.print("Temp: ");
    Serial.println(temp);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    Serial.print("Soil: ");
    Serial.println(soilPercent);

    if(!manualMode){
        if(temp > tempThreshold || soilPercent < soilThreshold){
            digitalWrite(RELAY_PIN, HIGH);
            Serial.println("AUTO: Pump ON");
        }
        else{
            digitalWrite(RELAY_PIN, LOW);
            Serial.println("AUTO: Pump OFF");
        }
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    dht.begin();

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    timer.setInterval(2000L, sendSensorData);
}

void loop() {
    Blynk.run();
    timer.run();
}