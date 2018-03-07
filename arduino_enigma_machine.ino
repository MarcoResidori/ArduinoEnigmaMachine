/*
 * Board:   Arduino Uno
 * Shield:  LCD Keypad Shield For Arduino - DFRobot
 * File:    arduino_enigma_machine
 * Version: 1.0
 * Author:  Marco Residori (residori@gmx.net) 
 * License: GPLv2
 *
 * Description: Simplified version of the enigma machine with 3 rotors
 *          
 */

//liquid crystal library
#include <LiquidCrystal.h>

//select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define pinBuzzer 2

//values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

//keys
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//sounds
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

#define LEFT 0 
#define RIGHT 1

#define NUM_CURSOR_POSITIONS 4

#define ROT_INITIAL_POS 'A' 
#define ROT1_NOTCH 'D'
#define ROT2_NOTCH 'B' 

char rot1Pos;
char rot1NotchPos;
char rot2Pos;
char rot2NotchPos;
char rot3Pos;

int cursPosId;
char input;
char output;

const int NUM_LETTERS = 'Z' - 'A' + 1;

struct DisplayPos 
{
  int col;
  int raw;
};

DisplayPos cursPos[NUM_CURSOR_POSITIONS] = {{3,0},{8,0},{13,0},{0,1}};

char reflector[NUM_LETTERS][2] =
{ {'A','F'},
  {'B','Z'},
  {'C','H'},
  {'D','L'},
  {'E','J'},
  {'F','A'},
  {'G','P'},
  {'H','C'},
  {'I','M'},
  {'J','E'},
  {'K','N'},
  {'L','D'},
  {'M','I'},
  {'N','K'},
  {'O','Y'},
  {'P','G'},
  {'Q','W'},
  {'R','S'},
  {'S','R'},
  {'T','V'},
  {'U','X'},
  {'V','T'},
  {'W','Q'},
  {'X','U'},
  {'Y','O'},
  {'Z','B'},
};

char rotor1[NUM_LETTERS][2] =
{ {'A','D'},
  {'B','M'},
  {'C','T'},
  {'D','W'},
  {'E','S'},
  {'F','I'},
  {'G','L'},
  {'H','R'},
  {'I','U'},
  {'J','Y'},
  {'K','Q'},
  {'L','N'},
  {'M','K'},
  {'N','F'},
  {'O','E'},
  {'P','J'},
  {'Q','C'},
  {'R','A'},
  {'S','Z'},
  {'T','B'},
  {'U','P'},
  {'V','G'},
  {'W','X'},
  {'X','O'},
  {'Y','H'},
  {'Z','V'},
};

char rotor2[NUM_LETTERS][2] =
{ {'A','A'},
  {'B','J'},
  {'C','D'},
  {'D','K'},
  {'E','S'},
  {'F','I'},
  {'G','R'},
  {'H','U'},
  {'I','X'},
  {'J','B'},
  {'K','L'},
  {'L','H'},
  {'M','W'},
  {'N','T'},
  {'O','M'},
  {'P','C'},
  {'Q','Q'},
  {'R','G'},
  {'S','Z'},
  {'T','N'},
  {'U','P'},
  {'V','Y'},
  {'W','F'},
  {'X','V'},
  {'Y','O'},
  {'Z','E'},
};

char rotor3[NUM_LETTERS][2] =
{ {'A','E'},
  {'B','K'},
  {'C','M'},
  {'D','F'},
  {'E','L'},
  {'F','G'},
  {'G','D'},
  {'H','Q'},
  {'I','V'},
  {'J','Z'},
  {'K','N'},
  {'L','T'},
  {'M','O'},
  {'N','W'},
  {'O','Y'},
  {'P','H'},
  {'Q','X'},
  {'R','U'},
  {'S','S'},
  {'T','P'},
  {'U','A'},
  {'V','I'},
  {'W','B'},
  {'X','R'},
  {'Y','C'},
  {'Z','J'},
};

int getOffset(char c)
{
  if(c < 'A' || c > 'Z') 
  {
    return -1;
  }
  
  return c - 'A';
}

char getLetter(int i)
{
  if(i < 0 || i >= NUM_LETTERS) 
  {
    return '!';
  }
  
  return 'A' + i;
}

char getRotorLetter(char disc[][2], int index, int pos)
{
  char c = '!';
  
  if(disc == NULL) 
  {
    return c;
  }
  
  if(*disc == NULL) 
  {
    return c;
  }
  
  if(index < 0 || index >= NUM_LETTERS) 
  {
    return c;
  }
  
  if(pos != LEFT && pos != RIGHT) 
  {
    return c;
  }
  
  c = disc[index][pos];
  
  if(c < 'A' || c > 'Z') 
  {
    return '!';
  }
  
  return c;
}

int getRotorOffset(char disc[][2], char c, int pos)
{
  if(disc == NULL) 
  {
    return -1;
  }
  
  if(*disc == NULL) 
  {
    return -1;
  }
  
  if(pos != LEFT && pos != RIGHT) 
  {
    return -1;
  }
  
  if(c < 'A' || c > 'Z') 
  {
    return -1;
  }
  
  for (int i = 0; i < NUM_LETTERS; i ++)
  {
    if(disc[i][pos] == c)
    {
        return i;
    }
  }
  
  return -1;
}

void incrementInput()
{
  input++;
  if(input > 'Z')
  {
     input = 'A';
  }
}

void incrementRotor1()
{
  rot1Pos++;
  if(rot1Pos > 'Z')
  {
     rot1Pos = 'A';
  }
  
  if(rot1Pos == rot1NotchPos)
  {
    rot2Pos++;
    if(rot2Pos > 'Z')
    {
      rot2Pos = 'A';
    }

    if(rot2Pos == rot2NotchPos)
    {
      rot3Pos++;
      if(rot3Pos > 'Z')
      {
         rot3Pos = 'A';
      }
    }
  }
}

void incrementRotor2()
{
  rot2Pos++;
  if(rot2Pos > 'Z')
  {
    rot2Pos = 'A';
  }

  if(rot2Pos == rot2NotchPos)
  {
    rot3Pos++;
    if(rot3Pos > 'Z')
    {
       rot3Pos = 'A';
    }
  }
}

void incrementRotor3()
{
  rot3Pos++; 
  if(rot3Pos > 'Z')
  {
    rot3Pos = 'A';
  }
}

void decrementInput()
{
  input--;
  if(input < 'A')
  {
     input = 'Z';
  }
}

void decrementRotor1()
{
  rot1Pos--;
  if(rot1Pos < 'A')
  {
     rot1Pos = 'Z';
  }
}

void decrementRotor2()
{
  rot2Pos--;
  if(rot2Pos < 'A')
  {
    rot2Pos = 'Z';
  }
}

void decrementRotor3()
{
  rot3Pos--;
  if(rot3Pos < 'A')
  {
    rot3Pos = 'Z';
  }
}

int modulo(int val)
{
  return ((val + NUM_LETTERS) % NUM_LETTERS);
}

void updateDisplay()
{
  lcd.setCursor(0,0);
  lcd.print("R1:  R2:  R3:  ");
  
  lcd.setCursor(3,0);
  lcd.print(rot1Pos);
  
  lcd.setCursor(8,0);
  lcd.print(rot2Pos);
  
  lcd.setCursor(13,0);
  lcd.print(rot3Pos);
  
  lcd.setCursor(0,1); 
  lcd.print(input);
  
  lcd.setCursor(2,1); 
  lcd.print("=>");
  
  lcd.setCursor(5,1); 
  lcd.print(output);
  
  lcd.setCursor(cursPos[cursPosId].col,cursPos[cursPosId].raw);
  lcd.blink();
}


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);   
 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNONE;  
}

int checkMinInterval(unsigned long minInterval)
{
  int ret;
  static unsigned long time = 0;
  static unsigned long time2 = 0;
 
  time = millis(); 
  ret = ((time - time2) >= minInterval);
  time2 = time;
  
  //Serial.print("Ret:");
  //Serial.println(ret);

  return ret;
} 

void setup()
{
  
 Serial.begin(9600);
 lcd.begin(16, 2);            
 lcd.setCursor(0,0);
 lcd.print("Enigma Machine"); 
 delay(1500);
 
 rot1Pos = ROT_INITIAL_POS;
 rot1NotchPos = ROT1_NOTCH;
 
 rot2Pos = ROT_INITIAL_POS;
 rot2NotchPos = ROT2_NOTCH;
 
 rot3Pos = ROT_INITIAL_POS;
 //rot3NotchPos = 'B';
 
 cursPosId = 0;
 
 input = 'A';
 output = '?';
 
 updateDisplay();
}
 
void loop()
{
 char letter;
 char letter2;
 int offset;
 
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)              
 {
   case btnRIGHT:
     tone(pinBuzzer, NOTE_A4, 20);
     
     cursPosId++;
     if(cursPosId >= NUM_CURSOR_POSITIONS)
     {
       cursPosId = 0;
     }
     output = '?';
     break;

   case btnLEFT:
     tone(pinBuzzer, NOTE_A4, 20);
     
     cursPosId--;
     if(cursPosId < 0)
     {
       cursPosId = NUM_CURSOR_POSITIONS - 1;
     }
     output = '?';
     break;
     
   case btnUP:
     tone(pinBuzzer, NOTE_A4, 20);
     
     switch (cursPosId)
     {  
       case 0:
         incrementRotor1();
         break; 
       case 1:
         incrementRotor2();
         break; 
       case 2:
         incrementRotor3();
         break; 
       case 3:
         output = '?';
         incrementInput();
         break; 
       default:
         Serial.println("Error");
         break;
     } 
     break;
     
   case btnDOWN:
     tone(pinBuzzer, NOTE_A4, 20);
     
     switch (cursPosId)
     {  
       case 0:
         decrementRotor1();
         break; 
       case 1:
         decrementRotor2();
         break; 
       case 2:
         decrementRotor3();
         break; 
       case 3:
         output = '?';
         decrementInput();
         break; 
       default:
         Serial.println("Error");
         break;
     }  
     break;

   case btnSELECT:
 
     if(checkMinInterval(500) == 0)
     { 
       break; //prevent accidental multiple presses of the select button, by introducing a minimal delay between presses
     }

     tone(pinBuzzer, NOTE_A4, 20);
     
     //--------------------------------------------------
     Serial.print("Input:");
     Serial.println(input);
     //--------------------------------------------------
     Serial.print("R1:");
     offset = modulo(getOffset(input) + getOffset(rot1Pos) - 0); //the offset of the static disc is 0
     letter = getRotorLetter(rotor1, offset, RIGHT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor1, letter, LEFT);
     letter2 = getRotorLetter(rotor1, offset, LEFT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     Serial.print("R2:");
     offset = modulo(offset + getOffset(rot2Pos) - getOffset(rot1Pos));
     letter = getRotorLetter(rotor2, offset, RIGHT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor2, letter, LEFT);
     letter2 = getRotorLetter(rotor2, offset, LEFT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     Serial.print("R3:");
     offset = modulo(offset + getOffset(rot3Pos) - getOffset(rot2Pos)); 
     letter = getRotorLetter(rotor3, offset, RIGHT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor3, letter, LEFT);
     letter2 = getRotorLetter(rotor3, offset, LEFT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     Serial.print("Refl:");
     offset = modulo(offset + 0 - getOffset(rot3Pos));  //the offset of the reflector is 0
     letter = getRotorLetter(reflector, offset, LEFT);
     letter2 = getRotorLetter(reflector, offset, RIGHT);
     Serial.print(letter);
     Serial.print(",");
     Serial.println(letter2);
     offset = getRotorOffset(reflector, letter, RIGHT);
     //-------------------------------------------------- 
     Serial.print("R3:");
     offset = modulo(offset + getOffset(rot3Pos) - 0); //the offset of the reflector is 0
     letter = getRotorLetter(rotor3, offset, LEFT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor3, letter, RIGHT);
     letter2 = getRotorLetter(rotor3, offset, RIGHT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     Serial.print("R2:");
     offset = modulo(offset + getOffset(rot2Pos) - getOffset(rot3Pos)); 
     letter = getRotorLetter(rotor2, offset, LEFT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor2, letter, RIGHT); 
     letter2 = getRotorLetter(rotor2, offset, RIGHT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     Serial.print("R1:");
     offset = modulo(offset + getOffset(rot1Pos) - getOffset(rot2Pos)); 
     letter = getRotorLetter(rotor1, offset, LEFT);
     Serial.print(letter);
     Serial.print(",");
     offset = getRotorOffset(rotor1, letter, RIGHT);
     letter2 = getRotorLetter(rotor1, offset, RIGHT); 
     Serial.println(letter2);
     if(letter != letter2) 
     {
       output = '!'; //error
       Serial.println("Error");
       break;
     }
     //--------------------------------------------------
     offset = modulo(offset + 0 - getOffset(rot1Pos));
     output = getLetter(offset);
     Serial.print("Output:");
     Serial.println(output);
     //--------------------------------------------------
     
     incrementRotor1();
     
     break;
 
   case btnNONE:
     break;
 }
 
 updateDisplay();
 delay(150);

}
