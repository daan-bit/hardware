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
int oldtemp;
int oldhum;

void setup()
{
  Serial.begin(9600);
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.lineWrap();
  for(int i = 0; i < strlen(start); i++){
    char c = start[i];
    lcd.print(c);
    delay(50);
  }
  delay(2000);
  lcd.clear();
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
  int newtemp = DHT11.temperature;
  int newhum = DHT11.humidity;
  
  if(newtemp != oldtemp || newhum != oldhum){
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
  }
  delay(60000);
}
