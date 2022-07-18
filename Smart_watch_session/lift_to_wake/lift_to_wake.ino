#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <U8g2lib.h>



#include <WiFi.h>
#include "time.h"
#include "math.h"

Adafruit_MPU6050 mpu;

int c = 0;
#define PI 3.141592

const char* ssid     = "realme8";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 

char d_str[3];
char mn_str[3];
char y_str[3];


struct tm timenow;

void setup() {
  u8g2.begin();   
  u8g2.setFontPosCenter();
  u8g2.setFont(u8g2_font_t0_11_tf);
  
  // Connect to Wi-Fi
  Serial.begin(115200);
  u8g2.clearBuffer(); 
  u8g2.setCursor(0,32);
  u8g2.drawStr(0,32,"Connecting to ");
  u8g2.drawStr(83,32,ssid);
  u8g2.sendBuffer();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  u8g2.clearBuffer();
  u8g2.setCursor(0,32);
  u8g2.drawStr(25,32,"WiFi connected.");
  u8g2.sendBuffer();
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timenow))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(500);
  

  // Try to initialize!
  if (!mpu.begin()) {
    //Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //Serial.println("");
  
  delay(100);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  if(c == 0)
  {
    if(a.acceleration.z>12.0)
      c = 1;
  }
  else if(c == 1)
    if(a.acceleration.z<8.0)
    {
      c = 0;
      display_time(); 
    }
  Serial.println(c);
}

void display_time()
{
  if(!getLocalTime(&timenow))
   {
     Serial.println("Failed to obtain time");
     return;
   }  
   u8g2.clearBuffer();   
   u8g2.setDrawColor(1);
   u8g2.drawCircle(63,31,31,U8G2_DRAW_ALL);
   int ii = timenow.tm_hour>=12 ? (timenow.tm_hour>=15 ? timenow.tm_hour+(timenow.tm_min/60.0)-15.0 : timenow.tm_hour+(timenow.tm_min/60.0)-3.0) : (timenow.tm_hour>=3 ? timenow.tm_hour+(timenow.tm_min/60.0)-3.0 : timenow.tm_hour+(timenow.tm_min/60.0)+9.0) ;
   int i = timenow.tm_min>=15 ? timenow.tm_min-15 : timenow.tm_min+45;
   int s = timenow.tm_sec>=15 ? timenow.tm_sec-15 : timenow.tm_sec+45;
   u8g2.drawLine(63,31,floor(63+15*cos(30.0*PI*ii/180.0)),floor(31+15*sin(30.0*PI*ii/180.0))); 
   u8g2.drawLine(63,31,floor(63+23*cos(6.0*PI*i/180.0)),floor(31+23*sin(6.0*PI*i/180.0)));
   u8g2.drawLine(63,31,floor(63+30*cos(6.0*PI*s/180.0)),floor(31+30*sin(6.0*PI*s/180.0)));
   u8g2.sendBuffer();
   u8g2.clear();
}
