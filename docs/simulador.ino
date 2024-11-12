#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 21
#define DHTTYPE DHT22 
#define TRIG_PIN 19 
#define ECHO_PIN 18 
#define BUZZER_PIN 4 
#define LED_PIN 2 

const char* default_SSID = "Wokwi-GUEST";
const char* default_PASSWORD = ""; 
const char* default_BROKER_MQTT = "18.220.18.202";
const int default_BROKER_PORT = 1883; 
const char* default_TOPIC_SUBSCRIBE = "/iot/listen";
const char* default_TOPIC_PUBLISH = "/iot/sensor";
const char* default_ID_MQTT = "fiware_001";

WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);

void initWiFi() {
  delay(10);
  WiFi.begin(default_SSID, default_PASSWORD);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi!");
}

void initMQTT() {
  MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  dht.begin();
  initWiFi();
  initMQTT();
  MQTT.publish(default_TOPIC_PUBLISH, "s|on");
}

void loop() {
  verifyConnections();
  publishSensorData();
  MQTT.loop();
}

void publishSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  long duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  String tempStr = String(temperature);
  String humiStr = String(humidity);
  String distStr = String(distance);
  
  MQTT.publish("/iot/temperature", tempStr.c_str());
  MQTT.publish("/iot/humidity", humiStr.c_str());
  MQTT.publish("/iot/distance", distStr.c_str());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  if (msg == "led/on") {
    digitalWrite(LED_PIN, HIGH);
  } else if (msg == "led/off") {
    digitalWrite(LED_PIN, LOW);
  } else if (msg == "buzzer/high") {
    tone(BUZZER_PIN, 1000);
    delay(500);
    noTone(BUZZER_PIN);
  } else if (msg == "buzzer/low") {
    tone(BUZZER_PIN, 500);
    delay(500);
    noTone(BUZZER_PIN);
  }
}

void verifyConnections() {
  if (!MQTT.connected()) {
    while (!MQTT.connected()) {
      Serial.print("Conectando ao broker MQTT...");
      if (MQTT.connect(default_ID_MQTT)) {
        Serial.println("Conectado ao broker MQTT!");
        MQTT.subscribe(default_TOPIC_SUBSCRIBE);
      } else {
        Serial.print("Falha ao conectar ao broker. Código de erro: ");
        Serial.println(MQTT.state());
        delay(2000);
      }
    }
  }
}
