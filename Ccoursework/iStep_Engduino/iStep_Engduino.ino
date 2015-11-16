/*
This is the main part of the engduino application. Created by Mengyang Wu in 2015.
*/

#include <EngduinoSD.h>
#include <EEPROM.h>
#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>
#include <EngduinoMagnetometer.h>
#include <EngduinoThermistor.h>
#include <EngduinoAccelerometer.h>
#include <Wire.h>

// Setting the default value of user's weight(70kg) and step length(0.7m)
#define stepLength 0.70
#define weight 70.0
#define walk 0
#define run 1

// Initialize the variables
int noteStatus = walk;
float noteDistance = 0.0;
float noteAverageSpeed = 0.0;
float noteInstantaneousSpeed = 0.0;
float xPrevious = 0.0;
float yPrevious = 0.0;
float zPrevious = 0.0;
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
float temperature = 0.0;

// Output the real-time data to Serial
// Add a special number before the string for processing app to recognise
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
  Serial.print('7');
  Serial.println(temperature);
}

// Update the status of variables and send them to display on engduino or processing
void update(int status){
  temperature = EngduinoThermistor.temperature();
  step += 2;
  noteStatus = status;
  noteDistance = noteDistance + 2 * stepLength;
  noteAverageSpeed = noteDistance / (time / 10.0);
  noteInstantaneousSpeed = 2 * stepLength / (time - timePrevious) * 10.0;
  if (flag){
    outputSerial();
  }else{
    showDistance();
  }
  turningCount = 0;
  timePrevious = time;
}

// Core algorithm of step counting.
// Basic idea is to compare the data from accelerometer in a short time period and take
// note if the user finishes one step.
// From the observation of the collected data, the x value will change from positive to
// negative and then return to positive during walking process.
// It's same with y value when the user is running.
// So detect the turing point will give the number of steps, and the special features 
// of running and walking help to recognise the user's status.
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
      if (x * 100 * xPrevious < 0){
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
      if (y * 100 * yPrevious < 0.0){
        turningCount++;
        if (turningCount == 2){
          update(run);
        }
      }
    }
  }
  xPrevious = x;
  yPrevious = y;
  zPrevious = z;
}

// Initialise Engduino device
void setup(){
  Serial.begin(9600);
  EngduinoButton.begin();
  EngduinoAccelerometer.begin();
  EngduinoLEDs.begin();
  EngduinoThermistor.begin();
  EngduinoSD.begin();
}

// Choose functions, starting with animation, following by counting process and ending
// with saving data to SD card.
void loop(){
  while(!EngduinoButton.wasPressed()){
    delay(1000);
  }
  jump();
  flag = (Serial) ? true : false;
  while(!EngduinoButton.wasPressed()){
      stepCounting();
      delay(100);
  }
  toSD();
  jump();
}
