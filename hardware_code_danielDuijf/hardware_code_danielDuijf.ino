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

SevenSegmentTM1637 display(DISPLAY_CLK, DISPLAY_DIO);

int data = 0;
char object = ' ';

int opSensor = 1;

//SETUP 
void setup() {
   Serial.begin(9600);
  
   pinMode(redLeds, OUTPUT);
   digitalWrite(redLeds, LOW);

   pinMode(greenLeds, OUTPUT);
   digitalWrite(greenLeds, LOW);

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
  
  display.print("TEL");

}

//LOOP
void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;        
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 1000);

  //Voert uit wanneer iets over de seriële verbinding komt
  while(Serial.available() > 0){
    data = Serial.read();

    //Als s1 binnenkrijgt, dan wordt de afgelopen functie aangroepen
    switch(data){
    case 's': 
      object = 's'; 
      break;
    case '1': //1 (één)
      if(object == 's'){         
         while(true){
           afgelopen();                    
         }
      }
      break;
     case '0': //0
      if(object == 's'){
        }
        break;
      }
      break;
  }
  //als de NFC tag gelezen wordt
  if(success){
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    delay(100);

    //gekeken naar de value van de licht sensor
    if(sensorValue < 40){      
      aftellen();
    }else if (sensorValue > 40){      
      geenTelefoon();
    }
  }

}

//Dit doet ie wanneer de telefoon wel aanwezig is.
void aftellen(){
  if(opSensor == 1){
    Serial.println("A"); //Telefoon wel aanwezig
    opSensor -= 1;
  }
  display.clear();
  display.print("JA");
  digitalWrite(redLeds, LOW);
  digitalWrite(greenLeds, HIGH);
  noTone(buzzer);
  delay(1000);  
}

//Dit doet ie wanneer de telefoon niet aanwezig is.
void geenTelefoon(){
   if(opSensor == 0){
    Serial.println("N");  // niet aanwezig
    opSensor += 1;
   }
    display.clear();
    display.print("NEE");
    digitalWrite(redLeds, HIGH);
    digitalWrite(greenLeds, LOW);
    tone(buzzer, 1000);
}

//Dit doet ie wanneer de noodgevalbutton wordt ingedrukt of als de timer is afgelopen.
void afgelopen(){
    digitalWrite(greenLeds, HIGH);
    digitalWrite(redLeds, HIGH);
    noTone(buzzer);
    display.print("STOP");
}
