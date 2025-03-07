#include <IRremote.hpp> //include the library for the IR remote control
#include <Servo.h> //include the library for servos

//define the pin in which the IR sensor is connected
#define IR_RECEIVE_PIN 11
//define the buttons on the remote control
#define Button_Up 0x11
#define Button_Down 0x4
#define Button_Left 0x12
#define Button_Right 0x4C
#define Button_Enter 0x16
#define Button_Music 0x19
#define Button_VolUP 0x17
#define Button_VolDOWN 0x40

//define a variable for each servo
Servo leftservo;  
Servo rightservo;
Servo servo_motor;
Servo servo_tail;

//define the pin numbers for the ultrasonic sensor
const int pingPin = 5; 
const int echoPin = 4;

//declaring functions so that they can be reused for later
void MoveForward() {

  leftservo.write(120);
  rightservo.write(60);
}

void MoveBackward() {
  leftservo.write(60);
  rightservo.write(120);
}

void TurnLeft() {
  leftservo.write(90);
  rightservo.write(60);
}

void TurnRight() {
  leftservo.write(150);
  rightservo.write(90);
}

void TurnAround() {
  leftservo.write(150);
  rightservo.write(150);
}

void Stop() {
  leftservo.write(90);
  rightservo.write(90);
}

void TurnHead() {
  servo_motor.write(150);
  delay(500);
  servo_motor.write(30);
  delay(500);
  servo_motor.write(90);
}

void SpinAround() {
  leftservo.write(50);
  rightservo.write(50);
}

void WagTail() {
  servo_tail.write(150);
  delay(50);
  servo_tail.write(30);
  delay(50);
  servo_tail.write(0);
  delay(50);
}

void setup() {
  
  Serial.begin(9600);

  //define the pin numbers for each servo
  servo_motor.attach(7);
  servo_tail.attach(8);
  leftservo.attach(9);  
  rightservo.attach(10);

  //allow the IR receiver to receive signal
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 

  //allow the ultrasonic sensor to send and receive signal
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}
void loop() {
  //define variables for distance and duration
  int distance;
  long duration;

  //for how many microseconds should the ultrasonic sensor be triggered
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  //measures the time for the echo to go and return to the sensor
  duration = pulseIn(echoPin, HIGH);
  //calculate the distance between the robot and the obstacle
  distance= duration*0.034/2;
  
  //decodes the received data and triggers actions based on which data the IR sensor received
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.address == 0) {
      if (IrReceiver.decodedIRData.command == Button_Enter) {
        Stop();
      }
      else if (IrReceiver.decodedIRData.command == Button_Up) {
        MoveForward();
      }
      else if (IrReceiver.decodedIRData.command == Button_Down) {
        MoveBackward();
      }
      else if (IrReceiver.decodedIRData.command == Button_Left) {
        TurnLeft();
      }
      else if (IrReceiver.decodedIRData.command == Button_Right) {
        TurnRight();
      }
      else if (IrReceiver.decodedIRData.command == Button_Music) {
        if (distance < 20) {
        Stop();
        delay(300);
        MoveBackward();
        delay(500);
        Stop();
        delay(300);
        TurnHead();
        delay(500);
        TurnAround();
        delay(700);
        Stop();
        delay(300);
        }
      }
      else if (IrReceiver.decodedIRData.command == Button_VolUP) {
        WagTail();
        SpinAround();
      }
      else if (IrReceiver.decodedIRData.command == Button_VolDOWN) {
        Stop();
        if (distance < 20) {
          MoveForward();
          WagTail();
          TurnHead();
          delay(1000);
        }
      }
    //allows the IR sensor to receive the next data
    IrReceiver.resume();
    }
  }
  delay(1000);
}
