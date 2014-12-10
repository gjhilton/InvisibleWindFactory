#include <Arduino.h>
#include "devices_singleton.h"

#ifdef DEVICE_BEEPER
	#ifdef SERIAL_DEBUG
		const String CONFIGURATION_NAME = "Beeper";
	#endif
	#include "ServiceLEDRGB.h"
	ServiceLEDRGB rgb;
	Service* services[] = {&rgb};
#endif

#ifdef DEVICE_LIGHTSTICK
	#ifdef SERIAL_DEBUG
		const String CONFIGURATION_NAME = "Lightstick";
	#endif
	#include "ServiceLEDTape.h"
	ServiceLEDTape tape;
	Service* services[] = {&tape};
#endif

#ifdef DEVICE_AUDIO
	#ifdef SERIAL_DEBUG
		const String CONFIGURATION_NAME = "Audio";
	#endif
	#include "ServiceAudio.h"
	ServiceAudio audio;
	Service* services[] = {&audio};
	SoftwareSerial trigSerial = SoftwareSerial(MP3_RX, MP3_TX);
	MP3Trigger trigger;
#endif

#ifdef DEVICE_HEADLIGHT
	#ifdef SERIAL_DEBUG
		const String CONFIGURATION_NAME = "Headlight";
	#endif
	#include "ServiceLEDRGB.h"
	#include "ServiceLEDWhite.h"
	ServiceLEDRGB rgb;
	ServiceLEDWhite white;
	Service* services[] = {&rgb, &white};
#endif

