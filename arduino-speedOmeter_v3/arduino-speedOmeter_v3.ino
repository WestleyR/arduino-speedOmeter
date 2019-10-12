// Created by: WestleyR
// Email: westleyR@nym.hush.com
// Date: Oct 12, 2019
// Repo: https://github.com/WestleyR/arduino-speedOmeter
//
// Version: 1.0.0-beta-16
//
// License: The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//
//
// This project will take a pulse (from a spinning wheel) and
// convert that to MPH (miles per hour), and also has a odometer
// to print the total trip.
//

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the lcd and address.
LiquidCrystal_I2C lcd(0x27, 16, 4);

// The pulse input from the wheel.
#define SPEED_IN 7

// This is the magic number, its the miles/per (2)? wheel
// rotations (per/input pulse). Needs to be adjusted as
// needed for your wheel size. ~This is for 8 inch wheel.~
const float milesPerPulse = 0.000189394;

// The refresh rate for the display, in milliseconds.
// Recommend you leave as is.
const unsigned long displayUpdateDelay = 500;

// A couple of timer vars. Do not change!
unsigned long updateTimer = 0;
bool updateReset = false;

// Display update timers. Do not change!
unsigned long breakTimerStart;
float currentMiles = 0;
unsigned long displayUpdateRate = 0;

void setup() {
  // The pulse input.
  pinMode(SPEED_IN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the lcd.
  lcd.init();
  lcd.backlight();
}

// updateLcd will update the lcd, without loosing
// the odometer trip.
void updateLcd(float currentSpeed) {
  // Clear the lcd before printing anything,
  // this will also clear any stray chars.
  lcd.clear();

  // Print the MPH.
  lcd.setCursor(6, 0);
  lcd.print("MPH");
  lcd.setCursor(0, 0);
  lcd.print(currentSpeed);

  // Print the total miles.
  lcd.setCursor(6, 1);
  lcd.print("TOT");
  lcd.setCursor(0, 1);

  // Get the live update rate, use that as our time references.
  unsigned long lastUpdate = millis() - displayUpdateRate;
  long double mileSpeed = currentSpeed * lastUpdate;
  currentMiles += mileSpeed;

  // Divide the currentMiles by 3.6e+6 to get total miles,
  // sinse currentMiles is in milliseconds.
  // TODO: not sure why to *2 on pro-mini, not necessary
  // on Arduino Mega...
  lcd.print((currentMiles / 3600000) * 2);

  // Reset the update counter.
  displayUpdateRate = millis();
}

void calcSpeed(long double timePulse) {
  float currentSpeed = 0;
  if (timePulse > 0) {
    currentSpeed = (milesPerPulse / timePulse) * 3600000;
  }
  updateLcd(currentSpeed);
}

bool timerBreak(unsigned long timer) {
  if ((timer - breakTimerStart) >= (displayUpdateDelay * 2.5)) {
    return (true);
  }
  return (false);
}

// updateWheel will return the time in milliseconds
// it takes for the wheel to rotate.
double updateWheel() {
  double wheelRotate = 0;
  breakTimerStart = millis();
  while (digitalRead(SPEED_IN)) {
    if (timerBreak(millis())) return (wheelRotate);
  }
  unsigned long wheelTime = micros();
  delay(2);
  breakTimerStart = millis();
  pinMode(LED_BUILTIN, HIGH);
  while (!digitalRead(SPEED_IN)) {
    if (timerBreak(millis())) return (wheelRotate);
  }
  delay(2);
  breakTimerStart = millis();
  pinMode(LED_BUILTIN, LOW);
  while (digitalRead(SPEED_IN)) {
    if (timerBreak(millis())) return (wheelRotate);
  }
  delay(1);
  wheelRotate = (micros() - wheelTime) / 1000;

  return (wheelRotate);
}

void loop() {
  if (!updateReset) {
    updateTimer = millis();
    updateReset = true;
  }

  // Check if its time to update the display.
  if ((millis() - updateTimer) >= displayUpdateDelay) {
    double wheelSpeed = updateWheel();
    calcSpeed(wheelSpeed);
    updateReset = false;
  }
}
