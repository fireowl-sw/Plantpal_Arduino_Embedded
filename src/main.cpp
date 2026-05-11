#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "戴墨镜的喵仔";
char pass[] = "cherry123456";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "iot.dfrobot.com";
int port = 1883;
const char mqttUser[] = "um6XR5tvR";
const char mqttPass[] = "um6XR5tDgz";
const char topic[] = "FiF3R5pvg";

int lightPin = A0;
int soilPin = A1;
int lightValue;
float lightVoltage;
float lightPercent;
int soilValue;
float soilPercent;
float humidity;
float temperature;
int darkThreshold = 880;

unsigned long lastPublish = 0;
const long publishInterval = 1800000; // 10000 1800000

void setup() {
  Serial.begin(9600);
  unsigned long serialStart = millis();
  while (!Serial && millis() - serialStart < 3000) {}

  pinMode(LED_BUILTIN, OUTPUT);

  dht.begin();
  analogReadResolution(12);

  if (WiFi.begin(ssid, pass) == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  }

  mqttClient.setUsernamePassword(mqttUser, mqttPass);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  } else {
    Serial.print("MQTT connected to ");
    Serial.print(broker);
    Serial.print(":");
    Serial.println(port);
  }

  lastPublish = millis() - publishInterval;
}

void loop() {
  mqttClient.poll();

  lightValue = analogRead(lightPin);
  lightVoltage = lightValue * 3.3 / 4095.0;
  lightPercent = (4095 - lightValue) * 100.0 / 4095.0;

  soilValue = analogRead(soilPin);
  soilPercent = (4095 - soilValue) * 100.0 / 4095.0;

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (lightValue < darkThreshold) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.print("light = ");
  Serial.print(lightPercent);
  Serial.print("%, temp = ");
  Serial.print(temperature);
  Serial.print(", humidity = ");
  Serial.print(humidity);
  Serial.print(", soil = ");
  Serial.print(soilPercent);
  Serial.println("%");

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;

    mqttClient.beginMessage(topic);
    mqttClient.print("{\"light\":\"");
    mqttClient.print(lightPercent);
    mqttClient.print("%\"");
    mqttClient.print(",\"temp\":");
    mqttClient.print(temperature);
    mqttClient.print(",\"humidity\":");
    mqttClient.print(humidity);
    mqttClient.print(",\"soil\":\"");
    mqttClient.print(soilPercent);
    mqttClient.print("%\"}");
    mqttClient.endMessage();
  }

  delay(1000);
}
