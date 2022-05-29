#include "DHT.h" // Library Sensor Suhu DHT (konversi dari local directory)
#define DHTTYPE DHT11 // Menggunakan Sensor Suhu tipe DHT11
#define DHTPin 8 // Digital Pin Arduino yang dihubungkan ke Pin Data dari Sensor Suhu DHT11
#include <Wire.h> // Library LCD
#include "LiquidCrystal_I2C.h" // Library LCD 

int pinTrigger = 12; // Pin Trigger Sensor Jarak Ultrasonic
int pinEcho = 11; // Pin Echo Sensor Jarak Ultrasonic
long jarak, cm; // Jarak Ketinggian Batas Atas dan Batas Bawah Tempat Pengisian Air
int pinMotor = 13; // Digital Pin Arduino yang dihubungkan ke Motor DC
int pinTombolMotor = 5; // Push Button untuk kendali manual pada Saluran Air
bool statusMotor = false; // Jika false maka Saluran Air mati, tapi jika true maka Saluran Air menyala
bool statusTombolMotor = false; // Jika ditekan maka Motor menyala (true), tapi jika ditekan lagi maka Motor mati (false)
int pinPemanas = 10; // Jika LED Red menyala maka Pemanas Air sedang menyala, jika LED Red mati maka Pemanas Air sedang mati
int pinTombolPemanas = 9; // Push Button untuk kendali manual pada Pemanas Air
bool statusPemanas = false; // Jika false maka Pemanas Air mati, tapi jika true maka Pemanas Air menyala
bool statusTombolPemanas = false; // Jika ditekan maka Pemanas menyala (true), tapi jika ditekan lagi maka Pemanas mati (false)

DHT dht(DHTPin, DHTTYPE); // Object Sensor Suhu DHT11
LiquidCrystal_I2C lcd(0x27, 2,1,0,4,5,6,7,3, POSITIVE); // Object LCD dengan parameter Alamat, Pinout, Konstanta Backlight

//? put your setup code here, to run once:
void setup() {
  pinMode(pinTombolMotor, INPUT_PULLUP);
  pinMode(pinTombolPemanas, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  pinMode(pinPemanas, OUTPUT);

  dht.begin();
  
  Serial.begin(9600); // Baud Rate Serial Monitor
  
  lcd.begin(20, 4); // Ukuran LCD 20 * 4
  lcd.backlight(); 
  lcd.setCursor(0, 0); // Mengatur peletakan output pada kolom ke-1, baris ke-1
  lcd.print("Faumation");
  delay(3000);
}

//? put your main code here, to run repeatedly:
void loop() {
  int sisa; // Persentase Sisa Air
  
  //* Start of Water Filling (Manual)
  // Membaca value dari Push Button Saluran Air 
  if (digitalRead(pinTombolMotor) == LOW) {
    statusMotor = !statusMotor; // Mengubah value bool statusMotor
    statusTombolMotor = !statusTombolMotor; // Mengubah value bool statusTombolMotor
  }
  digitalWrite(pinMotor, statusMotor);
  delay(100); 
  //* End of Water Filling (Manual) 

  pinMode(pinTrigger, OUTPUT);
  digitalWrite(pinTrigger, LOW); 
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);

  pinMode(pinEcho, INPUT);
  jarak = pulseIn(pinEcho, HIGH);
  cm = jarakCentimeter(jarak);
  sisa = map(cm, 4, 1122, 100, 0); // Konversi Potentiometer 0 % berarti sisa air 100 % dan sebaliknya
  
  // Menampilkan output pada Virtual Terminal
  Serial.println("");
  Serial.print(F("Sisa Air: "));
  Serial.print(sisa);
  Serial.println(" %");  
  
  // Menampilkan output pada LCD
  lcd.setCursor(0, 0); // Mengatur peletakan output pada kolom ke-1, baris ke-1
  lcd.print(F("Sisa Air: "));
  lcd.print(sisa);
  lcd.println(" %");  
  
  //* Start of Water Filling (Automation)  
  if (sisa <= 10) {
    statusMotor = true; // Jika sisa air <= 10% (hampir habis), maka Saluran Air menyala
    delay(250);
  } else if (sisa >= 90) {
    statusMotor = false; // Jika sisa air >= 90% (hampir penuh), maka Saluran Air mati
    statusTombolMotor = false; // Menjaga agar Saluran Air tetap mati meskipun Push Button ditekan jika sisa air >= 90% (hampir penuh)
    delay(250);
  }

  // Statement On / Off
  if (statusMotor) {
      Serial.println("Saluran Air Menyala.");
    } else {
      Serial.println("Saluran Air Mati."); 
  }  
  //* End of Water Filling (Automation)

  //* Start of Water Heater (Manual)
  if (digitalRead(pinTombolPemanas) == LOW) {
    statusPemanas = !statusPemanas; // Mengubah value bool statusPemanas
    statusTombolPemanas = !statusTombolPemanas; // Mengubah value bool statusTombolPemanas
  }
  digitalWrite(pinPemanas, statusPemanas);
  delay(100);
  //* End of Water Heater (Manual)
  
  //* Start of Water Heater (Automation)
  float t = dht.readTemperature(); // Membaca data suhu
  int i;
  i = (int) t; // Konversi float ke integer untuk menghilangkan desimal pada angka suhu
  if (isnan(i)) {
    Serial.println("Gagal membaca data."); // Error Handling jika menampilkan NaN (tidak dapat membaca data suhu)
  } else {
    // Menampilkan output pada Virtual Terminal
    Serial.print(F("Suhu Air: "));
    Serial.print(i);
    Serial.println("\xB0 C"); // Hexadecimal code untuk degree symbol

    // Menampilkan output pada LCD
    lcd.setCursor(0, 2); // Mengatur peletakan output pada kolom ke-1, baris ke-3
    lcd.print(F("Suhu Air: "));
    lcd.print(i);
    lcd.print(" Celsius");

    if (i < 20) {
      statusPemanas = true; // Jika suhu air < 20 derajat Celsius, maka Pemanas Air menyala
      delay(250);
    } else if (i > 40) {
      statusPemanas = false; // Jika suhu air > 40 derajat Celsius, maka Pemanas Air mati
      statusTombolPemanas = false; // Menjaga agar Pemanas Air tetap mati meskipun Push Button ditekan jika suhu air > 40 derajat Celsius
      delay(250);
    }
    
    // Statement On / Off
    if (statusPemanas) {
      Serial.println("Pemanas Air Menyala.");
    } else {
      Serial.println("Pemanas Air Mati."); 
    }   
  }
  //* End of Water Heater (Automation)  
}

long jarakCentimeter(long ns) {
return ns / 29 / 2;
}
