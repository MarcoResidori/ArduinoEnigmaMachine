# Arduino Enigma Machine

Arduino sketch that implements a very simplified version of the [enigma machine](https://en.wikipedia.org/wiki/Enigma_machine "Enigma Machine").

![image 1](images/enigma-machine-1.png) 

## Hardware Setup
* Arduino Uno 
* DFRobot LCD Keypad Shield
* Buzzer
* USB Cable

## Usage
*Phase 1 - Set the rotors*  
* Turn on the Arduino board
* Press the buttons 'left' and 'right' to select a rotor
* Press the buttons 'up' and 'down' to set the initial rotor value  
![image 2](images/enigma-machine-2.png)  

*Phase 2 - Set the value to be encrypted/decrypted*  
* Move the cursor to the first position of the LCD display's second line   
* Press the buttons 'up' and 'down' to set the value to be encrypted/decrypted  
![image 3](images/enigma-machine-3.png)  

*Phase 3 - Encrypt/decrypt*  
* Press the button 'select' to encrypt/decrypt the selected value  
  Please note that the rotors may change value (rotation) when 'select' is pressed  
![image 4](images/enigma-machine-4.png)  

Repeat the procedure for each value of your 'secret' message.  

## Example
Initial rotor values: R1=**D**, R2=**E**, R3=**F**  
Message to encrypt: **HELLOWORLD**  
Encrypted message: **STGALBRCEW**  
