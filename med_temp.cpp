//Bibliotecas
#include <Wire.h>  //Protocolo de comunicação
#include <Adafruit_VL53L0X.h>  //Sensor de distância
#include <Adafruit_MLX90614.h>  //Sensor de temperatura
#include <Adafruit_GFX.h>  //Núcleo gráfico da tela
#include <Adafruit_SSD1306.h>  //Controle da tela

//Objeto de controle da tela
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Objetos de controle dos sensores
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Inicialização
void setup() {

  mlx.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Inicializa a tela usando o endereço I²C padrão 0x3C
  Serial.begin(9600);

}

void loop() {
  //Objeto para guardar a distância
  VL53L0X_RangingMeasurementData_t medida;
  
  //Efetua a leitura da distância
  lox.rangingTest(&medida, false);

  //Verificação de distância
  //RangeStatus != 4 significa que o sensor conseguiu detetar um alvo válido
  if (medida.RangeStatus != 4 && medida.RangeMilliMeter >= 30 && medida.RangeMilliMeter <= 50) {
    
    display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15, 10);  //Posição do texto na tela
    display.println("Lendo...");
    display.display();

    //Aguarda 3 segundos para estabilização
    delay(3000);

    //Leitura de distância para confirmação
    lox.rangingTest(&medida, false);

    if (medida.RangeStatus != 4 && medida.RangeMilliMeter >= 30 && medida.RangeMilliMeter <= 50) {
      
      float tempC = mlx.readObjectTempC(); //Salva a temperatura em uma variável para não precisar ler várias vezes
      
      Serial.print("Temperatura: ");
      Serial.println(tempC);

      //Estado de febre
      if (tempC >= 37.8) {
        
        //Alerta sonoro CRÍTICO: 3 bipes rápidos e graves
        for (int i = 0; i < 3; i++) {
          tone(8, 500, 150);  //(pino, frequência, duração)
          delay(250);        
        }

        display.clearDisplay();
        display.invertDisplay(true); // Inverte a tela (fundo branco, letras pretas)
        display.setCursor(15, 0);
        display.println("CRITICO!");
        
        //Exibe o valor da temperatura
        display.setCursor(25, 18);
        display.print(tempC, 1);
        display.print(" C");
        display.display();

        delay(3000); //Mantém o alerta na tela por 3 segundos
        display.invertDisplay(false); //Desfaz a inversão da tela

      } 
      //Estado de hipotermia
      else if (tempC <= 35) {
        
        //Alerta sonoro CRÍTICO: 3 bipes rápidos e graves
        for (int i = 0; i < 3; i++) {
          tone(8, 500, 150); 
          delay(250);        
        }

        display.clearDisplay();
        display.invertDisplay(true);
        display.setCursor(15, 0); 
        display.println("CRITICO!");
        
        // Exibe o valor da temperatura
        display.setCursor(25, 18);
        display.print(tempC, 1);
        display.print(" C");
        display.display();

        delay(3000); 
        display.invertDisplay(false); 

      } 
      //Estado normal
      else {

        //Alerta sonoro normal
        tone(8, 1200, 1000); // Toca o buzzer por 1 segundo
        
        display.clearDisplay();
        display.invertDisplay(false);
        display.setCursor(8, 0);
        display.println("Body Temp:");
        
        display.setCursor(25, 18);
        display.print(tempC, 1);
        display.print(" C");
        display.display();
        delay(3000); 
      }
      
    } else {
      //Caso a pessoa tenha se movido antes do tempo de leitura
      display.clearDisplay();
      display.setCursor(5, 10);
      display.println("Moveu!");
      display.display();
      delay(2000);
    }
  }

  //Caso não encontre nada próximo o suficiente
  else {
    delay(500);

    display.clearDisplay();
    display.invertDisplay(false);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("BUSCANDO...");
    display.display();

  }

}