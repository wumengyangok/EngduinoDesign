import processing.serial.*;
import g4p_controls.*;

Serial myPort;
GButton b1,b2;
GEvent event;
final int help = 2;
final int monitor = 1;
final int review = 0;
int status = help;
final float weight = 70.0;
final int walk = 0;
final int run = 1;

public class Current{
  public String speed;
  public String maximumSpeed;
  public String averageSpeed;
  public String distance;
  public int steps;
  public String calorieBurn;
  public String time;
  public int status;
  Current(String sp,String m,String a,String d,int st,String c,String t,int sta){
    speed = sp;
    maximumSpeed = m;
    averageSpeed = a;
    distance = d;
    steps = st;
    calorieBurn = c;
    time = t;
    status = sta;
  }
}
Current current;
String inString;
boolean ifFinished = false;
int walkCount = 0;
int runCount = 0;
String totDistance, totTime;



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
  status = help;
  background(100);
  
}

void monitorPage(){
      background(0);
      status = monitor;
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
      displayText(current.speed,24,240,170,10,8,139);
      displayText("AverageSpeed:",24,120,230,10,8,139);
      displayText(current.averageSpeed,24,240,230,10,8,139);
      displayText("Distance:",24,150,290,10,8,139);
      displayText(current.distance,24,240,290,10,8,139);
      displayText("Steps:",24,170,350,10,8,139);
      displayText(str(current.steps),24,240,350,10,8,139);
      
      displayText("m/s",20,360,170,10,8,139);
      displayText("m/s",20,360,230,10,8,139);
      displayText("m",20,360,290,10,8,139);
      displayText("CalorieBurn:              calories",40,20,520,255,255,255);
      displayText(current.calorieBurn,40,160,520,10,8,139);
}

void reviewPage(){
      background(255);
      status = review;
      stroke(0);
      line(50,280,50,30);
      line(50,280,420,280);
      displayText("Speed",15,5,20,39,37,242);
      displayText("Time",15,400,300,39,37,242);
      float x = map(float(current.time),0,float(totTime),0,370);
      float y = map(float(current.speed),0,6,0,370);
      x = x + 50;
      y = 280 - y;
      if (current.status == walk) {
        stroke(#15BDE5);
      }else{
        stroke(#620AF2);
      }
      line(x,280,x,y);
      displayText("Steps:",24,150,310,39,37,242);
      displayText(str(current.steps),24,240,310,39,37,242);
      displayText("TotalTime:     s",24,20,360,39,37,242);
      displayText(totTime,24,20,360,39,37,242);
      displayText("Distance:      m",24,255,360,39,37,242);
      displayText(totDistance,24,360,360,39,37,242);
      displayText("% Running:     %",24,20,420,39,37,242);
      displayText(str(runCount / current.steps * 100),24,130,420,39,37,242);
      displayText("% Walking:     %",24,20,480,39,37,242);
      displayText(str(walkCount / current.steps * 100),24,130,480,39,37,242);
      displayText("MaxSpeed:      m/s",24,250,420,39,37,242);
      displayText(current.maximumSpeed,24,360,420,39,37,242);
      displayText("AvgSpeed:      m/s",24,250,480,39,37,242);
      displayText(current.averageSpeed,24,360,480,39,37,242);
      displayText("CalorieBurn:          calories",24,200,540,39,37,242);
      displayText(current.calorieBurn,24,200,540,39,37,242);
}

void waitUntilValid(){
  inString = myPort.readStringUntil('\n');
  while(inString == null){
    inString = myPort.readStringUntil('\n');
    if (inString.equals("$")) {
      inString = null;
    }
  }
  inString = trim(inString);
}

void doMonitor(){
  waitUntilValid();
  current.speed = inString;
  waitUntilValid();
  current.averageSpeed = inString;
  waitUntilValid();
  current.distance = inString;
  waitUntilValid();
  current.steps = int(inString);
  waitUntilValid();
  current.time = inString;
  waitUntilValid();
  current.calorieBurn = str(calculateCalorie(float(current.averageSpeed),float(current.time)));
  monitorPage();
}

void doReview(){
  if(ifFinished){
    current.steps += 2;
    waitUntilValid();
    current.status = (inString.equals("0")) ? walk : run;
    if (current.status == run) {
      runCount++;
    }else{
      walkCount++;
    }
    waitUntilValid();
    current.speed = inString;
    if (float(current.speed) > float(current.maximumSpeed)) {
      current.maximumSpeed = current.speed;
    }
    waitUntilValid();
    current.time = inString;
  }else{
    waitUntilValid();
    totTime = inString;
    waitUntilValid();
    totDistance = inString;
    current.averageSpeed = str(float(totDistance)/float(totTime));
    current.calorieBurn = str(calculateCalorie(float(current.averageSpeed),float(totTime)));
    ifFinished = true;
  }
}







void mouseClicked(){
  if (mouseX > 20 && mouseX < 240 && mouseY > 560 && mouseY < 650){
      monitorPage();
  }
  if (mouseX > 240 && mouseX < 460 && mouseY > 560 && mouseY < 650){
      reviewPage();
  }
  if (mouseX > 430 && mouseX < 460 && mouseY > 20 && mouseY < 50){
      helpPage();
  }
  
}
void setup(){
  size(480, 640);
  println(Serial.list());
s
  myPort.bufferUntil('\n');
  helpPage();
  GButton buttonM = new GButton(this, 20, 560, 220, 70, "Monitor");
  GButton buttonR = new GButton(this, 240, 560, 220, 70, "Review");
  GButton buttonH = new GButton(this, 430, 20, 30, 30, "Help");
  current = new Current("0.0","0.0","0.0","0.0",0,"0.0","0.0",0);
  inString = new String();

}
void draw(){ 
  switch (status) {
    case monitor: 
    doMonitor();
    break;
    case review: 
    doReview();
    break;
    case help:
    break;
    default:
    break;
  }
}