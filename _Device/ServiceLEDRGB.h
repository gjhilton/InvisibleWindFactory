#ifndef __invisible_light_factory_service_ledrgb__
#define __invisible_light_factory_service_ledrgb__

class ServiceLEDRGB : public Service {
  public:
    void init() {
      pinMode(LED_RED_PIN, OUTPUT);
      pinMode(LED_GREEN_PIN, OUTPUT);
      pinMode(LED_BLUE_PIN, OUTPUT);
      // because of the unused GPS unit on the boards, we need to pull pins LOW to reset them
      digitalWrite(LED_RED_PIN, LOW);
      digitalWrite(LED_GREEN_PIN, LOW);
      digitalWrite(LED_BLUE_PIN, LOW);
      strobeTime = 0;
      flipflop = false;
    }
    void service(unsigned long elapsedMillis) {
      if (strobeTime == 0) {
        // solid colour
        red.update(elapsedMillis);
        green.update(elapsedMillis);
        blue.update(elapsedMillis);
        setLED(red.currentValue, green.currentValue, blue.currentValue);
      } else {
        // strobe mode
        int r,g,b = 0;
        if (flipflop){
          red.update(elapsedMillis);
          green.update(elapsedMillis);
          blue.update(elapsedMillis);
          r = red.currentValue;
          g = green.currentValue;
          b = blue.currentValue;
        } else {
          rStrobe.update(elapsedMillis);
          gStrobe.update(elapsedMillis);
          bStrobe.update(elapsedMillis);
          r = rStrobe.currentValue;
          g = gStrobe.currentValue;
          b = bStrobe.currentValue;
        }
        flipflop = !flipflop;
        delay(strobeTime); // FIXME - not idela to block here
        setLED(r, g, b);
      }
    };
    void setLED(int r, int g, int b) {
      analogWrite(LED_RED_PIN, r);
      analogWrite(LED_GREEN_PIN, g);
      analogWrite(LED_BLUE_PIN, b);
    }
    void cue() {
      strobeTime = queuedStrobeTime;
      queuedStrobeTime = 0;

      red.cue();
      green.cue();
      blue.cue();

      rStrobe.cue();
      gStrobe.cue();
      bStrobe.cue();
    }
    void presetColour(int r, int g, int b) {
      red.queuedValue = r;
      green.queuedValue = g;
      blue.queuedValue = b;
    }
    void presetStrobeColour(int r, int g, int b) {
      rStrobe.queuedValue = r;
      gStrobe.queuedValue = g;
      bStrobe.queuedValue = b;
    }
    void presetSegueSeconds(int s) {
      red.queuedDuration = s;
      green.queuedDuration = s;
      blue.queuedDuration = s;
      rStrobe.queuedDuration = s;
      gStrobe.queuedDuration = s;
      bStrobe.queuedDuration = s;
    }
    void presetStrobeTime(int t) {
      queuedStrobeTime = t - LOOP_DELAY; // take into account the fact that we already pause a little bit between cycles
      if (queuedStrobeTime < 0) queuedStrobeTime = 0; // if the requested strobe time is too short set it to 0
    }
  private:
    boolean flipflop;
    int strobeTime, queuedStrobeTime;
    Channel red, green, blue, rStrobe, gStrobe, bStrobe;
};

#endif
