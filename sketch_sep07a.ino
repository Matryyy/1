#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  
int fotorezistorPin = A0;             
int diodaPin = 9;                     
int diodaPin2 = 10;
int dioda1 = 4; 
int dioda2 = 5;
int dioda3 = 6;
int tlacitko = 11;
int stav = 0;  

#define DHTPIN 7          // Pin, ke kterému je připojen senzor teploty
#define DHTTYPE DHT22     // Typ senzoru (DHT11 nebo DHT22)
DHT dht(DHTPIN, DHTTYPE);   // Inicializace DHT senzoru

float hum;  
float temp; 

void Print_Display(int radek, int sloupec, bool clear, String text, bool back){
  lcd.setCursor(sloupec, radek);
  if (clear) {
    lcd.clear();
  }
  lcd.print(text);
}

void setup() {
  
  
  //Initialize serial port
  Serial.begin(9600);

  Serial.println("DHT22 sensor testing");
  
  //Initialize the DHT sensor
  dht.begin();  

  
  lcd.init();                      
  lcd.backlight();                 
  lcd.setCursor(0, 0);            
  lcd.print("svetlo:"); 

  pinMode(fotorezistorPin, INPUT); 
  pinMode(diodaPin, OUTPUT);       
  pinMode(diodaPin2, OUTPUT);
  pinMode(dioda1, OUTPUT);
  pinMode(dioda2, OUTPUT);
  pinMode(dioda3, OUTPUT);
  pinMode(tlacitko, INPUT);
  pinMode(DHTPIN, INPUT);

  dht.begin();
}

void loop() {
  
  int tlacitkoStisknuto = digitalRead(tlacitko);  

  if (tlacitkoStisknuto == HIGH) {
    stav = 1 - stav;  
    delay(100);       
  }
  
    float converted = 0.00;
    
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

    Serial.print("Celsius = ");
    Serial.print(temp);
    //Print degree symbol
    Serial.write(176); 
    Serial.println("C");

  if (stav == 0) {
    int hodnotaFotorezistoru = analogRead(fotorezistorPin);  

    lcd.setCursor(8, 0);                                  
    lcd.print(hodnotaFotorezistoru);                       

    if (hodnotaFotorezistoru < 20) {
      digitalWrite(diodaPin, HIGH);  
      digitalWrite(diodaPin2, LOW);
      digitalWrite(dioda1, HIGH);
      digitalWrite(dioda2, LOW);
      digitalWrite(dioda3, LOW);
    } else {
      digitalWrite(diodaPin, LOW);   
      digitalWrite(diodaPin2, HIGH);
      digitalWrite(dioda1, LOW);
      digitalWrite(dioda2, HIGH);
      digitalWrite(dioda3, HIGH);
    }

    // Čtení teploty ze senzoru DHT
    float teplota = dht.readTemperature();

    // Zobrazte teplotu na LCD displeji
    lcd.setCursor(0, 1);
    lcd.print("Teplota: " + String(teplota) + " C");
  } else {
    digitalWrite(dioda1, HIGH);
    digitalWrite(dioda2, HIGH);
    digitalWrite(dioda3, HIGH);

    digitalWrite(diodaPin, HIGH);
    digitalWrite(diodaPin2, HIGH);
    delay(100);
    digitalWrite(diodaPin, LOW);
    digitalWrite(diodaPin2, LOW);
  }

  delay(100);  
}