#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);
SoftwareSerial pH(0, 1);

float calibration_value7 = 23.04 - 0.7; //21.34 - 0.7
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pH.begin(9600);
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("pH Meter");
  LCD.setCursor(0, 1);
  LCD.print("IoT");
  delay(5000);
  LCD.clear();
}

void loop(){
  for(int i=0;i<10;i++){ 
    buffer_arr[i]=analogRead(A0);
    delay(30);
  }
  for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(buffer_arr[i]>buffer_arr[j]){
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }
  avgval=0;
  for(int i=2;i<8;i++)
  avgval+=buffer_arr[i];
  float volt=(float)avgval*5/1024.0/6;  
  //Serial.print("Voltage: ");
  //Serial.println(volt);
  ph_act = -5.70 * volt + calibration_value7;
  
  Serial.println(ph_act);
  pH.println(ph_act);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("pH Value =");
  LCD.setCursor(0, 1);
  LCD.print(ph_act);
  delay(15000);
}