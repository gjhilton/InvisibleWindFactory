/*


  Q: SEEING A COMPILATION ERROR IN THIS FILE?
  A: Check your device declarations at the top of the main sketch 

*/

// FORCE COMPILE ERROR if you fail to specify a type of device

#ifndef DEVICE_BEEPER
  #ifndef DEVICE_LIGHTSTICK
    #ifndef DEVICE_AUDIO
      #ifndef DEVICE_HEADLIGHT
        "DELIBERATE_COMPILE_ERROR -> you need to define a device"
      #endif
    #endif
  #endif
#endif

// FORCE COMPILE ERRORS if you try to specify more than one type of device

#ifdef DEVICE_BEEPER
  #ifdef DEVICE_LIGHTSTICK
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_AUDIO
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_HEADLIGHT
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
#endif

#ifdef DEVICE_LIGHTSTICK
  #ifdef DEVICE_BEEPER
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_AUDIO
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_HEADLIGHT
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
#endif

#ifdef DEVICE_AUDIO
  #ifdef DEVICE_BEEPER
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_LIGHTSTICK
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_HEADLIGHT
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
#endif

#ifdef DEVICE_HEADLIGHT
  #ifdef DEVICE_BEEPER
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_LIGHTSTICK
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
  #ifdef DEVICE_AUDIO
    "DELIBERATE COMPILER ERROR -> attempt to define multiple devices"
  #endif
#endif
