
#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 
Adafruit_MPU6050 mpu;

void setup() {
  // put your setup code here, to run once:
  pinMode(32,INPUT);
  pinMode(33,INPUT);
  pinMode(34,INPUT);
  u8g2.begin();   
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

sensors_event_t a, g, temp;
int x_adj, y_adj;

void loop()
{
  leveler();
  Serial.println("exited");
}

void leveler() {
  // put your main code here, to run repeatedly:
  while(digitalRead(34))
  {
    mpu.getEvent(&a, &g, &temp);
    x_adj = map(a.acceleration.x*10,100.0,-100.0,0,63);
    y_adj = map(a.acceleration.y*10,100.0,-100.0,0,127);
    u8g2.clearBuffer(); 
    u8g2.drawRFrame(58,0,10,63,3);
    u8g2.drawRFrame(0,26,127,10,3);
    if(y_adj>63)
      u8g2.drawBox(63,26,y_adj-63,10);
    else
      u8g2.drawBox(y_adj,26,63-y_adj,10);
    if(x_adj>31)
      u8g2.drawBox(58,31,10,x_adj-31);
    else
      u8g2.drawBox(58,x_adj,10,31-x_adj);
    u8g2.sendBuffer(); 
  }
  u8g2.clear(); 
}
