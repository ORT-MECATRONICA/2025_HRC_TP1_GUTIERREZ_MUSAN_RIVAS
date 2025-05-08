//VALENTIN RIVAS, PEDRO MUSAN, GUALTIER GUTIERREZ
#include <U8g2lib.h>
#include "DHT.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN 23
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define LED 26

void  printBMP_OLED(void );

void setup()
{
  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  Serial.println(F("OLED test"));
  Serial.println(F("DHTxx test!"));
  u8g2.begin();
  dht.begin();
}

void loop()
{
  printBMP_OLED();
  delay(2000);
}

void printBMP_OLED(void)
{
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  if (temp>=28){
    digitalWrite (LED,HIGH);
  }
  else{
    digitalWrite (LED,LOW);
  }
  Serial.println (temp);
  char stringtemp[10];
  u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  sprintf (stringtemp, "%.2f" , temp); ///convierto el valor float a string
  u8g2.drawStr(25, 25, stringtemp); // write something to the internal memory
  u8g2.sendBuffer();          // transfer interna memory to the display*/
}
