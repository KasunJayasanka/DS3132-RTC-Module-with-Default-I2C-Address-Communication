#include "Wire.h"
#include <ArduinoJson.h>
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}


void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}


void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

String displayDateTime(){

  StaticJsonDocument<200> dateTime;
  String jsonStringdateTime="";

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  //Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  // Serial.print(":");
  // if (minute<10){
  //   Serial.print("0");
  // }
  // Serial.print(minute, DEC);
  // Serial.print(":");
  // if (second<10){
  //   Serial.print("0");
  // }
  // Serial.print(second, DEC);
  // Serial.print(" ");
  // Serial.print(dayOfMonth, DEC);
  // Serial.print("/");
  // Serial.print(month, DEC);
  // Serial.print("/");
  // Serial.print(year, DEC);
 
  
  //String dateTime="Date:"+String(year,DEC)+"/"+String(month,DEC)+"/"+String(dayOfMonth,DEC)+",Time: "+String(hour,DEC)+":"+String(minute,DEC)+":"+String(second,DEC);
  String date=String(year,DEC)+"/"+String(month,DEC)+"/"+String(dayOfMonth,DEC);
  dateTime["date"]=date;
  String time=String(hour,DEC)+":"+String(minute,DEC)+":"+String(second,DEC);
  dateTime["time"]=time;

  serializeJson(dateTime,jsonStringdateTime);
  return jsonStringdateTime;
}

