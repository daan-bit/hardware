//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
//lcd.setCursor(0, 0); // top left
//lcd.setCursor(15, 0); // top right
//lcd.setCursor(0, 1); // bottom left
//lcd.setCursor(15, 1); // bottom right
const int TRIGGER_PIN3 = 4;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN3 = 5; 
const int TRIGGER_PIN2 = 6;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN2 = 7;  // Arduino pin tied to echo pin on the ultrasonic sensor.
const int TRIGGER_PIN1 = 8;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int ECHO_PIN1 = 9;  // Arduino pin tied to echo pin on the ultrasonic sensor.
const int buttonPin = 10;
int huidigeSensor = 2;
const int rood = 2;
const int groen = 3;
int waarde = 1;
String sensor = "Huidige sensor: ";


int MAX_DISTANCE = 200;


NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing huidige_sensor = sonar1;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(rood, OUTPUT);
  pinMode(groen, OUTPUT);

  
}

void maakLeeg(int positie, int rij)
{
   lcd.setCursor(positie,rij);
   lcd.print("    ");
}

void checkAfstand(int afstand)
{
  if(waarde ==1)
  {
      if(afstand <50 or afstand>60)
      {
        digitalWrite(groen, LOW);
        digitalWrite(rood, HIGH);
      }
      else
      {
        digitalWrite(rood, LOW);
        digitalWrite(groen, HIGH);
      } 
  }
  if(waarde ==2)
  {
      if(afstand <20)
      {
        digitalWrite(groen, LOW);
        digitalWrite(rood, HIGH);
      }
      else
      {
        digitalWrite(rood, LOW);
        digitalWrite(groen, HIGH);
      } 
  }
  if(waarde ==3)
  {
      if(afstand <40 or afstand>60)
      {
        digitalWrite(groen, LOW);
        digitalWrite(rood, HIGH);
      }
      else
      {
        digitalWrite(rood, LOW);
        digitalWrite(groen, HIGH);
      } 
  }

}

void loop()
{
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //Serial.print(huidige_sensor.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  int distance = huidige_sensor.ping_cm();// store the distance value in "distance" variable
  delay(400);
  if(waarde == 1)
  {
    if (!digitalRead(buttonPin)) 
    {
      huidige_sensor = sonar2; 
      huidigeSensor = 2;
      waarde = 2;            
    }
  }
  else if(waarde ==2)
  {
     if (!digitalRead(buttonPin))
    {
      huidige_sensor = sonar3; 
      huidigeSensor = 3;     
      waarde = 3;      
    }
  }
  else if(waarde ==3)
  {
    if(!digitalRead(buttonPin))
    {
      huidige_sensor =sonar1;
      huidigeSensor=1;
      waarde = 1;
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor: ");
  lcd.setCursor(8,0);
  lcd.print(huidigeSensor);
  lcd.setCursor(0,1);
  lcd.print("Afstand: ");
  lcd.setCursor(9,1);
  lcd.print(distance);
  Serial.print(sensor + huidigeSensor);
  Serial.print("\t");
  Serial.print(distance);
  Serial.print("\n");
  checkAfstand(distance);
  delay(250);

  
}
