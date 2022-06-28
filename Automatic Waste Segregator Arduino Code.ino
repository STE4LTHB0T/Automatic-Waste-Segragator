// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library. 
#include <Wire.h>


#define RX 2
#define TX 3

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//int ThermistorPin = A2;
//int Vo;
//float R1 = 1000;
//float logR2, R2, T;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

String AP = "iot project";       // CHANGE ME
String PASS = "123123123"; // CHANGE ME
String API = "TDO8ZC5F69BCJILX";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
int countTrueCommand;
int countTimeCommand;
boolean found = false;

int dry_sensor = A0;
int wet_sensor = A1;
int metal_sensor = A2;

int drylevel_sensor = A3;
int wetlevel_sensor = A4;
int metallevel_sensor = A5;


int relay1 = 7;
int relay2 = 6;
int relay3 = 4;
int relay4 = 5;
int buzzer = 1;

//  Variables
//const int PulseWire = A3;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
//const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

SoftwareSerial esp8266(RX, TX);
 int dry,wet,metal,level,t,g,s,location;
void setup() {

  
   
  //Serial.begin(9600);
  esp8266.begin(115200);
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  
    digitalWrite(buzzer, LOW);
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    
  lcd.setCursor(0, 0); 
  lcd.print(" SMART  DUSTBIN ");
  lcd.setCursor(0, 1);
  lcd.print("    SYSTEM      ");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("CONNECTING...   ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
 
}

void loop() {
  
  lcd.clear();


//  int temp = map(analogRead(temp_sensor), 0, 1023, 100, 0);
//  int gas = map(analogRead(gas_sensor), 0, 1023, 100, 0);
//  int sound = map(analogRead(sound_sensor), 0, 1023, 100, 0);

 dry = digitalRead(dry_sensor);
 wet = digitalRead(wet_sensor);
 metal = digitalRead(metal_sensor);

//  t=(temp/2);
//  g=(gas/10);
  

  
  int drylevel; digitalRead(drylevel_sensor) == LOW ? drylevel = 1 : drylevel = 0;
  int wetlevel; digitalRead(wetlevel_sensor) == LOW ? wetlevel = 1 : wetlevel = 0;
  int metallevel; digitalRead(metallevel_sensor) == LOW ? metallevel = 1 : metallevel = 0;
 
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.setCursor(5, 0);
  lcd.print("W:");
  lcd.setCursor(10, 0);
  lcd.print("M:");
  
  lcd.setCursor(0, 1);
  lcd.print("DL:");
  lcd.setCursor(6, 1);
  lcd.print("WL:");
  lcd.setCursor(12, 1);
  lcd.print("M:");
  
    if (dry == LOW)
        { 
          lcd.setCursor(2, 0);
          lcd.print("YS");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          vopen();
          delay(2000);
          vclose();
          } 
          else 
          {
            lcd.setCursor(2, 0);
            lcd.print("NO");
            digitalWrite(buzzer, LOW);
         } 

   if (wet == LOW)
        { 
          lcd.setCursor(7, 0);
          lcd.print("YS");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          motor_fwd();
          delay(1500);
          motor_stop();
          delay(2000);
          vopen();
          delay(2000);
          vclose();
          delay(500);
          motor_rws();
          delay(1500);
          motor_stop();
          } 
          else 
          {
            lcd.setCursor(7, 0);
            lcd.print("NO");
            digitalWrite(buzzer, LOW);
  } 
   if (metal == LOW)
         { 
          lcd.setCursor(12, 0);
          lcd.print("YS");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          motor_rws();
          delay(1500);
          motor_stop();
          delay(2000);
          vopen();
          delay(2000);
          vclose();
          delay(500);
          motor_fwd();
          delay(1500);
          motor_stop();
          } 
          else 
          {
            lcd.setCursor(12, 0);
            lcd.print("NO");
            digitalWrite(buzzer, LOW);
          }
          if (drylevel == HIGH)
         { 
          lcd.setCursor(3, 1);
          lcd.print("FL");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          } 
          else 
          {
            lcd.setCursor(3, 1);
            lcd.print("NR");
            digitalWrite(buzzer, LOW);
          }
            if (wetlevel == HIGH)
         { 
          lcd.setCursor(9, 1);
          lcd.print("FL");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          } 
          else 
          {
            lcd.setCursor(9, 1);
            lcd.print("NR");
            digitalWrite(buzzer, LOW);
          }
        if (metallevel == HIGH)
         { 
          lcd.setCursor(14, 1);
          lcd.print("FL");
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
          } 
          else 
          {
            lcd.setCursor(14, 1);
            lcd.print("NR");
            digitalWrite(buzzer, LOW);
          }


          
  
  String getData = "GET /update?api_key=" + API + "&" + field1 + "=" + String(drylevel)  + "&" + field2 + "=" + String(wetlevel) + "&" + field3 + "=" + String(metallevel);
  sendCommand("AT+CIPMUX=1", 2, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 3, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 2, ">");
  esp8266.println(getData); delay(1); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 2, "OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}

void motor_fwd()
{
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
}
void motor_rws()
{     digitalWrite(relay1,LOW);
      digitalWrite(relay2, HIGH);
}
void motor_stop()
{
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
}
void vopen()
{
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
      delay(2500);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
}
void vclose()
{
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
      delay(2500);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
}
