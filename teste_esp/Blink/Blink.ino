void setup() // configurações
{
  pinMode(2, OUTPUT); // ação
}

void loop() // laço infinito
{
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}