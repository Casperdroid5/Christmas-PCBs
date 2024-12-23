#define F_CPU 9600000 // Must stay at this speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "light_ws2812.c"
#include <Arduino.h>

#define NUM_LEDS 11 //number of ws2812 leds
#define HALF_LEDS ((NUM_LEDS + 1) / 2)
#define STRIP_LENGTH (41 - HALF_LEDS)
#define BRIGHT_POT A3
#define SPEED_POT A2
#define BUTTON 0
#define OFF 0
#define ON 1
#define ONCE 5

// Function Prototypes
void SetSpeed();
void SetBrightness();
bool CheckReleased();
bool CheckButton();
void CreateEye(int8_t center);
void SendLeds(uint8_t eye);
void setPixelColor(uint8_t pix, cRGB color);

cRGB p[NUM_LEDS];
cRGB black[1] ;
uint16_t brightness;
uint8_t mode = ON;
bool pressed = false;
int8_t eye = 0;
int8_t inc = 1;
uint16_t moment;
uint8_t spd;

void setup() {
}


void SetSpeed() {
  spd = 7 + (analogRead(SPEED_POT) >> 5) ;
}

void SetBrightness() {
  brightness = analogRead(BRIGHT_POT) >> 2;
}

bool CheckReleased() {
  delay(1);
  if (digitalRead(BUTTON)) return false;
  return true;
}

bool CheckButton() {
  uint16_t counter = 0;
  if (digitalRead(BUTTON)) return false;
  while ((!digitalRead(BUTTON)) && (counter < 2000)) {
    delay(1);
    counter++;
    if ((mode == ON) && (counter > 500)) {
      mode = OFF;
      return true;
    }
  }
  if ((counter > 200) && (counter < 2000)) mode = ONCE;
  else mode = ON;
  moment = (uint16_t) millis();
  eye = 0;
  return true;
}

void setPixelColor(uint8_t pix, cRGB color) {
  if (pix < NUM_LEDS) p[pix] = color;
}

void loop() {
  SetBrightness();
  SetSpeed();
  if (pressed)  pressed = CheckReleased();
  else pressed = CheckButton();
  if ((uint16_t) millis() - moment > spd) {
    moment = millis();
    if (eye < HALF_LEDS) CreateEye(eye);
    else if (STRIP_LENGTH - eye < HALF_LEDS) CreateEye(NUM_LEDS - (STRIP_LENGTH - eye));
    else CreateEye(HALF_LEDS);
    eye = eye + inc;
    if (eye <= 0) {
      inc = 1;
      if (mode == ONCE) mode = OFF;
    }
    if (eye >= STRIP_LENGTH) inc = -1;
  }
  SendLeds(eye);
}

void SendLeds(uint8_t eye) {
  for (uint8_t i = 0; i < (STRIP_LENGTH); i++) {
    if (i == eye) ws2812_setleds((struct cRGB *)&p, NUM_LEDS, 0);
    ws2812_setleds((struct cRGB *)&black, 1, 0);
  }
  ws2812_setleds((struct cRGB *)&black, 1, 1);
}

void CreateEye(int8_t center) {
  cRGB col;
  col.r = 0;
  for (uint8_t i = 0; i < NUM_LEDS; i++) setPixelColor(i, col);
  if ((!mode) || (center < 0)) return;
  col.r = brightness;
  for (uint8_t i = 0; i < HALF_LEDS; i++) {
    setPixelColor(center + i, col);
    setPixelColor(center - i, col);
    int16_t br = col.r - (1 + brightness / 7);
    if (br < 0) br = 0;
    col.r = br;
  }
}
