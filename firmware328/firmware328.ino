/*
  Giant NES controller
  Arduino sketch for the atmega328p on the Arduino Uno board

  Note that compiling this sketch requires you to select the "Arduino Uno" board.
  
  Flashing the firmware requires the use of an AVR programmer, once you've connected one to the ISP header of the atmega328p
  select your programmer type in the "Tools > Programmer: ..." menu, then press "Sketch > Upload Using Programmer (Ctrl+Shift+U)"
  to upload the firmware to the atmega328p using your programmer.

  In addition to this firwmare a separate firmware for the atmega16u2 on your Arduino Uno needs to be flashed to actually be able
  to send the button state to your computer!

  - CC0, Renze Nicolai 2019, TkkrLab hackerspace Enschede
*/

//Amount of pins used
#define NUM_BUTTONS 8

//First pin to be used (pin 2 is button #1, pin 3 is button #2 and so on...
#define FIRST_PIN 2

//Array to store the previous state of a button. Initially we assume all buttons to be released
bool prevState[NUM_BUTTONS] = {false};

void setup() {
  Serial.begin(115200); //Start the UART serial port (this is the connection to the atmega16u2)
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) { //For each button
    pinMode(FIRST_PIN + i, INPUT_PULLUP); //Set the mode of the pin corresponding to the button to input with pullup
  }
}

void loop() {
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) { //For each button
    bool currentState = !digitalRead(FIRST_PIN + i); //Read the button state, note: buttons are active low due to the use of the pullup
    if (currentState != prevState[i]) { //If the state of the button changed
      //Send an 'A' (65) + current button number to the atmega16u2 if the button has been released
      //or send an 'a' (97) + current button number if the button has been pressed
      Serial.write(65+i+32*currentState); //("A", "B", "C" etc when releasing the button and "a", "b", "c" etc when pressing the button)
      prevState[i] = currentState; //Store the current button state for use in the next cycle
    }
  }
  delay(50); //Wait for a short while (50ms), as a sort-of debounce feature
}
