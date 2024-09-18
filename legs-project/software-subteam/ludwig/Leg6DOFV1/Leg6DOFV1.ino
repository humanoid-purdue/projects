#include <Servo.h>
/*
* Single file program for generating parameterized walking gait for small desktop prototype leg with 6 DOF: LR Hip, LR Knee, LR Ankle
* Generates forward motion by pushing off the ground with back foot represented by stepwise spike in knee joint.
* 
* Parameter description:
* theta af1: hip forward angle
* theta ab1: hip back angle
*
* theta af2: knee rest angle
* theta ab2: knee spike angle
* theta as2: knee swing forward retract angle
* 
* T: Period
* T1: spike start time
* T2: retract end time
*/

Servo hipL;
Servo kneeL;
Servo ankleL;
Servo hipR;
Servo kneeR;
Servo ankleR;
double factor = 1.2;
double T = 2 * factor;
double T1 = 0.8 * factor;
double T2 = 1.8 * factor;
double thetaf1 = 90 - 20;
double thetab1 = 90 + 15;
double thetaf2 = 180 - 20;
double thetab2 = 180 - 0;
double thetas2 = 180 - 35;

unsigned long mytime;
unsigned long startTime;

void setup() {
  // put your setup code here, to run once:
  hipR.attach(6);
  ankleR.attach(3);
  kneeR.attach(5);
  hipL.attach(9);
  ankleL.attach(11);
  kneeL.attach(10);
  Serial.begin(9600);
  startTime = millis();
}

double joint1(float t){
  float j1 = ( (thetab1 + thetaf1)/2 ) - ( (thetab1 - thetaf1) /2 ) * cos(6.28 * t / T);
  return j1;
}
float dmod(float x, double b){
  return ((float) (((int) (x * 1000)) % (int) (b * 1000)))/1000;
}
double joint2(float t){
  float t2 = dmod(t, T);
  if (t2 < T1){
    return thetaf2;
  }
  else if (t2 > T1 && t2 < T/2){
    return thetab2;
  }
  else if (T/2 > T1 && t2 < T2){
    return thetas2;
  }
  else{
    return thetaf2;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  mytime = millis();
  float t = ((float) (mytime - startTime))/1000;
  Serial.println(t);
  float yl2 = joint2(t);
  float yl1 = joint1(t);
  float yl3 = yl2 - yl1;

  float yr2 = joint2(t + T/2);
  float yr1 = joint1(t + T/2);
  float yr3 = yr2 - yr1;
  hipL.write(180 - yl1);
  kneeL.write(270 - yl2);
  ankleL.write(min(max(yl3, 50), 140));

  hipR.write(yr1);
  kneeR.write(yr2 - 90);
  ankleR.write(min(max(180 - yr3, 50), 140));

  delay(50);
}
