/////////////////////////////////////////////////////////////////////////////////////////////////
// SERIALCOMMAND PROTOCOL
////////////////////////////////////////////////////////////////////////////////////////////////

const char COMMAND_DELIMITER_END = '\n';

const char COMMAND_DELIMITER_ADDRESS = '@';
const char ADDRESS_WILDCARD = '*';
const short NUMBER_OF_ADDRESS_DIGITS = 2;

const char COMMAND_BANG = '!';
const char COMMAND_SOFTBANG = '#';

const char COMMAND_DELIMITER_COLOUR = 'c';
const char COMMAND_DELIMITER_ALT_COLOUR = 'x';
const char COMMAND_DELIMITER_SEGUE_DURATION = 's';
const char COMMAND_DELIMITER_ANIMATION_ROUTINE = 'r';
const char COMMAND_DELIMITER_AUDIO = 'a';
const char COMMAND_DELIMITER_STROBE_TIME = 't';

const char COMMAND_DELIMITER_BRIGHTNESS = 'b';
const char COMMAND_DELIMITER_HUE = 'h';
const char COMMAND_DELIMITER_SATURATION = 'u';
const char COMMAND_DELIMITER_RUNTIME = 'n';
const char COMMAND_DELIMITER_DITTO = '"';