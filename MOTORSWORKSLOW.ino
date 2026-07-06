#include <WiFi.h>
#include <esp_now.h>

const int AIN1 = 6;
const int AIN2 = 7;

const int BIN1 = 2;
const int BIN2 = 3;

typedef struct {
    uint8_t sensorNumber;
    float rawData;
    uint8_t pressureCategory;
} struct_message;

struct_message incomingData;

void onReceive(const esp_now_recv_info_t *info,
               const uint8_t *data,
               int len)
{
    if (len == sizeof(incomingData)) {
        memcpy(&incomingData, data, sizeof(incomingData));

        Serial.print(incomingData.sensorNumber);
        Serial.print(",");
        Serial.print(incomingData.rawData, 2);
        Serial.print(",");
        Serial.println(incomingData.pressureCategory);
    }
}

void setup() {
  Serial.begin(115200);
  // delay(1000);


  Serial.println("hello!");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); 
  delay(100);

  Serial.println(WiFi.macAddress());

  esp_now_init();
  esp_now_register_recv_cb(onReceive); 

  pinMode(AIN2, OUTPUT);
  pinMode(BIN2, OUTPUT);

  ledcAttach(AIN1, 1000, 8);
  ledcAttach(BIN1, 1000, 8);

  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, LOW);
}

void loop() {
//   if(Serial.available() > 0){
//      incomingByte = Serial.read();

//     Serial.print("I received: ");
//     Serial.println(incomingByte, DEC);

// }

   // Motor A
    ledcWrite(AIN1, 90);
    ledcWrite(BIN1, 0);
   delay(1000);
//   // Stop both
  ledcWrite(AIN1, 0);
  ledcWrite(BIN1, 0);
  delay(250);

//   // // Motor B
  ledcWrite(AIN1, 0);
  ledcWrite(BIN1, 90);
delay(1000);

   // // Stop both
  ledcWrite(AIN1, 0);
  ledcWrite(BIN1, 0);
  delay(250);
}
