// Created by: WestleyK
// Email: westleyk@nym.hush.com
// Date: Sep 21, 2019
// Repo: https://github.com/WestleyR/arduino-speedOmeter
//
// Version: 1.0.0-beta-1
//
// License: The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

// BCD pins, these go to the bcd decoder/driver.
#define BCD_1 10
#define BCD_2 11
#define BCD_3 12
#define BCD_4 13

// The output pins to the BCD decoder, in revers order.
int BCD_OUTPUT[] = {13, 12, 11, 10};

void writeBcdOutput(int out[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(BCD_OUTPUT[i], out[i]);
  }
}

// This converts a int, to a BCD array. Eg,
// 3 => [ 0 0 1 1 ].
// NOTE: there will need to be 4 of these to
// get the final output.
int getBcdArray(int ret[], int msg) {
  for (int b = 0; b < 4; b++) {
    int rbyte = msg & (0x008 >> b);
    ret[b] = rbyte ? 1 : 0;
  }

  return (0);
}


void writeOutput(int num) {
  int outputArr[] = {0, 0, 0, 0};

  getBcdArray(outputArr, num);
  writeBcdOutput(outputArr);
}

void setup() {
  pinMode(BCD_1, OUTPUT);
  pinMode(BCD_2, OUTPUT);
  pinMode(BCD_3, OUTPUT);
  pinMode(BCD_4, OUTPUT);
}

void loop() {
  // As a test, write 0-9
  for (int i = 0; i < 10; i++) {
    writeOutput(i);
    delay(500);
  }
}
