#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> 
#include <stdlib.h> // Required for itoa

// RECEPTOR


Servo motor;


uint8_t peerMacAddress[] = {0xE0, 0x5A, 0x1B, 0x77, 0x1A, 0xCC};  

// Definición mac Esp
// Este comando está escrito en esta notacion poque son bits hexadecimales, no binarios,


typedef struct{
  int velocidad;
} Data;
Data data;


//hhhh

// Esta función es lo que se conoce como callback, algo parecido a lo que vimos del watchdog en automatica
// Basicamente es una función que se declara y se ejecuta cuando pasa x cosa
void onDataReceived(const esp_now_recv_info *info, const uint8_t *dataRcv, int len) {
  // Trigge de que se ha recibido datos
  // conversion uint8-t -> char* -> String -> int
  memcpy(&data,dataRcv,sizeof(data));
  Serial.println("pretty please:" + String(data.velocidad));
  int mensajeInt = data.velocidad;
  int motorSpeed;
// LOGICA COMUICACION ->
  
  if (mensajeInt > 2048) { // Movimiento hacia adelante
    motorSpeed = map(mensajeInt, 2048, 4095, 1500, 2000); // De 1500 a 2000
  } else if (mensajeInt <= 2048) { // Movimiento hacia atrás
    motorSpeed = map(mensajeInt, 2048, 0, 1500, 1000); // De 1500 a 1000
  } else { // Zona muerta (joystick en posición central)
    motorSpeed = 1500; // Neutral
  }
  
  motor.writeMicroseconds(motorSpeed); // Enviar señal PWM al ESC
  delay(20); // Pequeño retardo para estabilizar
  
// <- 


}


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));  
  motor.attach(5);
  motor.write(1500);
  WiFi.mode(WIFI_STA);
  delay(300);
  Serial.println(WiFi.macAddress());


  // Para sabe la Mac de la ESP, ejecuta este comando

  
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
}

