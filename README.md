# Example Commands

NB that this document is correct but we've added some more commands that aren't yet documented.

## LED Strobe

Here's a sequence that starts red, then animates to green all the while strobing with blue.

cff0000      -- colour red (alternative colour default to black)
!1           -- cue

c0000ff      -- colour blue
x00ff00      -- alternative colour green
s10          -- segue: ten second colour transition
t40          -- strobe time 40ms
!2           -- cue

## LED Tape

Here's a solid colour transition for the LED tape

h10
u255
b255
r1		-- animation routine 1 = solid colour
!1
"
h255
s5
!2

Here's another sequence for the LED tape

h1       -- hue
u255     -- saturation
b255       -- brightness
r1       -- animation routine 2 (single pixel asc)
!1       -- cue = solid red
"
h100
s5
!2       -- cue = transition to green
"
r3       -- animation routine 4 (single pixel cylon)
o120       -- 120bpm
!3


# Serial Protocol

There are two types of message packet:

- Presetters (configure the next state)
- Bangs (go to the next state)

The expected message flow is a series of presetters followed by a bang, eg

- presetter
- bang
- presetter
- presetter
- presetter
- bang
- (etc)

## Presetters

Presetters have three parts, and are terminated by a newline character

	<address><command><arguments>\n

### Address

The address component is three characters

	@<char><char>

The char sections (by convention) are a digit [0-9] or a wildcard character *

Each unit has an unique identifier. A command will be executed if both digits match the unique id, or if all the characters besides the wildcard character match the ocrresponding positions.

eg if we have a unit with id 12, the following will match...

	@12
	@1*
	@*2
	@**

... while the following (among many others) will not match

	@13
	@2*
	@*1

If the address matches that of the unit, the command will be executed.

### Commands

Commands are a single character, by convention lowercase, which determines what the message will affect. The following commands are presently defined

- c (set the colour for the LED)
- s (set the duration of the segue into the LED colour in seconds)
- r (select an animation to play back on the LED strip)
- a (select an audio file to play)

### Arguments

The arguments that follow the command are eg the quantities that will be applied to the function called. The formats are specific to the comamnd invoked.

#### Colour (c)

A six digit hexadecimal colour value. eg 000000 = black, FFFFFF = white, FF0000 = red etc

#### Segue (s)

A number of seconds

#### Animation Routine (r)

An integer index of the desired animation

#### Audio File (a)

An integer index of the desired audio file

### Example Commands

	@**c00FF00

Addresses all units. Preset the colour green (00FF00)

	@*1s5000

Addresses all units with a second ID digit of 1. Preset the segue length to 5000 milliseconds

	@12r3

Addresses unit 12. Presets the LED strip to play animation routine 3

	@1*a1

Addresses all units with a first ID digit of 1. Presets audio file 1 for playback.

## Bangs

Two types of bang are available. In each case the format is

	<bang type><cue number><newline>

The cue number itself is not used for anything at present - all that's important is that it isn't the same as the previous one. (Because we may send each cue more than once for better radio reliability, we use this to avoid re-cuing the same cue)

### Bang

The standard bang

	!x\n (where x is an integer, possibly with more than one digit)

Causes all units to go to the next preset cue. If units have not had any cue parameters preset, they will go to black, stop any animations and audio.

### Soft Bang

The soft bang

	#x\n (where x is an integer, possibly with more than one digit)

Causes only units which have preset states to go to the next cue. Other units will continue doing what they were doing before.

## Example Sequence

	@**cFF0000\n
	!1\n

All units preset to red, then cued.
Effect - all go red instantly

	@**c0000FF\n
	@**s20\n
	!2\n

All units preset to blue, then cued.
Effect - all transition from red to blue over 20 seconds

	@01cFFFFFF\n
	#3\n

Unit 01 preset to white, then soft cued.
Effect: all units remain blue, except only unit 01, which instantly goes white.

	@01cFF0000\n
	!4\n

Unit 01 preset to red, then cued.
Effect: all units go black, except only unit 01, which instantly goes red.

## Example Commands

(Not informative - just here for convenient copy and paste when testing. Assumes device UID 18)

	@**cFF9900
	@18cFF9900
	@*8cFF9900
	@1*cFF9900

	@9*cFF0000 // should fail
	@*9cFF0000 // should fail
	@99cFF0000 // should fail

	@18s2
	@18r20
	@18a20

	!1
	!2
	!3

	#1
	#2
	#3

