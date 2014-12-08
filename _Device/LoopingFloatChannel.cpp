#include "Arduino.h"
#include "LoopingFloatChannel.h"

// CONSTRUCTOR

LoopingFloatChannel::LoopingFloatChannel() {
  duration = queuedDuration = 0;
  loopMode = LOOP_MODE_ASCENDING;
  loopMode = LOOP_MODE_CYLON;
  flipflopCylon = false;
}

// PUBLIC INTERFACE

void LoopingFloatChannel::update() {
  // doesn't make sense unless we have a loop length
  if (duration == 0) {
    currentValue = 0;
    return;
  }

  // calculate elapsed time
  unsigned long cycleDurationMillis = duration * 1000;
  unsigned long now = millis();
  unsigned long elapsedMillis = now - cycleStartTime;
  
  // new cycle if required
  if (elapsedMillis >= cycleDurationMillis){
    cycleStartTime = now;
    elapsedMillis = elapsedMillis - cycleDurationMillis;
    flipflopCylon = !flipflopCylon; 
  }

  // set value
  currentValue = interpolateLinear(elapsedMillis, 0, max, cycleDurationMillis);

  if (((loopMode == LOOP_MODE_CYLON) && (flipflopCylon == true)) || (loopMode == LOOP_MODE_DESCENDING)) {
   currentValue = max - currentValue;
  }
}

void LoopingFloatChannel::cue() {
  duration = queuedDuration;
  queuedDuration = 0;
  cycleStartTime = millis();
  flipflopCylon = false;
}

void LoopingFloatChannel::ditto() {
  queuedDuration = duration;
}

void LoopingFloatChannel::setMax(int m) {
  max = m;
}

void LoopingFloatChannel::setLoopMode(LoopingFloatChannelLoopMode mode){
   loopMode = mode;
}

// PRIVATE IMPLEMENTATION

float LoopingFloatChannel::interpolateLinear (float t, float b, float c, float d) {
  float f = c * t / d + b;
  return f;
}