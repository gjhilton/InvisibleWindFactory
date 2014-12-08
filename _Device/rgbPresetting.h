#include <Arduino.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - HEADLIGHT AND BEEPER
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SERIAL_DEBUG
void debugColour(String s, int r, int g, int b) {
  Serial.print("PRESET: ");
  Serial.print(s);
  Serial.print(" -> ");
  Serial.print(r);
  Serial.print(",");
  Serial.print(g);
  Serial.print(",");
  Serial.println(b);
}
#endif

void presetStrobeTime(String params) {
  int t = params.toInt();
  rgb.presetStrobeTime(t);
  #ifdef SERIAL_DEBUG
    debugPreset("rgb strobe time",t);
  #endif
};

void presetColour(String hex) {
  long colour = rgbFromHex(hex);
  int r = (colour >> 16) & 0xff;
  int g = (colour >> 8) & 0xff;
  int b = (colour >> 0) & 0xff;
  rgb.presetColour(r, g, b);
#ifdef SERIAL_DEBUG
  debugColour("rgb", r, g, b);
#endif
};

void presetAltColour(String hex) {
  long colour = rgbFromHex(hex);
  int r = (colour >> 16) & 0xff;
  int g = (colour >> 8) & 0xff;
  int b = (colour >> 0) & 0xff;
  rgb.presetStrobeColour(r, g, b);
#ifdef SERIAL_DEBUG
  debugColour("rgb strobe", r, g, b);
#endif
};

void presetSegue(String params) {
  int t = params.toInt();
  rgb.presetSegueSeconds(t);
  #ifdef SERIAL_DEBUG
    debugPreset("rgb segue",t);
  #endif
};

