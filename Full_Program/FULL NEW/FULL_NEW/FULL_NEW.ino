#include <SoftwareSerial.h>
#include <HCSR04.h>
#include "dht.h"
SoftwareSerial BT(2, 3); // RX, TX
int triggerPin = A1;
int echoPin = A0;
int RST = 0;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
#define dataPin A5
dht DHT;
#define enA 9
#define in1 8
#define in2 11
#define in3 12
#define in4 13
#define enB 10
int xAxis=140, yAxis=140;
int motorSpeedA = 0;
int motorSpeedB = 0;
double distance;
int temp;
int hum;
int readData;

const int AirValue = 600;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int stepTime=1;
int dir = 1;
int steps = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(RST, OUTPUT);

  pinMode(steps, OUTPUT);
  pinMode(dir, OUTPUT);

  Serial.begin(9600);
  BT.begin(9600); // Default communication rate of the Bluetooth module
  delay(500);
}

void loop() {
  double distance = distanceSensor.measureDistanceCm();
  distance = distance;
  delay(1);
  if(distance>0){distance=distance;} 
    BT.print("A");
    BT.print(";");
    BT.print(distance); //send distance to MIT App
    BT.println(";");
    delay(10);
  
  for(int i=0;i<1;i++){
    readData = DHT.read11(dataPin);
    hum = DHT.humidity;
    temp = DHT.temperature;
  }
    BT.print("B");
    BT.print(";");
    BT.print(temp); //send temperature to MIT App
    BT.print(";");
    BT.print(hum); //send humidity to MIT App 
    BT.println(";");
    delay(50);
    
  // Default value - no movement when the Joystick stays in the center
  //xAxis = 140;
  //yAxis = 140;
  // Read the incoming data from the Smartphone Android App
    while (BT.available() >= 2) {
      xAxis = BT.read();
      delay(10);
      yAxis = BT.read();
      Serial.print(xAxis);
      Serial.print(",");
      Serial.println(yAxis);
      delay(10);
   }
    if ((xAxis > 130 && xAxis <150 && yAxis > 130 && yAxis <150)){
      Stop();
    }
    if (yAxis > 130 && yAxis <150){    
      if (xAxis < 130){
        turnRight();
        motorSpeedA = map(xAxis, 130, 60, 0, 255);
        motorSpeedB = map(xAxis, 130, 60, 0, 255);    
      }
      if (xAxis > 150) {
        turnLeft();
        motorSpeedA = map(xAxis, 150, 220, 0, 255);
        motorSpeedB = map(xAxis, 150, 220, 0, 255); 
      }
    }
    else{
      if (xAxis > 130 && xAxis <150){
        if ((yAxis < 130)&&(distance>30)){
          forword();
        }
        else if (distance<=30){
          Stop();
        }
        if (yAxis > 150){
          backword();
        }
        if (yAxis < 130){
          motorSpeedA = map(yAxis, 130, 60, 0, 255);
          motorSpeedB = map(yAxis, 130, 60, 0, 255); 
        }
        if (yAxis > 150){
          motorSpeedA = map(yAxis, 150, 220, 0, 255);
          motorSpeedB = map(yAxis, 150, 220, 0, 255);
        }
        if (xAxis < 130){
      }
      else{
        if ((yAxis < 130)&&(distance>30)){
          forword();
        }
        else if (distance<=30){
          Stop();
        }
        if (yAxis > 150){
          backword();
        }
          motorSpeedA = map(xAxis, 130, 60, 255, 50);
          motorSpeedB = 255; 
        }
        if (xAxis > 150){
          motorSpeedA = 255;
          motorSpeedB = map(xAxis, 150, 220, 255, 50); 
        }
      } 
    }
   //Serial.print(motorSpeedA);
   //Serial.print(",");
   //Serial.println(motorSpeedA);
    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B  
}

void forword(){
Serial.println("forword");
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}
void backword(){Serial.println("backword");
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}

void turnRight(){Serial.println("turnRight");
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}

void turnLeft(){Serial.println("turnLeft");
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH); 
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}

void Stop(){
digitalWrite(in1, LOW);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
Serial.println("stop");
}
void EMERGENCY_STOP(){
digitalWrite(in1, LOW);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
Serial.println("EMERGENCY STOP");
Serial.println(BT.read());
}
void Restart(){
  Serial.println("Restart!");
  digitalWrite(RST, HIGH);
}
void Soil_Moisture(){
  for (int i = 0; i<2700 ; i++){
  digitalWrite(dir, LOW);
  digitalWrite(steps, HIGH);   
  delay(stepTime);                      
  digitalWrite(steps, LOW);    
  delay(stepTime);
  }
  delay(3000);
  for(;;){
    soilMoistureValue = analogRead(A0);
    Serial.println(soilMoistureValue);
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
      if(soilmoisturepercent > 100){
        Serial.println("Soil Moisture");
        Serial.println("100 %");
        delay(250);
      }
      else if(soilmoisturepercent <0){
        Serial.println("Soil Moisture");
        Serial.println("0 %");
        delay(250);
      }
      else if(soilmoisturepercent >0 && soilmoisturepercent < 100){
        Serial.println("Soil Moisture");
        Serial.print(soilmoisturepercent);
        Serial.print(" %");
        delay(250);
      }
        else if(soilmoisturepercent >0 && soilmoisturepercent < 100){
        Serial.println(soilmoisturepercent);
        Serial.print("Soil Moisture");
        Serial.print(((soilmoisturepercent+soilmoisturepercent)/2));
        Serial.print(" %");
        delay(250);
      }
      break;  
  }
    delay(5000);
    BT.print("H");
    BT.print(";");
    BT.print(hum); //send humidity to MIT App 
    BT.println(";");
    delay(50);
  for (int i = 0; i<2700 ; i++){
  digitalWrite(dir, HIGH);
  digitalWrite(steps, HIGH);   
  delay(stepTime);                      
  digitalWrite(steps, LOW);    
  delay(stepTime);
  }
  delay(3000);
  
}
