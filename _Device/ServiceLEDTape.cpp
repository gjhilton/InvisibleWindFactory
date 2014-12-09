#include "Arduino.h"
#include "ServiceLEDTape.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTOR
////////////////////////////////////////////////////////////////////////////////////////////////

ServiceLEDTape::ServiceLEDTape () {
  animationType = animationTypeQueued = 0;
  run.setMax(NUM_LEDS);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// SERVICE INTERFACE
////////////////////////////////////////////////////////////////////////////////////////////////

void ServiceLEDTape::cue() {
  animationType = animationTypeQueued;
  hue.cue();
  saturation.cue();
  brightness.cue();
  // cue and configure runloop
  switch (animationType) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      run.setMax(NUM_LEDS);
      run.setLoopMode(LOOP_MODE_ASCENDING);
      break;
    case 3:
      run.setMax(NUM_LEDS);
      run.setLoopMode(LOOP_MODE_DESCENDING);
      break;
    case 4:
      run.setMax(NUM_LEDS);
      run.setLoopMode(LOOP_MODE_CYLON);
      break;
  }
    run.cue();
}

void ServiceLEDTape::init() {
  FastLED.addLeds<WS2801, LED_TAPE_DATA_PIN, LED_TAPE_CLOCK_PIN, RGB>(leds, NUM_LEDS);
  clear();
  draw();
}

void ServiceLEDTape::service(unsigned long elapsedMillis) {

  // update colour channels
  hue.update(elapsedMillis);
  saturation.update(elapsedMillis);
  brightness.update(elapsedMillis);
  run.update();

  // clean slate before animation frame
  clear();

  // set animation frame
  switch (animationType) {
    case 0: // nothing
      break;
    case 1:
      doRoutineSolid(); // solid colour
      break;
    case 2:
      doRoutineSingle(); // one travelling pixel asc
      break;
    case 3:
      doRoutineSingle(); // one travelling pixel desc
      break;
    case 4:
      doRoutineSingle(); // one travelling pixel cylon
      break;
  }

  // display
  draw();

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ANIMATIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void ServiceLEDTape::doRoutineSolid(){
  CRGB c = getCurrentColour();
  setAll(c);
}

void ServiceLEDTape::doRoutineSingle(){
  setAtAntiAliased(run.currentValue, hue.currentValue, saturation.currentValue, brightness.currentValue);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// PRESETTERS
////////////////////////////////////////////////////////////////////////////////////////////////

void ServiceLEDTape::presetAnimationType(int n) {animationTypeQueued = n;}
void ServiceLEDTape::presetHue(int h){hue.queuedValue = h;}
void ServiceLEDTape::presetSaturation(int s){saturation.queuedValue = s;}
void ServiceLEDTape::presetBrightness(int b){brightness.queuedValue = b;}
void ServiceLEDTape::presetTempo(int bpm){
  // float numBars = bpm / 4.0f; //assuming four beats to the bar. number of complete cycles in 1 minute
  float numBars = bpm * 1.0f;
  unsigned long millisInAMinute = 60000;
  float millisPerCycle = millisInAMinute / numBars;
  int msDuration = round(millisPerCycle);
/*
  Serial.print("Bars: ");
  Serial.println(numBars);
  Serial.print("MPC: ");
  Serial.println(millisPerCycle);
  Serial.print("d: ");
  Serial.println(msDuration);
*/
  run.queuedDuration = msDuration;
}

void ServiceLEDTape::presetSegueSeconds(int s) {
  hue.queuedDuration = s;
  saturation.queuedDuration = s;
  brightness.queuedDuration = s;
}

void ServiceLEDTape::presetDitto() {
  animationTypeQueued = animationType;
  hue.ditto();
  saturation.ditto();
  brightness.ditto();
  run.ditto();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////

void ServiceLEDTape::clear() {
  setAll(CRGB::Black);
}

void ServiceLEDTape::setAll(CRGB colour) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = colour;
  }
}

void ServiceLEDTape::draw() {
  FastLED.show();
}

void ServiceLEDTape::setAtAntiAliased(float led, int h, int s, int b) {
  int lowIndex = floor(led);
  int highIndex = ceil(led);
  int lowValue = (highIndex - led) * b;
  int highValue =  (led - lowIndex) * b;
  CHSV lowColour = CHSV(h, s, lowValue);
  CHSV highColour = CHSV(h, s, highValue);
  if (lowIndex < 0) lowIndex = NUM_LEDS - 1;
  if (highIndex >= NUM_LEDS) highIndex = 0;
  setAt(lowIndex, lowColour);
  setAt(highIndex, highColour);
}

void ServiceLEDTape::setAt(int led, CRGB colour) {
  // TODO modulo or error
  leds[led] = colour;
}

CRGB ServiceLEDTape::getCurrentColour() {
  CRGB c = CRGB(CHSV(hue.currentValue, saturation.currentValue, brightness .currentValue));
  return c;
}
