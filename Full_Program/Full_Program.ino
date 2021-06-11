// Library are using in the programe
// Bluetooth Library
#include <SoftwareSerial.h>
#include <HCSR04.h>
// Define Pin Input and Output
// Define Bluetooth Pins
SoftwareSerial BT(2, 3); // RX, TX
int triggerPin = 13;
int echoPin = 12;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// Define Driver L298N Pins
int enA =9;
int in1 =4;
int in2 =5;
int in3 =22;
int in4 =24;
int enB =10;
// Define variables are using 2byte reserved real number
int xAxis=140, yAxis=140;
int motorSpeedA = 0;
int motorSpeedB = 0;
double distance;

// Define Program exucution one time

void setup() {
// Define Driver L298N Pins Output
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
// Define Blutooth Serial Begin
  Serial.begin(9600);
  BT.begin(9600); // Default communication rate of the Bluetooth module
  delay(500);
}

// Define Program exucution repetally

void loop() {

    double distance = distanceSensor.measureDistanceCm();
    distance = distance;
    delay(1);

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
        if (xAxis < 130){
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
