#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPin 8

int pinPemanas = 10; // Jika LED Red menyala maka pemanas air sedang menyala, jika LED Red mati maka pemanas air sedang mati
int pinButtonPemanas = 9;
bool statusPemanas = false; // Jika false maka pemanas air mati, tapi jika true maka pemanas air menyala
//int pinSensorSuhu = 8;

DHT dht(DHTPin, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(pinButtonPemanas, INPUT_PULLUP);
  pinMode(pinPemanas, OUTPUT);
  // digitalWrite(pinButtonPemanas, HIGH); // Untuk menyalakan INPUT_PULLUP sehingga secara default kondisinya HIGH
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(pinButtonPemanas) == LOW) {
    statusPemanas = !statusPemanas;
    delay(20);
  }
  digitalWrite(pinPemanas, statusPemanas);
  delay(1000);

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.println(t);
  Serial.println(h);
}
