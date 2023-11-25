/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL2Z3saITou"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "YWotqChDWGi8ONWQG0IcFDRUqOzuB_ej"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"
const int DHT_PIN = 15;
DHTesp dhtSensor;
int RELE_PIN = 5;  // Estou definindo a porta do meu rele //

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Yuri";
char pass[] = "12345678";



// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}



void setup()
{
  // Debug console
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(RELE_PIN, OUTPUT); // Estou configurando minha porta digital como saida, ou seja meu relé, ele vai mandar ligado ou desligado //

  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
}

void loop()
{
  delay(2000);  // Aguarda 2 segundos entre as leituras
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  Blynk.virtualWrite(V5, data.temperature); // V5 é o número do pino virtual no qual o widget do Blynk está configurado
  Blynk.virtualWrite(V6, data.humidity);     // V6 é o número do pino virtual no qual o widget do Blynk está configurado
  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.print(" °C | Umidade: ");
  Serial.print(data.humidity);
  Serial.println(" %");
  
  if (data.temperature >= 22) { // Esse if controla o Relé (led) com base na leitura da temperatura. Se a temperatura for maior ou igual a 22 graus Celsius, //
// o dispositivo é ativado em 5 segundos, e se a temperatura estiver abaixo desse limite, o dispositivo é desativado em 5 segundos. //
    digitalWrite(RELE_PIN, HIGH);
    delay(5000);
  } else {
    digitalWrite(RELE_PIN, LOW);
    delay(5000);
  }

}
