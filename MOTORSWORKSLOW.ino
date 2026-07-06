const int AIN1 = 4;
const int AIN2 = 5;

const int BIN1 = 6;
const int BIN2 = 7;

void setup() {
  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcAttach(AIN1, 1000, 8);
  ledcAttach(BIN1, 1000, 8);

  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, LOW);
}

void loop() {
  ledcWrite(AIN1, 45);   // Motor A speed
  ledcWrite(BIN1, 45);   // Motor B speed
}