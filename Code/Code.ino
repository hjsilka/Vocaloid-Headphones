/* 
  Written by Rupak Poddar
  Last Updated: 03 April 2024
  Modified by Ilka Wertz (15 March 2026)
*/

#include <FastLED.h>        // https://github.com/FastLED/FastLED
#include <Servo.h>          // Servo library
FASTLED_USING_NAMESPACE

Servo myServo;

#define NUM_LEDS         24 // Number of LEDs on the WS2812
#define DATA_PIN          3 // Addressable LED Strip
#define LED_TYPE     WS2812
#define COLOR_ORDER     RGB // Default Color Order
#define ENVELOPE_PIN     A0 // Envelope Pin of the Sound Detector Module
#define SERVO_PIN         9 // Servomotor

#define BRIGHTNESS      200 // Min: 0, Max: 255
#define SATURATION      150 // Min: 0, Max: 255
#define MIN_VAL          1  // Min: 0, Max: 75
#define MAX_VAL         300 // Min: 75, Max: 750
#define HUE_INIT         10 // < 255
#define HUE_CHANGE        2 // < 255


CRGB leds[NUM_LEDS];
byte dynamicHue = HUE_INIT;
int analogVal = 0;
int val = 0;

void setup() { 
  pinMode(ENVELOPE_PIN, INPUT);
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  // Update the LED Strip
  FastLED.show();

  myServo.attach(SERVO_PIN); 
}

unsigned long lastMove = 0;
bool pos = false;

void loop() {
  analogVal = analogRead(ENVELOPE_PIN);

  if(analogVal > MAX_VAL)
    analogVal = MAX_VAL;

  if(analogVal < MIN_VAL)
    analogVal = MIN_VAL;

  
  LinearFlowing();
  // Update the LED Strip
  FastLED.show();

// Servo movement
  if (millis() - lastMove > 500) {
    lastMove = millis();
    if(pos) myServo.write(150);
    else myServo.write(180);
    pos = !pos;
  }

}

void LinearFlowing() {
  val = map(analogVal, MIN_VAL, MAX_VAL, 0, BRIGHTNESS);
  int dynamicDelay = map(analogVal, MIN_VAL, MAX_VAL, 20, 1);
  
  for (int i = 0; i < NUM_LEDS-1; i++) {
    leds[i] = leds[i+1];
  }

  leds[NUM_LEDS-1] = CHSV(dynamicHue += HUE_CHANGE, SATURATION, val);

  delay(dynamicDelay);
}
