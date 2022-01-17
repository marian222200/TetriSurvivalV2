# TetriSurvival

A Tetris-like game made using an Arduino board, a led matrix, a lcd and a joystick.

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


## Additional information useful for understaning the code:

<details>
<summary>The player name codification</summary>
<br>
I store the player name in an array of type byte. Each letter in the array can be translated into a char using the vector myAlphabet as follows:
myAlphabet[playerName[index]] = decoded letter (char). Everywhere I decode the name I must use the function "pgm_read_byte" because I stored the
myAlphabet array in progamabile memory using PROGMEM. For additional information see: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

  <a></a>
  
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

  <a></a>
  
I will explain for the first piece:

  <a></a>
  
                                 0 degrees:         90 degrees:         etc.
                                 0  1  2  3         12  8  4  0
                                 4  5  6  7         13  9  5  1
                                 8  9 10 11         14 10  6  2
                                12 13 14 15         15 11  7  3

  <a></a>
  
Imagine in the above image that the numbers 2, 6, 10 and 14 are bold. Than the first piece (the 4 blocks long bar) will have that "mini map". In the array
that represents that bar only on thesee positions will be True value (meaning a solid block is there) and on the other positions will be False value
(empty space). You do this for every piece and this is how you get the array of arrays (aka the array of tetromino pieces).

  <a></a>
  
The image above helps me explain how I make the rotations and how from x and y and a rotation I get just one index.
Let's start with the 0 degrees rotation (so we can ignore it for now). I can translate a position given by x and y into an index (like in the above image)
so for example at x = 0 and y = 3 we have 8 = (y - 1) * tetrominoSize + x. So this is how I got the formula for that index. If we add the rotation into an account
than we have different formulas:
-    0 degrees: py * tetrominoSize + px
-   90 degrees: (tetrominoSize - 1) * tetrominoSize + py - (px * tetrominoSize)
-  180 degrees: tetrominoSize * tetrominoSize - 1 - (py * 4) - px
-  270 degrees: tetrominoSize - 1  - py + (px * tetrominoSize)

  <a></a>
  
So given a x, y and a rotation, and the tetromino vector we can say if at the x, y coordinates, on a certain piece that is rotated on 0, 90, 180 or 270 degrees if there is an empty block or a solid one

</details>

<details>
<summary>The states of the program</summary>
<br>
• title screen - 0
• main menu - 1
• start game - 2 (the number is never used because when you click "start game" you get redirected to enter player name, so it would have been an useless transition)
    • enter player name - 20
    • select difficulty - 21
    • before playing - 22
    • in game - 23
    • "game over" screen - 24
    • "level passed" screen - 25
    • "-1 heart" screen - 26
    • "you achived a highscore do you want to save highscore?" screen - 27
• settings - 3
    • lcd contrast - 30
    • lcd brightness - 31
    • matrix brightness control - 32
    • invert x axis - 33
    • invert y axis - 34
• highscores - 4
• credits - 5
• help - 6
• back to title - 7
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
  
  <a></a>
  
For the joystick the principle is allmost the same, the only difference is that the token only gets destroyed after it was used ("newLeft = newRight = newDown = newUp = 0").
</details>

<details>
<summary>EEPROM mapping</summary>
<br>
•     address - byte number | stored data
  
  <a></a>
  
•    00 -> 03 | biggest highscore (unsigned long = 4 bytes)
•    04 -> 07 | second  highscore (-||-)
•    08 -> 11 | last highscore (-||-)
•    12 -> 35 | biggest highscore name (char[24] = 24 bytes)
•    36 -> 59 | second highscore name (-||-)
•    60 -> 83 | last highscore (-||-)
•    84 -> 84 | lcd contrast (1 byte)
•    85 -> 85 | lcd brightness (-||-)
•    86 -> 86 | matrix brightness (-||-)
</details>
