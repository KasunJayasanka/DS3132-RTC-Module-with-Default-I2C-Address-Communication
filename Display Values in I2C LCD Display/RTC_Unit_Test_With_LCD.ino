#include "RTCds3231Settings.h"
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

//LCD Display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

StaticJsonDocument<200> doc;


void setup(){
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(30,11,10,5,30,6,23);

  lcd.init();
  lcd.clear();         
  lcd.backlight();
}

void loop(){
  String dateTime = displayDateTime(); // display the real-time clock data on the Serial Monitor,
  deserializeJson(doc, dateTime);
  String date=doc["date"];
  String time=doc["time"];
  Serial.println(date+" "+time);
  delay(1000); // every second


  lcd.setCursor(0,0);   //Move cursor to character 2 on line 1
  lcd.print("DateTime:");

  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print(date+" "+time);
}
