//RECEPTOR FUNCIONAL:

//CONEXIONES:

//MOTOR AL PIN 5
// SERVO AL PIN 4

#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> 
#include <stdlib.h> // Required for itoa (Creo q esta ya se puede quitar pero la dejo x si aca)

// RECEPTOR

Servo direccionServo; 
Servo motor;

int pinServo=4;
int pinMotor=5;


typedef struct{
  int angulo;
    int velocidad; 
} Data;
Data data;

//Callback cuando se reciben datos
void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.println("Ángulo recibido: " + String(data.angulo));
  Serial.println("Velocidad recibida: " + String(data.velocidad));

  //Aplicamos los valores al servo y al motor:
  direccionServo.write(data.angulo);
  motor.writeMicroseconds(data.velocidad);
}


void setup() {

Serial.begin(115200);

//Inicializamos el servo y el motor, asociando los valores con los pines correspondientes:
direccionServo.attach(pinServo);
motor.attach(pinMotor);
//Configuramos el motor al valor neutral:
motor.writeMicroseconds(1500);
delay(300);

  // Inicializamos ESP en modo estación
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Configuramos el callback para recibir datos
  esp_now_register_recv_cb(onDataReceived);
Serial.println("Receptor listo para recibir datos.");

}

void loop() {

}


