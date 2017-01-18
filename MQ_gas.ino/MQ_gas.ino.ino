// include the library code:
#include <Sodaq_DS3231.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <Wire.h>

char weekDay[][4] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);        // initialize the library with the numbers of the interface pins

const int gasSensor = A0;                     // Gas sensor is connected to Analog pin 0
const int CS_PIN = 8;                         // Sparkfun micro SD card shield uses pin 8

void setup() {

    lcd.begin(16, 2);                           // set up the LCD's number of columns and rows:
    Serial.begin(9600);
    Wire.begin();
    rtc.begin();
    
    lcd.print("Voltage:");                      // Print a message to the LCD.
    
    Serial.println("Initializing Card...");
    pinMode(CS_PIN, OUTPUT);
  
    if(!SD.begin(CS_PIN)){
      
      Serial.println("Card Failure");
      return;
    }
    Serial.println("Card Ready");
}

uint32_t old_timeStamp;

void loop() {

  String dataString = "";
  DateTime now = rtc.now();
      uint32_t ts = now.getEpoch();

  // give analogVal the values getting read from the analog output
  int analogVal = analogRead(gasSensor);

  // convert analog values to a voltage value between 0V-5V
  int voltage = (int)((analogVal*5000L)/1024);

  File dataFile = SD.open("LOG.txt", FILE_WRITE);

  if(dataFile){
      if(old_timeStamp == 0 || old_timeStamp != ts){
          old_timeStamp = ts;

          dataFile.print(now.hour(), DEC);
          dataFile.print(":");
          dataFile.print(now.minute(), DEC);
          dataFile.print(":");
          dataFile.print(now.second(), DEC);
          dataFile.print(",");
          dataFile.print(voltage);
          dataFile.print("");
          dataFile.println();
          Serial.print(now.hour(), DEC);
          Serial.print(":");
          Serial.print(now.minute(), DEC);
          Serial.print(":");
          Serial.print(now.second(), DEC);
          Serial.print(",");
          Serial.print(voltage);
          Serial.print(" millivolts");
          Serial.println();
      
          //dataFile.println();
          dataFile.close();
      }

  }
  else{
      Serial.println("error opening log.txt");
  }

  
  // set the cursor to column 0, line 1
  //(note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  // print out the voltage (as millivolts)
  lcd.print(voltage);
  lcd.print(" mV");
  delay(1000);

}


