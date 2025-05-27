#include "Arduino.h"
#include "Enes100.h"
#include <Servo.h>


const int M1Forward = 0; // In1
const int M1Backward = 0; // In2

const int M2Forward = 0; // In3
const int M2Backward = 0; // In4

const int M3Forward = 0; // In1
const int M3Backward = 0; // In2

const int M4Forward = 0; // In3
const int M4Backward = 0; // In4

/*
M1  M3
M2  M4
*/

// Ultrasonic Sensors

const int trigPinMiddle = 0;  
const int echoPinMiddle = 0; 


// Piston
const int expand = 0;
const int retract = 0;
const int pistonENA = 0;

// Servo
Servo myservo;


void setup() {
  // put your setup code here, to run once:
  Enes100.begin("Bobs Ballroom Builders", SEED, 236, 52, 50);
  Enes100.println("Connected To Wifi");
  
  // Motor Pin Setup
  pinMode(M1Forward, OUTPUT);
  pinMode(M1Backward, OUTPUT);
  pinMode(M2Forward, OUTPUT);
  pinMode(M2Backward, OUTPUT);
  pinMode(M3Forward, OUTPUT);
  pinMode(M3Backward, OUTPUT);
  pinMode(M4Backward, OUTPUT);
  pinMode(M4Forward, OUTPUT);

  // Ultrasonics
  pinMode(trigPinMiddle, OUTPUT);
  pinMode(echoPinMiddle, INPUT);


  // Piston
  pinMode(expand, OUTPUT);
  pinMode(retract, OUTPUT);
  pinMode(pistonENA, OUTPUT);  

  // Servo
  myservo.attach(5);
  int pos = 0;
}

void loop() {
    delay(1000);
    double Yloc = Enes100.getY();
    Enes100.println(Yloc);
    if (Yloc >= 1.4){
        Enes100.println("Down Mission");
        toMissionDOWN();
    } else if (Yloc <= 0.6){
        Enes100.println("UP Mission");
        toMissionUP();
    }  
}

int turnTime(){
    double angleTurn = Enes100.getTheta() - (-1.57);
    Enes100.println("angle turn");
    Enes100.println(angleTurn);
    int TimetoTurn = (angleTurn*1000) / // How far it turn in 1 sec;
    Enes100.println("Time to Turn");
    Enes100.println(TimetoTurn);
    return TimetoTurn;
}

int turnTimeUP(){
    double angleTurn = 1.57 - Enes100.getTheta();
    Enes100.println("angle turnUP");
    Enes100.println(angleTurn);
    int TimetoTurn = (angleTurn*1000) / // How far it turn in 1 sec;
    Enes100.println("Time to TurnUP");
    Enes100.println(TimetoTurn);
    return TimetoTurn;
}

int turnTimeR(){
    double angleTurn = abs(Enes100.getTheta());
    Enes100.println("angle turnR");
    Enes100.println(angleTurn);
    int TimetoTurn = (angleTurn*1000) / // How far it turn in 1 sec;
    Enes100.println("Time to TurnR");
    Enes100.println(TimetoTurn);
    return TimetoTurn;
}


// Movement Functinons
void moveForward(){
   digitalWrite(M1Forward, HIGH);
   digitalWrite(M2Forward, HIGH);
   digitalWrite(M3Forward, HIGH);
   digitalWrite(M4Forward, HIGH);
}

void turnRight(int mins, int power) {
    // Tank.setLeftMotorPWM(power);
    digitalWrite(M1Forward, HIGH);
    digitalWrite(M2Forward, HIGH);

    // Tank.setRightMotorPWM(-power);
    digitalWrite(M3Backward, HIGH);
    digitalWrite(M4Backward, HIGH);
    delay(mins);
    // Tank.turnOffMotors();
    turnOFfMotors();
}

void turnLeft(int mins, int power) {
    // Tank.setLeftMotorPWM(-power);
    digitalWrite(M1Backward, HIGH);
    digitalWrite(M2Backward, HIGH);
    
    // Tank.setRightMotorPWM(power);
    digitalWrite(M3Forward, HIGH);
    digitalWrite(M4Forward, HIGH);
    delay(mins);
    // Tank.turnOffMotors();
    turnOFfMotors();
}

void turnOFfMotors() {
   digitalWrite(M1Forward, LOW);
   digitalWrite(M2Forward, LOW);
   digitalWrite(M3Forward, LOW);
   digitalWrite(M4Forward, LOW);

   digitalWrite(M1Backward, LOW);
   digitalWrite(M2Backward, LOW);
   digitalWrite(M3Backward, LOW);
   digitalWrite(M4Backward, LOW);
}


double getDistance() {
  long durationMiddle;
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds to send a pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, which returns the pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm (sound speed = 343 m/s)
  int distance = duration * 0.034 / 2;

  return distance;
}

void moveForward2(){
    moveForward();
    double obj = getDistance();
    if (abs(obj) < 0.1) {
        avoidObj();
    }
}

void moveForward3(){
    moveForward();
    double obj = getDistance();
    if (abs(obj) < 0.1) {
        avoidObj2();
    }
}

void avoidObj(){
    turnOFfMotors();
    if(Enes100.getY() < 1){
        turnLeft(turnTimeUP(), 127);
        moveForward();
        delay(3000);
        turnOFfMotors()
        turnRight(turnTimeR(), 127);
    }
}

void avoidObj2(){
    turnOFfMotors();
    if(Enes100.getY() > 1){
        turnRight(turnTime(), 127);
        moveForward();
        delay(3000);
        turnOFfMotors(); 
        turnLeft(turnTimeR(), 127);
    }
}


// Down Mission
void toMissionDOWN() {
    double currY = Enes100.getY();
    double myTheta = Enes100.getTheta();
    if (currY >= 1) {
        while (myTheta >= 0) {
        myTheta = Enes100.getTheta();
        turnRight(turnTime(), 127);
        Enes100.println("Down Turn 1 Done");
        delay(1000);
        Enes100.println(Enes100.getTheta());
        myTheta = Enes100.getTheta();
        Enes100.println("While Down theta");
        Enes100.println(myTheta);
        }
        while(abs(myTheta) < 1.57){
            delay(1000);
            myTheta = Enes100.getTheta();
            turnRight(turnTimeR(), 127);
        }
    }
    
    while(Enes100.getY() > 0.5){
        moveForward();
    }
    
    turnOFfMotors();
    while(Enes100.getTheta() < 0){
        turnLeft(turnTimeR(), 127);
    }
    turnOFfMotors();
    while(Enes100.getX() < 3.1){
        moveForward2();
    }
    turnOFfMotors();
}


// Mission Going UP
void toMissionUP() {
    double currY = Enes100.getY();
    double myTheta = Enes100.getTheta();
    if (currY <= 0.9) {
        while (myTheta < 0) {
        myTheta = Enes100.getTheta();
        turnLeft(turnTimeUP(), 127);
        Enes100.println("Up Turn 1 Done");
        myTheta = Enes100.getTheta();
        Enes100.println("While up theta");
        Enes100.println(myTheta);
        }
        while(abs(myTheta) < 1.55){
            myTheta = Enes100.getTheta();
            turnLeft(200, 10);
            myTheta = Enes100.getTheta();
        }
    }
    
    while(Enes100.getY() < 1.45){
        moveForward();
    }
    
    turnOffMotors();
    
    while(Enes100.getTheta() >= 0){
        turnRight(turnTimeR(), 50);
    }
    
    turnOffMotors();

    while(Enes100.getX() < 3.1){
        moveForward3();
    }
    turnOffMotors();
}
