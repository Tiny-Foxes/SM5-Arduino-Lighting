# SM5-Arduino-Lighting
**Use an Arduino to get game-controlled lights in StepMania 5!**

## What's it do?
StepMania 5 can be set up to control lights you hook up to an Arduino as if they were lights on an Arcade cabinet.

This Arduino code will help you do that!



## Requirements
To set this up, all you need is:
 * StepMania 5.x (tested on StepMania 5.3, free download at https://projectmoon.dance/downloads)
 * Windows or Linux (Windows lighting only supported on StepMania 5.3)
 * An Arduino (Nearly any model should work, like the Uno, Mega, Micro, etc)
 * Lights to control (LEDs? Dance pad lights? Neons? Anything goes!)

---

## Step 1: Wiring setup
First, you'll want to decide how you want to wire up your lights:



### Wire single lights to single Arduino pins (easier, recommended)
By default, the code is set up so you can wire a single light to each Arduino pin.

If you go this route, you don't need to modify the code! Just wire up your Arduino as shown below and upload the code as-is to your Arduino:


By default, the code controls nearly all the lights for a complete DDR setup - 4 arrow panel and start/menu button lights for each player, 4 marquee lights, and a bass neon light.


### Wire up tons of lights to shift registers
StepMania can control a *lot* more lights than your Arduino has pins, which is a problem if you want to wire up *all* the lights StepMania can control. Ahhh!

To wire up more lights, you can use **Shift Registers** to get more output pins on your Arduino.

[Adafruit has a great tutorial on them here](https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/overview) that's well worth a read. TL;DR: Wire up your Arduino like this, and you're off to the races:



By default, the code outputs data to 4 shift registers: 1 per player for gameplay buttons (like arrow panel lights on a DDR cab), 1 for cabinet lighting, and 1 for the bass light (with tons of extra pins if you want to mod the code to control even *more* lights).

---



## Step 2: Software Setup
There's two methods that'll work to set up StepMania to output lights data to an Arduino:

### Windows
On Windows, skogaby's Win32Serial Lights Driver is used. As far as I can tell, this driver is only in StepMania 5.3 and on [his StepMania fork](https://github.com/skogaby/stepmania/).


### Linux
On Linux (potentially Mac as well), the SextetStreamToFile Lights Driver is used. This is available in most versions of StepMania 5 (including 5.3), and requires a bit more setup than Win32Serial.



## Step 3: Enjoy! :D