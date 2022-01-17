# TetriSurvival

A Tetris-like game made using an Arduino board, a led matrix, an lcd and a joystick.

## Game contents

<details>
<summary>Game features</summary>
<br>
The purpose of the game is to accumulate as many points as you can while you survive as many levels as posible. Each level has a duration that you have to survive. If you lose a level you lose one heart. You lose the game if you have no hearts left.
This game has:
  
• 3 difficulties
  
• multiple levels, each harder than the last one
  
• points gained by taking risks: you get more points for completing more lines at the same time.
</details>

<details>
<summary>Other features</summary>
<br>
Game has the following features, besides the gameplay:
  
• input player name
  
• save your score
  
• brightness settings

• inverting x and y axis

• highscores saved in EEPROM

• new players tips

• credits
</details>

<details>
<summary>Link to trailer</summary>
<br>
https://www.youtube.com/watch?v=IfsCczCO6Gw&ab_channel=MarianDimofte
</details>


##Additional information useful for understaning the code:

<details>
<summary>The player namer codification</summary>
<br>
I store the player name in an array of type byte. Each letter in the array can be translated into a char using the vector myAlphabet as follows:
myAlphabet[playerName[index]] = decoded letter (char). Everywhere I decode the name I must use the function "pgm_read_byte" because I stored the
myAlphabet array in progamabile memory using PROGMEM. For additional information see: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

In addition in my coddification the "$" represents the tail of the name, (you can kinda say that strlen(playerName) = the position of the "$" but it isn't correct
because playerName is an array of bytes). I also use $ when the user adds/removes one letter. When you add another letter (at the end), by default it is 
set to be the void one ($) and you can change it after. Only the last letter can be set to be void by the user and if you do it and move your cursor from there,
the letter gets deleted (this is how the user shortens his name).
</details>

<details>
<summary>The tetromino codification</summary>
<br>
I will begin explaining all the codification by explaining how i stored the tetromino. I stored tetromino into a matrix of type bool, aka a array of
types of tetromino pieces. The tetromino piece has a "mini map" of 4 x 4 and on that "mini map" it takes some solid blocks. The map is then translated
row by row into an array and that array represents a tetromino piece in the array of types of tetromino pieces (aka the tetromino matrix).

I will explain for the first piece:

                                0 degrees:          90 degrees:         etc.
                                 0  1  2  3         12  8  4  0
                                 4  5  6  7         13  9  5  1
                                 8  9 10 11         14 10  6  2
                                12 13 14 15         15 11  7  3

Imagine in the above image that the numbers 2, 6, 10 and 14 are bold. Than the first piece (the 4 blocks long bar) will have that "mini map". In the array
that represents that bar only on thesee positions will be True value (meaning a solid block is there) and on the other positions will be False value
(empty space). You do this for every piece and this is how you get the array of arrays (aka the array of tetromino pieces).

The image above helps me explain how I make the rotations and how from x and y and a rotation I get just one index.
Let's start with the 0 degrees rotation (so we can ignore it for now). I can translate a position given by x and y into an index (like in the above image)
so for example at x = 0 and y = 3 we have 8 = (y - 1) * tetrominoSize + x. So this is how I got the formula for that index. If we add the rotation into an account
than we have different formulas:
-    0 degrees: py * tetrominoSize + px
-   90 degrees: (tetrominoSize - 1) * tetrominoSize + py - (px * tetrominoSize)
-  180 degrees: tetrominoSize * tetrominoSize - 1 - (py * 4) - px
-  270 degrees: tetrominoSize - 1  - py + (px * tetrominoSize)

So given a x, y and a rotation, and the tetromino vector we can say if at the x, y coordinates, on a certain piece that is rotated on 0, 90, 180 or 270 degrees if there is an empty block or a solid one

</details>

<details>
<summary>The states of the program</summary>
<br>
title screen - 0
main menu - 1
start game - 2 (the number is never used because when you click "start game" you get redirected to enter player name, so it would have been an useless transition)
    enter player name - 20
    select difficulty - 21
    before playing - 22
    in game - 23
    "game over" screen - 24
    "level passed" screen - 25
    "-1 heart" screen - 26
    "you achived a highscore do you want to save highscore?" screen - 27
settings - 3
    lcd contrast - 30
    lcd brightness - 31
    matrix brightness control - 32
    invert x axis - 33
    invert y axis - 34
highscores - 4
credits - 5
help - 6
back to title - 7
</details>

<details>
<summary>The matrix state</summary>
<br>
The matrix has 3 states: in game (0), filling(1), emptying(2). When in game the matrix shows the map and the current piece. When in filling the display the matrix
turns on the current position and advances to the next one. If the current position was the last one, we set the matrix filled variable to True. The same goes for
when emptying the display.
</details>

<details>
<summary>Sensors reading</summary>
<br>
The sensor are being read once at a certain interval. For the button we create a token when we detect a new press of the button (if a token was not created in the
past 0.1 seconds) and after that, depends of the state if that token gets used, otherwise at the next reading it gets destroyed.

For the joystick the principle is allmost the same, the only difference is that the token only gets destroyed after it was used ("newLeft = newRight = newDown = newUp = 0").
</details>

<details>
<summary>EEPROM mapping</summary>
<br>
    address | stored data
byte number | unsigned long / char[24] / byte
   00 -> 03 | biggest highscore (unsigned long = 4 bytes)
   04 -> 07 | second  highscore (-||-)
   08 -> 11 | last highscore (-||-)
   12 -> 35 | biggest highscore name (char[24] = 24 bytes)
   36 -> 59 | second highscore name (-||-)
   60 -> 83 | last highscore (-||-)
   84 -> 84 | lcd contrast (1 byte)
   85 -> 85 | lcd brightness (-||-)
   86 -> 86 | matrix brightness (-||-)
</details>




















<details>
<summary>Homework 3</summary>
<br>
The homework consists of the following:
  
• Components: a buzzer and a 7-segment display (or a led bar / or 8 leds in a row that simulate a led bar)
  
• Task: should detect EMF (check body and near outlets). It should print the value on the 7-segment display (or light the led bar) and should make a sound based on the intensity.

• The catch: it’s really easy, but one of the mistakes you can do is not to calibrate for the interval on the values you are going to get. Check the link on calibrations.

• Publishing task: You must add the code to the Github repo and continue updating the readme with at least the following details (but feel free to be more creative).
1.  Task Requirements
2.  Picture of the setup
3.  Link to video showcasing functionality

Just to be clear: the input is the EMF using a home-made antenna (can be a wire and a resistor) and the output is some sort of lighting (7-segment values, leds lighting up etc.) and a buzzer with intensity (change the 2nd parameter in the tone function based on the intensity).
The purpose of this project is, first of all, to have a bit of fun and also let you be creative (or lazy) with it. Bonus points for extra effort.


The solution for the homework is in the folder "Tema 3":
  
• The picture from above is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/tema3_poza_deasupra.jpeg

• The picture from the side is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/tema3_poza_deasupra.jpeg

• The video showcasing the functionality is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/tema3_clip.mp4

• The first method code (average of a sample of readings) is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/variant_average_of_a_sample.ino

• The second method code (maximum of multimple averages of a samples of readings over a certain period of time) is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/variant_max_of_average_of_sample.ino
  
• The third method code (maximum of readings over a certain period of time): https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%203/variant_max_over_time.ino
</details>

<details>
<summary>Homework 4</summary>
<br>
The current homework involves using a joystick in setting the values on each digit of a 4-digit 7-segment display connected to a shift register 74hc595. For maximum grade, you muse use interrupts, and for bonus you can save the last value in eeprom. The homework consists of the following:
  
• Components: a joystick, a 4-digit 7-segment display, a 74hc595 shiftregister
  
• Task:
1.	First state: you can use a joystick axis to cycle through the 4 digits; using the other axis does nothing. A blinking decimal point shows the current digit position. When pressing the button (must use interrupt for maximum points!), you lock in on the selected digit and enter the second state.
2.	Second state: the decimal point stays always on, no longer blinking and you can no longer use the axis to cycle through the 4 digits. Instead, using the other axis, you can increment or decrement the number on the current digit. Pressing the button again returns you to the previous state. For bonus, save the last value in eeprom and load it when starting arduino. Also, keep in mind that when changing the number, you must increment it for each joystick movement - it should not work continuosly increment if you keep the joystick in one position.

• The catch: upgrading the information from the laboratory to cover all the cases, using interrupts for the button and even eeprom for bonus.

• Publishing task: You must add the code to the Github repo and continue updating the readme with at least the following details (but feel free to be more creative).
1.  Task Requirements
2.  Picture of the setup
3.  Link to video showcasing functionality

The solution for the homework is in the folder "Tema 4":
  
• The picture is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%204/pozaTema4.jpeg

• The video showcasing the functionality is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%204/clipTema4.mp4

• The code is here: https://github.com/marian222200/IntroductionToRobotics/blob/main/Tema%204/codTema4.ino
</details>
