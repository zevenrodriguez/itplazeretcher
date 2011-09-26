void LCD_clear() {
  LCD_text("?f");                       // clear the LCD
}

void LCD_line(int lineNumber, char lineText [20]) {
  LCD_lineNumber(lineNumber);
  LCD_text(lineText);
}

void LCD_text(char lineText[20]) {
  mySerial.print(lineText);  
}

void LCD_lineNumber(int lineNumber) {
  LCD_text("?x00?y");
  mySerial.print(lineNumber);
   mySerial.print("?BFF");         // set backlight to DF hex 255 dec
    delay(300);                     // pause to allow LCD EEPROM to program
    mySerial.print("?c0");          // turn cursor off 
  delay(300);                     // pause to allow LCD EEPROM to program

}
