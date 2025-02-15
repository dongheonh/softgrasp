// Pin configuration
int IN1 = 2;  // Solenoid 1 control pin
int IN2 = 3;  // Solenoid 2 control pin
int IN3 = 4;
int IN4 = 5;
int IN5 = 6;
int IN6 = 7;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
}

void loop() {
  bool t = true;
  bool f = false;
  //example configuration
  //controlActuators(f, t, f, t, f, t, 2000); //Atm for all, 2000 ms
  //controlActuators(t, f, f, t, f, t, 4000); //First actuator actuated, Atm for rest of them, 4000 ms
  //controlActuators(t, f, t, f, f, t, 2000); //First and second actuator actuated, Atm for third actuator, 2000ms
  //controlActuators(t, f, t, f, t, f, 4000); //Actuate all

  for (int i = 0; i <= 1000; i++) {
    controlActuators(f, t, f, t, f, t, 500); //Atm for all
    controlActuators(t, f, t, f, t, f, 500); //width actuation
  }
}

void controlActuators(bool in1, bool in2, bool in3, bool in4, bool in5, bool in6, int delayTime) {
  digitalWrite(IN1, in1 ? HIGH : LOW);
  digitalWrite(IN2, in2 ? HIGH : LOW);
  digitalWrite(IN3, in3 ? HIGH : LOW);
  digitalWrite(IN4, in4 ? HIGH : LOW);
  digitalWrite(IN5, in5 ? HIGH : LOW);
  digitalWrite(IN6, in6 ? HIGH : LOW);
  delay(delayTime);
}