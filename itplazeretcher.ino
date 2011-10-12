/* Arduino laser cutter and controller
 * v0.1 by EDR - June 10, 2009 initial software (er0sentha1@gmail.com)
 * v0.2 by EDR - June 14, 2009 added menus
 * v0.3 by EDR - June 18, 2009 added calibration routines each step = .0002777"  1 inch = 3600 steps
 * Eric Rosenthal
 * 191 Beacon Hill Rd
 * Morganville, NJ 07751
 * 732-817-1720
 * www.creative-technology.net
 * Copyright 2009-2011 ALL RIGHTS RESERVED 
 * Arduino DV with ATmega 328
 *
 * v0.4 by Zach Taylor - July 18, 2009 (ztaylor4@gmail.com)
 *
 * also using code from:
 * Arduino G-code Interpreter
 * v1.0 by Mike Ellery - initial software (mellery@gmail.com)
 * v1.1 by Zach Hoeken - cleaned up and did lots of tweaks (hoeken@gmail.com)
 * v1.2 by Chris Meighan - cleanup / G2&G3 support (cmeighan@gmail.com)
 * v1.3 by Zach Hoeken - added thermocouple support and multi-sample temp readings. (hoeken@gmail.com)
 * v1.3x by Zach Taylor - update for Arduino 16 and Diecimila compatability (ztaylor4@gmail.com)
 * v1.4 by Eric Rosenthal -July 15, 2011 -  revision for laser cutter using hackvision board
 * v1.5 by Zevensuy Rodriguez - August 01,2011 - re-revision for laser cutter 
 
 */

#include "config.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
boolean mainMenuState = false;

#define upButton 5
#define downButton 4
#define leftButton 3
#define rightButton 2
#define okButton 10

int upButtonSwitch = 0;
long buttonTimer = 0;
int pressInterval = 100;

long curButton = 0;



#define COMMAND_SIZE 128
char gWord[COMMAND_SIZE];
byte serial_count;
int no_data = 0;
long idle_time;
boolean comment = false;
boolean bytes_received = false;


void setup(){

  //Pins for motor control
  // pinMode(X_DIR_PIN, OUTPUT);
  // pinMode(X_STEP_PIN, OUTPUT);
  // pinMode(Y_DIR_PIN, OUTPUT);
  // pinMode(Y_STEP_PIN, OUTPUT);
  // pinMode(Z_DIR_PIN, OUTPUT);
  // pinMode(Z_STEP_PIN, OUTPUT);
  init_steppers(); // Starts up the steppers in stepper control file
  pinMode (txPin, OUTPUT); // Software Serial to control input to the screen


  //Pins for button control
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(okButton, INPUT);

  //With the hackvision board you have to set pins to High and read for when the buttons go to ground
  digitalWrite(upButton, HIGH);
  digitalWrite(downButton, HIGH);
  digitalWrite(leftButton, HIGH);
  digitalWrite(rightButton, HIGH);
  digitalWrite(okButton, HIGH);


  mySerial.begin(9600);          // 9600 baud is LCD comm speed
  Serial.begin(9600);            // 9600 baud is USB comm speed	

  displayBootup();
  mainMenu(mainMenuState);
}


void loop(){

  curButton = readButton();
  if (curButton > 0) {
    Serial.println(curButton);
    motorStep(curButton,100);
  }
  //   motorStep(curButton,1000); //up
  //Serial.println(readButton());
  // motorStep(0,1000); //up  
  // motorStep(1,1000); //down
  // motorStep(2,1000); //right
  // motorStep(3,1000); //left
  //  motorStep(4,1000); //in
  //  motorStep(5,1000); // out

}

void displayBootup() {

  mySerial.print("?S0");
  delay(1500);
  LCD_clear();                       // clear the LCD
  LCD_line(0, " Arduino Controlled");      // displays on the LCD
  LCD_line(1, "   Laser Cutter    ");      // displays on the LCD
  LCD_line(2, "                   ");          // displays on the LCD
  LCD_line(3, "      Rev 0.5      ");          // displays on the LCD
  delay(3000);		                      // pause a few secs
  LCD_clear();
  mainMenuState = true;
}

void mainMenu(boolean _mainMenuState){

  if(_mainMenuState){
    LCD_line(0, "      Main menu     ");      // displays on the LCD
    LCD_line(1, "____________________");      // displays on the LCD
    LCD_line(2, "____________________");          // displays on the LCD
    LCD_line(3, "-___________________");          // displays on the LCD
  }

}

long readButton() {
  long buttonPressed = 0;
  if (millis() - buttonTimer > pressInterval){

    if(digitalRead(upButton) == LOW){
      Serial.println("upButton");
      buttonTimer = millis();
      buttonPressed = upButton;
    }
    if(digitalRead(downButton) == LOW){
      Serial.println("downButton");
      buttonTimer = millis();
      buttonPressed = downButton;
    }
    if(digitalRead(leftButton) == LOW){
      Serial.println("leftButton");
      buttonTimer = millis();
      buttonPressed = leftButton;
    }
    if(digitalRead(rightButton) == LOW){
      Serial.println("rightButton");
      buttonTimer = millis();
      buttonPressed = rightButton;
    }
    if(digitalRead(okButton) == LOW){
      Serial.println("okButton");
      buttonTimer = millis();
      buttonPressed = okButton;
    }

  }

  return buttonPressed;
}



int lineSpace = 0;
void listen() {
  char c;

  //read in characters if we got them.
  if (Serial.available() > 0)
  {
    c = Serial.read();
    no_data = 0;
    if (lineSpace == 0) {
      LCD_clearLine(3);
    }
    LCD_charPos(c, lineSpace, 3);
    lineSpace = (lineSpace+1)%20;
    //newlines are ends of commands.
    if (c != '\n')
    {
      // Start of comment - ignore any bytes received from now on
      if (c == '(')
        comment = true;

      // If we're not in comment mode, add it to our array.
      if (!comment)
      {
        gWord[serial_count] = c;
        serial_count++;
      }
      if (c == ')')
        comment = false; // End of comment - start listening again
    }

    bytes_received = true;
  }
  //mark no data if nothing heard for 100 milliseconds
  else
  {
    if ((millis() - idle_time) >= 100)
    {
      no_data++;
      idle_time = millis();
    }
  }

  //if theres a pause or we got a real command, do it
  if (bytes_received && (c == '\n' || no_data ))
  {
    //process our command!
    process_string(gWord, serial_count);

    //clear command.
    init_process_string();
  }

}








