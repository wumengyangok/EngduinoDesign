#include <EngduinoLEDs.h>
#include <EngduinoLight.h>
#include <EngduinoButton.h>
#include <EngduinoMagnetometer.h>
#include <EngduinoThermistor.h>
#include <EngduinoAccelerometer.h>
#include <Wire.h>
void setup() {
  // put your setup code here, to run once:
  EngduinoMagnetometer.begin();
  EngduinoLEDs.begin();
  EngduinoLight.begin();
  EngduinoButton.begin();
  EngduinoThermistor.begin();
  EngduinoAccelerometer.begin();
}
void loop(){

}