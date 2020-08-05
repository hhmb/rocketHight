
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
}

int FileInit(){
     
     if(SD.exists(fileName)){
        SD.remove(fileName);
        //Serial.println("File existed - deleted File");
     }
     
     myFile = SD.open(fileName, FILE_WRITE);
    // if the file opened okay, write to it:
    if (!myFile) {
        //Serial.println("File opening successful");
        statusSum+=10;
    }
}

void SDwrite(){

         // Zeit seit arduino start;sdWrite simbolisiert Messbeginn
        millisMessbeginn = millis();
        
        Serial.print("begin writing on SD Card ...");
        myFile.println("SekSinceWrite;TempInC;Pressure;AltitudeInM");
       
        myFile.print((float)((millis()-millisMessbeginn)/1000));myFile.print(";");
        myFile.print(temperature);myFile.print(";");
        myFile.print(pressure);myFile.print(";");
        myFile.println(altimeter);
        delay(300);     
}

// FOR BMP
void BMPInit(){
    if (!bmp.begin(0x76)) {
          Serial.println("BMP initialization failed!");
          statusSum+=100;
          }
}
void BMPReadValues(){
    //Read values from the sensor:
  pressure = bmp.readPressure();
  temperature = bmp.readTemperature();
  altimeter = bmp.readAltitude (1013.2); //1013.2 Standard baromterischer Druck. Change the to your city current barrometric pressure
}
//############ AUTORUN FUNCTIONS #######
void setup() {
  Serial.begin(9600);
  
  //BMP Part
  BMPInit();
   
   //SD Code Part
  SDInit(); //initialize SD
  FileInit();

  //Status
  pinMode(StatusGreenPin, OUTPUT);
  pinMode(StatusRedPin, OUTPUT);
   
    
  if(statusSum==0){
    digitalWrite(StatusGreenPin, HIGH);
  }else{
    digitalWrite(StatusRedPin, HIGH);
  }
     
}

void loop() {
  // ...
  //Wenn kein Fehlerstatus
  Serial.print("StatusCode:");
  Serial.println(statusSum);
  if(statusSum=0){
     BMPReadValues();
     SDwrite();     
  }else {
    myFile.close();
  }


//ABORTKONDITION
  // Nach 2 min File Close
  if (millis()>120000){
    myFile.close();
    digitalWrite(StatusGreenPin, HIGH);
    digitalWrite(StatusRedPin, HIGH);
  }
  
}
