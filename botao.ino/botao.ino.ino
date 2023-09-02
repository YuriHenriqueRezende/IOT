#include "config.h"

#define BUTTON_PIN 4 // pin 5

// estados dos botões
bool current = false;
bool last = false;

AdafruitIO_Feed *digital = io.feed("digital");


void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  while(io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}

void loop() 
{
 io.run();

 if(digitalRead(BUTTON_PIN) == HIGH)
    current = true;
  else
    current = false;

  if(current == last)
    return;

  Serial.print("sending button -> ");
  Serial.println(current);
  Serial.println(current);

  last = current;
}
