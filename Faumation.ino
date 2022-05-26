#include "DHT.h" // Library Sensor Suhu DHT
#define DHTTYPE DHT11 // Menggunakan Sensor Suhu tipe DHT11
#define DHTPin 8 // Digital Pin Arduino yang dihubungkan ke Pin Data dari Sensor Suhu DHT11

int pinTrigger = 12; // Pin Trigger Sensor Jarak Ultrasonic
int pinEcho = 11; // Pin Echo Sensor Jarak Ultrasonic
long jarak, cm; // Jarak Ketinggian Batas Atas dan Batas Bawah Tempat Pengisian Air
int pinMotor = 13; // Motor DC
int pinTombolMotor = 5;
bool statusMotor = false; // Jika false maka saluran air mati, tapi jika true maka saluran air menyala
bool statusTombolMotor = false; // Jika ditekan maka Motor menyala (true), tapi jika ditekan lagi maka Motor mati (false)
int pinPemanas = 10; // Jika LED Red menyala maka pemanas air sedang menyala, jika LED Red mati maka pemanas air sedang mati
int pinTombolPemanas = 9;
bool statusPemanas = false; // Jika false maka pemanas air mati, tapi jika true maka pemanas air menyala
bool statusTombolPemanas = false; // Jika ditekan maka Pemanas menyala (true), tapi jika ditekan lagi maka Pemanas mati (false)
// long int waktu = 0;

DHT dht(DHTPin, DHTTYPE); // Object Sensor Suhu DHT11

//? put your setup code here, to run once:
void setup() {
  pinMode(pinTombolMotor, INPUT_PULLUP);
  pinMode(pinTombolPemanas, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  pinMode(pinPemanas, OUTPUT);
  Serial.begin(9600); // Baud Rate Serial Monitor
  dht.begin();
}

//? put your main code here, to run repeatedly:
void loop() {
  // long int t1 = millis(); // Waktu yang diukur
  // waktu = waktu + t1;
  //Serial.println(waktu);
  int sisa;
  
  //* Start of Water Filling (Manual)
  if (digitalRead(pinTombolMotor) == LOW) {
    statusMotor = !statusMotor; // Mengubah value bool statusMotor
    statusTombolMotor = !statusTombolMotor; // Mengubah value bool statusTombolMotor
    delay(20);
  }
  digitalWrite(pinMotor, statusMotor);
  delay(50); 
  //* End of Water Filling (Manual) 

  pinMode(pinTrigger, OUTPUT);
  digitalWrite(pinTrigger, LOW); // Melakukan Trigger dengan Pulse
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);

  pinMode(pinEcho, INPUT);
  jarak = pulseIn(pinEcho, HIGH);
  cm = jarakCentimeter(jarak);
  sisa = map(cm, 4, 1122, 100, 0); // Konversi Potentiometer 0 % berarti sisa 100 % dan sebaliknya
  Serial.println("");
  Serial.print(F("Sisa Air: "));
  Serial.print(sisa);
  Serial.println(" %");  
  
  //* Start of Water Filling (Automation)  
  if (sisa <= 10) {
    Serial.println("Saluran Air Menyala.");    
    statusMotor = true;
    delay(250);
  } 
  
  if (sisa > 90 && statusTombolMotor == true) {
    Serial.println("Saluran Air Mati.");   
    statusMotor = false; 
    statusTombolMotor = false;
    delay(250);
  }
  //* End of Water Filling (Automation)

  //* Start of Water Heater (Manual)
  if (digitalRead(pinTombolPemanas) == LOW) {
    statusPemanas = !statusPemanas; // Mengubah value bool statusPemanas
    statusTombolPemanas = !statusTombolPemanas; // Mengubah value bool statusTombolPemanas
    delay(20);
  }
  digitalWrite(pinPemanas, statusPemanas);
  delay(50);
  //* End of Water Heater (Manual)
  
  //* Start of Water Heater (Automation)
  float t = dht.readTemperature();
  int i;
  i = (int) t;
  if (isnan(t)) {
    Serial.println("Gagal membaca data.");
  } else {
    Serial.print(F("Temperatur: "));
    Serial.println(i);
    if (i <= 40) {
      Serial.println("Pemanas Air Menyala.");
      statusPemanas = true;
      delay(250);
    } 
    if (i > 40 && statusPemanas == true) {
      Serial.println("Pemanas Air Mati.");   
      statusPemanas = false;
      statusTombolPemanas = false;
      delay(250);
    }    
  }
  //* End of Water Heater (Automation)  
}

long jarakCentimeter(long ns) {
return ns / 29 / 2;
}

//   if (sisa == 11) {
//     Serial.println("Saluran Air Menyala.");        
//     statusMotor = true;
//     if (waktu > 5000) {

//     Serial.println("Saluran Air Mati.");   
//     statusMotor = false; 
//     }
//     waktu = 0;
//   } 
  
//   if (sisa <= 10) {
//     Serial.println("Saluran Air Menyala.");    
//     statusMotor = true;
//   } 
  
//   if (sisa >= 90) {
//     Serial.println("Saluran Air Mati.");   
//     statusMotor = false; 
//   }
//   delay(100);
// }
