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






