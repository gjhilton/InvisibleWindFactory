#ifndef __invisible_light_factory_service_audio__
#define __invisible_light_factory_service_audio__

#include <SoftwareSerial.h>
#include <MP3Trigger.h>

class ServiceAudio : public Service {
  public:
    void init() {
      track = byte(0);
      queuedVolume = byte(0);
      playWhenCued = stopWhenCued = volumeWhenCued = false;
    };

    void attachTrigger(MP3Trigger *t) {
      trigger = t;
    }

    void cue() {
      if (playWhenCued) {
        trigger->play(track);
      }
      if (stopWhenCued) {
        trigger->stop();
      }
      if (volumeWhenCued) {
        trigger->setVolume(queuedVolume);
        queuedVolume = byte(0);
      }
      // if none is true just keep doing what you were doing... :-)
      playWhenCued = stopWhenCued = false; //volumeWhenCued = false;
    }

    void service(unsigned long elapsedMillis) {
      trigger->update();
    };

    void setTrack(int t) {
      track = byte(t);
      playWhenCued = true;
    }
    void setVolume(int v) {
      v = 255-v; // oddly 255 = silent and 0 is loud without this step... ??
      if (v>255) v = 255;
      if (v<0) v = 0;
      queuedVolume = byte(v);
      volumeWhenCued = true;
    }

    void stopOnNextCue() {
      stopWhenCued = true;
    }

    byte track, queuedVolume;
    MP3Trigger *trigger;
    boolean playWhenCued, stopWhenCued, volumeWhenCued;
};

#endif
