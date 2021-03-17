#include "SevenSegmentTM1637.h"

const int lichtSensor = A0;
int sensorValue = 0;

const int redLeds = 5;
const int greenLeds = 4;

const int DISPLAY_CLK = 10;
const int DISPLAY_DIO = 9;

int timer = 10;

SevenSegmentTM1637 display(DISPLAY_CLK, DISPLAY_DIO);

void setup() {
   Serial.begin(9600);

   pinMode(lichtSensor, INPUT);
  
   pinMode(redLeds, OUTPUT);
   pinMode(redLeds, LOW);

   pinMode(greenLeds, OUTPUT);
   pinMode(greenLeds, LOW);
   
   display.begin();
   display.setBacklight(100);
   display.clear();
   delay(1000);



}

void loop() {
  sensorValue = analogRead(lichtSensor);
  delay(100);
  Serial.println(sensorValue);

  //voor nettere code, maak een andere else if met if(sensorValue >5 or <5 && timer == 0). Niet nog een 
  // keer een if-statement in een if-statement. Nu dubbele code!!
  if(sensorValue < 5){
    if(timer <= 0){
      display.clear();
      display.print("op");
    }else{
      aftellen();
    }
  }else if(sensorValue > 5){
    if(timer <= 0){
      display.clear();
      display.print("op");
    }else{
    display.clear();
    display.print(timer);
    digitalWrite(redLeds, HIGH);
    digitalWrite(greenLeds, LOW);  
    }  
  }
}

void aftellen(){
  display.print(timer);
  digitalWrite(redLeds, LOW);
  digitalWrite(greenLeds, HIGH);
  timer-= 1;
  delay(1000);
}
