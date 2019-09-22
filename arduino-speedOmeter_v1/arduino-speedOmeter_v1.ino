// Created by: WestleyK
// Email: westleyk@nym.hush.com
// Date: Sep 21, 2019
// Repo: https://github.com/WestleyR/arduino-speedOmeter
//
// Version: 1.0.0-beta-2
//
// License: The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//
//
// This project is designed to take a input pulse (like from a spinning wheel)
// and convert those pulses to MPH, and display it on 2 (or 3) 7-segment LED
// displays.
//


// The output pins to the BCD decoder, in revers order.
// BCD_A_OUTPUT = first 7-segment display,
// BCD_B_OUTPUT = second 7-segment display.
int BCD_A_OUTPUT[] = {13, 12, 11, 10};
int BCD_B_OUTPUT[] = {9, 8, 7, 6};

void writeBcdAOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_A_OUTPUT[i], out[i]);
  }
}

void writeBcdBOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_B_OUTPUT[i], out[i]);
  }
}

// This converts a int, to a BCD array. Eg,
// 3 => [ 0 0 1 1 ].
int getBcdArray(int ret[], int msg) {
  for (int b = 0; b < 4; b++) {
    int rbyte = msg & (0x008 >> b);
    ret[b] = rbyte ? 1 : 0;
  }

  return (0);
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

// splitInt will take a num, and split it into
// a array.
void splitInt(int arr[], int num) {
  int i = 0;
  while (num > 0) {
    int mod = num % 10;
    arr[i] = mod;
    num = num / 10;
    i++;
  }
}

// writeAllOutput will take a num, and write
// it to all displays.
void writeAllOutput(int num) {
  if (num > 9) {
    int arr[2];

    splitInt(arr, num);
    writeBOutput(arr[0]);
    writeAOutput(arr[1]);
  } else {
    // num is less then 10, can only use the last digit to print it.
    writeBOutput(num);
    writeAOutput(0);
  }
}

void setup() {
  // Declare BCD pins are outputs
  for (int i = 0; i < 4; i++) {
    pinMode(BCD_A_OUTPUT[i], OUTPUT);
    pinMode(BCD_B_OUTPUT[i], OUTPUT);
  }
}

void loop() {
  // As a test, write 0-9
  for (int i = 0; i < 90; i++) {
    writeAllOutput(i);
    delay(100);
  }
}
