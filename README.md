# SM5-Arduino-Lighting
**Use an Arduino to get game-controlled lights in StepMania 5!**

## What's it do?



## Wiring setup
First, you'll want to decide how you want to wire up your lights:


> **Direct** mode - Wire individual lights to individual Arduino pins (default).
Simple, but you can only control as many lights as you have pins.


> **Shift register** mode - Wire as many lights as you want to shift registers
Slightly more complex wiring + extra parts, but lets you control more lights than you have Arduino pins!


## Adding more lights.
SM *can* control a *ton* of lights, but this code only controls the most common ones.

**Want to control a light this code doesn't control by default? Read on!**