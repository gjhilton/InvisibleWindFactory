#ifndef __invisible_light_factory_service_ledtape__
#define __invisible_light_factory_service_ledtape__
#include "Service.h"
#include "Channel.h"
#include "LoopingFloatChannel.h"
#include <FastLED.h>
#include "pins.h"

#define NUM_LEDS 20

class ServiceLEDTape : public Service {
  public:
    Channel hue, saturation, brightness;
    LoopingFloatChannel run;
    ServiceLEDTape ();

    // service interface
    void cue();
    void init();
    void service(unsigned long elapsedMillis);

    // presetters
    void presetDitto();
    void presetHue(int h);
    void presetSaturation(int s);
    void presetBrightness(int b);
    void presetAnimationType(int n);
    void presetRuntime(int t); // ie animation loop time
    void presetSegueSeconds(int s); // for colour

  private:
    // instance state
    int animationType, animationTypeQueued;
    CRGB leds[NUM_LEDS];

    // private implementation
    void clear();
    void setAll(CRGB colour);
    void setAt(int led, CRGB colour);
    void setAtAntiAliased(float led, int hue, int saturation, int brightness);
    void draw();
    void beginDescendingRun();
    void beginAscendingRun();
    CRGB getCurrentColour();
    void doRoutineSolid();
    void doRoutineSingle();
};

#endif