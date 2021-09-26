# SM5-Arduino-Lighting
**Use an Arduino to get game-controlled lights in StepMania 5!**

## What's it do?
StepMania 5 can be set up to control lights you hook up to an Arduino as if they were lights on an Arcade cabinet.

This Arduino code will help you do that!



## Requirements
To set this up, all you need is:
 * StepMania 5.x (tested on StepMania 5.3/Project Outfox, free download at https://projectmoon.dance/downloads)
 * Windows or Linux (Windows lighting only supported on StepMania Outfox)
 * An Arduino (Nearly any model should work, like the Uno, Mega, Micro, etc)
 * Lights to control (LEDs? Dance pad lights? Neons? Anything goes!)

---

## Step 1: Wiring setup
First, you'll want to decide how you want to wire up your lights:

---

### Wire single lights to single Arduino pins (easier, recommended)
By default, the code is set up so you can wire a single light to each Arduino pin, already set up for a DDR/ITG-style 4-panel setup.

If you go this route, you don't need to modify the code! Just wire up your Arduino as shown below and upload the code as-is to your Arduino:

![Direct wiring diagram](/Wiring-Direct.png)

By default, the code controls nearly all the lights for a complete DDR/ITG-style setup - 4 arrow panel and start/menu button lights for each player, 4 marquee lights, and a bass neon light.

**To change what pin** each light is on, find the section near the top with a ton of `#define PIN_<something>_<something>` lines - you can change these numbers to **any Arduino pin**, even the analog input pins.


---

### Wire up tons of lights to shift registers
StepMania can control a *lot* more lights than your Arduino has pins, which is a problem if you want to wire up *all* the lights StepMania can control. Ahhh!!

To wire up more lights, you can use **Shift Registers** to get more output pins on your Arduino.

[Adafruit has a great tutorial on them here](https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/overview) thats well worth a read. TL;DR: Wire up your Arduino like this, and you're off to the races:



By default, the code outputs data to 4 shift registers: 1 per player for gameplay buttons (like arrow panel lights on a DDR cab), 1 for cabinet lighting, and 1 for the bass light (with tons of extra pins if you want to mod the code to control even *more* lights).

> **To use Shift Registers**, go into the code and find this line near the top:
> 
> ```//#define USE_SHIFT_REGISTERS```
> 
> **Remove** the **two slashes** to uncomment this line, and the code will output to shift registers instead of single LEDs.

**To change what pins** the shift registers connect to, find the section near the top with the `#define PIN_SHIFT_CLOCK`, `LATCH` and `DATA` lines - You can change these numbers to any Arduino pin.

---



## Step 2: Software Setup
There's two methods that'll work to set up StepMania to output lights data to an Arduino:

### Windows
On Windows, the Win32Serial Lights Driver is used. As far as I can tell, this driver is only in StepMania 5.3 and on [the driver author's StepMania fork](https://github.com/skogaby/stepmania/).

1) In `Preferences.ini`, set `LightsDriver=Win32Serial` and `LightsComPort=` to whatever COM port your Arduino is on 
   - Tip: The port is the one you choose in the Arduino IDE to upload code, along the lines of `COM2`, etc).
2) Start Stepmania. It may take a tad longer than normal to start, but if you don't get an error message you're done! :D


### Linux
On Linux (potentially Mac as well), the SextetStreamToFile Lights Driver is used. This is available in most versions of StepMania 5 (including 5.3), and requires a bit more setup than Win32Serial.

With SextetStreamToFile, Stepmania will write all the lighting data to a file you specify. Any other program can read this file, and do *anything* with the data in real-time. In this case, we'll send whatever Stepmania writes to the file to the Arduino!


#### Step 1: Set up lighting output

1) In `Preferences.ini`, set `LightsDriver=SextetStreamToFile`.

2) Note what `SextetStreamOutputFilename=` is set to (by default: `Data/StepMania-Lights-SextetStream.out`. You can change this if you want.)

2) Open a terminal inside your Stepmania installation's folder. Run `mkfifo ./` and the filename you put down in step 2. This is the file Stepmania will write lighting data to, and the file you'll read from to send data to the Arduino.
   - *(So in the above example, run `mkfifo ./Data/StepMania-Lights-SextetStream.out`)*

3) Run StepMania - it'll hang on the loading window. This is normal, *I swear!*

4) Run `cat ` and your filename, Stepmania should now start. If you see a jumble of characters fly by, SextetStream is working!
   - *(With the above example, this command is `cat ./Data/StepMania-Lights-SextetStream.out`)*


#### Step 2: Set up Arduino output

1) If you haven't in the past, you may need to run `sudo usermod -a -G dialout <your username>` to add yourself to the dialout group. This gives you permission to actually talk to the Arduino over USB serial.
2) Run `cat <sextet filename/path> > <arduino device name>`
   - Tip: Use the same device name as shown in the Arduino IDE, usually starting with `/dev/ttyACM`
   - Socat should work here too: `socat <sextet filename/path> <arduino device name>`


## Step 3: Enjoy! :D