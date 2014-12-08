#ifndef __invisible_light_factory_service_ledwhite__
#define __invisible_light_factory_service_ledwhite__

class ServiceLEDWhite : public Service {
  public:
    void init() {
      pinMode(LED_WHITE_PIN, OUTPUT);
    };
    void service(unsigned long elapsedMillis) {
        white.update(elapsedMillis);
        setLED(white.currentValue);
    };
    void cue() {
      white.cue();
    }
    void presetBrightness(int b) {
      white.queuedValue = b;
    }
    void setLED(int b) {
      analogWrite(LED_WHITE_PIN, b);
    }
  private:
    Channel white;
};
#endif
