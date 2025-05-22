#include <Servo.h>

//Hello

Servo ArmBaseLeft;
Servo ArmBaseRight;
Servo ArmEnd;
Servo BaseRot;

// Current positions (initialized in setup)
int posBaseLeft = 90;
int posBaseRight = 90;
int posEnd = 90;
int posRot = 90;

const int pumpPin = 7;

const int sensorPinOne = A0;
const int sensorPinTwo = A2;
const int sensorPinThree = A3;
const int sensorPinFour = A5;

const int POSITION_TOLERANCE = 2; // Acceptable error in degrees


void setup() {
  ArmBaseLeft.attach(11);
  ArmBaseRight.attach(8);
  ArmEnd.attach(9);
  BaseRot.attach(10);
  pinMode(pumpPin, OUTPUT);

  // Set initial positions
  ArmBaseLeft.write(posBaseLeft);
  ArmBaseRight.write(posBaseRight);
  ArmEnd.write(posEnd);
  BaseRot.write(posRot);

  digitalWrite(pumpPin, LOW);

  delay(500); // Let them settle
}

// Smoothly move a servo from current to target position
void smoothMove(Servo &servo, int &currentPos, int targetPos, int stepDelay = 10) {
  if (abs(currentPos - targetPos) <= POSITION_TOLERANCE) return;

  int step = (targetPos > currentPos) ? 1 : -1;

  while (abs(currentPos - targetPos) > POSITION_TOLERANCE) {
    currentPos += step;
    servo.write(currentPos);
    delay(stepDelay);
  }

  // Snap to target at the end for accuracy
  currentPos = targetPos;
  servo.write(currentPos);
}

// Composite smooth movement function
void moveToPosition(int baseRotTarget, int endTarget, int leftTarget, int rightTarget, int stepDelay = 5) {
  while (
    posRot != baseRotTarget ||
    posEnd != endTarget ||
    posBaseLeft != leftTarget ||
    posBaseRight != rightTarget
  ) {
    if (posRot != baseRotTarget)
      smoothMove(BaseRot, posRot, baseRotTarget, stepDelay);
    if (posEnd != endTarget)
      smoothMove(ArmEnd, posEnd, endTarget, stepDelay);
    if (posBaseLeft != leftTarget)
      smoothMove(ArmBaseLeft, posBaseLeft, leftTarget, stepDelay);
    if (posBaseRight != rightTarget)
      smoothMove(ArmBaseRight, posBaseRight, rightTarget, stepDelay);
  }
}

// Plant positions
void PlantOne() {

  moveToPosition(45, 0, 100, 80, 5);
  delay(1500);
  moveToPosition(25, 0, 130, 50, 20);
  delay(1500);
  moveToPosition(65, 0, 130, 50, 20);
  delay(1500);
  moveToPosition(65, 90, 89, 91);
}

void PlantTwo() {

  moveToPosition(145, 0, 100, 80, 5);
  delay(1500);
  moveToPosition(165, 0, 130, 50, 20);
  delay(1500);
  moveToPosition(125, 0, 130, 50, 20);
  delay(1500);
  moveToPosition(125, 90, 89, 91);
}

void PlantThree() {

  moveToPosition(45, 180, 80, 100, 5);
  delay(1500);
  moveToPosition(65, 180, 50, 130, 20);
  delay(1500);
  moveToPosition(25, 180, 50, 130, 20);
  delay(1500);
  moveToPosition(25, 90, 90, 90);
}

void PlantFour() {

  moveToPosition(145, 180, 80, 100, 5);
  delay(1500);
  moveToPosition(125, 180, 50, 130, 20);
  delay(1500);
  moveToPosition(165, 180, 50, 130, 20);
  delay(1500);
  moveToPosition(165, 90, 90, 90);
}

void loop() {
  
  digitalWrite(pumpPin, LOW);

  int moistureOne = analogRead(sensorPinOne);
  int moistureTwo = analogRead(sensorPinTwo); 
  int moistureThree = analogRead(sensorPinThree); 
  int moistureFour = analogRead(sensorPinFour); 

  if (moistureOne < 500) {
    digitalWrite(pumpPin, HIGH);

    PlantOne();
  }

  if (moistureTwo < 500) {
    digitalWrite(pumpPin, HIGH);

    PlantTwo();
  }

  if (moistureThree < 500) {
    digitalWrite(pumpPin, HIGH);

    PlantThree();
  }

  if (moistureFour < 500) {
    digitalWrite(pumpPin, HIGH);

    PlantFour();
  }

  delay(100);
}
