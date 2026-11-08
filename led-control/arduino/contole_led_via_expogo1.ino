#include <WiFi.h>
#include <WebServer.h>

// Dados da rede Wi-Fi
const char* ssid = "Nome Do Wifi";
const char* password = "Senha";

// Criando o servidor HTTP na porta 80
WebServer server(80);

// Pinos dos LEDs
const int ledVermelho = 21;
const int ledAmarelo = 5;
const int ledVerde = 4;

// Rota principal
void rota() {
  server.send(200, "text/plain", "Servidor funcionando!");
}

// LED vermelho
void ligarVermelho() {
  digitalWrite(ledVermelho, HIGH);
  server.send(200, "text/plain", "LED vermelho ligado!");
}

void desligarVermelho() {
  digitalWrite(ledVermelho, LOW);
  server.send(200, "text/plain", "LED vermelho desligado!");
}

// LED amarelo
void ligarAmarelo() {
  digitalWrite(ledAmarelo, HIGH);
  server.send(200, "text/plain", "LED amarelo ligado!");
}

void desligarAmarelo() {
  digitalWrite(ledAmarelo, LOW);
  server.send(200, "text/plain", "LED amarelo desligado!");
}

// LED verde
void ligarVerde() {
  digitalWrite(ledVerde, HIGH);
  server.send(200, "text/plain", "LED verde ligado!");
}

void desligarVerde() {
  digitalWrite(ledVerde, LOW);
  server.send(200, "text/plain", "LED verde desligado!");
}

void setup() {
  Serial.begin(115200);

  // Configurando os pinos como saída
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  // Começando com todos os LEDs desligados
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVerde, LOW);

  // Conectando ao Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");

  // Exibindo o IP do ESP32
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  // Configurando as rotas HTTP
  server.on("/", rota);

  server.on("/vermelho/ligar", ligarVermelho);
  server.on("/vermelho/desligar", desligarVermelho);

  server.on("/amarelo/ligar", ligarAmarelo);
  server.on("/amarelo/desligar", desligarAmarelo);

  server.on("/verde/ligar", ligarVerde);
  server.on("/verde/desligar", desligarVerde);

  // Resposta para rotas inexistentes
  server.onNotFound([]() {
    server.send(404, "text/plain", "Rota nao encontrada!");
  });

  // Iniciando o servidor
  server.begin();

  Serial.println("Servidor HTTP iniciado!");
}

void loop() {
  // Verificando se chegaram requisições HTTP
  server.handleClient();
}
