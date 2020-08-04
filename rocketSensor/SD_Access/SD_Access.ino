// Check https://create.arduino.cc/projecthub/electropeak/sd-card-module-with-arduino-how-to-read-write-data-37f390 for information
// The goal is to write flight sensor data to the sd card


#include <SPI.h>
#include <SD.h>

//global Variables
File myFile;
int SSPin = 10;
unsigned long millisMessbeginn; 
String fileName = "rocket.csv";

//Functions
void SDInit(){
    if (!SD.begin(SSPin)) {
          Serial.println("initialization failed!");
          while (1);
          }
    Serial.println("initialization done.");  
}

int FileInit(){
     
     if(SD.exists(fileName)){
        SD.remove(fileName);
        Serial.println("File existed - deleted File");
     }
     
     myFile = SD.open(fileName, FILE_WRITE);
    // if the file opened okay, write to it:
    if (myFile) {
        Serial.println("File opening successful");
        return(1);
    }else{
        Serial.println("File opening failed XXXX");
        return(0);
    }   
}


void SDwrite(){

         // Zeit seit arduino start;sdWrite simbolisiert Messbeginn
        millisMessbeginn = millis();
        
        Serial.print("begin writing on SD Card ...");
        myFile.println("SekSinceWrite;Temp;Pressure;Altitude");
       
        for (int i = 0; i<100; i++){ 
        myFile.print((millis()-millisMessbeginn)/1000);myFile.print(";");
        myFile.print(20.5+i);myFile.print(";");
        myFile.print(1015.8+i);myFile.print(";");
        myFile.println(23+i);
        delay(200);     
        }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  //SD Code Part
  SDInit(); //initialize SD
  if((int)FileInit() == 1) { //initialize File
      SDwrite(); 
      myFile.close();
      Serial.println("done writing / File closed.");
  }else{
      Serial.println("FAIL");
  }
  


}

void loop() {
  // put your main code here, to run repeatedly:

}
