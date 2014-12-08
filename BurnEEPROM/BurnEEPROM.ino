/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// INVISIBLE LIGHT FACTORY EPPROM BURNER
//
//  To use:
//
//  0) set the include path below to the right thing!
//  1) configure the constants you want to burn in the section immediately below
//  2) upload to arduino
//  3) check sanity tests are passed using serial window
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <EEPROM.h>
#include "/Absolute/path/to/VisionCommission/InvisibleWindFactory/shared/readEEPROM.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// SET UP THE VALUE YOU WANT TO BURN HERE
////////////////////////////////////////////////////////////////////////////////////////////////

// Unique identifier (ie address) for device
const byte UID_TO_STORE = 19; // valid values are 1 - 99

////////////////////////////////////////////////////////////////////////////////////////////////
// ARDUINO LIFECYCLE
////////////////////////////////////////////////////////////////////////////////////////////////

const int BAUD_RATE = 9600;

void setup() {
  delay(1000);
  Serial.begin(BAUD_RATE);
  burn();
}

void loop() {
  verify();
  delay(10000);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// BURN EEPROM
////////////////////////////////////////////////////////////////////////////////////////////////

void burn() {
  l();
  Serial.println("Beginning burn process..");
  burnUID();
  l();
}

void burnValue(String name, int address, byte value){
  Serial.print("Burning: ");
  Serial.print(name);
  Serial.print(" ");
  Serial.print(value);
  Serial.print(" ");
  EEPROM.write(address, value);
  Serial.println("... done");
}

void burnUID() {
  if ((UID_TO_STORE >= 0) && (UID_TO_STORE <= MAX_VALID_UID)) {
    burnValue("UID",STORAGE_ADDRESS_UID,UID_TO_STORE);
  } else {
    Serial.println("ERROR: UID must be between 0-99");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// VERIFY BURN
////////////////////////////////////////////////////////////////////////////////////////////////

void l() {
  Serial.println("------------------------------");
}

void verify() {
  l();
  Serial.println("Beginning verification process..");
  verifyUID();
  l();
}

void verifyUID() {
  Serial.print("Verify: UID...");
  String expected = String(UID_TO_STORE);
  String actual = readUID();
  compare(expected, actual);
}

void compare(String expected, String actual){
  if (expected != actual) {
    Serial.print("ERROR! Expected: ");
    Serial.print(expected);
    Serial.print(" Actual: ");
    Serial.println(actual);
  } else {
    Serial.println(" success");
  }
}

