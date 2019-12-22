# Line Invaders

## Description

   Line invaders is a 8x8 LED Matrix Arduino game which follow Chicken Invader's mechanics. The name "Line invaders" comes from the fact that in this game the monsters are replaced with lines. The game contains 4 levels and there is some difference between them. The monsters have different respawn, the speed of the bullets decreases as the level increases and the enemies speed increases as the level decreases (implemented but not working because settings dont works properly).
    
## How to play

   The main menu is controlled using the Y-axis of the joystick. Pressing the JS enters the shown menu. In the rest of documentation, I will present what the menu contains.
    
## Play Game
  
   The ship movement is controlled with X-axis of the joystick and the firing system is realised pressing the JS. The bullets goes from the center of the ship to the top of the matrix. The enemies are coming from top to bottom. If they reach the bottom of the matrix without dying, the player will lose one life. You have 3 lives, if you lose all of them, you died. If you kill all the enemies without losing all of your lives, you passed up the level.
    
## High Score
    
   This function shows the best score that was inregistred by a player. The score is saved on EEPROM, so the highscore remains even if the arduino is restarted. To exit the High Score function, press the JS.
    
## Settings

   Implemented but not working properly.
    
## Demo:

   A short video with the game that shows how it works.
   https://youtu.be/oq3nEfFHTH0
 
## Hardware Required:

    1. 1x 8x8 LED Matrix
    2. 1x Liquid Crystal Display
    3. 2x Joystick
    4. Resistors
    5. Driver for Matrix
    6. Wires
    7. Conector Cable
    8. Arduino
    
## Used libraries:

   1. Liquid menu (https://github.com/VaSe7u/LiquidMenu),  is a very cool and easy tool to create awesome menus, but that comes with some disadvances. The menu consume many memory resources and that can be very uncomfortable in larger projects.
   2. LedControl is a tool used to control the Led's on the matrix.
   3. LiquidCrystal is a tool used to control the LCD.


  
  

