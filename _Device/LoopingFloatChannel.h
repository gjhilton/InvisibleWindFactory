#ifndef invisible_light_factory_LoopingFloatChannel
#define invisible_light_factory_LoopingFloatChannel

enum LoopingFloatChannelLoopMode {
    LOOP_MODE_ASCENDING,
    LOOP_MODE_DESCENDING,
    LOOP_MODE_CYLON
};

class LoopingFloatChannel {
  public:
    LoopingFloatChannel();
    void update();
    void cue();
    void ditto();

    void setMax(int m);
    void setLoopMode(LoopingFloatChannelLoopMode mode);

    float currentValue;
    int duration, queuedDuration; // in milliseconds

  private:
    float interpolateLinear (float t, float b, float c, float d);
    unsigned long cycleStartTime;
    int max;
    LoopingFloatChannelLoopMode loopMode;
    boolean flipflopCylon;
};

#endif
