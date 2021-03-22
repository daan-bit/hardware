#include "SevenSegmentTM1637.h"

const int lichtSensor = A0;
int sensorValue = 0;

const int redLeds = 5;
const int greenLeds = 4;

const int buzzer = 7;

//const int magneet = 2;

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

   pinMode(buzzer, OUTPUT);

   //pinMode(magneet, INPUT_PULLUP); werkt hetzelfde als een button
   
   display.begin();
   display.setBacklight(100);
   display.clear();
   delay(1000);



}

void loop() {
  sensorValue = analogRead(lichtSensor);
  delay(100);
  Serial.println(sensorValue);

  if(sensorValue < 5){
      aftellen();
  }else if(sensorValue > 5){
    if(timer <= 0){
      afgelopen();
    } 
    display.clear();
    display.print(timer);
    digitalWrite(redLeds, HIGH);
    digitalWrite(greenLeds, LOW);
    tone(buzzer, 1000);     
   } 
  }

void aftellen(){
  if(timer == 0){
    afgelopen();
  }
  display.print(timer);
  digitalWrite(redLeds, LOW);
  digitalWrite(greenLeds, HIGH);
  noTone(buzzer);
  timer-= 1;
  delay(1000);  
}

void afgelopen(){
  display.clear();
  while(timer <= 0){  
  display.print("op");
  }
}
