#ifndef invisible_light_factory_channel
#define invisible_light_factory_channel

class Channel {
  public:
    Channel();
    void update(unsigned long elapsedMillis);
    void cue();
    void describe();
    void ditto();

    int currentValue, inboundValue, outboundValue, queuedValue;
    int duration, queuedDuration;

  private:
    int interpolateLinear (float t, float b, float c, float d);

};

#endif
