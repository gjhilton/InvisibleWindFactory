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
	ServiceAudio mp3;
	Service* services[] = {&mp3};
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

