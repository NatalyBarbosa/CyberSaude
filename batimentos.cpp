#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "heartRate.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

// --- BITMAPS ---
// Coração 16x16 para o flash final do boot
const unsigned char PROGMEM icone_coracao_16x16[] = {
  0x00, 0x00, 0x0e, 0x70, 0x1f, 0xf8, 0x3f, 0xfc, 
  0x7f, 0xfe, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 
  0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80
};

// Coração 8x8 para o cabeçalho das métricas
const unsigned char PROGMEM icone_coracao_8x8[] = {
  0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00
};

// --- MÁQUINA DE ESTADOS DO DISPLAY ---
enum EstadoTela {
  TELA_SPLASH,
  TELA_METRICAS,
  TELA_GRAFICO
};

EstadoTela telaAtual = TELA_SPLASH;
unsigned long tempoUltimaTroca = 0;
const unsigned long INTERVALO_SPLASH = 4000;     // 4 segundos para a animação cyberpunk completa
const unsigned long INTERVALO_ALTERNANCIA = 4000; // 4 segundos entre métricas e gráfico

// Temporizadores de display e taxa de scroll da onda
unsigned long tempoUltimoDisplay = 0;
const unsigned long INTERVALO_DISPLAY = 35;

unsigned long tempoUltimoPontoOnda = 0;
const unsigned long INTERVALO_PONTO_ONDA = 25;

// --- BUFFER DA ONDA (PPG) ---
int bufferOnda[SCREEN_WIDTH];
float filtroDC_IR = 0;
float sinalFiltrado = 0;
float picoMax = 200.0;
float picoMin = -200.0;

// --- FILTRO PARA SPO2 ---
float dcRed = 0, dcIR = 0;
float acRed = 0, acIR = 0;
float maxACRed = 0, minACRed = 0;
float maxACIR = 0, minACIR = 0;
int contadorAmostras = 0;

// --- VARIÁVEIS CARDÍACAS ---
const byte TAM_TAXA = 4;
byte taxas[TAM_TAXA];
byte taxaSpot = 0;
long ultimaBatida = 0;

int mediaBPM = 0;
int spo2Calculado = 0;
bool dedoPresente = false;
int nivelBateria = 85;

// Texto do terminal
const char textoBoot[] = "> CyberSaude";
const int totalCharsTexto = 12;

// --- FUNÇÃO AUXILIAR: Desenha o ícone de bateria ---
void desenharBateria(int x, int y, int porcentagem) {
  porcentagem = constrain(porcentagem, 0, 100);
  display.drawRect(x, y, 16, 8, SSD1306_WHITE);
  display.drawFastVLine(x + 16, y + 2, 4, SSD1306_WHITE);
  int preenchimento = map(porcentagem, 0, 100, 0, 12);
  if (preenchimento > 0) {
    display.fillRect(x + 2, y + 2, preenchimento, 4, SSD1306_WHITE);
  }
}

// --- TELA 1: Splash Cyberpunk (Typewriter + Boot + Flash) ---
void renderizarTelaSplash(unsigned long tempoDecorrido) {
  display.clearDisplay();

  // Linhas decorativas do sistema terminal no topo
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IEEE SMCS/CS UFRB");
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE);

  // 1. Fase de Digitação (0 a 1600 ms)
  int charsParaExibir = 0;
  if (tempoDecorrido < 1600) {
    charsParaExibir = map(tempoDecorrido, 0, 1500, 0, totalCharsTexto);
    charsParaExibir = constrain(charsParaExibir, 0, totalCharsTexto);
  } else {
    charsParaExibir = totalCharsTexto;
  }

  display.setCursor(8, 16);
  for (int i = 0; i < charsParaExibir; i++) {
    display.print(textoBoot[i]);
  }

  // Cursor piscante estilo prompt terminal
  if (tempoDecorrido < 2800 && (tempoDecorrido / 150) % 2 == 0) {
    display.print("_");
  }

  // 2. Fase da Barra de Carregamento (600 a 2800 ms)
  if (tempoDecorrido >= 600 && tempoDecorrido < 2900) {
    int progresso = map(tempoDecorrido, 600, 2700, 0, 100);
    progresso = constrain(progresso, 0, 100);

    // Texto de status de carregamento
    display.setCursor(8, 32);
    display.print("INIT CORE: ");
    display.print(progresso);
    display.print("%");

    // Borda externa da barra de progresso
    display.drawRect(8, 44, 112, 10, SSD1306_WHITE);

    // Segmentos pixelados internos
    int larguraPreenchimento = map(progresso, 0, 100, 0, 108);
    for (int x = 10; x < 10 + larguraPreenchimento; x += 3) {
      display.drawFastVLine(x, 46, 6, SSD1306_WHITE);
    }
  }

  // 3. Fase Final: Coração Pisca 2 Vezes (2900 a 4000 ms)
  if (tempoDecorrido >= 2900) {
    display.setCursor(20, 28);
    display.print("SYSTEM READY");

    // Ciclo de piscar o coração duas vezes
    unsigned long tempoFlash = tempoDecorrido - 2900;
    bool visivel = false;

    // Pisca 1: 0 a 220ms | Pisca 2: 440 a 660ms
    if ((tempoFlash >= 0 && tempoFlash < 220) || (tempoFlash >= 440 && tempoFlash < 660) || tempoFlash >= 850) {
      visivel = true;
    }

    if (visivel) {
      display.drawBitmap(56, 42, icone_coracao_16x16, 16, 16, SSD1306_WHITE);
    }
  }

  display.display();
}

// --- TELA 2: Frequência e Oxigenação ---
void renderizarTelaMetricas() {
  display.clearDisplay();
  display.drawBitmap(0, 0, icone_coracao_8x8, 8, 8, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(12, 0);
  display.print("Freq.");
  desenharBateria(108, 0, nivelBateria);

  if (!dedoPresente) {
    display.setCursor(10, 28);
    display.setTextSize(1);
    display.print("Posicione o dedo...");
  } else {
    display.setTextSize(3);
    display.setCursor(10, 12);
    if (mediaBPM >= 40 && mediaBPM <= 200) {
      display.print(mediaBPM);
    } else {
      display.print("--");
    }
    display.setTextSize(1);
    display.setCursor(65, 26);
    display.print("BPM");

    display.drawLine(0, 42, 128, 42, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("SpO2: ");
    display.setTextSize(2);
    display.setCursor(42, 46);
    if (spo2Calculado > 0) {
      display.print(spo2Calculado);
      display.print("%");
    } else {
      display.print("--%");
    }
  }
  display.display();
}

// --- TELA 3: Forma de Onda (Osciloscópio) ---
void renderizarTelaGrafico() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Pulso: ");
  if (dedoPresente && mediaBPM >= 40 && mediaBPM <= 200) {
    display.print(mediaBPM);
    display.print(" BPM");
  } else {
    display.print("-- BPM");
  }
  desenharBateria(108, 0, nivelBateria);
  display.drawLine(0, 11, 128, 11, SSD1306_WHITE);

  if (dedoPresente) {
    for (int x = 0; x < SCREEN_WIDTH - 1; x++) {
      display.drawLine(x, bufferOnda[x], x + 1, bufferOnda[x + 1], SSD1306_WHITE);
    }
  } else {
    display.drawLine(0, 38, 128, 38, SSD1306_WHITE);
  }
  display.display();
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha ao iniciar OLED");
    for (;;);
  }

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 nao encontrado!");
    for (;;);
  }

  byte ledBrightness = 60;   
  byte sampleAverage = 4;
  byte ledMode = 2;          
  int sampleRate = 200;      
  int pulseWidth = 411;      
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  for (int i = 0; i < SCREEN_WIDTH; i++) {
    bufferOnda[i] = 38;
  }

  tempoUltimaTroca = millis();
}

// --- LOOP PRINCIPAL ---
void loop() {
  unsigned long agora = millis();

  // 1. Amostragem contínua do sensor
  long valorIR = particleSensor.getIR();
  long valorRed = particleSensor.getRed();

  if (valorIR < 50000) {
    dedoPresente = false;
    mediaBPM = 0;
    spo2Calculado = 0;
    filtroDC_IR = 0;
    dcRed = 0;
    dcIR = 0;
    picoMax = 200.0;
    picoMin = -200.0;
  } else {
    dedoPresente = true;

    // Detecção de batimento cardíaco
    if (checkForBeat(valorIR)) {
      long delta = agora - ultimaBatida;
      ultimaBatida = agora;

      if (delta > 270 && delta < 1500) {
        int bpmInstantaneo = 60000 / delta;
        taxas[taxaSpot++] = (byte)bpmInstantaneo;
        taxaSpot %= TAM_TAXA;

        int soma = 0, contagem = 0;
        for (byte x = 0; x < TAM_TAXA; x++) {
          if (taxas[x] > 0) {
            soma += taxas[x];
            contagem++;
          }
        }
        if (contagem > 0) mediaBPM = soma / contagem;
      }
    }

    // Filtro Passa-Alta para isolar AC do IR
    if (filtroDC_IR == 0) filtroDC_IR = valorIR;
    filtroDC_IR = (filtroDC_IR * 0.92) + ((float)valorIR * 0.08);
    sinalFiltrado = (float)valorIR - filtroDC_IR;

    // Atualização cadenciada da onda a 40 Hz
    if (agora - tempoUltimoPontoOnda >= INTERVALO_PONTO_ONDA) {
      tempoUltimoPontoOnda = agora;

      if (sinalFiltrado > picoMax) picoMax = sinalFiltrado;
      if (sinalFiltrado < picoMin) picoMin = sinalFiltrado;

      picoMax *= 0.985;
      picoMin *= 0.985;

      if (picoMax < 60) picoMax = 60;
      if (picoMin > -60) picoMin = -60;

      int yPixel = map((long)sinalFiltrado, (long)picoMin, (long)picoMax, 58, 16);
      yPixel = constrain(yPixel, 14, 60);

      for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
        bufferOnda[i] = bufferOnda[i + 1];
      }
      bufferOnda[SCREEN_WIDTH - 1] = yPixel;
    }

    // Filtro de oximetria AC/DC
    dcRed = (dcRed * 0.95) + ((float)valorRed * 0.05);
    dcIR  = (dcIR  * 0.95) + ((float)valorIR  * 0.05);

    acRed = (float)valorRed - dcRed;
    acIR  = (float)valorIR  - dcIR;

    if (acRed > maxACRed) maxACRed = acRed;
    if (acRed < minACRed) minACRed = acRed;
    if (acIR > maxACIR)   maxACIR  = acIR;
    if (acIR < minACIR)   minACIR  = acIR;

    contadorAmostras++;
    if (contadorAmostras > 150) {
      float vppRed = maxACRed - minACRed;
      float vppIR  = maxACIR  - minACIR;

      if (vppIR > 0 && dcIR > 0 && dcRed > 0) {
        float razaoR = (vppRed / dcRed) / (vppIR / dcIR);
        int spo2Real = (int)(110.0 - (20.0 * razaoR));
        spo2Calculado = constrain(spo2Real, 85, 99);
      }
      maxACRed = -9999; minACRed = 9999;
      maxACIR  = -9999; minACIR  = 9999;
      contadorAmostras = 0;
    }
  }

  // 2. Renderização periódica do display
  if (agora - tempoUltimoDisplay >= INTERVALO_DISPLAY) {
    tempoUltimoDisplay = agora;

    switch (telaAtual) {
      case TELA_SPLASH:
        renderizarTelaSplash(agora);
        if (agora - tempoUltimaTroca >= INTERVALO_SPLASH) {
          telaAtual = TELA_METRICAS;
          tempoUltimaTroca = agora;
        }
        break;

      case TELA_METRICAS:
        renderizarTelaMetricas();
        if (agora - tempoUltimaTroca >= INTERVALO_ALTERNANCIA) {
          telaAtual = TELA_GRAFICO;
          tempoUltimaTroca = agora;
        }
        break;

      case TELA_GRAFICO:
        renderizarTelaGrafico();
        if (agora - tempoUltimaTroca >= INTERVALO_ALTERNANCIA) {
          telaAtual = TELA_METRICAS;
          tempoUltimaTroca = agora;
        }
        break;
    }
  }
}