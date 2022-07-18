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
#define sw 34

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
  pinMode(sw,INPUT);
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
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFontPosCenter();
  u8g2.setFont(u8g2_font_t0_18_tf);
  u8g2.drawStr(10,10,"Balance Game");
  u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(0,25,"Tilt to move the ball,");
  u8g2.drawStr(15,42,"Collect squares");
  u8g2.drawStr(0,58,"Press button to start");
  u8g2.sendBuffer();
  if(!digitalRead(sw))
    game();
}

void game()
{
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(10,5,"Hold flat to ground");
  u8g2.drawStr(10,15,"Press button to exit");
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.drawStr(55,45,"3");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(10,5,"Hold flat to ground");
  u8g2.drawStr(10,15,"Press button to exit");
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.drawStr(55,45,"2");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(10,5,"Hold flat to ground");
  u8g2.drawStr(10,15,"Press button to exit");
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.drawStr(55,45,"1");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_22_tf);
  u8g2.drawStr(50,31,"GO");
  u8g2.sendBuffer();
  delay(1000);

  int pos_x = 63;
  int pos_y = 31;
  float sensitivity   =  0.5;
  int sq_x, sq_y, points=0, clctd = 1;
  int gm_ovr = 0;
  int dis_x,dis_y;
  
  sensors_event_t a, g, temp;
  
  while(digitalRead(sw)&(!gm_ovr))
  {
     u8g2.clearBuffer();
     u8g2.drawDisc(pos_x,pos_y,3);
     u8g2.drawFrame(0,0,127,63);
     if(clctd)
     {
      sq_x = random(3,124);
      sq_y = random(3,60);
      clctd = 0;
     }
     u8g2.drawBox(sq_x,sq_y,2,2);
     mpu.getEvent(&a, &g, &temp);
     pos_x += int( (-1*a.acceleration.y)*(sensitivity+points*0.1) );
     pos_y += int( (-1*a.acceleration.x)*(sensitivity+points*0.1) );
     u8g2.sendBuffer();
     dis_x = abs(pos_x-sq_x);
     dis_y = abs(pos_y-sq_y);
     if(dis_x<=5 & dis_y<=5)
     {
      clctd = 1;
      points++;
     }
     if(pos_x<3 | pos_y<3 | pos_x>124 | pos_y>60)
       gm_ovr = 1;
     Serial.println(points);
  }
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_18_tf);
  u8g2.drawButtonUTF8(63, 10, U8G2_BTN_INV|U8G2_BTN_HCENTER, 0,  2,  2, "GAME OVER!" );
  u8g2.setFont(u8g2_font_inb38_mn);
  u8g2.drawStr(32,45,u8x8_u8toa(points,2));
  u8g2.sendBuffer();
  while(digitalRead(sw))
  {
    delay(20);
  }
  delay(500);
}
