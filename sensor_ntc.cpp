#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configurações do Display OLED (128x64 pixels)
#define TELA_LARGURA 128
#define TELA_ALTURA 64
#define PINO_RESET -1 // -1 indica que o display compartilha o reset do ESP32
#define ENDERECO_I2C 0x3C // Endereço padrão da maioria dos displays SSD1306

Adafruit_SSD1306 display(TELA_LARGURA, TELA_ALTURA, &Wire, PINO_RESET);

// Definições do pino e parâmetros do sensor NTC
const int pinoNTC = 15;           
const float resistorFixo = 10000; 
const float ntcNominal = 10000;   
const float tempNominal = 25.0;   
const float beta = 3950;          

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  // Inicializa o display OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, ENDERECO_I2C)) {
    Serial.println(F("Falha: Display OLED nao encontrado. Verifique os fios."));
    for(;;); // Trava o código aqui se der erro
  }

  // Limpa o buffer do display na inicialização
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  int valorADC = analogRead(pinoNTC);

  // Verificação de erro
  if (valorADC == 0 || valorADC >= 4095) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print("Erro no Sensor NTC!");
    display.display();
    delay(2000);
    return;
  }

  // Cálculos de temperatura
  float resistenciaNTC = resistorFixo * (4095.0 / (float)valorADC - 1.0);
  float temperaturaK = resistenciaNTC / ntcNominal;
  temperaturaK = log(temperaturaK);
  temperaturaK /= beta;
  temperaturaK += 1.0 / (tempNominal + 273.15);
  temperaturaK = 1.0 / temperaturaK;
  float temperaturaC = temperaturaK - 273.15;

  // Atualização do Monitor Serial
  Serial.print("Temp: ");
  Serial.print(temperaturaC);
  Serial.println(" C");

  // --- Atualização da Tela OLED ---
  display.clearDisplay(); // Apaga o texto anterior
  
  // Título em texto pequeno
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.print("Temperatura Atual:");

  // Valor em texto grande centralizado
  display.setTextSize(3);
  display.setCursor(10, 25);
  display.print(temperaturaC, 1); // Imprime com 1 casa decimal
  display.print(" C");
  
  // Envia o buffer atualizado para a tela
  display.display();

  delay(1000); 
}