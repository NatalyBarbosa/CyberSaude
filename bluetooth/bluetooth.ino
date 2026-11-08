
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string>

// UUID do serviço BLE UART
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_UUID     "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_UUID     "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Pinos dos LEDs
#define LED_VERMELHO 21
#define LED_AMARELO  5
#define LED_VERDE    4

class ReceiveCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) override {
    std::string comando = characteristic->getValue();

    Serial.print("Comando recebido: ");
    Serial.println(comando.c_str());

    if (comando == "1") {
      digitalWrite(LED_VERMELHO, HIGH);
    }
    else if (comando == "2") {
      digitalWrite(LED_VERMELHO, LOW);
    }
    else if (comando == "3") {
      digitalWrite(LED_AMARELO, HIGH);
    }
    else if (comando == "4") {
      digitalWrite(LED_AMARELO, LOW);
    }
    else if (comando == "5") {
      digitalWrite(LED_VERDE, HIGH);
    }
    else if (comando == "6") {
      digitalWrite(LED_VERDE, LOW);
    }
    else if (comando == "7") {
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_AMARELO, HIGH);
      digitalWrite(LED_VERDE, HIGH);
    }
    else if (comando == "0") {
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_AMARELO, LOW);
      digitalWrite(LED_VERDE, LOW);
    }
  }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *server) override {
    Serial.println("Celular conectado!");
  }

  void onDisconnect(BLEServer *server) override {
    Serial.println("Celular desconectado!");
    BLEDevice::startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERDE, LOW);

  // Inicializa Bluetooth BLE
  BLEDevice::init("ESP32-BLE");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);

  BLECharacteristic *rxCharacteristic =
    service->createCharacteristic(
      RX_UUID,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_WRITE_NR
    );

  rxCharacteristic->setCallbacks(new ReceiveCallbacks());

  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->start();

  Serial.println("ESP32 pronto para conectar!");
}

void loop() {
  delay(20);
}