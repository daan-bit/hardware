#include "SevenSegmentTM1637.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

//NFC tag
#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

const int redLeds = 5;
const int greenLeds = 4;

const int buzzer = 7;

const int DISPLAY_CLK = 10;
const int DISPLAY_DIO = 9;

int timer = 10;

SevenSegmentTM1637 display(DISPLAY_CLK, DISPLAY_DIO);

int data = 0;
char object = ' ';

void setup() {
   Serial.begin(9600);
  
   pinMode(redLeds, OUTPUT);
   pinMode(redLeds, LOW);

   pinMode(greenLeds, OUTPUT);
   pinMode(greenLeds, LOW);

   pinMode(buzzer, OUTPUT);
   
   display.begin();
   display.setBacklight(100);
   display.clear();
   delay(1000);

   nfc.begin();

   uint32_t versiondata = nfc.getFirmwareVersion();
   if (! versiondata) {
    Serial.print("Geen board gevonden");
    while (1);
  }else{
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  }

  nfc.setPassiveActivationRetries(0xFF);
  
  nfc.SAMConfig();
  
  display.print("PAS");

}

void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;        
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

//  while(Serial.available() > 0){
//    data = Serial.read();
//  }

  while(success){
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    delay(100);
    
    if(sensorValue < 25){
      Serial.println("A"); //aanwezig
      aftellen();
    }else if (sensorValue > 25){
      Serial.println("N");  // niet aanwezig
      geenTelefoon();
    }
  }          
 }

void aftellen(){
  if(timer == 0){
    afgelopen();
  }
  display.clear();
  display.print(timer);
  digitalWrite(redLeds, LOW);
  digitalWrite(greenLeds, HIGH);
  noTone(buzzer);
  timer-= 1;
  delay(1000);  
}

void geenTelefoon(){
   if(timer <= 0){
      afgelopen();
    }
    display.clear();
    display.print(timer);
    digitalWrite(redLeds, HIGH);
    digitalWrite(greenLeds, LOW);
    tone(buzzer, 1000);
}

void afgelopen(){
  display.clear();
  while(timer <= 0){  
    display.print("op");
  }
}
