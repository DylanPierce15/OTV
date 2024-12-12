#include "Arduino.h"
#include "Enes100.h"
#include <Servo.h>

int s0 = 3;
int s1 = 4;
int s2 = 47;
int s3 = 45;
int outPin = 49;


int openCount = 0;
int pos = 0;
String substance = "";


Servo myservo;
//Servo pin 39

int IN1 = 6;
int IN2 = 5;
int ENA = 7;


int redValue = 0;
int greenValue = 0;
int blueValue = 0;

bool cycleComplete = false;

bool scenario1 = true;
char currentPlot = 'D bh ';


float x,y,t;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT); 

  stopActuator();

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);


  myservo.attach(39);
  x = Enes100.getX();
  y = Enes100.getY();  
  t = Enes100.getTheta();



}

void loop() {
    


      // Extend actuator
  extendActuator();
  delay(10000);  // Adjust time based on actuator speed

      // Stop actuator
  stopActuator();
  delay(500);

      // Retract actuator
  retractActuator();
  delay(10000);  // Adjust time based on actuator speed


}
void getValues(){
  // Measure red light
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  redValue = pulseIn(outPin, LOW); // Added timeout
  //Serial.print("Red pulse: ");
  //Serial.println(redValue);

  // Measure green light
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  greenValue = pulseIn(outPin, LOW); // Added timeout
  //Serial.print("Green pulse: ");
  //Serial.println(greenValue);

  // Measure blue light
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  blueValue = pulseIn(outPin, LOW); // Added timeout
  //Serial.print("Blue pulse: ");
  //Serial.println(blueValue);

  delay(2000); // Delay for readability
}

bool isPlantable(){
  return redValue < 200;
}

void openBox(){
  Serial.println("opening");
  for (pos = 0; pos >= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    openCount++;
  }
}
void closeBox(){
  for (pos = 90; pos >= 180; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void extendActuator() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  analogWrite(ENA, 255);
}

// Function to retract actuator
void retractActuator() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  analogWrite(ENA, 255); // Full speed (optional if using PWM)
}

// Function to stop actuator
void stopActuator() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void mission()
{
  if (isPlantable()) {
    Serial.println("Plantable substrate detected!");
    Enes100.println("The plantable substrate is in: " + currentPlot);
      // Open the box
    openBox();
    delay(1000);  // Allow time for the servo to fully rotate

    delay(10000); //Allow time to rotate
    


      // Extend actuator
    extendActuator();
    delay(10000);  // Adjust time based on actuator speed

      // Stop actuator
    stopActuator();
    delay(500);

      // Retract actuator
    retractActuator();
    delay(10000);  // Adjust time based on actuator speed

      // Stop actuator
    stopActuator();
    delay(500);

      // Close the box
    closeBox();
    delay(1000);  // Allow time for the servo to reset
    Enes100.println("The nonplantable substrate was collected from: " + currentPlot);

    cycleComplete = true;
  }
  else{
    if (scenario1) 
    { // Scenario 1: D → C → B → A
      if (currentPlot == 'D') 
      {
        currentPlot = 'C';
      } else if (currentPlot == 'C') 
      {
        currentPlot = 'B';
      } else if (currentPlot == 'B') 
      {
        currentPlot = 'A';
      }
    } 
    else 
    { // Scenario 2: B → A → C → D
      if (currentPlot == 'B') 
      {
        currentPlot = 'A';
      } 
      else if (currentPlot == 'A') 
      {
        currentPlot = 'C';
      } 
      else if (currentPlot == 'C') 
      {
        currentPlot = 'D';
      }
    }
  }
}
