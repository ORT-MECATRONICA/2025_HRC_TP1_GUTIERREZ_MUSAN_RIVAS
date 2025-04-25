#include <U8g2lib.h>
#include <Wire.h>
#include <DHT.h>

#define DHTPIN 23
#define DHTTYPE DHT11
#define BTN1 35
#define BTN2 34
#define LED 25

#define P1 80
#define ESPERA1 81
#define P2 82
#define ESPERA2 83

#define ESPERANDO 84
#define PRESIONADO 85
#define ESPERANDO_LIBERACION 86

DHT dht(DHTPIN, DHTTYPE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int estadoActual = P1;
int umbral = 28;

int estadoBTN1 = ESPERANDO;
int estadoBTN2 = ESPERANDO;


void setup() {
  Serial.begin(9600);
  dht.begin();
  u8g2.begin();

  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  float temp = dht.readTemperature();
  if (isnan(temp)){
    return;
  }

  switch (estadoActual) {
    case P1:
      pantalla1(temp);
      if (temp>umbral){
  digitalWrite(LED,HIGH);
}
      if (temp<umbral){
  digitalWrite(LED,LOW);
}
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        estadoActual = ESPERA1;
      }
      break;

    case ESPERA1:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        estadoActual = P2;
      }
      break;

    case P2:
      manejarBotones();
      pantalla2();
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        estadoActual = ESPERA2;
      }
      break;

    case ESPERA2:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        estadoActual = P1;
      }
      break;
  }
}

void manejarBotones() {
  // maquina de estado para BTN1 
  switch (estadoBTN1) {
    case ESPERANDO:
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == HIGH) {
        estadoBTN1 = PRESIONADO;
      }
      break;
    case PRESIONADO:
      if (digitalRead(BTN1) == HIGH) {
        umbral++;
        estadoBTN1 = ESPERANDO;
      } else if (digitalRead(BTN2) == LOW) {
        // Si se presiona SW2 mientras BTN1 está presionado, cancelar acción
        estadoBTN1 = ESPERANDO;
      }
      break;
  }

  // maquina de estado para BTN2 
  switch (estadoBTN2) {
    case ESPERANDO:
      if (digitalRead(BTN2) == LOW && digitalRead(BTN1) == HIGH) {
        estadoBTN2 = PRESIONADO;
      }
      break;
    case PRESIONADO:
      if (digitalRead(BTN2) == HIGH) {
        umbral--;
        estadoBTN2 = ESPERANDO;
      } else if (digitalRead(BTN1) == LOW) {
        // Si se presiona BTN1 mientras SW2 está presionado, cancelar acción
        estadoBTN2 = ESPERANDO;
      }
      break;
  }
}


void pantalla1(float temperatura) {
  char stringTemp[10], stringUmbral[10];
  dtostrf(temperatura, 4, 1, stringTemp);
  sprintf(stringUmbral, "%d C", umbral);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(10, 20, "temp:");
  u8g2.drawStr(70, 20, stringTemp);
  u8g2.drawStr(10, 40, "umbral:");
  u8g2.drawStr(90, 40, stringUmbral);
  u8g2.sendBuffer();
}

void pantalla2() {
  char stringUmbral[10];
  sprintf(stringUmbral, "%d C", umbral);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(10, 30, "ajustar UMBRAL:");
  u8g2.drawStr(90, 50, stringUmbral);
  u8g2.sendBuffer();
}
