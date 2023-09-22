#include <Wifi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 4
#define DHTTYPE HT22
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
DHT_Unifed dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
unsigned long lasMsg = 0;
int value = 0;




void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.print(F("Sensor DHT22"));
  sensor_t sensor;
  dht.temperature(.getSensor(&sensor));
  Serial.println(F("-----------------------------------------------------------"));
  Serial.println(F("Temperatura"));
  Serial.print(F("Sensor"));
  Serial.println(sensor.name);
  Serial.print(F("Valor maximo: "));
  Serial.println(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print(F("Valor minimo: "));
  Serial.println(sensor.min_value);
  Serial.println(F("°C"));
  Serial.print(F("Resolução: "));
  Serial.print(sensor.resolution);
  Serial.println(F("°C"));
  Serial.println(F("-----------------------------------------------------------"));

  dht.humidity(.getSensor(&sensor));
  Serial.println(F("-----------------------------------------------------------"));
  Serial.println(F("Umidade"));
  Serial.print(F("Sensor"));
  Serial.println(sensor.name);
  Serial.print(F("Valor maximo: "));
  Serial.println(sensor.max_value);
  Serial.println(F("°%"));
  Serial.print(F("Valor minimo: "));
  Serial.println(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolução: "));
  Serial.print(sensor.resolution);
  Serial.println(F("°%"));
  Serial.println(F("-----------------------------------------------------------"));
}

void loop() {
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)){
    Serial.println(F("Erro na leitura da temperatura"));
  }
  else{
    Serial.print(F("Temperatura: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    sprintf(msg,"%f",event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)){
    Serial.println(F("Erro na leitura da umidade"));
  }
  else{
    Serial.print(F("Umidade: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    sprintf(msg,"%f",event.relative_humidity);
  }
}

