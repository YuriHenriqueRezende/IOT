#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>



#include <PubSubClient.h>
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi(){
  delay(10);
  Serial.println("");
  Serial.print("Conectando com ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void reconnect(){
  while (!client.connected()){
    Serial.print("Aguardando conexao MQTT.......");
    String clientId = "ESP32Client";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())){
      Serial.println("Conectado");
      client.subscribe("casa/publisher");
    } else{
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println("Tente novamente em 5s");
      delay(5000);
    }
  }
}
