/////////////////////////////////////////////////////////////////////////////////////////////////
// READ CAPABILITIES FROM EEPROM
////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

const short STORAGE_ADDRESS_UID = 0;
const short MAX_VALID_UID = 99;

String readUID() {
  byte read = EEPROM.read(STORAGE_ADDRESS_UID);
  if (read <= MAX_VALID_UID) {
    return String(read);
  } else {
    // an invalid value has been read, possibly 255 (uninitialised), so return default
    return "00";
  }
}