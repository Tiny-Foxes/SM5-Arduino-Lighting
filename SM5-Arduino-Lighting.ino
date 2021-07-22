/*************************************
 ***** SM5 ARDUINO LIGHTS DRIVER *****
 ************************************/

// Get game-controlled cabinet and controller/pad lights in StepMania 5!
// All you need is any kind of Arduino that can connect to a PC via serial and you're good to go!
// (a.k.a. use pretty much any Arduino - Uno, mega, micro, etc)

//  Written to be easy to understand/modify by 48productions




/*********************
 *** WIRING CONFIG ***
 ********************/
 
// There's two routes for wiring, with and without shift registers
//  - Hooking up individual lights to individual arduino pins (the default)? Comment out the below #define line.
//  - Using shift registers (use only 3 pins for all the lights)? Uncomment the below line

//#define USE_SHIFT_REGISTERS

//Don't touch this next line please kthx
#ifndef USE_SHIFT_REGISTERS



/************************
 * WIRING PINS (direct) *
 ***********************/
// PINS - What pins should the Arduino use?
// IF USING DIRECT LIGHTING MODE (#define USE_SHIFT_REGISTERS is COMMENTED OUT)
// Set which lights are output to what pins, here!

//P1 pad light pins
#define PIN_P1_LEFT 2 //P1 Left
#define PIN_P1_RIGHT 3 //P1 Right
#define PIN_P1_UP 4 //P1 Up
#define PIN_P1_DOWN 5 //P1 Down

// P2 pad light pins
#define PIN_P2_LEFT 6 //P2 Left
#define PIN_P2_RIGHT 7 //P2 Right
#define PIN_P2_UP 8 //P2 Up
#define PIN_P2_DOWN 9 //P2 Down

// Cab light pins
#define PIN_MARQUEE_1 A0 //Marquee up-left
#define PIN_MARQUEE_2 A1 //Marquee up-right
#define PIN_MARQUEE_3 A2 //Marquee down-left
#define PIN_MARQUEE_4 A3 //Marquee down-right
#define PIN_BASS A4 //Bass left

// Menu light pins
#define PIN_P1_START 10 //P1 start button
#define PIN_P1_MENU 11  //P1 menu buttons
#define PIN_P2_START 12 //P2 start button
#define PIN_P2_MENU 13  //P2 menu buttons



/********************************
 * WIRING PINS (shift register) *
 *******************************/
// IF USING SHIFT REGISTER LIGHTING (#define USE_SHIFT_REGISTERS is NOT commented)
// Set which pins you'll hook up your shift registers to here.
// That's right, only 3 pins for EVERY light!
#else
#define PIN_SHIFT_CLOCK 2
#define PIN_SHIFT_DATA 3
#define PIN_SHIFT_LATCH 4


#endif






/*****************
 *** MAIN CODE ***
 ****************/

// First, define variables:
// (Each byte contains 8 bits, individual lights are tracked with individual bits)
byte p1LEDs = 0; //Tracks P1's lights (SM gameplay buttons 0-7)
byte p2LEDs = 0; //Tracks P2's lights (SM gameplay buttons 0-7)
byte cabLEDs = 0; //Tracks cabinet lighting. 0-3: Marquee lights, 4: P1 Start, 5: P1 Menu, 6: P2 Start, 7: P2 Menu
byte etcLEDs = 0; //Tracks other lights. 0: Bass neon. All other slots unused - if you want to output more lights from SM, here's a good spot.

byte receivedData = 0; //The last byte of data we received from Stepmania
short lightBytePos = 0; //Tracks how many bytes of lighting data we've received this update


// setup() - Run once on startup
void setup() {
  Serial.begin(115200);
  
  //Pinmode ALL the pins!
  // ... except the ones we don't need

#ifdef USE_SHIFT_REGISTERS  // If using shift registers, only pinmode the pins needed for shift registers
  pinMode(PIN_SHIFT_CLOCK, OUTPUT);
  pinMode(PIN_SHIFT_DATA, OUTPUT);
  pinMode(PIN_SHIFT_LATCH, OUTPUT);

#else  // In direct lighting mode? Only pinmode what's needed here
  //P1 pad lights 
  pinMode(PIN_P1_LEFT, OUTPUT);
  pinMode(PIN_P1_RIGHT, OUTPUT);
  pinMode(PIN_P1_UP, OUTPUT);
  pinMode(PIN_P1_DOWN, OUTPUT);
  
  //P2 pad lights
  pinMode(PIN_P2_LEFT, OUTPUT);
  pinMode(PIN_P2_RIGHT, OUTPUT);
  pinMode(PIN_P2_UP, OUTPUT);
  pinMode(PIN_P2_DOWN, OUTPUT);
  
  //Cabinet lights
  pinMode(PIN_MARQUEE_1, OUTPUT);
  pinMode(PIN_MARQUEE_2, OUTPUT);
  pinMode(PIN_MARQUEE_3, OUTPUT);
  pinMode(PIN_MARQUEE_4, OUTPUT);
  pinMode(PIN_P1_START, OUTPUT);
  pinMode(PIN_P1_MENU, OUTPUT);
  pinMode(PIN_P2_START, OUTPUT);
  pinMode(PIN_P2_MENU, OUTPUT);
  pinMode(PIN_BASS, OUTPUT);
#endif
}



// loop() - runs over and over again
void loop() {
  if (Serial.available() > 0) { //Do we have lights data to receive?
    readSerialLightingData();
  }
  //Done receiving serial data this loop

}




//Writes SM lighting directly to arduino pins
// Called whenever new lighting data is received, if not using shift registers

#ifndef USE_SHIFT_REGISTERS //This function is only compiled if we're not in shift register mode
void writeDirectLighting() {

  // SIDENOTE: Want to make mods to change which specific lights from SM are output to the Arduino pins?
  // Or: Have an Arduino Mega and want to add more lights?
  // This is the place for that!

  //Data is placed into p1LEDs/p2LEDs/cabLEDs/etcLEDs when we read lighting data from Stepmania in "void loop()"
  //We'll read specific lights from that data here, and turn on/off arduino pins based on them

  digitalWrite(PIN_P1_LEFT, bitRead(p1LEDs, 0)); //P1 Pad
  digitalWrite(PIN_P1_RIGHT, bitRead(p1LEDs, 1));
  digitalWrite(PIN_P1_UP, bitRead(p1LEDs, 2));
  digitalWrite(PIN_P1_DOWN, bitRead(p1LEDs, 3));
  
  digitalWrite(PIN_P2_LEFT, bitRead(p2LEDs, 0)); //P2 pad
  digitalWrite(PIN_P2_RIGHT, bitRead(p2LEDs, 1));
  digitalWrite(PIN_P2_UP, bitRead(p2LEDs, 2));
  digitalWrite(PIN_P2_DOWN, bitRead(p2LEDs, 3));
  
  digitalWrite(PIN_MARQUEE_1, bitRead(cabLEDs, 0)); //Cabinet lights
  digitalWrite(PIN_MARQUEE_2, bitRead(cabLEDs, 1));
  digitalWrite(PIN_MARQUEE_3, bitRead(cabLEDs, 2));
  digitalWrite(PIN_MARQUEE_4, bitRead(cabLEDs, 3));
  digitalWrite(PIN_P1_START, bitRead(cabLEDs, 4));
  digitalWrite(PIN_P1_MENU, bitRead(cabLEDs, 5));
  digitalWrite(PIN_P2_START, bitRead(cabLEDs, 6));
  digitalWrite(PIN_P2_MENU, bitRead(cabLEDs, 7));
  digitalWrite(PIN_BASS, bitRead(etcLEDs, 0));
}
#endif




//Writes SM lighting data to a set of shift registers
// Called whenever new lighting data is received, when using shift registers

#ifdef USE_SHIFT_REGISTERS //This function is only compiled if we're in shift register mode
void writeShiftRegisterLighting() {
  digitalWrite(PIN_SHIFT_LATCH, LOW); //Pull latch pin low, shift out data, and throw latch pin high again

  // Add more shiftOut lines here if you want to add more shift registers
  //  (the first shiftOut line goes to the last shift register in the chain)

  shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, LSBFIRST, etcLEDs);
  shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, LSBFIRST, cabLEDs);
  shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, LSBFIRST, p2LEDs);
  shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, LSBFIRST, p1LEDs);
  
  digitalWrite(PIN_SHIFT_LATCH, HIGH);
}
#endif


//Reads sextetstream-formatted lighting data from serial when called
// Updates p1LEDs, p2LEDs, cabLEDs, and etcLEDs
void readSerialLightingData() {
  while (Serial.available() > 0) { //While we have lights data to receive, receive and process it!
      receivedData = Serial.read(); //Read the next byte of serial data
      //Serial.println(receivedData);


      // > Q&A: "What format does Stepmania send lighting updates in?"
      // Each lighting update sent by Stepmania contains 13 bytes of data, and a newline (\n) character.
      // We track how many of these bytes we've received with lightBytePos - every time we get a byte, we increment this number.
      // If we receive a newline character, we know the update is done and can reset this counter to 0
      // Knowing how much data we've received so far tells us what lights the next byte of data controls!
      // (More technical info at https://github.com/stepmania/stepmania/blob/master/src/arch/Lights/LightsDriver_SextetStream.md )

      
      if (receivedData == '\n') { //If we got a newline (\n), we're done receiving new light states for this update
        lightBytePos = 0; //The next byte of lighting data will be the first byte
        
        //When we're done processing the serial data we have right now, write it out!
#ifdef USE_SHIFT_REGISTERS
        writeShiftRegisterLighting();
#else
        writeDirectLighting();
#endif
      } else {

        switch (lightBytePos) { //Which byte of lighting data are we now receiving?
          case 0: //First byte of lighting data (cabinet lights)
            //Read x bit from the received byte using bitRead(), then set the corresponding light to that state
            bitWrite(cabLEDs, 0, bitRead(receivedData, 0)); //Marquee up left
            bitWrite(cabLEDs, 1, bitRead(receivedData, 1)); //Marquee up right
            bitWrite(cabLEDs, 2, bitRead(receivedData, 2)); //Marquee down left
            bitWrite(cabLEDs, 3, bitRead(receivedData, 3)); //Marquee down right
            
            bitWrite(etcLEDs, 0, bitRead(receivedData, 4)); //Bass L
            //bitWrite(etcLEDs, 1, bitRead(receivedData, 5)); //Bass R (unless a song's lighting chart says otherwise, almost always the same state as Bass L)
            break;
            
          case 1: //Second byte of lighting data (P1 menu button lights)
            bitWrite(cabLEDs, 5, bitRead(receivedData, 0)); //P1 menu left (menu right usually same state)
            bitWrite(cabLEDs, 4, bitRead(receivedData, 4)); //P1 start

            //bitWrite(etcLEDs, 2, bitRead(receivedData, 5)); //P1 select
            break;
            
          case 3: //Byte 4: P1 gameplay button lights 1-6
            //The first 6 gameplay button lights are in receivedData.
            //For this and P2's gameplay lights, we'll copy receivedData directly to p1LEDs.
            //BUT: We'll omit the highest 2 bytes of receivedData with "& 0x3F" (bitwise AND) since they don't contain lighting data.
            p1LEDs = receivedData & 0x3F;
            break;

          case 4: //Byte 5: P1 gameplay button lights 7-12
            //I can almost certainly bitwise this to be more compact. Buuut, nah.
            bitWrite(p1LEDs, 6, bitRead(receivedData, 0));
            bitWrite(p1LEDs, 7, bitRead(receivedData, 1));
            break;

          //Bytes 6-7 are for more P1 gameplay buttons that we don't read
          
          case 7: //Byte 8: P2 menu
            bitWrite(cabLEDs, 7, bitRead(receivedData, 0)); //P2 menu left (menu right usually same state)
            bitWrite(cabLEDs, 6, bitRead(receivedData, 4)); //P2 start

            //bitWrite(etcLEDs, 3, bitRead(receivedData, 5)); //P2 select
            break;
            
          case 9: //Byte 10: P2 gameplay button lights 1-6
            //Same as P1's gameplay lights, copy the lower 6 bis of receivedData to p2LEDs (high 2 bits omitted with & 0x3F)
            p2LEDs = receivedData & 0x3F;
            break;

          case 10: //Byte 11: P2 gameplay button lights 7-12
            bitWrite(p2LEDs, 6, bitRead(receivedData, 0));
            bitWrite(p2LEDs, 7, bitRead(receivedData, 1));
            break;

          //Bytes 12-13 are for more P2 gameplay buttons we don't read
          
        }
        lightBytePos++; //Finally, update how many bytes of lighting data we've received.
        
      }
      //Done receiving this byte of data!
      
  }
  //We've read all the serial data we can for now
}
