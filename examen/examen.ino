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

//{*usuarios para lector de targeta
byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x73, 0x27, 0x75, 0x03} ; //código del usuario 1
byte Usuario2[4]= {0x99, 0x74, 0x05, 0xC3} ; //código del usuario 2
//}

//{*variables y librerias de lector de targeta
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
//}

//led
int led = 5;

//variables de procesos
int m = 0;
int l = 0;
boolean d = 0;

//lobrerias y variables para lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

String acceso = "acceso...";

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

void lcd1(){
  lcd.setCursor(0, 0);
  lcd.print(acceso);  
}


void led1(){
  digitalWrite(led , HIGH);   
  delay(3000);                   
  digitalWrite(led , LOW);    
  delay(1000);                   
  
  }

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
  Serial.println(t);
  lcd.setCursor(0, 1);
  lcd.print(t);
  }

void ldr(){
  lectura = analogRead(0); 
  Serial.println(lectura);
  if(lectura > 500){
    l = 0;

  }else if(lectura < 500){
    l++;
  }
    }

void pir(){
        
    int value= digitalRead(PIRPin);
  
    if (value == HIGH){
        Serial.println("movimiento detectado");
        l++;
    }else{
        Serial.println("sin movimiento");
        l = 0;
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
  lcd.setCursor(7, 1);
  lcd.print(d);
  delay(2000);
  lcd.setCursor(7, 1);
  lcd.print("...");
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
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  pinMode(led, OUTPUT);
  Wire.begin();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("........");
 
}
 
void loop() {
  temperatura();
  m = 0;
  l = 0;
  // lector de targeta
  if ( mfrc522.PICC_IsNewCardPresent()){  
      //Seleccionamos una tarjeta
      if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1)){
                    Serial.println("acceso consedido");
                    lcd.setCursor(0, 0);
                    lcd.print("aceptado");
                    led1();
                    motor();
                    delay(2000);
                    lcd.print("........");
                    
                  }else if(compareArray(ActualUID,Usuario2)){
                    acceso = "acceso concedido";
                    Serial.println("acceso consedido");
                    lcd.print("aceptado");
                    delay(2000);
                    led1();
                    motor();
                    lcd.setCursor(0, 0);
                    lcd.print("........");
                    
                  }else{
                    Serial.println("acceso denegado");
                    acceso = "acceso denegado";
                    led1();
                    lcd.setCursor(0, 0);
                    lcd.print("denegado");
                    delay(5000);
                    lcd.print("........");
                    
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                    mfrc522.PICC_HaltA();
                  }
          
            }
      
  }
    
    pir();
    Serial.println(l);
    delay(3000);
    ldr();
    Serial.println(l);
    delay(3000);

    if(l>=2){
        ultrasonico();
        led1();
      }

}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
