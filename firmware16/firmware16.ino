/*
  Giant NES controller
  Arduino sketch for the atmega16u2 on the Arduino Uno board

  Based on the joystick example for the NicoHood HID Arduino library
  https://github.com/NicoHood/HID

  Note that compiling this sketch requires you to select the "HoodLoader2 16u2" board found using the following board manager URL:
  https://raw.githubusercontent.com/NicoHood/HoodLoader2/master/package_NicoHood_HoodLoader2_index.json
  
  Flashing the firmware requires the use of an AVR programmer, once you've connected one to the ISP header of the atmega16u2
  select your programmer type in the "Tools > Programmer: ..." menu, then press "Sketch > Upload Using Programmer (Ctrl+Shift+U)"
  to upload the firmware to the atmega16u2 using your programmer.

  After programming unplug the USB cable and plug it back in to get your Arduino recognized as both a USB to serial adapter and as
  joystick/gamepad device. 

  In addition to this firwmare a separate firmware for the atmega328p on your Arduino Uno needs to be flashed to actually be able
  to press some buttons!

  - CC0, Renze Nicolai 2019, TkkrLab hackerspace Enschede
*/

#include "HID-Project.h"

void setup() {
  Gamepad.begin(); //Start the HID gamepad library
  Serial.begin(115200); //Start the USB serial port (used for debug messages)
  Serial1.begin(115200); //Start the UART serial port (this is the connection to the atmega328p)
}

void loop() {
  bool doSend = false; //Flag that is set whenever a button changes state, triggers gamepad write
  
  while (Serial1.available()) { //While there are characters received on the UART serial port
    uint8_t val = Serial1.read(); //Read the received character
    if (val >= 65) { //Check if the character is equal or higher than 65, that is the ascii number for 'A'
      val -= 65; //Modify the value so that 'A' corresponds to 0
      if (val >= 32) { //If the value is equal or higher than 32 at this point then the character is lower-case (ascii code for 'a' is 65+32=97)
        val -= 32; //Modify the value so that 'a' corresponds to 0
        Serial.println("Button "+String(val+1)+" pressed!");
        Gamepad.press(val+1); //The value goes from 'a'=0 to 'z'=25 (or even '~'=29), add one so that 0 corresponds to button #1
      } else {
        Serial.println("Button "+String(val+1)+" released!");
        Gamepad.release(val+1); //The value goes from 'A'=0 to 'Z'=25 (or even '`'=29), add one so that 0 corresponds to button #1
      }
      doSend = true; //A button state has been changed, set the flag
    }
  }
  
  if (doSend) { //If the flag is set
    doSend = false; //Reset the flag
    Gamepad.write(); //Send the button state to the computer
  }
}
