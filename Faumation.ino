#include "DHT.h" // Library Sensor Suhu DHT
#define DHTTYPE DHT11 // Menggunakan Sensor Suhu tipe DHT11
#define DHTPin 8 // Digital Pin Arduino yang dihubungkan ke Pin Data dari Sensor Suhu DHT11

int pinPemanas = 10; // Jika LED Red menyala maka pemanas air sedang menyala, jika LED Red mati maka pemanas air sedang mati
int pinButtonPemanas = 9;
bool statusPemanas = false; // Jika false maka pemanas air mati, tapi jika true maka pemanas air menyala
bool statusButtonPemanas = false; // Jika ditekan maka Pemanas menyala (true), tapi jika ditekan lagi maka Pemanas mati (false)
int pinTrigger = 12; // Pin Trigger Sensor Jarak Ultrasonic
int pinEcho = 11; // Pin Echo Sensor Jarak Ultrasonic
long duration, cm; // Jarak Ketinggian Batas Atas dan Batas Bawah Tempat Pengisian Air
int pinMotor = 13; // Motor DC
int pinButtonMotor = 5;
bool statusMotor = false; 
bool statusButtonMotor = false; // Jika ditekan maka Motor menyala (true), tapi jika ditekan lagi maka Motor mati (false)
// long int waktu = 0;

DHT dht(DHTPin, DHTTYPE); // Object Sensor Suhu DHT11

// put your setup code here, to run once:
void setup() {
  pinMode(pinButtonPemanas, INPUT_PULLUP);
  pinMode(pinButtonMotor, INPUT_PULLUP);
  pinMode(pinPemanas, OUTPUT);
  pinMode(pinMotor, OUTPUT);
  Serial.begin(9600); // Baud Rate Serial Monitor
  dht.begin();
}

// put your main code here, to run repeatedly:
void loop() {
  // long int t1 = millis(); // Waktu yang diukur
  // waktu = waktu + t1;
  //Serial.println(waktu);
  int kapasitas;
  
  if (digitalRead(pinButtonPemanas) == LOW) {
    statusPemanas = !statusPemanas; // Mengubah status bool statusPemanas
    statusButtonPemanas = !statusButtonPemanas; // Mengubah status bool statusButtonPemanas
    delay(20);
  }
  digitalWrite(pinPemanas, statusPemanas);
  delay(50);

  if (digitalRead(pinButtonMotor) == LOW) {
    statusMotor = !statusMotor; // Mengubah status bool statusMotor
    statusButtonMotor = !statusButtonMotor; // Mengubah status bool statusButtonPemanas
    delay(20);
  }
  digitalWrite(pinMotor, statusMotor);
  delay(50);

  float t = dht.readTemperature();
  int i;
  i = (int) t;
  if (isnan(t)) {
    Serial.println("Gagal membaca data.");
  } else {
    Serial.print(F("Temperatur: "));
    Serial.println(i);
    if (i <= 40) {
      statusPemanas = true;
      Serial.println("Pemanas Air Menyala.");
    } 
    if (i > 40 && statusPemanas == true) {
      Serial.println("Pemanas Air Mati.");   
      statusPemanas = false;
      statusButtonPemanas = false;
    }    
  }
  delay(500);  

  pinMode(pinTrigger, OUTPUT);
  digitalWrite(pinTrigger, LOW); // Melakukan Trigger dengan Pulse
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);

  pinMode(pinEcho, INPUT);
  duration = pulseIn(pinEcho, HIGH);
  cm = durationToCentimeter(duration);
  kapasitas = map(cm, 4, 1122, 100, 0); // Potentiometer 0 % berarti kapasitas 99 % dan sebaliknya
  Serial.print(F("Kapasitas Air: "));
  Serial.print(kapasitas);
  Serial.println(" %");  
  
  if (kapasitas <= 10) {
    Serial.println("Saluran Air Menyala.");    
    statusMotor = true;
  } 
  
  if (kapasitas >= 90 && statusButtonMotor == true) {
    Serial.println("Saluran Air Mati.");   
    statusMotor = false; 
    statusButtonMotor = false;
  }
  delay(100);
}

//   if (kapasitas == 11) {
//     Serial.println("Saluran Air Menyala.");        
//     statusMotor = true;
//     if (waktu > 5000) {
      
//       Serial.println("Saluran Air Mati.");   
//       statusMotor = false; 
//     }
//     waktu = 0;
//   } 
  
//   if (kapasitas <= 10) {
//     Serial.println("Saluran Air Menyala.");    
//     statusMotor = true;
//   } 
  
//   if (kapasitas >= 90) {
//     Serial.println("Saluran Air Mati.");   
//     statusMotor = false; 
//   }
//   delay(100);
// }

long durationToCentimeter(long ns) {
  return ns / 29 / 2;
}
