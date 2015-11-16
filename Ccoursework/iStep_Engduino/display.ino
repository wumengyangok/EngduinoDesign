/*
This file is about the display on EngduinoLEDs. 
*/

// display the distance using EngduinoLEDs
void showDistance(){
  int distance = (int) (noteDistance / 100.0);
  if (distance > 15){
    distance = 15;
  }
  for (int i = 0; i < distance; ++i){
    EngduinoLEDs.setLED(i,GREEN,light);
  }
}

// Animation between two functions
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
