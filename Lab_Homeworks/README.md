# Details about my homeworks

# First homework:

  Control a RGB led with 3 potentiometers.
  
  This shows you how to read from three analog input pins, map the result to a range from 0 to 255, use that result to set the pulse width   modulation (PWM) of three output pins to dim or brighten an RGB LED and print the values on the serial monitor of the Arduino Software.
  
  Hardware Required:
      
      1. RGB led
      2. 3x Resistors (recommended 220ohm)
      3. Wires
      4. 3x Potentiometers
      5. Arduino
      6. Connector cable

# Second homework:
  
  Play a tune when passive piezo buzzer detect knock's.
  
  After 5 seconds since knock detection, passive piezo buzzers will start play a tune which have frequency 1000. We use tone() function     for that.
  You can stop the tune when you press the PULL_UP button.
  
  Hardware Required:
      
      1. 1x Passive buzzer for knock detection
      2. 1x Active buzzer for sound
      3. 1x Pushbutton
      4. 2x Resistors, for buzzers (recommended 100ohm)
      5. Wires
      6. Arduino
      7. Connector cable
      
# Third homework:

  Control a 4 digits 7-segments led display with a joystick.
  
  This program is individually controlling each digit of the segment with the joystick. Use the X-axis, cycle through the digits and chose which one to modify. We light up the DP led of the current position so we know what digit is selected. When the button is pressed again, the selected digit is locked. After that, use the Y-axis to modify the value of the digit, both up and down. Press the button again to unlock the selection and be able to cycle through the 4 digits.
  
  Hardware Required:
    
      1. 1x 4 digits 7-segments display
      2. 1x Joystick
      3. 4x Resistors (330ohm)
      4. Wires
      5. Arduino
      6. Connector cable
  
  
 # Fourth homework:
 
  Build a menu game.
  
  At the start, on lcd display will show 3 things:
  
  - Start game:
      When you press the switch button, the game will start. You can set what level do you want to play from Settings. As default, the level is set to be 1. After 5 seconds, the level is increasing and the score will be modified according to (score = lvl * 3). After 10 seconds, the game is over and "Congratulations!" is printed to the screen. If you want to return to the main menu, you should press switch button.
      
  - Highscore:
       If you choose "Highscore", the display will show the biggest score set by a player. The value is stored with EEPROM. This maskes the score to remain unchanged even if you close the arduino. If you want to return to the main menu, you should press switch button.
       
  - Settings:
      If you go for "Settings", the display will show your name and your start level. The name is fixed and the default level is 1. If you press the switch button on "Level", you can choose with joystick what level do you want to play. After you choose the starting level, you should press again the switch button to save the modifies. If you want to return to the main menu, you should push your joystick to the left.
     
  Hardware Required:
      
        1. 1x Liquid Crystal Display
        2. 1x Joystick
        3. 1x Resistors (330 ohm)
        4. 1x Potentiometer
        5. Wires
        6. Arduino
        7. Connector cable


  
  
  
