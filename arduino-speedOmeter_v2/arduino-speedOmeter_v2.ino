// Created by: WestleyK
// Email: westleyk@nym.hush.com
// Date: Sep 22, 2019
// Repo: https://github.com/WestleyR/arduino-speedOmeter
//
// Version: 1.0.0-beta-5
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

// The pulse input.
#define SPEED_IN 14

// The output pins to the BCD decoder, in high to low.
// BCD_A_OUTPUT = first 7-segment display,
// BCD_B_OUTPUT = second 7-segment display,
// BCD_C_OUTPUT = last 7-segment display.
//  _   _   _
// |_| |_| |_|
// |_| |_|.|_|
//  A   B   C
//
// NOTE: The "dot" is always on.
// NOTE: The pins need to be from high
// to low, or revers the pin order to your
// BCD decoders.
int BCD_B_OUTPUT[] = {13, 12, 11, 10};
int BCD_C_OUTPUT[] = {9, 8, 7, 6};
int BCD_A_OUTPUT[] = {5, 4, 3, 2};

// A couple of timer vars.
bool rset = false;
bool waitPulse = false;
unsigned long start;

// This is the magic number, its the miles/per wheel
// rotation (per/input pulse). Needs to be adgusted
// as needed for your wheel size.
const float milesPerPulse = 0.000189394;

// writeBcdAOutput will take a array, and
// write it to the output pins for A.
void writeBcdAOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_A_OUTPUT[i], out[i]);
  }
}

// writeBcdBOutput will take a array, and
// write it to the output pins for B.
void writeBcdBOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_B_OUTPUT[i], out[i]);
  }
}

// writeBcdCOutput will take a array, and
// write it to the output pins for C.
void writeBcdCOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_C_OUTPUT[i], out[i]);
  }
}

// This converts a int (num), to a BCD array. Eg,
// 3 => [ 0 0 1 1 ], 5 => [ 0 1 0 1 ].
void getBcdArray(int ret[], int num) {
  int rindex = 0;
  for (int c = 3; c >= 0; c--) {
    int b = num >> c;

    if (b & 1) {
      ret[rindex] = 1;
    } else {
      ret[rindex] = 0;
    }
    rindex++;
  }
}

// writeAOutput will write to display A.
void writeAOutput(int num) {
  int outputArr[] = {0, 0, 0, 0};

  getBcdArray(outputArr, num);
  writeBcdAOutput(outputArr);
}

// writeAOutput will write to display B.
void writeBOutput(int num) {
  int outputArr[] = {0, 0, 0, 0};

  getBcdArray(outputArr, num);
  writeBcdBOutput(outputArr);
}

// writeAOutput will write to display C.
void writeCOutput(int num) {
  int outputArr[] = {0, 0, 0, 0};

  getBcdArray(outputArr, num);
  writeBcdCOutput(outputArr);
}

// splitInt will take a num, and split it into
// a array, but reversed. eg, 23 => [ 3 2 ].
void splitInt(int arr[], int num) {
  int i = 0;
  int b = 0;
  while (num > 0) {
    int mod = num % 10;
    arr[i] = mod;
    num = num / 10;
    i++;
    b++;
    if (b > 3) break;
  }
}

// writeAllOutput will take a num, and write
// it to all displays.
void writeAllOutput(int num) {
  int arr[] = {0, 0, 0};
  splitInt(arr, num);
  writeAOutput(arr[2]);
  writeBOutput(arr[1]);
  writeCOutput(arr[0]);
}

void setup() {
  // Declare BCD pins are outputs
  for (int i = 0; i < 4; i++) {
    pinMode(BCD_A_OUTPUT[i], OUTPUT);
    pinMode(BCD_B_OUTPUT[i], OUTPUT);
    pinMode(BCD_C_OUTPUT[i], OUTPUT);
  }
  pinMode(SPEED_IN, INPUT);
}

void calcSpeed(long double timePulse) {
  float currentSpeed = (milesPerPulse / timePulse) * 36000;
  writeAllOutput((int)currentSpeed);
}

unsigned long startTime;
unsigned long endTime;
unsigned long timerReset;
bool timerRunning = false;

void loop() {
  if (!timerRunning && digitalRead(SPEED_IN)) {
    startTime = millis();
    timerRunning = true;
    timerReset = millis();
    delay(10);
  }
  if (timerRunning && !digitalRead(SPEED_IN)) {
    endTime = millis();
    timerRunning = false;
    long double timeSpeed = (endTime - startTime) / (long double)1000;
    calcSpeed(timeSpeed);
    delay(200);
  }

  if ((millis() - timerReset) > 2000) {
    writeAllOutput(0);
    timerReset = millis();
  }
}
