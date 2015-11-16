import processing.serial.*;
import g4p_controls.*;

Serial myPort;
final float weight = 70.0;
final int walk = 0;
final int run = 1;

public class Current{
  public String speed;
  public String maximumSpeed;
  public String averageSpeed;
  public String distance;
  public String steps;
  public String calorieBurn;
  public String time;
  public int status;
  public String temperature;
  Current(String sp,String m,String a,String d,String st,String c,String t,int sta,String tem){
    speed = sp;
    maximumSpeed = m;
    averageSpeed = a;
    distance = d;
    steps = st;
    calorieBurn = c;
    time = t;
    status = sta;
    temperature = tem;
  }
}
Current current;
String inString;
boolean show = false;



float calculateCalorie(float v, float t){
  v = v * 3.6;
  t = t / 3600.0;
  float ans = (0.0215 * pow(v,3) - 0.1765 * pow(v,2) + 0.8710 * v + 1.4577) * weight * t;
  return ans;
}

void displayText(String s,int size, int x, int y, int c1, int c2, int c3){
  fill(c1,c2,c3);
  textSize(size);
  text(s,x,y);
  
}

void helpPage(){
  background(100);
  displayText("iStep",55,168,250,10,8,139); 
  displayText("by Mengyang Wu",20,155,400,10,8,139); 
  
}

void monitorPage(){
      background(0);
      ellipseMode(RADIUS);
      fill(#3735E3);
      ellipse(240, 240, 220, 220);
      ellipseMode(CENTER);
      fill(#ADACF0);
      ellipse(240, 240, 380, 380);
      if (current.status == walk) {
        displayText("WALKING",45,140,130,10,8,139);
      }else{
        displayText("RUNNING",45,140,130,10,8,139);
      }
      displayText("CurrentSpeed:",24,120,170,10,8,139);
      displayText(current.speed,24,300,170,10,8,139);
      displayText("AverageSpeed:",24,120,230,10,8,139);
      displayText(current.averageSpeed,24,300,230,10,8,139);
      displayText("Distance:",24,150,290,10,8,139);
      displayText(current.distance,24,260,290,10,8,139);
      displayText("Steps:",24,170,350,10,8,139);
      displayText(current.steps,24,300,350,10,8,139);
      displayText("m/s",20,360,170,10,8,139);
      displayText("m/s",20,360,230,10,8,139);
      displayText("m",20,360,290,10,8,139);
      displayText("CalorieBurn:           calories",30,40,520,255,255,255);
      displayText(current.calorieBurn,30,260,520,255,255,255);
      displayText("°C",20,85,30,255,255,255);
      displayText(current.temperature,20,30,30,255,255,255);
}
void mouseClicked(){
  if (mouseX > 200 && mouseX < 320 && mouseY > 560 && mouseY < 650){
      show = true;
  }
}
void setup(){
  size(480, 640);
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[3], 9600);
  myPort.bufferUntil('\n');
  helpPage();
  GButton buttonM = new GButton(this, 130, 560, 220, 70, "Monitor");
  current = new Current("0.0","0.0","0.0","0.0","0","0.0","0.0",0,"25.0");
  inString = new String();
}
void draw(){
  if (show){
  inString = myPort.readStringUntil('\n');
  if(inString != null){
    switch(inString.charAt(0)){
      case '1':
      current.status = (inString.charAt(1) == '0') ? walk : run;
      break;
      case '2':
      current.speed = inString.substring(1);
      break;
      case '3':
      current.averageSpeed = inString.substring(1);
      break;
      case '4':
      current.distance = inString.substring(1);
      break;
      case '5':
      current.steps = inString.substring(1); 
      break;  
      case '6':
      current.time = inString.substring(1);
      break;
      case '7':
      current.temperature = inString.substring(1);
      break;
      default:
      break;
    }
  }
  current.calorieBurn = str(int(calculateCalorie(float(current.averageSpeed),float(current.time))));
  monitorPage();
  }
}