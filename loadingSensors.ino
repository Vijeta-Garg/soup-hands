#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// Safe pin designations for ESP32-C3
const int HX711_dout_1 = 2;  // Connect to HX711 no 1 dout pin
const int HX711_sck_1 = 3;   // Connect to HX711 no 1 sck pin
const int HX711_dout_2 = 6;  // Connect to HX711 no 2 dout pin
const int HX711_sck_2 = 7;   // Connect to HX711 no 2 sck pin
const int HX711_dout_3 = 5;  // Connect to HX711 no 3 dout pin
const int HX711_sck_3 = 4;   // Connect to HX711 no 3 sck pin
// const int HX711_dout_4 = 10; // Connect to HX711 no 4 dout pin
// const int HX711_sck_4 = 21;  // Connect to HX711 no 4 sck pin

// HX711 constructors (dout pin, sck pin)
HX711_ADC LoadCell_1(HX711_dout_1, HX711_sck_1); 
HX711_ADC LoadCell_2(HX711_dout_2, HX711_sck_2); 
HX711_ADC LoadCell_3(HX711_dout_3, HX711_sck_3); 
// HX711_ADC LoadCell_4(HX711_dout_4, HX711_sck_4); 

unsigned long t = 0;

void setup() {
  Serial.begin(115200); 
  delay(10);

  float calibrationValue_1 = 696.0; 
  float calibrationValue_2 = 733.0; 
  float calibrationValue_3 = 1.0;   // TODO: Replace with your actual calibration factor for Sensor 3
  // float calibrationValue_4 = 1.0;   // TODO: Replace with your actual calibration factor for Sensor 4

  LoadCell_1.begin();
  LoadCell_2.begin();
  LoadCell_3.begin();
  // LoadCell_4.begin();

  unsigned long stabilizingtime = 2000; 
  boolean _tare = true; 
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  byte loadcell_3_rdy = 0;
  // byte loadcell_4_rdy = 0;

  // Wait until all 4 load cells are stabilized and ready
  while ((loadcell_1_rdy + loadcell_2_rdy + loadcell_3_rdy) < 3) { 
    if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilizingtime, _tare);
    if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilizingtime, _tare);
    if (!loadcell_3_rdy) loadcell_3_rdy = LoadCell_3.startMultiple(stabilizingtime, _tare);
    // if (!loadcell_4_rdy) loadcell_4_rdy = LoadCell_4.startMultiple(stabilizingtime, _tare);
  }

  LoadCell_1.setCalFactor(calibrationValue_1); 
  LoadCell_2.setCalFactor(calibrationValue_2); 
  LoadCell_3.setCalFactor(calibrationValue_3); 
  // LoadCell_4.setCalFactor(calibrationValue_4); 
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 200; // Sent every 200ms to not flood the serial stream

  // Check for new data (updating any of them can flag new data ready)
  if (LoadCell_1.update()) newDataReady = true;
  if (LoadCell_2.update()) newDataReady = true;
  if (LoadCell_3.update()) newDataReady = true;
  // if (LoadCell_4.update()) newDataReady = true;

  // Get smoothed value and format for Node.js
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float a = abs(LoadCell_1.getData());
      float b = abs(LoadCell_2.getData());
      float c = abs(LoadCell_3.getData());
      // float d = abs(LoadCell_4.getData());

      // Determine hardness category for Sensor 1
      int category_1 = 2;
      if (a < 6) {
        category_1 = 0;
      } else if (a < 50) {
        category_1 = 1;
      }

      // Determine hardness category for Sensor 2
      int category_2 = 2;
      if (b < 10) {
        category_2 = 0;
      } else if (b < 25) {
        category_2 = 1;
      }

      // Determine hardness category for Sensor 3
      int category_3 = 2;
      if (c < 300) {
        category_3 = 0;
      } else if (c < 50000) {
        category_3 = 1;
      }

      // // Determine hardness category for Sensor 4
      // int category_4 = 2;
      // if (d < 9000) { // TODO: Tweak thresholds for Sensor 4 as needed
      //   category_4 = 0;
      // } else if (d < 15000) {
      //   category_4 = 1;
      // }
      
      // Send Load Cell 1 (Format: sensor,value,category)
      Serial.print("1,");
      Serial.print(a);
      Serial.print(",");
      Serial.println(category_1); 

      // Send Load Cell 2 (Format: sensor,value,category)
      Serial.print("2,");
      Serial.print(b);
      Serial.print(",");
      Serial.println(category_2); 

      // Send Load Cell 3 (Format: sensor,value,category)
      Serial.print("3,");
      Serial.print(c);
      Serial.print(",");
      Serial.println(category_3); 

      // Send Load Cell 4 (Format: sensor,value,category)
      // Serial.print("4,");
      // Serial.print(d);
      // Serial.print(",");
      // Serial.println(category_4); 

      newDataReady = 0;
      t = millis();
    }
  }

  // Receive 't' from serial (or Node.js) to tare all sensors
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') {
      LoadCell_1.tareNoDelay();
      LoadCell_2.tareNoDelay();
      LoadCell_3.tareNoDelay();
      // LoadCell_4.tareNoDelay();
    }
  }
}
