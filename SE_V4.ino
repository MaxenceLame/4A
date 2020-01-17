#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <STM32RTC.h>

/////////////// TSL
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
//////////////////
 
 
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_HIGH, 12345);

/////////////// SD
#include <SPI.h>
#include <SD.h>
File myFile;
String buffer; 
//////////////////

STM32RTC& rtc = STM32RTC::getInstance();

// on initialise la date et l'heure
const byte seconds = 0;
const byte minutes = 55;
const byte hours = 18;
const byte weekDay = 7;
const byte day = 15;
const byte month = 12;
const byte year = 19;




void configureSensor(void)
{
  tsl.enableAutoRange(true);            
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}
 

void setup()
{
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));
  dht.begin();

  rtc.begin(); // initialize RTC 24H format

  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setWeekDay(weekDay);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

   Serial.print("Initializing SD card...");
   if (!SD.begin(4)) {
   Serial.println("initialization failed!");
   return;
   }
 Serial.println("initialization done.");




 ////////////////////////////TSL
 if(!tsl.begin())
  {
    Serial.print("no TSL2561 detected!");
    while(1);
  }
 
  configureSensor();
  ///////////////////////////
  
}

void loop()
{

  for (int i=0;i<10;i++){
  delay(5000);

  // nouvelle mesure de luminosité
  float lux=0;
  sensors_event_t event;
  tsl.getEvent(&event);
  if (event.light)
  {
    lux=event.light;
  }
  
  //Lecture Humidité et Température
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)){
    Serial.println(F("erreur de lecture du capteur DHT"));
    return;
  }

  
   myFile = SD.open("test.txt", FILE_WRITE);
   if (myFile) {
   Serial.print("Writing to test.txt...");
   // Print date...
    print2digits(rtc.getDay());
    myFile.print(";");
    print2digits(rtc.getMonth());
    myFile.print(";");
    print2digits(rtc.getYear());
    myFile.print(";");
    // ...and time
    print2digits(rtc.getHours());
    myFile.print(";");
    print2digits(rtc.getMinutes());
    myFile.print(";");
    myFile.print(h);//Humidité
    myFile.print(";");
    myFile.print(t);//Temp
    myFile.print(";");
    myFile.print(lux);//Lux
    myFile.print(";");
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening test.txt");
    }
   
   myFile.close();
   Serial.println("done.");
   
  }
}



void print2digits(int number) {
  if (number < 10) {
    myFile.print("0"); // print a 0 before if the number is < than 10
  }
  myFile.print(number);
}
