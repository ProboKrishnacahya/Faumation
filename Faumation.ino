#include "DHT.h" // Library Sensor Suhu DHT
#define DHTTYPE DHT11 // Menggunakan Sensor Suhu tipe DHT11
#define DHTPin 8 // Digital Pin Arduino yang dihubungkan ke Pin Data dari Sensor Suhu DHT11

int pinPemanas = 10; // Jika LED Red menyala maka pemanas air sedang menyala, jika LED Red mati maka pemanas air sedang mati
int pinButtonPemanas = 9;
bool statusPemanas = false; // Jika false maka pemanas air mati, tapi jika true maka pemanas air menyala
//int pinSensorSuhu = 8;
int pinTrigger = 12; // Pin Trigger Sensor Jarak Ultrasonic
int pinEcho = 11; // Pin Echo Sensor Jarak Ultrasonic
long duration, cm; // Jarak Ketinggian Batas Atas dan Batas Bawah Tempat Pengisian Air
int M1 = 13; // Motor DC

DHT dht(DHTPin, DHTTYPE); // Object Sensor Suhu DHT11

void setup() {
  // put your setup code here, to run once:
  pinMode(pinButtonPemanas, INPUT_PULLUP);
  pinMode(pinPemanas, OUTPUT);
  pinMode(M1, OUTPUT);
  // digitalWrite(pinButtonPemanas, HIGH); // Untuk menyalakan INPUT_PULLUP sehingga secara default kondisinya HIGH
  Serial.begin(9600); // Baud Rate Serial Monitor
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(pinButtonPemanas) == LOW) {
    statusPemanas = !statusPemanas;
    delay(20);
  }
  digitalWrite(pinPemanas, statusPemanas);
  delay(100);

  float t = dht.readTemperature();
  int i;
  i = (int) t;
  if (isnan(t)) {
    Serial.println("Gagal membaca data.");
  } else {
    Serial.print(F("Temperatur: "));
    Serial.println(i);
    if (i <= 20) {
      Serial.println("Pemanas Air Menyala.");    
    } 
    if (i >= 40) {
      Serial.println("Pemanas Air Mati.");    
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
  Serial.print(F("Kapasitas Air: "));
  Serial.print(cm);
  Serial.println(" %");
  if (cm <= 10) {
    Serial.println("Saluran Air Menyala.");    
  } 
  if (cm >= 90) {
    Serial.println("Saluran Air Mati.");    
  }
  delay(100);

  if (Serial.available()) {
    char data = Serial.read();
    Serial.print(F("Arah: "));
    Serial.println(data);
    if (data == 'A') {
      digitalWrite(M1, LOW);
    }
    if (data == 'C') {
      digitalWrite(M1, LOW);
    }
    if (data == 'S') {
      digitalWrite(M1, HIGH);
    }    
  }
}

long durationToCentimeter(long ns) {
  return ns / 29 / 2;
}
