#include "Arduino.h"
#include "Channel.h"

// CONSTRUCTOR

Channel::Channel() {
  inboundValue = outboundValue = currentValue = queuedValue = 0;
  duration = queuedDuration = 0;
}

// PUBLIC INTERFACE

void Channel::update(unsigned long elapsedMillis) {
  unsigned long durationMillis = duration * 1000;
  if ((duration==0) || (elapsedMillis >= durationMillis)) {
    currentValue = inboundValue;
  } else {
    currentValue = interpolateLinear(elapsedMillis, outboundValue, (inboundValue - outboundValue), durationMillis);
  }
}

void Channel::cue() {
  outboundValue = currentValue;
  inboundValue = queuedValue;
  queuedValue = 0;
  duration = queuedDuration;
  queuedDuration = 0;
}

void Channel::ditto() {
  queuedDuration = duration;
  queuedValue = inboundValue;
}

// PRIVATE IMPLEMENTATION

int Channel::interpolateLinear (float t, float b, float c, float d) {
  float f = c * t / d + b;
  return int(f);
}

void Channel::describe() {
  Serial.print("CHANNEL Current: ");
  Serial.print(currentValue);
  Serial.print(" (Outbound: ");
  Serial.print(outboundValue);
  Serial.print(", Inbound: ");
  Serial.print(inboundValue);
  Serial.print(", Queued: ");
  Serial.print(queuedValue);
  Serial.print(") Duration: ");
  Serial.print(duration);
  Serial.print("(");
  Serial.print(queuedDuration);
  Serial.println(")");
}

