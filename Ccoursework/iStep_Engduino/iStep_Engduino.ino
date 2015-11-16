#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>
#include <EngduinoMagnetometer.h>
#include <EngduinoThermistor.h>
#include <EngduinoAccelerometer.h>
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
File file;

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
  Serial.print('1');
  if (noteStatus)
    Serial.println("1");
  else
    Serial.println("0");
  Serial.print('2');
  Serial.println(noteInstantaneousSpeed);
  Serial.print('3');
  Serial.println(noteAverageSpeed);
  Serial.print('4');
  Serial.println(noteDistance);
  Serial.print('5');
  Serial.println(step);
  Serial.print('6');
  float t = time / 10.0;
  Serial.println(t);
}

void update(int status){
  step += 2;
  noteStatus = status;
  noteDistance = noteDistance + 2 * stepLength;
  noteAverageSpeed = noteDistance / (time / 10.0);
  noteInstantaneousSpeed = 2 * stepLength / (time - timePrevious) * 10.0;
  showDistance();
  outputSerial();
  turningCount = 0;
  timePrevious = time;
}

void stepCounting(){
  EngduinoAccelerometer.xyz(accelerations);
  float x = accelerations[0];
  float y = accelerations[1];
  float z = accelerations[2];
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

void setup(){
  Serial.begin(9600);
  EngduinoButton.begin();
  EngduinoAccelerometer.begin();
  EngduinoLEDs.begin();
  EngduinoThermistor.begin();
  EngduinoLight.begin();
}


void loop(){
  while(!EngduinoButton.wasPressed()){
    //startAnimation();
  }
  jump();
  while(!EngduinoButton.wasPressed()){
      stepCounting();
      delay(100);
  }
  Serial.println("$");
  jump();
}
