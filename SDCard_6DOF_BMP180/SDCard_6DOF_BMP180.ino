//////6 DOF//////
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);                       //Name the MPU6050 module "mpu6050"


//////SD Card//////
#include <SD.h>
#include <SPI.h>
File myFile;                                 //Create file named "File"
int pinCS = 10;                              // Pin 10 on Arduino Uno


//////BMP180//////
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
double currentAirPressure = 100865;          //Current air pressure of the location. VERY IMPORTANT
                                             //Set to pressure in Kenosha 4/25/19
                                             //10m=120Pa difference


//=================================================================================//
void setup() {
  Serial.begin(9600);
  // SD Card Initialization
  pinMode(pinCS, OUTPUT);
  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  }
  else {
    Serial.println("SD card initialization failed");
    return;
  }

  // BMP180 Initialization //
  Serial.println("REBOOT");
  bmp.begin();

  // 6 DOF Initialization //
  Wire.begin();
  mpu6050.begin();                   //Initialize MPU6050
  mpu6050.calcGyroOffsets(true);     //Initialize gyroscope calibration
}


//=================================================================================//
void loop() {
  printData();
  storeData();
  delay(10);
}

void printData() {
  mpu6050.update();
  Serial.print(" Acceleration (g)  X: "); Serial.print(mpu6050.getAccX());                       //Print acceleration values in G-force
  Serial.print(" Y: "); Serial.print(mpu6050.getAccY());
  Serial.print(" Z: "); Serial.print(mpu6050.getAccZ());                                         
  Serial.print("  Gyro (rad)  X: "); Serial.print(mpu6050.getGyroX());                           //Print gyroscope values in rad/s
  Serial.print(" Y: "); Serial.print(mpu6050.getGyroY());                                        
  Serial.print(" Z: "); Serial.print(mpu6050.getGyroZ());                                        
  Serial.print("  Temp (C)="); Serial.print(bmp.readTemperature());                              //Print temperature values in Celsius
  Serial.print("  Pressure (Pa)="); Serial.print(bmp.readPressure());                            //Print pressure values in Pascal
  Serial.print("  Altitude (m)="); Serial.print(bmp.readAltitude(currentAirPressure));           //Print altitude values in Meters
  Serial.print("  Time (ms)="); Serial.println(millis());                                         //Print time values in Milliseconds
}                                                                                                

void storeData() {
  myFile = SD.open("test.txt", FILE_WRITE);                                                      //Open file
  if (myFile){                                                                                   //Write onto file only if it is open
    myFile.print(mpu6050.getAccX()); myFile.print(",");                                            
    myFile.print(mpu6050.getAccY()); myFile.print(",");
    myFile.print(mpu6050.getAccZ()); myFile.print(",");                                          
    myFile.print(mpu6050.getGyroX()); myFile.print(",");                                         
    myFile.print(mpu6050.getGyroY()); myFile.print(",");                                         
    myFile.print(mpu6050.getGyroZ()); myFile.print(",");                                         
    myFile.print(bmp.readTemperature()); myFile.print(",");                                      //Temperature in Celsius
    myFile.print(bmp.readPressure()); myFile.print(",");                                         //Pressure in Pascal
    myFile.print(bmp.readAltitude(currentAirPressure)); myFile.print(",");                       //Altitude in Meters
    myFile.println(millis());                                                                    //Time in milliseconds
    myFile.close();                                                                              //Close the file
  }
}
