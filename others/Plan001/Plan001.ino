#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>
#include <EngduinoMagnetometer.h>
#include <EngduinoThermistor.h>
#include <EngduinoAccelerometer.h>
#include <EngduinoSD.h>
#include <EEPROM.h>
#include <Wire.h>
//#include <SD.h>
#define Maximum 5
//File file;
void setup() {
  // put your setup code here, to run once:
  
  // EngduinoMagnetometer.begin();
  // EngduinoMagnetometer.end();
  // EngduinoMagnetometer.xyz();
  // EngduinoMagnetometer.temperature();

  // EngduinoLEDs.begin();
  // EngduinoLEDs.end();
  // EngduinoLEDs.setLED();
  // EngduinoLEDs.setAll();
  // EngduinoLEDs.setLEDs();

  // EngduinoLight.begin();
  // EngduinoLight.end();
  // EngduinoLight.lightLevel();

   EngduinoButton.begin();
  // EngduinoButton.end();
  // EngduinoButton.reset();
  // EngduinoButton.isPressed();
  // EngduinoButton.waitUntilPressed();
  // EngduinoButton.waitUntilReleased();
  // EngduinoButton.wasPressed();
  // EngduinoButton.wasReleased();

  // EngduinoThermistor.begin();
  // EngduinoThermistor.end();
  // EngduinoThermistor.temperature();
  // EngduinoThermistor.temperatureRaw();

  // EngduinoAccelerometer.begin();
  // EngduinoAccelerometer.end();
  // EngduinoAccelerometer.xyz();
  // EngduinoAccelerometer.xyzRaw();
  EngduinoSD.begin();
  
   //EngduinoSD.begin();
  // EngduinoSD.open();
  // EngduinoSD.close();
  // EngduinoSD.end();
  // EngduinoSD.write();
  // EngduinoSD.writeln();
  // EngduinoSD.isAttached();
  // EngduinoSD.isInitialized();
  // EngduinoSD.available();
  // EngduinoSD.read();

   // SD.begin().exists().mkdir().open().remove()
   // .rmdir();
   // File.available.close.flush.peek.position.
   // seek.size.isDirectory.openNextFile.
   // rewindDirectory.println.print.read.write
  // EEPROM.read().write().update().get().
  // put(),EEPROM[];
  Serial.begin(9600);
  for (int i = 0 ; i < Maximum ; i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("Cleaned");
  
}
//int dataBase[Maximum];
void loop(){
  EngduinoButton.waitUntilPressed();
  Serial.println("Let's begin!");  
  if (EngduinoSD.isAttached()) {
   Serial.println("Found SD card");
  } else {
   Serial.println("No SD card");
  }
  Serial.println(EngduinoSD.open("/dataBase.txt",FILE_WRITE));
  for (int i = 0; i < Maximum; ++i)
  {
    EEPROM.write(i,i);
  }
  EngduinoButton.waitUntilPressed();
  for (int i = 0; i < Maximum; ++i)
  {
    int j=0;
    j=EEPROM.read(i);
    char ch[5]={'1','2','3','4','5'};
    EngduinoSD.write(ch);
  }
  Serial.println("Successfully send data from EEPROM to SD card");
  //Serial.println(file.available());
  EngduinoSD.close();
  EngduinoSD.open("dataBase.txt",FILE_READ);
  for (int i = 0; i < Maximum; ++i)
  {
    int t = EngduinoSD.read();
    Serial.println((char)t);
  }
  EngduinoSD.close();
  Serial.println("Wonderfully");
  //Serial.println(Maximum);
//  Serial.println("EEPROM Maximum storage = ");
//  Serial.println(EEPROM.length());
}
