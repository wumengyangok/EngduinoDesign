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
#define Maximum 256
//File file;
boolean flag=true;
float acceleration[3]={0,0,0};
int top=0;
void setup() 
{
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

  EngduinoAccelerometer.begin();
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
  delay(10000);
  flag = (EngduinoSD.isAttached()) ? true : false;
  if(flag)
  {
    EngduinoSD.open("dataBase.txt",FILE_WRITE);
    Serial.println("Opened");
    EngduinoSD.writeln("-------------------------------------");
  }else
  {
    for (int i = 0 ; i < Maximum ; i++) 
    {
      EEPROM.write(i, 0);
    }
    Serial.println("Cleaned");
  }
}
void serialOut(int x, int y, int z, int xy, int xz, int yz, int XYZ)
{
    Serial.print("x = "); 
    Serial.print(x);
    Serial.print("g y = "); 
    Serial.print(y);
    Serial.print("g z = "); 
    Serial.print(z); 
    Serial.print("g xy = "); 
    Serial.print(xy);
    Serial.print("g xz = "); 
    Serial.print(xz);
    Serial.print("g yz = "); 
    Serial.print(yz); 
    Serial.print("g xyz = ");  
    Serial.print(XYZ);
    Serial.println("g");  
}
void toChar(int n)
{
  char ch[6]={'0','0','0','0','0','\0'};
  int i = 0;
  if(n < 0)
  {
    n=0-n;
    ch[0]='-';
    i++;
  }
  while(n != 0)
  {
    ch[i] = char(n % 10 + 48);
    n = n / 10;
    i++;
  }
  for (int j = 0; j < i / 2; ++j)
  {
    char t = ch[j];
    ch[j] = ch[i-1-j];
    ch[i-1-j] = t;
  }
  ch[4]=' ';
  EngduinoSD.write(ch);
}
void loop()
{ 
  //delay(30000);
  while(!EngduinoButton.wasPressed()){
  EngduinoAccelerometer.xyz(acceleration);
  acceleration[0] *= 100;
  acceleration[1] *= 100;
  acceleration[2] *= 100;
  int x = (int)acceleration[0];
  int y = (int)acceleration[1];
  int z = (int)acceleration[2];
  int xy = (int) sqrt(x*x + y*y);
  int xz = (int) sqrt(x*x + z*z);
  int yz = (int) sqrt(y*y + z*z);
  int XYZ = (int) sqrt(x*x + y*y +z*z);
  serialOut(x,y,z,xy,xz,yz,XYZ);
//  if(flag)
//  {
//    EngduinoSD.write("x = ");
//    toChar(x);
//    EngduinoSD.write("y = ");
//    toChar(y);
//    EngduinoSD.write("z = ");
//    toChar(z);
//    EngduinoSD.writeln(" ");
//    //EngduinoSD.write(toChar(x));
//    //Serial.println(toChar(x));
//    //EngduinoSD.write(toChar(y));
//    //Serial.println(toChar(y));
//    //EngduinoSD.writeln(toChar(z));
//    //Serial.println(toChar(z));
//  }else{
//    if (top < Maximum)
//    {
//      EEPROM.write(top++,x);
//      EEPROM.write(top++,y);
//      EEPROM.write(top++,z);
//    }else
//    {
//      Serial.println("EEPROM is full, restart or insert a SD card");
//    }
//  }
  delay(200);
  }
  if(flag)
    {
      EngduinoSD.writeln("-------------------------------------");
      EngduinoSD.close();
      EngduinoSD.end();
    }
   flag=false;

}
