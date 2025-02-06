#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> 
#include <stdlib.h> // Required for itoa


// EMISOR

Servo motor;

// Definición mac Esp
// Este comando está escrito en esta notacion poque son bits hexadecimales, no binarios,

uint8_t peerMacAddress[] = {0xE0, 0x5A, 0x1B, 0x77, 0x3B, 0xC4};  // 1

// Esta función es lo que se conoce como callback, algo parecido a lo que vimos del watchdog en automatica
// Basicamente es una función que se declara y se ejecuta cuando pasa x cosa
void onDataReceived(const esp_now_recv_info *info, const uint8_t *data, int len) {
  int a = 1;
}

void setup() {

  Serial.begin(115200);
  motor.attach(5);
  // Para sabe la Mac de la ESP, ejecuta este comando

  WiFi.mode(WIFI_STA);
  delay(50);
  Serial.println(WiFi.macAddress());

  // se corre el comando y si no hay problemas se tira
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }
  else{
    Serial.println("ESP-NOW started");
  }

  // En este punto se declara el callback que se activará cuando se reciban mensajes
  // Es un metodo que encontré en la api peo debería funcionar
  esp_now_register_recv_cb(onDataReceived);

  // Estos son los parámetos que hay que determinar y solo se utiliza la mac el resto no se toca
  esp_now_peer_info_t peerInfo;
  // esto es una declaración de variable fancy xq es en hex y hay q hacerlo asi
  memcpy(peerInfo.peer_addr, peerMacAddress, 6);
  peerInfo.channel = 0;  // Use current channel
  peerInfo.encrypt = false;  // Disable encryption

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Setup finalizado");
}

int pin_analog = 34;

void loop() {
  // creo q se puede declarar como String q es más comodo pero por ahora tiramos con char*
  int medicionPote = analogRead(pin_analog);
  Serial.println("medicion Pote:" + String(medicionPote));
  const char* message1 = String(analogRead(pin_analog)).c_str();
  Serial.println("message1:" + String(message1));
  const char* message2 = "prueba";
  Serial.println("message1:" + String(message1));
  esp_now_send(peerMacAddress, (uint8_t *)message1, strlen(message1) + 1);
  esp_now_send(peerMacAddress, (uint8_t *)message2, strlen(message1) + 1);
  Serial.println("Mensaje enviado " + String(message1));
}


