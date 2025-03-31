// ------------------------
// USER-DEFINED SETTINGS
// ------------------------

const float controlFrequencyHz = 30.0;
const unsigned long cyclePeriod = 1000000.0 / controlFrequencyHz;

float patternDuty[4]  = {0, 0, 0, 0};  // pins 2–5
float pressureDuty[4] = {0, 0, 0, 0};  // pins 8–11

// ------------------------
// SYSTEM CONSTANTS
// ------------------------

const int patternPins[4]  = {2, 3, 4, 5};
const int pressurePins[4] = {8, 9, 10, 11};

unsigned long lastCycleStart = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(patternPins[i], OUTPUT);
    pinMode(pressurePins[i], OUTPUT);
    digitalWrite(patternPins[i], LOW);
    digitalWrite(pressurePins[i], LOW);
  }

  Serial.begin(9600);
  Serial.print("System ready. Control frequency: ");
  Serial.print(controlFrequencyHz);
  Serial.println(" Hz");
  Serial.println("Commands:");
  Serial.println("  f              → pressureDuty = 1.0");
  Serial.println("  r              → pressureDuty = 0.0");
  Serial.println("  p x1 x2 x3 x4  → patternDuty = {x1, x2, x3, x4}");
}

void loop() {
  unsigned long now = micros();

  // Handle serial input
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); // read full line

    cmd.trim(); // remove whitespace
    if (cmd == "f") {
      for (int i = 0; i < 4; i++) pressureDuty[i] = 1.0;
      Serial.println("pressureDuty set to 1.0");
    } 
    else if (cmd == "r") {
      for (int i = 0; i < 4; i++) pressureDuty[i] = 0.0;
      Serial.println("pressureDuty set to 0.0");
    } 
    else if (cmd.startsWith("p ")) {
      // parse 4 float values
      int index = 0;
      int lastSpace = 1;
      for (int i = 0; i < 4; i++) {
        int nextSpace = cmd.indexOf(' ', lastSpace);
        if (nextSpace == -1 && i < 3) break;
        String valStr = cmd.substring(lastSpace, nextSpace == -1 ? cmd.length() : nextSpace);
        float val = valStr.toFloat();
        val = constrain(val, 0.0, 1.0);
        patternDuty[i] = val;
        lastSpace = nextSpace + 1;
      }
      Serial.print("patternDuty updated: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(patternDuty[i], 2);
        Serial.print(" ");
      }
      Serial.println();
    } 
    else {
      Serial.println("Invalid command.");
    }
  }

  // Start of PWM cycle
  if (now - lastCycleStart >= cyclePeriod) {
    lastCycleStart = now;

    for (int i = 0; i < 4; i++) {
      if (patternDuty[i] > 0.0) digitalWrite(patternPins[i], HIGH);
      if (pressureDuty[i] > 0.0) digitalWrite(pressurePins[i], HIGH);
    }
  }

  // End of duty time → set pins LOW
  for (int i = 0; i < 4; i++) {
    if (patternDuty[i] < 1.0 && (micros() - lastCycleStart) >= patternDuty[i] * cyclePeriod) {
      digitalWrite(patternPins[i], LOW);
    }
    if (pressureDuty[i] < 1.0 && (micros() - lastCycleStart) >= pressureDuty[i] * cyclePeriod) {
      digitalWrite(pressurePins[i], LOW);
    }
  }
}
