//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Modified for FOUR HX711 modules
//-------------------------------------------------------------------------------------

uint8_t receiverMAC[] = {
  0xAC, 0x27, 0x6E, 0x81, 0x85, 0x64
};


typedef struct {
    uint8_t sensorNumber;
    float rawData;
    uint8_t pressureCategory;
} struct_message;

struct_message myData;


#include <HX711_ADC.h>
#include <math.h>
#include <stdio.h>
#include <esp_now.h>
#include <WiFi.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Pins:
const int HX711_dout_1 = 6;
const int HX711_sck_1  = 7;

const int HX711_dout_2 = 5;
const int HX711_sck_2  = 4;

const int HX711_dout_3 = 2;
const int HX711_sck_3  = 3;

const int HX711_dout_4 = 10;
const int HX711_sck_4  = 21;

// HX711 constructors
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1);
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2);
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3);
HX711_ADC LoadCell_4(HX711_dout_4, HX711_sck_4);

// EEPROM addresses (4 bytes each)
const int calVal_eepromAdress_1 = 0;
const int calVal_eepromAdress_2 = 4;
const int calVal_eepromAdress_3 = 8;
const int calVal_eepromAdress_4 = 12;

unsigned long t = 0;

void setup() {

  Serial.begin(57600);
  delay(10);

  Serial.println();
  Serial.println("Starting...");

  WiFi.mode(WIFI_STA);
  esp_now_init();

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  float calibrationValue_1;
  float calibrationValue_2;
  float calibrationValue_3;
  float calibrationValue_4;

  calibrationValue_1 = 200.0;
  calibrationValue_2 = 200.0;
  calibrationValue_3 = 200.0;
  calibrationValue_4 = 200.0;

#if defined(ESP8266) || defined(ESP32)
  //EEPROM.begin(512);
#endif

  //EEPROM.get(calVal_eepromAdress_1, calibrationValue_1);
  //EEPROM.get(calVal_eepromAdress_2, calibrationValue_2);
  //EEPROM.get(calVal_eepromAdress_3, calibrationValue_3);
  //EEPROM.get(calVal_eepromAdress_4, calibrationValue_4);

  LoadCell_1.begin();
  LoadCell_2.begin();
  LoadCell_3.begin();
  LoadCell_4.begin();

  //LoadCell_1.setReverseOutput();
  //LoadCell_2.setReverseOutput();
  //LoadCell_3.setReverseOutput();
  //LoadCell_4.setReverseOutput();

  unsigned long stabilizingtime = 2000;
  boolean _tare = true;

  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  byte loadcell_3_rdy = 0;
  byte loadcell_4_rdy = 0;

  while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy + loadcell_4_rdy) < 4) {

    if (!loadcell_1_rdy)
      loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);

    if (!loadcell_2_rdy)
      loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);

    if (!loadcell_3_rdy)
      loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);

    if (!loadcell_4_rdy)
      loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);
  }

  if (LoadCell_1.getTareTimeoutFlag())
    Serial.println("Timeout, check MCU>HX711 no.1 wiring");

  if (LoadCell_2.getTareTimeoutFlag())
    Serial.println("Timeout, check MCU>HX711 no.2 wiring");

  if (LoadCell_3.getTareTimeoutFlag())
    Serial.println("Timeout, check MCU>HX711 no.3 wiring");

  if (LoadCell_4.getTareTimeoutFlag())
    Serial.println("Timeout, check MCU>HX711 no.4 wiring");

  LoadCell_1.setCalFactor(calibrationValue_1);
  LoadCell_2.setCalFactor(calibrationValue_2);
  LoadCell_3.setCalFactor(calibrationValue_3);
  LoadCell_4.setCalFactor(calibrationValue_4);

  Serial.println("Startup is complete");
}

void loop() {

  static boolean newDataReady = 0;
  const int serialPrintInterval = 800;


  if (LoadCell_1.update()) newDataReady = true;
  LoadCell_2.update();
  LoadCell_3.update();
  LoadCell_4.update();

  if (newDataReady) {

    if (millis() > t + serialPrintInterval) {

      float a = abs(LoadCell_1.getData());
      float b = abs(LoadCell_2.getData());
      float c = abs(LoadCell_3.getData());
      float d = abs(LoadCell_4.getData());

    newDataReady = false;
    t = millis();

      myData.sensorNumber = 1;
      myData.rawData = a;

    if (a <= 15)
      myData.pressureCategory = 0;
    else if (a <= 120)
      myData.pressureCategory = 1;
    else
      myData.pressureCategory = 2;

esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));


    myData.sensorNumber = 2;
    myData.rawData = b;

    if (b <= 30)
       myData.pressureCategory = 0;
    else if (b <= 120)
      myData.pressureCategory = 1;
    else
      myData.pressureCategory = 2;

esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));
    myData.sensorNumber = 3;
    myData.rawData = c;

    if (c <= 30)
      myData.pressureCategory = 0;
    else if (c <= 120)
      myData.pressureCategory = 1;
  else
    myData.pressureCategory = 2;

esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));


      // ----- Load Cell 1 -----
      Serial.print("1,");
      Serial.print(a, 2);
      Serial.print(",");

      if (a <= 15)
        Serial.println(0);
      else if (a <= 120)
        Serial.println(1);
      else
        Serial.println(2);

      // ----- Load Cell 2 -----
      Serial.print("2,");
      Serial.print(b, 2);
      Serial.print(",");

      if (b <= 30)
        Serial.println(0);
      else if (b <= 120)
        Serial.println(1);
      else
        Serial.println(2);

      // ----- Load Cell 3 -----
      Serial.print("3,");
      Serial.print(c, 2);
      Serial.print(",");

      if (c <= 30)
        Serial.println(0);
      else if (c <= 120)
        Serial.println(1);
      else
        Serial.println(2);

      // ----- Load Cell 4 -----
      // Serial.print("4,");
      // Serial.print(d, 2);
      // Serial.print(",");

      // if (d <= 30)
      //   Serial.println(0);
      // else if (d <= 120)
      //   Serial.println(1);
      // else
      //   Serial.println(2);
    }
  }

  // Serial commands
  if (Serial.available() > 0) {

    char inByte = Serial.read();

    if (inByte == 't') {

      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
      LoadCell_3.tareNoDelay();
      LoadCell_4.tareNoDelay();

    }
  }

  if (LoadCell_1.getTareStatus())
    Serial.println("Tare load cell 1 complete");

  if (LoadCell_2.getTareStatus())
    Serial.println("Tare load cell 2 complete");

  if (LoadCell_3.getTareStatus())
    Serial.println("Tare load cell 3 complete");

  if (LoadCell_4.getTareStatus())
    Serial.println("Tare load cell 4 complete");
}
