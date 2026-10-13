#include <Wire.h>
#include "paj7620.h"

// Define custom I2C pins for your specific ESP32-S3 board if defaults don't work
#define I2C_SDA 17
#define I2C_SCL 18

void setup() {
  uint8_t error = 0;

  Serial.begin(115200);
  //while (!Serial) {
    //delay(10); // Wait for Serial Monitor to connect (needed for ESP32-S3 Native USB)
  //}

  Serial.println("\n--- PAJ7620 Gesture Sensor Initialization ---");

  // Initialize I2C with customized pins for ESP32-S3
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the PAJ7620 registers
  error = paj7620Init();
  if (error) {
    Serial.print("Initialisation ERROR, code: ");
    Serial.println(error);
    while (true) { 
      delay(1000); 
    } // Halt execution if sensor is not found
  } else {
    Serial.println("Initialisation OK. Wave your hand in front of the sensor...");
  }
}

void loop() {
  uint8_t data = 0;
  uint8_t error = 0;

  // Read Bank_0_Reg_0x43 for the primary gesture results
  error = paj7620ReadReg(0x43, 1, &data);

  if (!error) {
    if (data) {
      switch (data) {
        case GES_RIGHT_FLAG:
          delay(100); // Small debounce delay
          Serial.println("Gesture Detected: [ RIGHT ] --->");
          break;
          
        case GES_LEFT_FLAG:
          delay(100);
          Serial.println("Gesture Detected: <--- [ LEFT ]");
          break;
          
        case GES_UP_FLAG:
          delay(100);
          Serial.println("Gesture Detected: [ UP ] /\\");
          break;
          
        case GES_DOWN_FLAG:
          delay(100);
          Serial.println("Gesture Detected: [ DOWN ] \\/");
          break;
          
        case GES_FORWARD_FLAG:
          Serial.println("Gesture Detected: [ FORWARD ]");
          delay(100);
          break;
          
        case GES_BACKWARD_FLAG:
          Serial.println("Gesture Detected: [ BACKWARD ]");
          delay(100);
          break;
          
        case GES_CLOCKWISE_FLAG:
          Serial.println("Gesture Detected: [ CLOCKWISE ]");
          break;
          
        case GES_COUNT_CLOCKWISE_FLAG:
          Serial.println("Gesture Detected: [ COUNTER-CLOCKWISE ]");
          break;
          
        default:
          break;
      }
    }
  } else {
    Serial.println("Error reading register data.");
  }
  
  delay(100); // Polling rate limit
}
