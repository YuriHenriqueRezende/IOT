#include <PubSubClient.h> // Essa biblioteca permite que dispositivos baseados em Arduino se conectem a sistemas de mensagens e publiquem/assinem tópicos em um sistema de mensagens. //
#include <WiFi.h> // Essa biblioteca do Arduino que permite que dispositivos baseados em Arduino se conectem a redes Wi-Fi, possibilitando a comunicação com a Internet e outros dispositivos em uma rede local sem fio. //
#include "DHTesp.h" // Essa biblioteca específica para trabalhar com sensores de temperatura e umidade da série DHT (como DHT11, DHT22, etc.) //
const char* ssid = "yuri";  // E o nome da minha rede WIFI que eu quero conectar ao ESP //
const char* password = "123456789"; // A senha da minha rede wifi que eu quero conectar ao ESP //
const char* mqtt_server = "test.mosquitto.org";  // Endereço do servidor MQTT real ao meu ESP. //
int RELE_PIN = 5;  // Estou definindo a porta do meu rele //
const int DHT_PIN = 15;  // Estou definindo a porta do meu sensor de temperatura //
#define MSG_BUFFER_SIZE (50)  // Essa linha especifica o tamanho de um buffer ou array de mensagens, garantindo que o tamanho seja consistente em todo o programa. //
char msg[MSG_BUFFER_SIZE]; // Cria um array de caracteres chamado msg com um tamanho específico, determinado pela constante MSG_BUFFER_SIZE que foi definida anteriormente no código com #define MSG_BUFFER_SIZE (50). //
uint32_t delayMS; // Variável para controlar um atraso ou temporização em milissegundos. //
unsigned long lastMsg = 0; // Variável que rastreia o tempo, em milissegundos ou outra unidade, na qual ocorreu a última ação ou evento. //
int value = 0; // Armazenar valores inteiros e a inicializa com o valor 0. // 
WiFiClient espClient; // Serve para se conectar a um servidor remoto e enviar ou receber dados pela rede. //
PubSubClient client(espClient); //  Comunica com um serviço de mensagens ou um sistema de publicação e assinatura, como o protocolo MQTT (Message Queuing Telemetry Transport) //
DHTesp dhtSensor; // Interage com um sensor de temperatura e umidade do tipo DHT //

void setup() { // É uma função que é executada automaticamente uma única vez quando o microcontrolador ESP é inicializado //
  Serial.begin(115200); // Configura a taxa de transmissão (baud rate) da porta serial, permitindo que você comunique-se com o ESP por meio da porta serial //
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22); // Configura e inicializa um sensor de temperatura e umidade DHT22 conectado ao ESP //
  setup_WiFi(); // Chamo minha função setup_Wifi para funcionar 1 vez se conectada //
  client.setServer(mqtt_server, 1883); // Estabelece uma conexão com um servidor MQTT //
  pinMode(RELE_PIN, OUTPUT); // Estou configurando minha porta digital como saida, ou seja meu relé, ele vai mandar ligado ou desligado //
} // Finaliza minha função

void setup_WiFi() { // Essa função vai configurar meu ESP a conectar na rede WIFI //
  delay(10); // Atraso de 10 milisegundos para seguir o paradigma da programação //
  Serial.println(""); //  Imprime uma linha em branco (ou seja, uma linha vazia) na porta serial //
  Serial.print("conectando com"); // Simplesmente imprime a string "conectando com" na porta serial sem adicionar uma nova linha no final da mensagem. //
  Serial.println(ssid); // Imprime o nome da minha rede // 
  WiFi.begin(ssid, password); // Inicia a conexão Wi-Fi //
  while (WiFi.status() != WL_CONNECTED) { // Esse while vai rodar infinitamente até que a conexão Wi-Fi seja estabelecida // 
    delay(500);
    Serial.print(".");
  } // Finaliza meu loop while
  Serial.println(""); //  Imprime uma linha em branco (ou seja, uma linha vazia) na porta serial //
  Serial.print("WiFi conectado"); // Simplesmente imprime a string "WiFi conectado" na porta serial sem adicionar uma nova linha no final da mensagem. //
  Serial.println("IP: "); // Simplesmente imprime a string "IP: " na porta serial // 
  Serial.println(WiFi.localIP()); // Imprime na frente do "IP:" o IP que se conecto, e printa à porta serial sem adicionar uma nova linha no final da mensagem //
} // Finaliza minha função //

void reconnect() { // Essa função vai conectar como meu protocolo MQTT // 
  while (!client.connected()) { // Esse while é usado para aguardar até que uma conexão MQTT seja
// estabelecida com sucesso antes de continuar com a execução do programa. Ele também lida com falhas na conexão, tentando novamente após um curto atraso //
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
} // Finaliza minha função //

void loop() { // Essa função executada continuamente após a inicialização do ESP e é responsável por conter o código principal do programa. //
  delay(delayMS); // Criar um atraso (ou pausa) na execução. //
  TempAndHumidity data = dhtSensor.getTempAndHumidity(); //  Essa linha de código está lendo os dados de temperatura e umidade do sensor DHT e armazenando esses valores na variável data. //
  if (isnan(data.temperature)) { // É uma verificação de erro que testa se o valor da temperatura lido a partir do sensor DHT não é um número válido (NaN, "Not-a-Number"). //
    Serial.println(F(" ERRO NA LEITURA DA TEMPERATURA"));
  } else { // Esse else lida com as leituras de temperatura do sensor DHT, exibe a temperatura na porta serial, //
// e publica em um tópico MQTT e toma decisões com base na temperatura em relação a um limite de 22 graus Celsius, //
// que são representadas como "1" (ligado) ou "0" (desligado) no tópico MQTT "yuri/ligadotemp" //
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

  if (isnan(data.humidity)) { // É uma verificação de erro que testa se o valor da humidade lido a partir do sensor DHT não é um número válido (NaN, "Not-a-Number"). //
    Serial.println(F(" ERRO NA LEITURA DA HUMIDADE"));
  } else {// Esse else lida com as leituras de humidade do sensor DHT, exibe a humidade na porta serial, //
// e publica em um tópico MQTT e toma decisões com base na humidade em relação a um limite de 22 graus Celsius, //
// que são representadas como "1" (ligado) ou "0" (desligado) no tópico MQTT "yuri/ligar" //
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
  delay(2000); // Atraso de 2 segundos para seguir o paradigma da programação //

  if (data.temperature >= 22) { // Esse if controla o Relé (led) com base na leitura da temperatura. Se a temperatura for maior ou igual a 22 graus Celsius, //
// o dispositivo é ativado em 5 segundos, e se a temperatura estiver abaixo desse limite, o dispositivo é desativado em 5 segundos. //
    digitalWrite(RELE_PIN, HIGH);
    delay(5000);
  } else {
    digitalWrite(RELE_PIN, LOW);
    delay(5000);
  }

  if (!client.connected()) { // If serve para garantir que o cliente MQTT esteja sempre conectado ao servidor. //
//   Se a conexão MQTT for perdida por algum motivo (por exemplo, devido a uma interrupção na rede, que de energia), //
//   o código verifica essa condição e chama a função reconnect() para tentar reconectar automaticamente o cliente. //
//   Isso é útil para manter a comunicação MQTT estável e confiável, mesmo em situações de falha temporária na rede. //
    reconnect();
  }
  client.loop(); // Permiti que o cliente MQTT mantenha uma comunicação ativa e confiável com o servidor MQTT. //
}