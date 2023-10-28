#include <PubSubClient.h>                        // Essa biblioteca permite que dispositivos baseados em Arduino se conectem a sistemas de mensagens e publiquem/assinem tópicos em um sistema de mensagens. //
#include <WiFi.h>                                // Essa biblioteca do Arduino que permite que dispositivos baseados em Arduino se conectem a redes Wi-Fi, possibilitando a comunicação com a Internet e outros dispositivos em uma rede local sem fio. //
#include "DHTesp.h"                              // Essa biblioteca específica para trabalhar com sensores de temperatura e umidade da série DHT (como DHT11, DHT22, etc.) //
const char* ssid = "yuri";                       // E o nome da minha rede WIFI que eu quero conectar ao ESP //
const char* password = "123456789";              // A senha da minha rede wifi que eu quero conectar ao ESP //
const char* mqtt_server = "test.mosquitto.org";  // Endereço do servidor MQTT real ao meu ESP. //

int RELE_PIN = 5;  // Estou definindo a porta do meu rele //

const int DHT_PIN = 15;  // Estou definindo a porta do meu sensor de temperatura //

#define MSG_BUFFER_SIZE (50)  // Essa linha especifica o tamanho de um buffer ou array de mensagens, garantindo que o tamanho seja consistente em todo o programa. //

char msg[MSG_BUFFER_SIZE]; // Cria um array de caracteres chamado msg com um tamanho específico, determinado pela constante MSG_BUFFER_SIZE que foi definida anteriormente no código com #define MSG_BUFFER_SIZE (50). //

uint32_t delayMS; // Variável para controlar um atraso ou temporização em milissegundos. //
unsigned long lastMsg = 0; // Variável que rastreia o tempo, em milissegundos ou outra unidade, na qual ocorreu a última ação ou evento. //
int value = 0; // Armazenar valores inteiros e a inicializa com o valor 0. // 
WiFiClient espClient; // Serve para se conectar a um servidor remoto e enviar ou receber dados pela rede. //
PubSubClient client(espClient);

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
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

    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.print("tente novamente em 5s");
      delay(500);
    }
  }
}

void loop() {
  delay(delayMS);
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (isnan(data.temperature)) {
    Serial.println(F(" ERRO NA LEITURA DA TEMPERATURA"));
  } else {
    Serial.print(F("TEMPERATURA:"));
    Serial.print(data.temperature);
    Serial.println(F("°C"));
    sprintf(msg, "%f", data.temperature);
    client.publish("yuri/temperature", msg);

    if (data.temperature > 22) {
      sprintf(msg, "%i", 1);
      client.publish("yuri/ligadotemp", msg);
    } else {
      sprintf(msg, "%i", 0);
      client.publish("yuri/ligadotemp", msg);
    }
  }
  if (isnan(data.humidity)) {
    Serial.println(F(" ERRO NA LEITURA DA HUMIDADE"));
  } else {
    Serial.print(F("HUMIDADE:"));
    Serial.print(data.humidity);
    Serial.println(F("%"));
    sprintf(msg, "%f", data.humidity);
    client.publish("yuri/humidity", msg);

    if (data.humidity > 50) {
      sprintf(msg, "%i", 1);
      client.publish("yuri/ligar", msg);
    } else {
      sprintf(msg, "%i", 0);
      client.publish("yuri/ligar", msg);
    }
  }
  delay(2000);
  if (data.temperature >= 22) {
    digitalWrite(RELE_PIN, HIGH);
    delay(5000);
  } else {
    digitalWrite(RELE_PIN, LOW);
    delay(5000);
  }

  if (data.temperature >= 22) {
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