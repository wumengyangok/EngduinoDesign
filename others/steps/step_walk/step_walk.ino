
#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>
#include <EngduinoMagnetometer.h>
#include <EngduinoThermistor.h>
#include <EngduinoAccelerometer.h>
#include <EngduinoSD.h>
#include <EEPROM.h>
#include <Wire.h>


#define MaximumforEEPROM 256
#define stepLength 0.70
#define walk 0
#define run 1
#define SD 1
#define monitor 0


int noteStatus = walk;
//int noteStatusPrevious = walk;
float noteDistance = 0.0;
float noteAverageSpeed = 0.0;
float noteInstantaneousSpeed = 0.0;
//float noteDistancePrevious = 0.0;
float xPreviouse = 0.0;
float yPreviouse = 0.0;
float zPreviouse = 0.0;
boolean flag=true;
float accelerations[3]={0,0,0};
int top=0;
int time = 0;
int timePrevious = 0;
int turningCount = 0;
int step = 0;
int yNegativeCount = 0;
int yPositiveCount = 0;
int light = 5;

void showDistance(){
  int distance = (int) (noteDistance / 100.0);
  if (distance > 15){
    distance = 15;
  }
  for (int i = 0; i < distance; ++i){
    EngduinoLEDs.setLED(i,GREEN,light);
  }
}

void outputSerial(){
  if (noteStatus)
    Serial.println("1");
  else
    Serial.println("0");
  Serial.println(noteInstantaneousSpeed);
  Serial.println(noteAverageSpeed);
  Serial.println(noteDistance);
  Serial.println(step);
  float t = time / 10.0;
  Serial.println(t);
}

void toChar(int n)
{
 char ch[10];
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
 char newstr[i];
 for (int j = 0; j < i; ++j){
  newstr[j] = ch[j];
 }
 newstr[i] = '\n';
 EngduinoSD.writeln(newstr);
 //Serial.println(newstr);
}

void outputSD(){
  int iSpeed = (int) (noteInstantaneousSpeed * 100);
  if (noteStatus == walk){
    EngduinoSD.writeln("0");
  }else{
    EngduinoSD.writeln("1");
  }
  toChar(iSpeed);
  toChar(time);
}

void update(int status){
  step += 2;
  noteStatus = status;
  noteDistance = noteDistance + 2 * stepLength;
  noteAverageSpeed = noteDistance / (time / 10.0);
  noteInstantaneousSpeed = 2 * stepLength / (time - timePrevious) * 10.0;
  showDistance();
  //outputSerial();
  if (flag){
    outputSD();
  }else{
    outputSerial();
  }
  turningCount = 0;
  timePrevious = time;
}

void stepCounting(){
  EngduinoAccelerometer.xyz(accelerations);
  float x = accelerations[0];
  float y = accelerations[1];
  float z = accelerations[2];
  //Serial.println(xPreviouse);
  //Serial.println(x);
  //Serial.println(turningCount);
  time++;
  if (noteStatus == walk){
    if (y >= 0.0)
      yPositiveCount++;
    if (yPositiveCount >= 2){
      yPositiveCount = 0;
      noteStatus = run;
      turningCount = 0;
    }else{
      if (x * 100 * xPreviouse < 0){
        turningCount ++;
        if (turningCount == 2){
          update(walk);
        }
      }
    }
  }else{
    if (y < 0.0)
      yNegativeCount++;
    if (yNegativeCount >= 10){
      yNegativeCount = 0;
      noteStatus = walk;
      turningCount = 0;
    }else{
      if (y * 100 * yPreviouse < 0.0){
        turningCount++;
        if (turningCount == 2){
          update(run);
        }
      }
    }
  }
  xPreviouse = x;
  yPreviouse = y;
  zPreviouse = z;
}

void jump(){
  for(int i=2, j=15;i<=15;i++,j--){
    EngduinoLEDs.setLED(i,BLUE,i);
    EngduinoLEDs.setLED(i-1,BLUE,i-1);
    EngduinoLEDs.setLED(i-2,BLUE,i-2);
    delay(j*10);
        EngduinoLEDs.setLED(i,OFF);
    EngduinoLEDs.setLED(i-1,OFF);
    EngduinoLEDs.setLED(i-2,OFF);
  }
}

boolean ifend = false;
void readStringFromSD(){
  char str[10];
  int length = 0;
  str[length] = EngduinoSD.read();
  while (str[length] != '\n'){
    length++;
    str[length] = EngduinoSD.read();    
  }
  char newStr[length];
  for (int i = 0; i < length; ++i){
    newStr[i] = str[i];
  }
  if (newStr[0] != '$'){
    Serial.println(newStr);
  }else{
    ifend = true;
  }
}

void sentToClientEnd(){
  EngduinoSD.open("total.txt",FILE_READ);
  while (!ifend){
    readStringFromSD();
  }
  EngduinoSD.close();
  EngduinoSD.open("dataBase.txt",FILE_READ);
  while (!ifend){
    readStringFromSD();
  }
  Serial.println("$");
  EngduinoSD.close();
}





void setup(){
  //delay(1000);
  Serial.begin(9600);
  //Serial.println("Begin from here:");
  EngduinoButton.begin();
  EngduinoAccelerometer.begin();
  EngduinoLEDs.begin();
  EngduinoThermistor.begin();
  EngduinoLight.begin();
  //for SD card start
  EngduinoSD.begin();
  if (EngduinoSD.isAttached()){
    EngduinoSD.open("dataBase.txt",FILE_WRITE);
  }else{
    flag = false;
  }
  //for EEPROM start
}

void loop(){
  while(!EngduinoButton.wasPressed()){
    //startAnimation();
  }
  if (!flag){
    while (!Serial) {
      ;
    }
  }
  jump();
  // if (flag){
  //   //Serial.println("Writing into SD card");
  //   EngduinoSD.writeln("@");
  // }else{
  //   //Serial.println("monitor start:");
  // }
  while(!EngduinoButton.wasPressed()){
    stepCounting();
    delay(100);
  }
  if (flag){
    EngduinoSD.writeln("$");
    EngduinoSD.close();
    EngduinoSD.open("total.txt",FILE_WRITE);
    //int aSpeed = (int) (noteAverageSpeed * 100);
    int finalDistance = (int) (noteDistance * 100);
    //toChar(step);
    //toChar(aSpeed);
    toChar(finalDistance);
    toChar(time);
    EngduinoSD.writeln("$");
    EngduinoSD.close();
    //Serial.println("Finished saving");
  }else{
    Serial.println("$");
  }
  jump();
  if (flag){
    sentToClientEnd();
  }
}
