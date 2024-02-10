#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <Wire.h>

#define I2C_SDA 23
#define I2C_SCL 18
#define VELOCITA_DEL_SUONO 0.034
#define TENTATIVI_CALIBRAZIONE 8

const int echoPin = 21;
const int trigPin = 22;
const int PinPotenziometro = 13;

int   DistanzaCm;
long  DurataPing;

float ValorePotenziometro;   
float h[3];
float CalibrazioneSensore;   
float Livello;   

LiquidCrystal_I2C lcd(0x27,16,2);


void setup() 
{
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, 10000); 
  lcd.init(); 
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);  
  lcd.print("Inizio procedura"); 
  delay(3000);  

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int ix=1; ix<=TENTATIVI_CALIBRAZIONE; ix++) 
  {
    ValorePotenziometro = round(analogRead(PinPotenziometro)/10);
    h[ix%3] = ValorePotenziometro;  
    Serial.print("Trimmer: ");  
    Serial.println(ValorePotenziometro);  
    lcd.clear();
    lcd.print("Trimmer: "); 
    lcd.print(ValorePotenziometro); 
    delay(1000);  
  }

  Livello = 10 * int((h[0]+h[1]+h[2]) / 30);  
  CalibrazioneSensore = 0;   
  Serial.print("Livello iniziale (cm): ");  
  Serial.println(Livello);  
  Serial.println("Fine setup\n");  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Liv. iniziale cm:");  
  lcd.setCursor(0,1);
  lcd.print(Livello);  
  delay(3000);  
}


void loop() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  DurataPing = pulseIn(echoPin, HIGH);
  DistanzaCm = DurataPing * VELOCITA_DEL_SUONO/2;

  if (!CalibrazioneSensore) {
    CalibrazioneSensore = DistanzaCm;
    Serial.print("Aria libera iniziale (cm): ");  
    Serial.println(CalibrazioneSensore);  
    Serial.println("\n");  
  }

  Serial.print("Aria libera (cm): ");
  Serial.println(DistanzaCm);
  Serial.print("Livello liquido (cm): ");  
  Serial.println(Livello - DistanzaCm + CalibrazioneSensore);  
  Serial.println("\n");  

  lcd.setCursor(0,0);
  lcd.print("Livello liquido:");  
  lcd.setCursor(0,1);
  lcd.print("      ");  
  lcd.setCursor(0,1);
  lcd.print(Livello - DistanzaCm + CalibrazioneSensore);  
  lcd.setCursor(8,1);
  lcd.print("(cm)");  
  delay(2000);  
}
