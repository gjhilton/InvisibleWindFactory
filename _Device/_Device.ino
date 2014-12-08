/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// INVISIBLE LIGHT FACTORY DEVICE FIRMWARE
// v3.0.2
//
//  To use:
//
//  0) set the include paths below to the right thing
//  1) configure the device type
//  2) consider commenting out SERIAL_DEBUG
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// SET DEVICE TYPE
// uncomment exactly one line in this section
////////////////////////////////////////////////////////////////////////////////////////////////

// #define DEVICE_BEEPER
// #define DEVICE_LIGHTSTICK
// #define DEVICE_AUDIO
#define DEVICE_HEADLIGHT

/////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANT CONFIGURATION
////////////////////////////////////////////////////////////////////////////////////////////////

#define SERIAL_DEBUG // MUST be commented out in production

#define STARTUP_DELAY 500
#define LOOP_DELAY 10
#define BAUD_RATE 115200

#include "pins.h" // Harrware pin setup

/////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////

#include "Service.h"
#include "Channel.h"
#include <FastLED.h>
#include "devices.h"
#include <EEPROM.h>

#include "/Absolute/path/to/VisionCommission/InvisibleWindFactory/shared/arraysize.h"
#include "/Absolute/path/to/VisionCommission/InvisibleWindFactory/shared/readEEPROM.h"
#include "/Absolute/path/to/VisionCommission/InvisibleWindFactory/shared/IWFSerialProtocol.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////

int nServices = ARRAY_SIZE(services);
unsigned long lastCueTime = millis();

/////////////////////////////////////////////////////////////////////////////////////////////////
// ARDUINO LIFECYCLE
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  delay(STARTUP_DELAY);
  setupRadio();
  setupUID();
  killLED();
  for (int i = 0; i < nServices; i++) {
    services[i]->init();
  }
}

void loop() {
  loopRadio();
  unsigned long elapsedMillis = millis() - lastCueTime;
  for (int i = 0; i < nServices; i++) {
    services[i]->service(elapsedMillis);
  }
  delay(LOOP_DELAY);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// UID
////////////////////////////////////////////////////////////////////////////////////////////////

String UID = "00"; // immutable once read from EEPROM

void setupUID() {
  UID = readUID();
#ifdef SERIAL_DEBUG
  Serial.print("\n<");
  Serial.print(CONFIGURATION_NAME);
  Serial.print("@");
  Serial.print(UID);
  Serial.println(">");
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// RADIO
////////////////////////////////////////////////////////////////////////////////////////////////

String commandString = "";
boolean commandStringComplete = false;

void setupRadio() {
  pinMode(RADIO_PIN, OUTPUT);
  digitalWrite(RADIO_PIN, HIGH);
  Serial.begin(BAUD_RATE);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (!commandStringComplete) {
      if (inChar == COMMAND_DELIMITER_END) {
        commandStringComplete = true;
      } else {
        commandString += inChar;
      }
    } else {
      // nothing - discard the additional input
    }
  }
}

void loopRadio() {
  if (commandStringComplete) {
    parseCommandString(commandString);
    commandString = "";
    commandStringComplete = false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMANDS
////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*Callback)(String params);

typedef struct {
  char character;
  Callback callback;
} Command;

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - BEEPER
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEVICE_BEEPER

#include "rgbPresetting.h"

const Command COMMANDS[] = {
  // ADDRESSING
  {COMMAND_DELIMITER_ADDRESS,            parseAddressCommand},
  // CUEING
  {COMMAND_BANG,                         bangCommand},
  {COMMAND_SOFTBANG,                     softBangCommand},
  // PRESETTING
  {COMMAND_DELIMITER_COLOUR,             presetColour},
  {COMMAND_DELIMITER_ALT_COLOUR,         presetAltColour},
  {COMMAND_DELIMITER_SEGUE_DURATION,     presetSegue},
  {COMMAND_DELIMITER_STROBE_TIME,        presetStrobeTime}
};

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - LIGHTSTICK
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEVICE_LIGHTSTICK

const Command COMMANDS[] = {
  // ADDRESSING
  {COMMAND_DELIMITER_ADDRESS,            parseAddressCommand},
  // CUEING
  {COMMAND_BANG,                         bangCommand},
  {COMMAND_SOFTBANG,                     softBangCommand},
  // PRESETTING
  {COMMAND_DELIMITER_BRIGHTNESS,         presetBrightness},
  {COMMAND_DELIMITER_HUE,                presetHue},
  {COMMAND_DELIMITER_SATURATION,         presetSaturation},
  {COMMAND_DELIMITER_RUNTIME,            presetRuntime},
  {COMMAND_DELIMITER_SEGUE_DURATION,     presetSegue},
  {COMMAND_DELIMITER_ANIMATION_ROUTINE,  presetRoutine},
  {COMMAND_DELIMITER_DITTO,  presetDitto},
};

void presetDitto(String params) {
  // discard params if any
  tape.presetDitto();
  #ifdef SERIAL_DEBUG
    Serial.println("PRESET: tape dittoed");
  #endif
};

void presetBrightness(String params) {
  int b = params.toInt();
  tape.presetBrightness(b);
  #ifdef SERIAL_DEBUG
    debugPreset("tape b",b);
  #endif
};

void presetHue(String params) {
  int h = params.toInt();
  tape.presetHue(h);
  #ifdef SERIAL_DEBUG
    debugPreset("tape h",h);
  #endif
};

void presetSaturation(String params) {
  int s = params.toInt();
  tape.presetSaturation(s);
  #ifdef SERIAL_DEBUG
    debugPreset("tape s",s);
  #endif
};

void presetRuntime(String params) {
  int t = params.toInt();
  tape.presetRuntime(t);
  #ifdef SERIAL_DEBUG
    debugPreset("tape runtime",t);
  #endif
};

void presetRoutine(String params) {
  int r = params.toInt();
  tape.presetAnimationType(r);
  #ifdef SERIAL_DEBUG
    debugPreset("tape animation",r);
  #endif
};

void presetSegue(String params) {
  int t = params.toInt();
  tape.presetSegueSeconds(t);
  #ifdef SERIAL_DEBUG
    debugPreset("tape segue",t);
  #endif
};

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - AUDIO
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEVICE_AUDIO

const Command COMMANDS[] = {
  // ADDRESSING
  {COMMAND_DELIMITER_ADDRESS,            parseAddressCommand},
  // CUEING
  {COMMAND_BANG,                         bangCommand},
  {COMMAND_SOFTBANG,                     softBangCommand}
};

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - HEADLIGHT
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEVICE_HEADLIGHT

#include "rgbPresetting.h"

const Command COMMANDS[] = {
  // ADDRESSING
  {COMMAND_DELIMITER_ADDRESS,            parseAddressCommand},
  // CUEING
  {COMMAND_BANG,                         bangCommand},
  {COMMAND_SOFTBANG,                     softBangCommand},
  // PRESETTING
  {COMMAND_DELIMITER_COLOUR,             presetColour},
  {COMMAND_DELIMITER_ALT_COLOUR,         presetAltColour},
  {COMMAND_DELIMITER_SEGUE_DURATION,     presetSegue},
  {COMMAND_DELIMITER_STROBE_TIME,        presetStrobeTime},
  {COMMAND_DELIMITER_BRIGHTNESS,         presetWhiteBrightness}
};

void presetWhiteBrightness(String params) {
  int b = params.toInt();
  white.presetBrightness(b);
  #ifdef SERIAL_DEBUG
    debugPreset("w brightness",b);
  #endif
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// UTILITIES
////////////////////////////////////////////////////////////////////////////////////////////////

long rgbFromHex(String hex) { // eg rgbFromHex("FF9900");
  return hex.length() == 6 ? strtol(&hex[0], NULL, 16) : 0;
}

#ifdef SERIAL_DEBUG
void debugPreset(String s, int v){
  Serial.print("PRESET: ");
  Serial.print(s);
  Serial.print(" -> ");
  Serial.println(v);
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND PROCESSING
////////////////////////////////////////////////////////////////////////////////////////////////

const short N_COMMANDS = ARRAY_SIZE(COMMANDS);

void parseAddressCommand(String params) {
  // do we have a long enough params string to containt a complete address plus a valid command?
  if (params.length() > NUMBER_OF_ADDRESS_DIGITS + 1) {
    // loop over each character of the address
    for (int i = 0; i < NUMBER_OF_ADDRESS_DIGITS; i++) {
      char c = params.charAt(i);
      // if the address digit doesn't match ours (or the wildcard), bail and do nothing further with this command
      if (!((isDigit(c)) && (c == UID.charAt(i)) || (c == ADDRESS_WILDCARD))) return;
    }
    // if we've got here, the command was for us, so strip off the address and re-parse it for execution
    parseCommandString(params.substring(NUMBER_OF_ADDRESS_DIGITS));
  }
};

void parseCommandString(String commandStr) {
#ifdef SERIAL_DEBUG
  Serial.print("COMMAND: got -> ");
  Serial.println(commandStr);
#endif
  char c = commandStr.charAt(0);
  for (int i = 0; i < N_COMMANDS; i++) {
    if (c == COMMANDS[i].character) {
      String params = commandStr.substring(1);
      COMMANDS[i].callback(params);
      return;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND CALLBACKS - BANGS
////////////////////////////////////////////////////////////////////////////////////////////////

int lastBang = 0;

void bangCommand(String params) {
  if (isNewBang(params)) {
    // we got a bang - do the cue
    // ie if we haven't had any presets, we're going to black
    sendCue();
#ifdef SERIAL_DEBUG
    Serial.println("<BANG!>");
#endif
  }
};

void softBangCommand(String params) {
  if (isNewBang(params)) {
    // we got a bang - do the cue only if the cue is initialised
    // ie if we haven't had any presets, just carry on
    // TODO if (queuedState.isInitialised()) advanceStateMachine();
#ifdef SERIAL_DEBUG
    Serial.println("<soft bang>");
#endif
  }
};

void sendCue() {
  lastCueTime = millis();
  for (int i = 0; i < nServices; i++) {
    services[i]->cue();
  }
}

int cueNumberFromBang(String command) { // eg cueNumberFromBang("123");
  int n = (command.length() > 0) ? command.toInt() : 0;
#ifdef SERIAL_DEBUG
  Serial.print("Cue: ");
  Serial.println(n);
#endif
  return n;
}

boolean isNewBang(String commandStr) {
  int bangNumber = commandStr.toInt();
  if (bangNumber == lastBang) {
#ifdef SERIAL_DEBUG
    Serial.println("COMMAND: CUE ERROR ----> NOT a new bang");
#endif
    return false;
  }
  lastBang = bangNumber;
  return true;
}

void killLED(){
  // development utility - not needed in production
  // because of the unused GPS unit on the boards, we need to pull pins LOW to reset them
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
}
