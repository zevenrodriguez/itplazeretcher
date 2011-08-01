#define X_DIR_PIN 14   //A0
#define X_STEP_PIN 8

#define Y_DIR_PIN 15    //A1
#define Y_STEP_PIN 17   //A3

#define Z_DIR_PIN 12
#define Z_STEP_PIN 13

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define IN 4
#define OUT 5

int stepSpeed = 1000;


void setup(){

  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);



}


void loop(){

motorStep(3,4000);
delay(100);
}


void motorStep (int motorDirection, int numberSteps) {
  int dirPin;
  int stepPin;
  int moveDirection;

  switch (motorDirection) {
  case UP:
    dirPin = Z_DIR_PIN;
    stepPin = Z_STEP_PIN;
    moveDirection = HIGH;
    break;
  case DOWN:
    dirPin = Z_DIR_PIN;
    stepPin = Z_STEP_PIN;
    moveDirection = LOW;
    break;
  case LEFT:
    dirPin = X_DIR_PIN;
    stepPin = X_STEP_PIN;
    moveDirection = LOW;
    break;
  case RIGHT:
    dirPin = X_DIR_PIN;
    stepPin = X_STEP_PIN;
    moveDirection = HIGH;
    break;
  case OUT:
    dirPin = Y_DIR_PIN;
    stepPin = Y_STEP_PIN;
    moveDirection = LOW;
    break;
  case IN:
    dirPin = Y_DIR_PIN;
    stepPin = Y_STEP_PIN;
    moveDirection = HIGH;
    break;
  default:
    return;
  }

  digitalWrite(dirPin, moveDirection);
  delay(100);
  for (int i = 0; i < numberSteps; i++) {
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepSpeed);      // This delay time is close to top speed for this
  }
}






