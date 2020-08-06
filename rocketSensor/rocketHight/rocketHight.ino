
//############ INCLUDE BIBS #######

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"


#include <SPI.h>
#include <SD.h>


//############ DEFINE VARS #######
//Other
int statusSum = 0;

//StatusLEDs
int StatusGreenPin = 6;
int StatusRedPin = 7;

//For SD
File myFile;
int SSPin = 10;
unsigned long millisMessbeginn; 
String fileName = "rocket.csv";
boolean counterToggle = true;

// For BMP
Adafruit_BMP280 bmp; // I2C
float pressure;   //To store the barometric pressure (Pa)
float temperature;  //To store the temperature (oC)
int altimeter;    //To store the altimeter (m) (you can also use it as a float variable)

//############ CUSTOM FUNCTIONS #######

// FOR SD
void SDInit(){
    if (!SD.begin(SSPin)) {
          //Serial.println("initialization failed!");
          statusSum+=1;
          }
}// END SDInit

int FileInit(){
     
     if(SD.exists(fileName)){
        SD.remove(fileName);
        //Serial.println("File existed - deleted File");
     }
     

     // Init of file as begin of measurement
     millisMessbeginn = millis();
     
     myFile = SD.open(fileName, FILE_WRITE);
       
    // if the file opened okay, write to it:
    if (!myFile) {   //File opening failed
        statusSum+=10;
    }
}// END FileInit

void SDwrite(){

      


        //Header nur einmal schreiben
        if (counterToggle){ 
          myFile.println("Sek/10_SinceWrite;TempInC;Pressure;AltitudeInM");
          counterToggle = !counterToggle;               
          Serial.print("begin writing on SD Card ...");//NOT NEEDED IN PRODUCTION
        }
        
        
        Serial.print((millis()-millisMessbeginn)/100);Serial.print(";");
        Serial.print(temperature);Serial.print(";");
        Serial.print(pressure);Serial.print(";");
        Serial.println(altimeter);
        
    
        myFile.print((millis()-millisMessbeginn)/100);myFile.print(";");
        myFile.print(temperature);myFile.print(";");
        myFile.print(pressure);myFile.print(";");
        myFile.println(altimeter);
        delay(300);     
}// END SDwrite

// FOR BMP
void BMPInit(){
    if (!bmp.begin(0x76)) {
          Serial.println("BMP initialization failed!");//NOT NEEDED IN PRODUCTION
          statusSum+=100;
          }
}// END BMPInit

void BMPReadValues(){
  //Read values from the sensor:
  pressure = bmp.readPressure();
  temperature = bmp.readTemperature();
  altimeter = bmp.readAltitude (1013.2); //1013.2 Standard baromterischer Druck. Change the to your city current barrometric pressure
}// END BMPReadValues



//############ AUTORUN FUNCTIONS #######
void setup() {

  //NOT NEEDED IN P
  Serial.begin(9600);//NOT NEEDED IN PRODUCTION
  
  //BMP Part
  BMPInit();
   
   //SD Code Part
  SDInit(); //initialize SD
  FileInit();

  //Status Part
  pinMode(StatusGreenPin, OUTPUT);
  pinMode(StatusRedPin, OUTPUT);
  if(statusSum==0){
    digitalWrite(StatusGreenPin, HIGH);
  }else{
    digitalWrite(StatusRedPin, HIGH);
  }
     
}// END setup

void loop() {
  
  Serial.print("StatusCode:");//NOT NEEDED IN PRODUCTION
  Serial.println(statusSum);  //NOT NEEDED IN PRODUCTION

  if(statusSum==0){   //Wenn kein Fehlerstatus
  
    if (millis()<60000){ // Nach 2 min File Close 120000
      BMPReadValues();
      SDwrite();
    }else{
          Serial.println("END OF RUN - CLOSING FILE");  //NOT NEEDED IN PRODUCTION
          myFile.close();
          digitalWrite(StatusGreenPin, HIGH);
          digitalWrite(StatusRedPin, HIGH);
    }
  }
}// END loop
