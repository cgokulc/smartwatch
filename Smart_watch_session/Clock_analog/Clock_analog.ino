

#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h>

#include <WiFi.h>
#include "time.h"

const char* ssid     = "realme8";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 

char s_str[3];
char m_str[3];
char h_str[3];
char d_str[3];
char mn_str[3];
char y_str[3];


struct tm timenow;

void setup(void) {
   // Init display
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  u8g2.clearBuffer();
  u8g2.setCursor(0,32);
  u8g2.drawStr(25,32,"WiFi connected.");
  u8g2.sendBuffer();
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timenow)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(500);
  
}



void loop(void) {
    if(!getLocalTime(&timenow))
    {
      Serial.println("Failed to obtain time");
      return;
    }
    strcpy(s_str, u8x8_u8toa(timenow.tm_sec, 2));
    strcpy(h_str, u8x8_u8toa(timenow.tm_hour, 2));
    strcpy(m_str, u8x8_u8toa(timenow.tm_min, 2));
    strcpy(d_str, u8x8_u8toa(timenow.tm_mday, 2));
    strcpy(mn_str, u8x8_u8toa(timenow.tm_mon, 2));
    strcpy(y_str, u8x8_u8toa(timenow.tm_year, 2));
    Serial.println(timenow.tm_hour);/* convert m to a string with two digits */
    Serial.println(timenow.tm_min);
    Serial.println(timenow.tm_sec);
    u8g2.clearBuffer();                   // clear the internal memory
    u8g2.setFont(u8g2_font_logisoso20_tn);   // choose a suitable font
    u8g2.drawStr(0,25,h_str);    // write something to the internal memory
    u8g2.drawStr(35,25,":");
    u8g2.drawStr(50,25,m_str);
    u8g2.drawStr(85,25,":");
    u8g2.drawStr(100,25,s_str);
    u8g2.setFont(u8g2_font_t0_11_tf);
    switch(timenow.tm_wday)
    {
      case 0:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "SUN" );
        break;
      case 1:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "MON" );
        break;
      case 2:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "TUE" );
        break;
      case 3:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "WED" );
        break;
      case 4:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "THUR" );
        break;
      case 5:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "FRI" );
        break;
      case 6:
        u8g2.drawButtonUTF8(20 , 55, U8G2_BTN_INV, 0,  2,  2, "SAT" );
        break;
         
    }
    String date;
    date += d_str;
    date += "/";
    date += mn_str;
    date += "/";
    date += y_str;
    u8g2.drawButtonUTF8(60 , 55, U8G2_BTN_INV, 0,  2,  2, date.c_str() );
    u8g2.sendBuffer();                    // transfer internal memory to the display
    delay(100);
  
}
