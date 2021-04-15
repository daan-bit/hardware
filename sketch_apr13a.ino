//--------------------------IMPORTS---------------------------------------

//LCD SCHERM INSTELLINGEN
#include <Wire.h>
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h> 
hd44780_I2Cexp lcd;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;


//PLOTTER INSTELLINGEN
const int SENSORPIN = A0; 
int inputtime = 0;

//BOTTON INSTELLINGEN
const int button1 = A3;
const int button2 = A2;
const int button3 = A1;

// SERIAL INSTELLINGEN
String inByte = "";
char letter = ' ';
// Timer INSTELLINGEN
int minuten = 0;
int seconden = 0;

//--------------------------SETUP---------------------------------------

void setup(){
//LCD SETUP
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();

//PLOTTER SETUP
  pinMode(SENSORPIN, INPUT);
  
//BUTTON SETUP
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

//INTERUPTS SETUP
//attachInterrupt(digitalPinToInterrupt(button2), pauze, RISING);
//  attachInterrupt(digitalPinToInterrupt(button2), timer, RISING);

// SERIAL SETUP
  Serial.begin(9600);

}
//--------------------------LOOP---------------------------------------

void loop() {
//  while (!Serial) {}
  
  
  if (digitalRead(button1) == LOW){
    delay(10);
    if (digitalRead(button1) == LOW){
      Serial.println(inputtime + 1);
      timer(inputtime);
      
    }
  }
  inputtime = analogRead(SENSORPIN)/5;
  lcd.print("Werktijd: ");
  lcd.print(inputtime + 1);
  lcd.print(" Min");
  delay(100);
  lcd.clear();

 
  


}

//--------------------------FUNCTIONS---------------------------------------


int timer(int werktijd){
  while (true) {
    lcd.print("Select een vak");
    delay(10);
    lcd.clear();
    letter = Serial.read();
    if (letter == 'a'){
        break;
    } 
    else if (letter == 'b'){
        lcd.setCursor(0, 0);
        lcd.print("Kies ander vak!");
        lcd.setCursor(0, 1);
        lcd.print("Teveel tijd!");
        delay(5000);
        lcd.clear();
        loop();
        
    }
        
    }    
  
  
  for (int i = 0; i <= werktijd; i++) {
    minuten = werktijd - i;
    
    for (int x = 0; x <= 60; x++) {
      seconden = 60 - x;
      lcd.print(minuten);
      lcd.print(" min ");
      lcd.print(seconden);
      lcd.print(" sec");
      delay(100);
      lcd.clear();
      pauze();
  }
}
lcd.clear();
lcd.print("Je bent klaar!");
Serial.println("timedone");
delay(5000);

}


void pauze(){
    if (digitalRead(button2) == LOW){
    delay(10);
    if (digitalRead(button2) == LOW){
      lcd.clear();
      lcd.print("Pauze ");
      lcd.print(minuten);
      lcd.print(" min ");
      lcd.print(seconden);
      lcd.print(" sec");
      Serial.println("pauze");
      delay(5000);
      eindpauze();
    }
  }
    
}

void eindpauze(){
    
    while(digitalRead(button3) == HIGH){
    
    }
    Serial.println("eindpauze");
  }
    
