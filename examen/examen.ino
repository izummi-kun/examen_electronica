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

//{*
const int PIRPin= 3;
//}

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



 
void setup() {
// Inicializamos comunicación serie
  Serial.begin(9600);
// Comenzamos el sensor DHT
  dht.begin();
//iniciar pir
  pinMode(PIRPin, INPUT);
 
}
 
void loop() {
 temperatura();
 ldr();
 pir();
}
