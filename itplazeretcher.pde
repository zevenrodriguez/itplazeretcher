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

//#define upButton 5
#define downButton 4
#define leftButton 3
#define rightButton 2
#define okButton 10

int upButtonSwitch = 0;
int upButton = 10;
void setup(){

  //Pins for motor control
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);

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

}


void loop(){

  //mainMenu(mainMenuState);
  
  if(digitalRead(upButton) == LOW){
    Serial.println("hello");
  }
  
  //Serial.println(readButton());
  //motorStep(0,1000);
  //motorStep(1,1000);
  //motorStep(2,1000);
  //motorStep(3,1000);
  //motorStep(4,1000);
  //motorStep(5,1000);

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
    LCD_line(3, "____________________");          // displays on the LCD
  }

}

long readButton() {
  long buttonsPressed = 0;
  if (digitalRead(upButton)== 1){
    //buttonTimer = millis();
    buttonsPressed = upButton;
  }
  
  return buttonsPressed;
}









