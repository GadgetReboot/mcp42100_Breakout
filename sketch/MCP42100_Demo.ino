/*
   MCP42100 Dual 100K Digital Pot Demo
   Based on examples at https://wiki.dfrobot.com/Dual_Digital_Pot__100K__SKU__DFR0520

   Adjusts pot wipers between min and max (0-255)
   to allow measuring the resistance from the wiper to
   one of the end terminals and to observe the operation of the device.

   Wiper setting is displayed on the serial monitor for confirmation.

   Arduino Uno SPI connections:

   Uno/Nano       MCP42100
   -------------------------
   D2            CS  (pin 1)
   D11 (MOSI)    SI  (pin 3)
   D13 (SCK)     SCK (pin 2)

   Gadget Reboot
*/

#include <SPI.h>

const byte csPin           = 2;       // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 100000;  // 100k pot resistance between terminals A and B
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr
const byte pot1            = 0x12;    // pot1 addr
const byte potBoth         = 0x13;    // pot0 and pot1 simultaneous addr
const byte pot0Shutdown    = 0x21;    // pot0 shutdown
const byte pot1Shutdown    = 0x22;    // pot1 shutdown
const byte potBothShutdown = 0x23;    // pot0 and pot1 simultaneous shutdown

void setup() {

  Serial.begin(115200);

  digitalWrite(csPin, HIGH);        // chip select defaults to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();

  Serial.println("MCP42100 Demo");
  Serial.println("First pot0 wiper moves from min to max in approx. 25% increments.");
  Serial.println("Then pot0 wiper moves from min to max and max to min in single increments");
  Serial.println();

  doDemo1();  // move pot0 wiper as a startup demo

}

void loop() {

  // move pot0 wiper from min to max and max to min in increments of 1 position
  Serial.println("Pot 0 Control ");
  for (int i = 0; i < 256; i++) {
    setPotWiper(pot0, i, false);
    delay(250);
  }
  for (int i = 255; i > 0; i--) {
    setPotWiper(pot0, i, false);
    delay(250);
  }


  // repeat previous pot0 wiper movement and add pot1 wiper movements
  Serial.println("Pot 0 Control ");
  for (int i = 0; i < 256; i++) {
    setPotWiper(pot0, i, false);
    // for every 10 increments of pot0, run through a bunch of moves of pot1 wiper
    if (i % 10 == 0) {
      for (int j = 0; j < 256; j++) {
        setPotWiper(pot1, j, false);
        delay(50);
        setPotWiper(pot1, 128, false);
      }
    }
    delay(250);
  }
  for (int i = 255; i > 0; i--) {
    setPotWiper(pot0, i, false);
    // for every 10 increments of pot0, run through a bunch of moves of pot1 wiper
    if (i % 10 == 0) {
      for (int j = 0; j < 256; j++) {
        setPotWiper(pot1, j, false);
        delay(100);
        setPotWiper(pot1, 128, false);
      }
    }
    delay(250);
  }

}

void setPotWiper(int addr, int pos, bool debugPrint) {

  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255

  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  if (debugPrint) {
    // print pot resistance between wiper and B terminal
    long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
    Serial.print("Wiper position: ");
    Serial.print(pos);
    Serial.print(" Resistance wiper to B terminal: ");
    Serial.print(resistanceWB);
    Serial.println(" ohms");
  }
}

void doDemo1() {

  // move pot0 wiper directly to specific positions
  Serial.println("Pot 0 Control ");

  setPotWiper(pot0, 0, true);                    // minimum resistance
  delay(2000);

  setPotWiper(pot0, 64, true);                   // 25% resistance
  delay(2000);

  setPotWiper(pot0, 128, true);                  // 50% resistance
  delay(2000);

  setPotWiper(pot0, 192, true);                  // 75% resistance
  delay(2000);

  setPotWiper(pot0, 255, true);                  // 100% resistance
  delay(2000);


  // move pot0 wiper from min to max in increments of 1 position
  Serial.println("Pot 0 Control ");
  for (int i = 0; i < 256; i++) {
    setPotWiper(pot0, i, true);
    delay(250);
  }

  // move pot0 wiper from max to min in decrements of 1 position
  for (int i = 255; i > 0; i--) {
    setPotWiper(pot0, i, true);
    delay(250);
  }
}
