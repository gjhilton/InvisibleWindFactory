#ifndef invisible_light_factory_service
#define invisible_light_factory_service

class Service {
  public:
    virtual void cue() = 0;
    virtual void init() = 0;
    virtual void service(unsigned long elapsedMillis) = 0;
};


#endif
