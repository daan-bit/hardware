#include <dht11.h>
#define DHT11PIN 2
dht11 DHT11;

#include <Wire.h>
#include <hd44780.h>                      
#include <hd44780ioClass/hd44780_I2Cexp.h> 
hd44780_I2Cexp lcd; 

//VARIABLES
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
const char start[] = "Welcome back!";
int timeToSend = 12;
int oldtemp;
int oldhum;
int data = ' ';
int timePassed;
String usertemp = " ";
String userhum = " ";
String degree= " ";
char object = ' ';
bool otherScreen = true;
bool skipDelay = false;

//SETUP
void setup()
{
  Serial.begin(115200);
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.lineWrap();

  //print welcome message letter for letter
  for(int i = 0; i < strlen(start); i++){
    char c = start[i];
    lcd.print(c);
    delay(50);
  }
  delay(2000);
}


//LOOP
void loop()
{
  //read the new values of temperature and humidity
  int chk = DHT11.read(DHT11PIN);
  int newtemp = DHT11.temperature;
  int newhum = DHT11.humidity;

  //check and print if new values is NOT the same as old values
  if(newtemp != oldtemp || newhum != oldhum || otherScreen == true){
    //put values on lcd screen
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: " + String(newtemp) + (char)223 + "c");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: " + String(newhum) + "%");
    oldtemp = DHT11.temperature, 0;
    oldhum = DHT11.humidity;
    otherScreen == false;  
  }
  

  //every 60 send temperature/humidity values to pi 
  if(timeToSend == 12){
     String message = String(newtemp) + " " +  String(newhum);
     Serial.println(message);
     timeToSend = 0;
  }

  //reads the data send from the raspberry pi
  data = Serial.read();  
  updateUserPref(data);
  

  timePassed = timePassed + 1;
  timeToSend = timeToSend + 1;

  //if temp/hum shows on screen next repeat no delay
  if(skipDelay){
    delay(100);
    skipDelay = false;
  } else {
    delay(5000);
  }
}


//determine the code if temp/hum is high or low based on user preferences
void updateUserPref(int data){
  switch (data) {
    case 't':
      object = 't';
      break;
    case 'h':
      object = 'h';
      break;
    case '5':
      degree = "too high";
      setVariablesValues(degree, object);
      break;
    case '4':
      degree = "high";
      setVariablesValues(degree, object);
      break;
    case '3':
      degree = "good";
      setVariablesValues(degree, object);
      break;
    case '2':
      degree = "low";
      setVariablesValues(degree, object);
      break;
    case '1':
      degree = "too low";
      setVariablesValues(degree, object);
      break;
  }

  //if code is made, show it every 20 seconds on the screen
  if(usertemp != " "){
    if(userhum != " "){
      if(timePassed == 4){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature:");
        lcd.setCursor(0, 1);
        lcd.print(usertemp);
        delay(5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("humidity:");
        lcd.setCursor(0, 1);
        lcd.print(userhum);
        timePassed = 0;
        otherScreen = true;
        skipDelay = true;
        delay(100);
      } 
    }
  }
}


//sets the variables for the code
void setVariablesValues(String degree, char object){
  switch (object){
    case 't':
      usertemp = degree;
      break;
    case 'h':
      userhum = degree;
      break;
  }
}
