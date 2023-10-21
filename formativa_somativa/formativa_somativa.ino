#include <PubSubClient.h>
#include <WiFi.h>
#include "DHTesp.h"
#include <Adafruit_Sensor.h>
const char* ssid = "yuri";
const char* password = "123456789";
const char* mqtt_server = "test.mosquitto.org";

#include "DHTesp.h"

int RELE_PIN = 5;

const int DHT_PIN = 15;

#define MSG_BUFFER_SIZE (50)

char msg[MSG_BUFFER_SIZE];

uint32_t delayMS;
unsigned long lastMsg = 0;
int value = 0;
WiFiClient espClient;
PubSubClient client(espClient);

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  sensor_t sensor;
  setup_WiFi();
  client.setServer(mqtt_server, 1883);
  pinMode(RELE_PIN, OUTPUT);
}
void setup_WiFi() {
  delay(10);
  Serial.println("");
  Serial.print("conectando com");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi conectando");
  Serial.println("IP: ");  
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Aguardando conexão MQTT...");
    String clientId = "ESP32Client";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectando");

      sensors_event_t event; // Declare event here
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.print("tente novamente em 5s");
      delay(500);}
    }
}

void loop() {
  delay(delayMS);
  sensors_event_t event;
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  if (isnan(data.temperature)){
    Serial.println(F(" ERRO NA LEITURA DA TEMPERATURA"));
  } else {
    Serial.print(F("TEMPERATURA:"));
    Serial.print(data.temperature);
    Serial.println(F("°C"));
    sprintf(msg, "%f", data.temperature);
    client.publish("yuri/temperature", msg);

    if(data.temperature > 22) {
      sprintf(msg, "%i", 1);
      client.publish("yuri/ligadotemp", msg);
    } else {
      sprintf(msg, "%i", 0);
      client.publish("yuri/ligadotemp", msg);
    }
  }
  if (isnan(data.humidity)){
    Serial.println(F(" ERRO NA LEITURA DA HUMIDADE"));
  } else {
    Serial.print(F("HUMIDADE:"));
    Serial.print(data.humidity);
    Serial.println(F("%"));
    sprintf(msg, "%f", data.humidity);
    client.publish("yuri/humidity", msg);
    
    if(data.humidity > 50) {
      sprintf(msg, "%i", 1);
      client.publish("yuri/ligar", msg);
    } else {
      sprintf(msg, "%i", 0);
      client.publish("yuri/ligar", msg);  
    }
  }
  delay(2000);
  if(data.temperature >= 22){
    digitalWrite(RELE_PIN, HIGH);
    delay(5000);
  } else {
    digitalWrite(RELE_PIN, LOW);
    delay(5000);
  }

  if(data.temperature >= 22){
    digitalWrite(RELE_PIN, HIGH);
    delay(5000);
  } else {
    digitalWrite(RELE_PIN, LOW);
    delay(5000);
  }





  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}