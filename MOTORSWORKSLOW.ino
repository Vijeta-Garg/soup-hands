

const int AIN1 = 4;
const int AIN2 = 5;

const int BIN1 = 2;
const int BIN2 = 3;

void setup() {
  Serial.begin(115200);

  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcAttach(AIN1, 1000, 8);
  ledcAttach(BIN1, 1000, 8);

  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, LOW);
}

void loop() {
  if(Serial.available() > 0){
    incomingByte = Serial.read();

    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }

  // Motor A
   ledcWrite(AIN1, 90);
   ledcWrite(BIN1, 0);
  delay(1000);
  // Stop both
 ledcWrite(AIN1, 0);
 ledcWrite(BIN1, 0);
 delay(250);

  // // Motor B
 ledcWrite(AIN1, 0);
 ledcWrite(BIN1, 90);
 delay(1000);

  // // Stop both
 ledcWrite(AIN1, 0);
 ledcWrite(BIN1, 0);
 delay(250);
}
