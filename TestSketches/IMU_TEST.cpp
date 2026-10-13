#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h> // Swap with HMC5883L library if using older genuine chips

// Define ESP32-S3 default I2C pins
#define I2C_SDA 18
#define I2C_SCL 17

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
QMC5883LCompass compass;

bool mpuOk = false, bmpOk = false;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setTimeOut(100);

  mpuOk = mpu.begin();
  Serial.println(mpuOk ? "MPU6050 Found!" : "MPU6050 FAILED");

  // bypass writes here...
  Wire.beginTransmission(0x68); // MPU6050 Address
  Wire.write(0x6B);             // PWR_MGMT_1 register
  Wire.write(0x00);             // Wake up MPU6050
  Wire.endTransmission();

  Wire.beginTransmission(0x68); 
  Wire.write(0x37);             // INT_PIN_CFG register
  Wire.write(0x02);             // Set I2C Bypass Enable to true
  Wire.endTransmission();
  delay(100);

  compass.init();

  bmpOk = bmp.begin();
  Serial.println(bmpOk ? "BMP180 Found!" : "BMP180 FAILED");
}

void loop() {
  if (mpuOk) {
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    Serial.print("Accel X: "); Serial.print(a.acceleration.x);
  }
  if (bmpOk) {
    Serial.print(" | Temp: "); Serial.print(bmp.readTemperature());
  }
  Serial.println();
  delay(500);
}
