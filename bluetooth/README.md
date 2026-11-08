# CyberSaude — ESP32 BLE Controller

Projeto de controle de LEDs utilizando ESP32 e Bluetooth Low Energy (BLE).

## 📱 Aplicativo

Utilize o aplicativo **BLE Controller – Arduino ESP32** para controlar os LEDs através do Bluetooth.

[![BLE Controller](https://img.shields.io/badge/Google_Play-BLE_Controller-green?logo=googleplay)](https://play.google.com/store/apps/details?id=com.circuitmagic.iot)

[📥 Baixar BLE Controller na Play Store](https://play.google.com/store/apps/details?id=com.circuitmagic.iot)

## ⚙️ Configuração do Arduino IDE

### Bibliotecas utilizadas

- `Arduino.h`
- `BLEDevice.h`
- `BLEServer.h`
- `BLEUtils.h`
- `BLE2902.h`

As bibliotecas BLE são disponibilizadas pelo pacote ESP32 da Espressif.

### Configurações da placa

- **Placa:** ESP32 Dev Module
- **Baud Rate:** 115200
- **Bluetooth:** BLE (Bluetooth Low Energy)

## 🔵 Configuração Bluetooth

| Configuração | Valor |
|---|---|
| Nome Bluetooth | ESP32-BLE |
| Serviço BLE | 6E400001-B5A3-F393-E0A9-E50E24DCCA9E |
| RX Characteristic | 6E400002-B5A3-F393-E0A9-E50E24DCCA9E |
| TX Characteristic | 6E400003-B5A3-F393-E0A9-E50E24DCCA9E |

### Configuração no aplicativo

1. Abra o BLE Controller.
2. Procure o dispositivo `ESP32-BLE`.
3. Conecte-se ao dispositivo.
4. Selecione o serviço BLE correspondente.
5. Utilize a característica RX para enviar comandos.
6. Configure os botões personalizados conforme os comandos abaixo.

## 💡 Controle dos LEDs

| LED | GPIO | Ligar | Desligar |
|---|---|---|---|
| 🔴 Vermelho | 21 | `1` | `2` |
| 🟡 Amarelo | 5 | `3` | `4` |
| 🟢 Verde | 4 | `5` | `6` |
| Todos | — | `7` | `0` |

### Comandos disponíveis

- `1` — Liga LED vermelho.
- `2` — Desliga LED vermelho.
- `3` — Liga LED amarelo.
- `4` — Desliga LED amarelo.
- `5` — Liga LED verde.
- `6` — Desliga LED verde.
- `7` — Liga todos os LEDs.
- `0` — Desliga todos os LEDs.

## 🔌 Circuito

| LED | GPIO ESP32 |
|---|---|
| Vermelho | 21 |
| Amarelo | 5 |
| Verde | 4 |

Utilize resistores apropriados para limitar a corrente dos LEDs.

## 🚀 Como utilizar

1. Conecte os LEDs aos GPIOs configurados.
2. Abra o projeto na Arduino IDE.
3. Selecione a placa ESP32.
4. Carregue o código.
5. Abra o BLE Controller.
6. Conecte-se ao `ESP32-BLE`.
7. Configure os botões com os comandos.
8. Controle os LEDs pelo celular.

## 📂 Estrutura do projeto

```text
CyberSaude/
├── README.md
└── CyberSaude.ino
```

## 🛠️ Tecnologias

- Arduino IDE
- ESP32
- Bluetooth Low Energy (BLE)
- C++
- LEDs

## 📄 Licença

Projeto desenvolvido para fins educacionais e experimentais.