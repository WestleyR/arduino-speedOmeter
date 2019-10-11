// Created by: WestleyK
// Email: westleyk@nym.hush.com
// Date: Oct 12, 2019
// Repo: https://github.com/WestleyR/arduino-speedOmeter
//
// Version: 1.0.0-beta-12
//
// License: The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//
//
// The full project is designed to take a input pulse (like from a spinning wheel)
// and convert those pulses to MPH, and display it on 2 (or 3) 7-segment LED
// displays.
//


#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 4);


// The pulse input.
#define SPEED_IN 7

// The refresh rate for the display, in milliseconds.
const unsigned long displayUpdateDelay = 500;

// A couple of timer vars.
unsigned long updateTimer = 0;
bool updateReset = false;

// This is the magic number, its the miles/per wheel
// rotation (per/input pulse). Needs to be adgusted
// as needed for your wheel size.
const float milesPerPulse = 0.000189394;

void setup() {
  // The pulse input.
  pinMode(SPEED_IN, INPUT_PULLUP);

  // TODO: tmp
  pinMode(LED_BUILTIN, OUTPUT);
  tone(LED_BUILTIN, 31);

  // Setup the lcd.
  lcd.init();
  lcd.backlight();
}

float currentMiles = 0;
unsigned long displayUpdateRate = 0;

void updateLcd(float currentSpeed) {
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

  unsigned long lastUpdate = millis() - displayUpdateRate;

  long double tmp = currentSpeed * lastUpdate;
  currentMiles += tmp;

  lcd.print(currentMiles / 3600000);
//  lcd.print(lastUpdate);

  displayUpdateRate = millis();
}

void calcSpeed(long double timePulse) {
  float currentSpeed = 0;
  if (timePulse > 0) {
    currentSpeed = (milesPerPulse / timePulse) * 3600000;
  }
  updateLcd(currentSpeed);
}

unsigned long breakTimerStart;

bool timerBreak(unsigned long timer) {
  if ((timer - breakTimerStart) >= (displayUpdateDelay * 2)) {
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
  while (!digitalRead(SPEED_IN)) {
    if (timerBreak(millis())) return (wheelRotate);
  }
  delay(2);
  breakTimerStart = millis();
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
