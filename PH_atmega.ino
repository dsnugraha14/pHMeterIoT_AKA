#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 16, 2);

const int ph_Pin = A0;
float Po = 0;
float PH_step;
int nilai_analog_PH;
double TeganganPh;

float PH4 = 3.437;
float PH7 = 2.656;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(ph_Pin, INPUT);
  LCD.init();
  LCD.backlight();
}

void loop()
{
  nilai_analog_PH = analogRead(ph_Pin);
  Serial.print("Nilai ADC Ph: ");
  Serial.println(nilai_analog_PH);
  TeganganPh = 5 / 1024.0 * nilai_analog_PH;
  Serial.print("TeganganPh: ");
  Serial.println(TeganganPh, 3);

  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - TeganganPh) / PH_step);
  Serial.print("Nilai PH cairan: ");
  Serial.println(Po, 2);

  // Tampilkan data di LCD
  displayDataOnLCD(Po);

  delay(3000);
  LCD.clear();
}

void displayDataOnLCD(float pHValue)
{
  LCD.setCursor(0, 0);
  LCD.print("pH Larutan = ");
  LCD.setCursor(0, 1);
  LCD.print(pHValue, 2);
}