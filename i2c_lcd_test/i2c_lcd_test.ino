#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

LiquidCrystal_I2C lcd(0x27, 16, 4);


void setup() {
  lcd.init();                      // initialize the lcd
  //  lcd.clear();

  //lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
}


void loop() {
  lcd.clear();

  /*  for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, 0);
      lcd.print("#");
      delay(300);
    }
    for (int i = 0; i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.print("#");
      delay(300);
    }*/


  lcd.setCursor(6, 0);
  lcd.print("MPH");
  lcd.setCursor(0, 0);

  for (float i = 0; i < 99; i += 0.01) {
    lcd.setCursor(0, 0);
    lcd.print(i);

    delay(5);
  }



  /*lcd.setCursor(0, 0);
    lcd.print("hello world");
    //lcd.setCursor(1, 1);
    //lcd.print("Hello World");

    delay(3000);
    lcd.clear();
    delay(2000);*/

  delay(1000);

}
