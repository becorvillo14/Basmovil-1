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
  // Trigge de que se ha recibido datos
  char buffer[50];
  sprintf(buffer, "%u", *data);
  String mensaje = String(buffer);
  Serial.print("Data received: " + mensaje);
}


void setup() {
  delay(2000);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.begin(115200);
  randomSeed(analogRead(0));  
  motor.attach(5);
  
  Serial.println(WiFi.macAddress());

  // Para sabe la Mac de la ESP, ejecuta este comando
  Serial.println("saadas");
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println(WiFi.macAddress());

  // La esp puede estar en varios modos basicamente puede ser un router o un intermediario
  // de cosas de tcp pero lo ponemos y te olvidas


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

void loop() {
  // creo q se puede declarar como String q es más comodo pero por ahora tiramos con char*
  
  const char* message1 = String(1600).c_str();
  esp_now_send(peerMacAddress, (uint8_t *)message1, strlen(message1) + 1);
  Serial.println("Mensaje enviado " + String(message1));
  delay(100);  
    const char* message2 = String(1700).c_str();
  esp_now_send(peerMacAddress, (uint8_t *)message2, strlen(message2) + 1);
  Serial.println("Mensaje enviado " + String(message2));
  delay(100);
    const char* message3 = String(1800).c_str();
  esp_now_send(peerMacAddress, (uint8_t *)message3, strlen(message3) + 1);
  Serial.println("Mensaje enviado " + String(message3));
  delay(100);
}
