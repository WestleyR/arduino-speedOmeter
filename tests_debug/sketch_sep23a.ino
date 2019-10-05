



void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
}

unsigned long startTime;
unsigned long endTime;
float duration;
bool timerRunning = false;

const float milesPerPulse = 0.000189394;

void calcSpeed(long double timePulse) {
  float currentSpeed = (milesPerPulse / timePulse) * 3600000;
  Serial.println((float)currentSpeed);
}


const unsigned long displayUpdateDelay = 750;

unsigned long updateTimer = 0;
bool updateReset = false;

double updateWheel() {
  double wheelRotate = 0;
  while (digitalRead(7));
  if (!digitalRead(7)) {
    unsigned long wheelTime = micros();
    while (!digitalRead(7));
    delay(1);
    while (digitalRead(7));
    wheelRotate = (micros() - wheelTime) / 1000;
  }
  return (wheelRotate);
}

void loop() {
  if (!updateReset) {
    updateTimer = millis();
    updateReset = true;
  }


  if ((millis() - updateTimer) >= displayUpdateDelay) {
    double wheelSpeed = updateWheel();
    calcSpeed(wheelSpeed);
    Serial.print("Wheel timer: ");
    Serial.println(wheelSpeed);

    updateReset = false;
  }

  //Serial.println("MULTIPLEX!");

}
