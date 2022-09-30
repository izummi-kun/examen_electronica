//{*librerias y variables para el sensor de temperatura
// Incluimos librería
#include <DHT.h>
 // Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
//*}

//{* variables de ldr
int lectura = 0;
//}

//{* variables para pir
const int PIRPin= 3;
//}

//{*librerias y variables para servo Motor
#include <Servo.h>
Servo servoMotor;
//}

//{*variables del sensor ultrasonico
const int Trigger = 7;   //Pin digital 2 para el Trigger del sensor
const int Echo = 6;   //Pin digital 3 para el Echo del sensor
//}

//variables y librerias de lector de targeta
#include <SPI.h>
#include <MFRC522.h>

void temperatura(){
  // Esperamos 5 segundos entre medidas
  delay(5000);
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
  //impresion de datos
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("\n"); 
  }

void ldr(){
  lectura = analogRead(0); 
  Serial.println(lectura);
  delay(500);
    }

void pir(){
  int value= digitalRead(PIRPin);
  
  if (value == HIGH){
    Serial.println("movimiento detectado");
    delay(3000);
    }else{
    Serial.println("sin movimiento");
    delay(3000);
    }
  
  }

void motor(){
    // Desplazamos a la posición 0º
  servoMotor.write(0);
  // Esperamos 1 segundo
  delay(1000);
  
  // Desplazamos a la posición 90º
  servoMotor.write(90);
  // Esperamos 1 segundo
  delay(1000);
  
  // Desplazamos a la posición 180º
  servoMotor.write(180);
  // Esperamos 1 segundo
  delay(1000);
  }

void ultrasonico(){
  
  long t; 
  long d; 

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);  
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); 
  d = t/59;             
  
  Serial.print("Distancia: ");
  Serial.print(d);      
  Serial.print("cm");
  Serial.println();
  delay(800);  
  }
 
void setup() {
// Inicializamos comunicación serie
  Serial.begin(9600);
// Comenzamos el sensor DHT
  dht.begin();
//iniciar pir
  pinMode(PIRPin, INPUT);
  servoMotor.attach(8);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
 
}
 
void loop() {

}
