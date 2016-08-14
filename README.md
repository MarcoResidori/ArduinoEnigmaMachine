============================
Arduino Enigma Machine
============================

Arduino sketch that implments a very simplified version of the enigma machine.

![image 1](images/enigma-machine-1.png) 

----------------------------------
Hardware Setup
----------------------------------
* Arduino Uno 
* LCD Keypad Shield (form DFRobot)
* Buzzer
* USB Cable

----------------------------------
Usage
----------------------------------
*Phase 1 - Set the rotors*  
* Turn on the Arduino board
* Press the buttons 'left' and 'righ' to select a rotor
* Press the buttons 'up' and 'down' to set a rotor initial value  
![image 2](images/enigma-machine-2.png)  

*Set the value to be encrypted/decrypted*  
* Move the cursor to the first position of the second line of the LCD display
* Press the buttons 'up' and 'down' to set the value to be encrypted/decrypted
  Please note that only characters for A to Z are supported  
![image 3](images/enigma-machine-3.png)  

*Phase 3 - Encrypt/decrypt*  
* Press the button 'select' do encrypt/decrypt the selected value  
![image 4](images/enigma-machine-4.png)  

Repeat the procedure for each value of your secret message.  

