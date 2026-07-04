#include <ESP32Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const int servo1Pin = D0;
const int servo2Pin = D1;
const int servo3Pin = D2;

void setup() {
    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);
    servo3.setPeriodHertz(50);

    servo1.attach(servo1Pin, 500, 2400);
    servo2.attach(servo2Pin, 500, 2400);
    servo3.attach(servo3Pin, 500, 2400);


    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
}

void loop() {
    servo1.write(500);
    delay(500);
    servo1.write(90);
    delay(500);


    servo2.write(500);
    delay(500);
    servo2.write(90);
    delay(500);

    servo3.write(500);
    delay(500);
    servo3.write(90);
    delay(500);
}