#include <U8g2lib.h>

#define sw 34

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();   
  Serial.begin(115200);
  u8g2.setFontPosCenter();
  pinMode(34,INPUT);
}

int stpwch_run=0, stpwch_reset=0,stpwch_start=0;
long s_time_now;
int s_ms, s_s, s_m;
int s_c=0; 
char s_ms_str[3], s_s_str[3], s_m_str[3];
String s_msg;


void loop() {
  // put your main code here, to run repeatedly:
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_logisoso20_tn); 
  if(digitalRead(34))
  {
    s_c = 1;
  }
  else
  {
    if(s_c)
    {
      s_c = 0;
      if(stpwch_reset)
      {
        if(stpwch_run)
        {
          
        }
        else
        {
          stpwch_reset=0;
        }
      }
      else
      {
        if(stpwch_run)
        {
          stpwch_run=0;
          stpwch_reset=1;
        }
        else
        {
          time_now = millis();
          stpwch_run=1;
        }
      }
    }
  }

  if(stpwch_run)
  {
    if(stpwch_reset)
    {
      
    }
    else
    {
      s_m=(millis()-s_time_now)/60000;
      s_s=((millis()-s_time_now)%60000)/1000;
      s_ms=((millis()-s_time_now)%1000)/10;
      s_msg="Press to  STOP";
    }
  }
  else
  {
    if(stpwch_reset)
    {
      s_msg="Press to RESET";
    }
    else
    {
      s_m=0;
      s_s=0;
      s_ms=0;
      s_msg="Press to START";
    }
  }
  
  strcpy(s_s_str, u8x8_u8toa(s_s, 2));
  strcpy(s_ms_str, u8x8_u8toa(s_ms, 2));
  strcpy(s_m_str, u8x8_u8toa(s_m, 2));
  u8g2.setFont(u8g2_font_logisoso20_tn);   // choose a suitable font
  u8g2.drawStr(0,25,s_m_str);    // write something to the internal memory
  u8g2.drawStr(35,25,":");
  u8g2.drawStr(50,25,s_s_str);
  u8g2.drawStr(85,25,":");
  u8g2.drawStr(100,25,s_ms_str);
  u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(0,45,s_msg.c_str());
  u8g2.sendBuffer();
}
