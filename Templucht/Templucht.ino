#include <dht11.h>
#define DHT11PIN 2
dht11 DHT11;

#include <Wire.h>
#include <hd44780.h>                      
#include <hd44780ioClass/hd44780_I2Cexp.h> 
hd44780_I2Cexp lcd; 
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


void setup()
{
  Serial.begin(115200);
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.lineWrap();
  for(int i = 0; i < strlen(start); i++){
    char c = start[i];
    lcd.print(c);
    delay(50);
  }
  delay(2000);
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
  int newtemp = DHT11.temperature;
  int newhum = DHT11.humidity;
  
  if(newtemp != oldtemp || newhum != oldhum || otherScreen == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(newtemp);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(newhum);
    lcd.print("%");  
    oldtemp = DHT11.temperature, 0;
    oldhum = DHT11.humidity;
    otherScreen == false;  
  }
  

  if(timeToSend == 12){
     String message = String(newtemp) + " " +  String(newhum);
     Serial.println(message);
     timeToSend = 0;
  }


  data = Serial.read();  
  updateUserPref(data);
  

  timePassed = timePassed + 1;
  timeToSend = timeToSend + 1;
  delay(5000);
  
}



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
      printToScreen(degree, object);
      break;
    case '4':
      degree = "high";
      printToScreen(degree, object);
      break;
    case '3':
      degree = "good";
      printToScreen(degree, object);
      break;
    case '2':
      degree = "low";
      printToScreen(degree, object);
      break;
    case '1':
      degree = "too low";
      printToScreen(degree, object);
      break;
  }


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
        otherScreen == true;
        delay(100);
      } 
    }
  }
}


void printToScreen(String degree, char object){
  switch (object){
    case 't':
      usertemp = degree;
      break;
    case 'h':
      userhum = degree;
      break;
  }
}
