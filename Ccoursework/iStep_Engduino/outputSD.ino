/*
This file is to save data of your walking or running to a SD card, which will be 
well organised in "data.txt".
*/

// Calculation of calorie burn.
float calculateCalorie(float v, float t){
  v = v * 3.6;
  t = t / 3600.0;
  float ans = (0.0215 * pow(v,3) - 0.1765 * pow(v,2) + 0.8710 * v + 1.4577) * weight * t;
  return ans;
}

// Change int into char[], which is able to be used by EngduinoSD.writeln().
void toChar(int n){
  char ch[10];
  int i = 0;
  while(n != 0){
    ch[i] = (char)(n % 10 + 48);
    n = n / 10;
    i++;
  }
  for (int j = 0; j < i / 2; ++j){
    char t = ch[j];
    ch[j] = ch[i-1-j];
    ch[i-1-j] = t;
  }
  char newstr[i];
  for (int j = 0; j < i; ++j){
    newstr[j] = ch[j];
  }
  newstr[i] = '\0';
  EngduinoSD.write(newstr);
  EngduinoSD.write("  ");
}

// Open the file, organise the data in readable format and save them in SD card.
void toSD(){
  if (EngduinoSD.isAttached()){
    int n = EEPROM.read(0);
    EngduinoSD.open("data.txt",FILE_WRITE);
    EngduinoSD.writeln("----------------------");
    EngduinoSD.write("Note No. ");
    toChar(n);
    EngduinoSD.write("Steps: ");
    toChar(step);
    EngduinoSD.write("TotalDistance: ");
    toChar((int)noteDistance);
    EngduinoSD.writeln(" m");
    EngduinoSD.write("TotalTime: ");
    toChar((int)(time/10.0));
    EngduinoSD.writeln(" s");
    EngduinoSD.write("AverageSpeed: ");
    toChar((int)noteAverageSpeed);
    EngduinoSD.writeln(" m/s");
    EngduinoSD.write("TotalCalorieBurn: ");
    toChar((int)calculateCalorie(noteAverageSpeed,time/10.0));
    EngduinoSD.writeln(" calories");
    EngduinoSD.writeln("----------------------");
    n++;
    EEPROM.write(0,n);
    EngduinoSD.close();
  }
}
