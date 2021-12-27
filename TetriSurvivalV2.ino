#include <LedControl.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>


// the pins that are controlling the lcd
#define RS 2
#define enable 3
#define d4 4
#define d5 13
#define d6 8
#define d7 7
#define lcdContrastPin 5
#define lcdBrightnessPin 6

//limitations for the lcd contrast
#define lcdMinContrast 20
#define lcdMaxContrast 150

//limitations for the lcd brightness
#define lcdMinBrightness 10
#define lcdMaxBrightness 255

//lcd size parameters
#define lcdRows 2
#define lcdColumns 16

//the project uses only one lcd and it has the index 0
#define deviceIndex 0


//the pins controlling the matrix display
#define dinPin 12
#define clockPin 11
#define loadPin 10

//a square matrix 8 * 8 leds in size
#define matrixSize 8


//the pin controlling the buzzer
#define buzzerPin 9

//tetris theme parameters
#define notes 99 // the number of notes in the tetris song 
#define wholenote 1714 // the duration of a full note (in that song)

#define sfxDuration 100// the duration of the sound effects (the beep when you move joystick and the button press)


//the input pins that take receive information from the joystick
#define sWPin A2//the button pin
#define xPin A1
#define yPin A0

//joystick minimum and maximum thresholds to be considered an received input
#define minThreshold 100
#define maxThreshold 700 

//the time intervals between the sensors readings
#define joyReadInterval 10
#define sWInterval 10

//cooldown between the received inputs (you can't press a button faster than 10 times per second)
#define joyInterval 100
#define buttonPressInterval 100


//lcd display parameters
#define sliderLength 14 //lcdColumns - 2; the size of the slider (like the one in the lcd brightness from settings menu)

#define scrollInterval 500   //the interval for one scrolling of all the scrolling text (one step to the right, like the 
                             //bottom text in the title screen)

#define blinkInterval 300   //the blink interval of all the blinking things (arrows)

#define cursorPositionLCDRelative 7 //where the cursor in the input text pannel is (witch segment)


//menu parameters
//the number of options in each menu
#define mainMenuNumber 6 
#define difficultiesNumber 3
#define settingsNumber 6
#define creditsNumber 4
#define helpNumber 4
#define highscoresNumber 3

#define bufferSize 64   //the size of the buffers used for display (title & text aka top & bottom texts)

#define maxPlayerName 24 // maximum of the length of the player name vector
#define myAlphabetSize 54 // the length of the vector used for decoding the name

#define fillTime 50 // the interval of time between the filling of the leds from the screensaver (the model that appears 
                    // on the matrix when not in game)

#define emptyTime 50// same as above but it is between the emptying of the leds


//game parameters
#define tetrominoSpawnPosition 2 //where the tetromino spawns on the X axis (roughly in the middle of the screen)

#define tetrominoNumber 7  //number of all the types of tetromino
#define tetrominoSize 4  //the size of the tetromino is 4 * 4 witch includes the empty spaces around it, see the tetromino vector for details

#define mapHight 12 // the height of the map (the map has 4 extra rows in order to spawn the tetromino above the visibile space (the visibile rows))

#define tickDuration 50  //how long a game frame lasts
#define baseLevelDuration 60000 //how long the first level lasts, the rest of the levels are calculated based on this

#define baseLevelPointsValue 10000 //base points for passing a level, all the level pass points are calculated starting from this
#define baseLinePointsValue 100  //base points for scoring a line

#define defaultTicksPerFrocedStep 30  // how many game ticks until the current piece has to go one forced line downward

#define piecesForSpeedup 3 //how many pieces until the game speeds up


//the max and min of the output of an pin of arduino
#define minPinOutput 0
#define maxPinOutput 255


//EEPROM addresses, see EEPROM map for aditional details
#define biggestHighscoreAddress 0
#define secondHighscoreAddress 4
#define lastHighscoreAddress 8

#define biggestHighscoreNameAddress 12
#define secondHighscoreNameAddress 36
#define lastHighscoreNameAddress 60

#define lcdContrastAddress 84
#define lcdBrightnessAddress 85
#define matrixBrightnessAddress 86


//the music tones used in the tetris theme, and the sfx-es
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
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
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0


// long char[] used in the menus (PROGMEM forces them to be stored in the programable memory)
// their names are based on the codes of the states, for aditional info see 'states' section
// title reffers to the top text and text refers to the bottom text

const char state0Title[] PROGMEM = "TetriSurvival";


const char state1Title[] PROGMEM= "main menu";
const char state1Text0[] PROGMEM = "start game";
const char state1Text1[] PROGMEM = "settings";
const char state1Text2[] PROGMEM = "highscores";
const char state1Text3[] PROGMEM = "credits";
const char state1Text4[] PROGMEM = "help";
const char state1Text5[] PROGMEM = "title screen";


const char state20Title[] PROGMEM = "   enter your nickname";

const char state21Title[] PROGMEM = "   select difficulty";
const char state21Text0[] PROGMEM = "easy";
const char state21Text1[] PROGMEM = "medium";
const char state21Text2[] PROGMEM = "hardcore";

const char state24Title[] PROGMEM = "bad luck, game over, ";

const char state25Title[] PROGMEM = "congrats, level passed, ";

const char state26Title[] PROGMEM = "bummer, you died, ";
const char state26Text[] PROGMEM = "hearts left: ";

const char state27Title[] PROGMEM = "   you achived a highscore, do you want to save it?";


const char state3Title[] PROGMEM = "settings";
const char state3Text0[] PROGMEM = "lcd contrast";
const char state3Text1[] PROGMEM = "   lcd brightness";
const char state3Text2[] PROGMEM = "   matrix brightness";
const char state3Text3[] PROGMEM = "invert x axis";
const char state3Text4[] PROGMEM = "invert y axis";
const char state3Text5[] PROGMEM = "   back to main menu";

const char state30Title[] PROGMEM = "lcd contrast";
const char state31Titie[] PROGMEM = "   lcd brightness";
const char state32Title[] PROGMEM = "   matrix brightness";
const char state33Title[] PROGMEM = "   are you sure you want to invert the x axis?";
const char state34Title[] PROGMEM = "   are you sure you want to invert the y axis?";


const char state4Title[] PROGMEM = "highscores";

const char state5Title[] PROGMEM = "credits";
const char state5Text0[] PROGMEM = "   by Marian Dimofte";
const char state5Text1[] PROGMEM = "group 333";
const char state5Text2[] PROGMEM = "   teacher: Andrei Dumitriu";
const char state5Text3[] PROGMEM = "   https://github.com/marian222200/TetrisSurvivalV2";

const char state6Title[] PROGMEM = "help";
const char state6Text0[] PROGMEM = "   this game is a tetris game where you need to survive time to pass a level";
const char state6Text1[] PROGMEM = "   if you cant place anymore pieces, you lose one heart. lose them all and it's game over";
const char state6Text2[] PROGMEM = "   you make points by passing the level and filling lines. you get a bigger bonus if you fill more lines at the same time";
const char state6Text3[] PROGMEM = "   don't forget, the most important thing is to have fun";

const char state7Title[] PROGMEM = "   are you sure you want to go back to title screen?";

const char myAlphabet[] PROGMEM = "$ ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";// this is used for translating the text from my code into char[]

const char blankTitle[] PROGMEM = "             ";// used to delete the top text without deleting the bottom text (the top right 3-most segments are used for
                                                  // displaying the avabile keys at a given moment, so we cover only the top firs 13 segments)
const char blankText[] PROGMEM = "                ";//used to delete all the segments on the bottom row
const char blankTextWithArrow[] PROGMEM = "               ";// used to delete allmost all the segments on the bottom row



// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
const int melody[] PROGMEM = {
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

//theese are the custom characters for the lcd, like the arrows, the heart and the button

byte horizontalArrows[8] = {// the left-right moving possibility
  B00000,
  B01000,
  B11111,
  B01000,
  B00000,
  B00010,
  B11111,
  B00010
};
byte verticalArrows[8] = {// the up-down moving posibility
  B01000,
  B11100,
  B01000,
  B01000,
  B00010,
  B00010,
  B00111,
  B00010
};
byte pushButton[8] = {// the button press posibility
  B00111,
  B01111,
  B01111,
  B11110,
  B11100,
  B00000,
  B01110,
  B11111
};
byte cursor[8] = {// the arrow that signifies the selector
  B00000,
  B00000,
  B00100,
  B11110,
  B11111,
  B11110,
  B00100,
  B00000
};
byte heart[8] = {// the heart
  B00000,
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
byte emptyBox[8] = {// a mostly empty character
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};
byte fullBox[8] = {//a mostly filled character
  B11111,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B11111,
};


const bool tetromino[tetrominoNumber][tetrominoSize * tetrominoSize] = {//theese are the tetromino pieces
  {
    0, 0, 1, 0,//   0010
    0, 0, 1, 0,//   0010
    0, 0, 1, 0,//   0010
    0, 0, 1, 0//    0010
  },
  {
    0, 0, 1, 0,//   0010
    0, 1, 1, 0,//   0110
    0, 1, 0, 0,//   0100
    0, 0, 0, 0//    0000
  },
  {
    0, 1, 0, 0,//   0100
    0, 1, 1, 0,//   0110
    0, 0, 1, 0,//   0010
    0, 0, 0, 0//    0000
  },
  {
    0, 0, 0, 0,//   0000
    0, 1, 1, 0,//   0110
    0, 1, 1, 0,//   0110
    0, 0, 0, 0//    0000
  },
  {
    0, 0, 1, 0,//   0010
    0, 1, 1, 0,//   0110
    0, 0, 1, 0,//   0010
    0, 0, 0, 0//    0000
  },
  {
    0, 0, 0, 0,//   0000
    0, 1, 1, 0,//   0110
    0, 0, 1, 0,//   0010
    0, 0, 1, 0//    0010
  },
  {
    0, 0, 0, 0,//   0000
    0, 1, 1, 0,//   0110
    0, 1, 0, 0,//   0100
    0, 1, 0, 0//    0100
  }
};

//the map of the game, we show in the matrix the rows 4-12, 0-3 is where the next piece spawns
byte matrix[mapHight] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

//lcd parameters
byte lcdContrast = 10;//(0~sliderLength) 
byte lcdBrightness = 12;// (0~sliderLength)

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);


byte matrixBrightness = 2; // 0~14 (0~sliderLenght). mapping matrix Brightness can be from 0,14 to 0,16 but it wouldn't make a big difference
                           // and it is less smooth, at a certain point it will be a spike in brightness 

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);


//menus variables
byte state = 0;//the app is a state machine and this is the variable that keeps in what state the machine is at a given moment, for more information see the "states" section

byte selectedIndex = 0;//what option is currently selected from the current menu
byte lastMenuIndex = 0;// used when we come back to the main menu to keep the last selected option (for example if we enter settings and we return back to main menu we want to be on the "settings" options when we do so)
byte lastSettingsIndex = 0;// used for the settings menu in the same manner like the above variable

byte cursorPosition = 0; // the cursor position in the player name. the cursor stays fixed in the text input section and the name moves, so we keep where in the name is the cursor situated, it resets when playing again

//display text variables
unsigned long lastScrolled = 0;// when was the last time the text was scrolled
int scrolledAmmount = 0;//how much letters were scrolled from when the scroll started
bool startedToScroll = 0;//if we are currently scrolling this is true, otherwise it is false
bool scrolled = 0; //it is true when in this loop we have to move the letters to the left (aka scroll)

unsigned long lastBlink = 0;//when was the last time when a blinking thing blinked
bool blinkState = 0;// 0-visible 1-invisible; in what state a blinking object is


//display buffers
char title[bufferSize];//top text
char text[bufferSize];//bottom text

byte playerName[maxPlayerName]; // I used an byte vector with a codification of the name using the myLaphabet vector. the name allways ends with '$' (works like the NULL for char[])
byte playerNameLength = 1; //aka the position of the last $ in the playerName array, for more information see the "player name codification" section

unsigned long highscore; //used as a buffer mainly for the eeprom
char highscoreName[maxPlayerName]; // same as above


//accessibility variables
bool invertedXAxis = 0;//true if the x axis is inverted, for more information see the "sensors reading" section
bool invertedYAxis = 0;//true if the x axis is inverted

byte intensity = 0; // buffer for lcd contrast, lcd brightness and matrix brightness


//sensors parameters
unsigned long lastJoyRead = 0;// when was the last time we read the joystick input 

bool movedJoy = 0; // if we picked an joystick input

//if the current input is considered a trigger by the thresholds (on each axis)
bool movedLeft = 0;
bool movedRight = 0;
bool movedUp = 0;
bool movedDown = 0;

//if the input at the last reading was considered a trigger (on each axis)
bool lastMovedLeft = 0;
bool lastMovedRight = 0;
bool lastMovedUp = 0;
bool lastMovedDown = 0;

//the move input tokens
// i used a token system, when the input appears, it creates a token (one of theese variables gets changed to true), when the token is used in stateBehaviour() then all of theese are set to false
// it can't be more then one token at 0.1 sec (joyInterval), this is used to smoothen the input
bool newLeft = 0;
bool newRight = 0;
bool newUp = 0;
bool newDown = 0;

unsigned long lastJoyTime = 0;//when the last token was produced

//at buttonPress it is the same token system but simplified, the token is lost if in the current loop it is not used
unsigned long lastSWTime = 0;// when was the last reading of the button
bool swState = 1;//the current reading of the button (1 means no trigger, 0 is trigger)
bool lastSwState = 1;// the last reading of the button
bool newButtonPress = 0;//this is the token
unsigned long lastButtonTime = 0;//the last time a token was generated, same thing, no more than once every 0.1 sec (see buttonPressInterval)


//music variables
byte thisNote = 0; // the current note from the song that is played
unsigned long lastNoteTime = 0; // when the last note has been played
int lastNoteDuration = 0;//how long the last note should last
bool startedSong = 0; // true when the song is being played


//game parameters
byte difficulty = 0;
byte level = 0;
byte hearts = 1;
unsigned long score = 0;//the current score
unsigned long lastScore = 0; // score is saved in lastScore, the points are kept only when you pass the level

byte currentPiece = 0; // what type of tetromino is the current piece
byte currentRotation = 0; // the number of rotations this piece got, for aditional information see "tetromino codification"

// the current position of the tetromino piece that is falling
int currentX = tetrominoSpawnPosition;
int currentY = 0;

int tetrominoCount = 1; // number of tetrominos placed in from the start of the level

bool newTick = 0; // true if there is a new tick in the current game loop, otherwise false

const byte p2[] = {1, 2, 4, 8, 16, 32, 64, 128}; //vector for powers of 2 p2[i] = pow(2, i);

byte ticksPerForcedStep = defaultTicksPerFrocedStep;//how many game ticks it takes for the falling tetromino piece to fall one more row

byte tickCount = 0;//ticks from the start of the level
bool forcedStep = 0;//if we have to fall the tetromino one more row in this game loop (if it has to move one row down)

unsigned long levelStart = 0;//the time at witch the level started
unsigned long levelDuration = 0;//how long the current level lasts

unsigned int lastGameTick = 0;//when the last tick of the game was


//the screen saver helper variables
//the current positions of the led we are lighting
byte fillX;
byte fillY;

bool matrixFilled;//true if we finished to fill the matrix with the lighted up leds
unsigned long lastFilled;//when we lighted the last led

//the current positions of the led we are lighting off
byte emptyX;
byte emptyY;

bool matrixEmptyed;//true if we finished to turn off all the leds
unsigned long lastEmptyed;//when we turned off the last led

//the state of the matrix
byte matrixState = 0; // 0 -> display tetris map; 1 ->  we are filling the display; 2 ->we are clearing the display;

//true if the matrix is idle (aka in screen saver, aka not in game), false otherwise (when we play the game)
bool matrixIdle = 1;


void setup() {
  while (!Serial);  // wait for serial port to connect. Needed for native USB

  pinMode(sWPin, INPUT_PULLUP);//button pin set to input pullup using the internal resistor 

  pinMode(lcdContrastPin, OUTPUT);//the pin used for the lcd contrast
  pinMode(lcdBrightnessPin, OUTPUT);//the pin used for lcd brightness


  lcd.begin(lcdColumns, lcdRows);//initialising the lcd

//storing the costum chars
  lcd.createChar(0, horizontalArrows);
  lcd.createChar(1, verticalArrows);
  lcd.createChar(2, pushButton);
  lcd.createChar(3, cursor);
  lcd.createChar(4, heart);
  lcd.createChar(6, emptyBox);
  lcd.createChar(7, fullBox);


  lc.shutdown(0, false);//turning off the matrix standby mode
  lc.clearDisplay(deviceIndex);


//   eepromFirstWrite(); // in case we need to wipe the highscores
  loadPreferences();// we load from eeprom the last saved settings

  playerName[0] = 0;//we set the current player to have a void name
  playerName[1] = 0;

  //we start the screen saver
  matrixFilled = 0;
  matrixState = 1;//1 means the matrix is in the process of filling the screen
  matrixIdle = 1;
  fillX = 0;
  fillY = 0;

  //we enter the title screen
  enterState(0);
}


void loop() {
  displayLogic();

  sensorLogic();

  stateBehaviour();

  ledMatrixDisplay();

  idleMatrix();
}


//the display part of the program
//it uses the current state to determinate what to show and some functions to have the meanings of showing text and costum characters on the lcd
//most of the times we load the buffers (text and title) with what we want to appear on lcd and we call for a helper function to show it (the pannel functions)
void displayLogic() {
  if (state == 0) {//title screen
    strcpy_P(title, state0Title);// here we load the title of the game for example
    doAnyToContinuePannel();// and here we call a function that shows the given title and the "do anything to continue" text below it
  }
  else if (state == 1) {//main menu
    strcpy_P(title, state1Title);

// here we load the text of the current selected option from the main menu in the below text buffer
    if (selectedIndex == 0) strcpy_P(text, state1Text0); 
    else if (selectedIndex == 1) strcpy_P(text, state1Text1);
    else if (selectedIndex == 2) strcpy_P(text, state1Text2);
    else if (selectedIndex == 3) strcpy_P(text, state1Text3);
    else if (selectedIndex == 4) strcpy_P(text, state1Text4); 
    else if (selectedIndex == 5) strcpy_P(text, state1Text5);  
    
    listOfLinksPannel();
  }
  else if (state == 20) {//enter name
    for (byte i = 0; i < playerNameLength; i ++) {//we convert the text using myLaphabet in the "text" buffer
      text[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);// using PROGMEM implies the use of the "pgm_read_byte_near" that is a function
                                                                     // that returns what we stored in the programmable memory at a certain address
    }                                                                // so we decode letter by letter until the last "$"
    text[playerNameLength] = NULL;// we place a \0 (NULL) at the end to be able to use strcpy-like functions
    strcpy_P(title, state20Title);
    textInputPannel();
  }
  else if (state == 21) {//select difficulty
    strcpy_P(title, state21Title);

    if (selectedIndex == 0) strcpy_P(text, state21Text0);
    else if (selectedIndex == 1) strcpy_P(text, state21Text1);
    else if (selectedIndex == 2) strcpy_P(text, state21Text2);

    listOfLinksPannel();
  }
  else if (state == 22) {// pre level
    char aux[16] = "level ";//converting the level from int to char
    if (level < 10) {
      aux[6] = '0' + level;
      aux[7] = NULL;
    }
    else if (level < 100) {
      aux[6] = '0' + level / 10;
      aux[7] = '0' + level % 10;
      aux[8] = NULL;
    }
    else {
      aux[6] = '0' + (level / 100) % 10;
      aux[7] = '0' + (level / 10) % 10;
      aux[8] = '0' + level % 10;
      aux[9] = NULL;
    }

    strcpy(title, aux);
    doAnyToContinuePannel();
  }
  else if (state == 23) inGamePannel();//in game
  else if (state == 24) {// game over
    strcpy_P(title, state24Title);

    for (byte i = 0; i < playerNameLength; i ++) {// we decode the name using myAlphabet in the highScoreName buffer
      highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);
    }
    highscoreName[playerNameLength] = NULL;

    strcat(title, highscoreName);
    strcat(title, "!   ");

    afterLevelPannel();
  }
  else if (state == 25) {//level passed
    strcpy_P(title, state25Title);

    for (byte i = 0; i < playerNameLength; i ++) {// we decode the name using myAlphabet in the highScoreName buffer
      highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);
    }
    highscoreName[playerNameLength] = NULL;

    strcat(text, highscoreName);
    strcat(title, "!   ");

    afterLevelPannel();
  }
  else if (state == 26) {//-1 heart
    strcpy_P(title, state26Title);
    strcpy_P(text, state26Text);

    for (byte i = 0; i < playerNameLength; i ++) {// we decode the name using myAlphabet in side the highScoreName buffer
      highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);
    }
    highscoreName[playerNameLength] = NULL;

    strcat(title, highscoreName);
    strcat(title, "!   ");

    losingHeartPannel();
  }
  else if (state == 27) {//ask for register highscore
    strcpy_P(title, state27Title);
    yesOrNoPannel();
  }
  else if (state == 3) {//settings menu
    strcpy_P(title, state3Title);

    if (selectedIndex == 0) strcpy_P(text, state3Text0);
    else if (selectedIndex == 1) strcpy_P(text, state3Text1);
    else if (selectedIndex == 2) strcpy_P(text, state3Text2);
    else if (selectedIndex == 3) strcpy_P(text, state3Text3);
    else if (selectedIndex == 4) strcpy_P(text, state3Text4);
    else if (selectedIndex == 5) strcpy_P(text, state3Text5); 

    listOfLinksPannel();
  }
  else if (state == 30) {//lcdContrast
    strcpy_P(title, state30Title);
    intensity = lcdContrast;// load the buffer
    sliderPannel();
  }
  else if (state == 31) {//lcdBrightness
    strcpy_P(title, state31Titie);
    intensity = lcdBrightness;
    sliderPannel();
  }
  else if (state == 32) {//matrixBrightness
    strcpy_P(title, state32Title);
    intensity = matrixBrightness;
    sliderPannel();
  }
  else if (state == 33) {//invert x axis
    strcpy_P(title, state33Title);
    yesOrNoPannel();
  }
  else if (state == 34) {//invert y axis
    strcpy_P(title, state34Title);
    yesOrNoPannel();
  }
  else if (state == 4) {//highscores
    if (selectedIndex == 0) {// we pull from eeprom the selected highscore and we use the buffers and the helper functions to show it
      EEPROM.get(biggestHighscoreAddress, highscore);//we pull the score
      EEPROM.get(biggestHighscoreNameAddress, highscoreName);//we pull the player name

      if (strlen(highscoreName) > 7) {// if the text is longer than the space that is provided, we use scrolling. scrolling works better if you have some space between the head and the tail of the word
        strcpy(text, "   ");
        strcat(text, highscoreName);
      }
      else strcpy(text, highscoreName);
    }
    else if (selectedIndex == 1) {
      EEPROM.get(secondHighscoreAddress, highscore);
      EEPROM.get(secondHighscoreNameAddress, highscoreName);

      if (strlen(highscoreName) > 7) {
        strcpy(text, "   ");
        strcat(text, highscoreName);
      }
      else strcpy(text, highscoreName);
    }
    else if(selectedIndex == 2) {
      EEPROM.get(lastHighscoreAddress, highscore);
      EEPROM.get(lastHighscoreNameAddress, highscoreName);

      if (strlen(highscoreName) > 7) {
        strcpy(text, "   ");
        strcat(text, highscoreName);
      }
      else strcpy(text, highscoreName);
    }

    strcpy_P(title, state4Title);
    highscorePannel();
  }
  else if (state == 5) {//credits
    strcpy_P(title, state5Title);

    if (selectedIndex == 0) strcpy_P(text, state5Text0);
    else if (selectedIndex == 1) strcpy_P(text, state5Text1);
    else if (selectedIndex == 2) strcpy_P(text, state5Text2);
    else if (selectedIndex == 3) strcpy_P(text, state5Text3);

    readingPannel();
  }
  else if (state == 6) {//help
    strcpy_P(title, state6Title);

    if (selectedIndex == 0) strcpy_P(text, state6Text0);
    else if (selectedIndex == 1) strcpy_P(text, state6Text1);
    else if (selectedIndex == 2) strcpy_P(text, state6Text2);
    else if (selectedIndex == 3) strcpy_P(text, state6Text3);

    readingPannel();
  }
  else if (state == 7) {//back to title
    strcpy_P(title, state7Title);
    yesOrNoPannel();
  }
}


void yesOrNoPannel() {// pannel designed for yes/no questions
  displayText(title, 0, 0, 12);// we show the title 
  displayAvabileKeys(1, 0, 1);// we show what keys we can use in this pannel

//we show the answers (one in the left part of the screen, and one in the right part)
  lcd.setCursor(1, 1);
  lcd.print(" yes");
  lcd.setCursor(13, 1);
  lcd.print(" no");
  
//we show the arrow where the selected option is
  if (selectedIndex == 0) lcd.setCursor(0, 1);
  else lcd.setCursor(12, 1);
//this is the part where we control the blinking of the arrow
  if (blinkState) lcd.write(byte(3));
  else lcd.print(" ");
}

void listOfLinksPannel() {// pannel designed for a list that has a long title and long options (a lot of characters), where we can scroll and we have an arrow that shows the user that he can chose one of the options
  displayText(title, 0, 0, 12);
  displayText(text, 1, 1, 15);
  displayAvabileKeys(0, 1, 1);

//just text with a blinking arrow in the bottom first segment
  lcd.setCursor(0, 1);
  if (blinkState) lcd.write(byte(3));
  else lcd.print(" ");
}

void sliderPannel() {// pannel designed for a slider like the one for matrix brightness, that shows an image that rpresents the current intensity level
  displayText(title, 0, 0, 12);
  displayAvabileKeys(1, 0, 1);

//a - in the leftmost bottom and a + in the rightmost bottom 
  lcd.setCursor(0, 1);
  lcd.print("-");
  lcd.setCursor(15, 1);
  lcd.print("+");

  //we use the allmost filled segment custom character to signify a filled part of the slider
  for (byte i = 0; i < intensity; i ++) {
    lcd.setCursor(i + 1, 1);
    lcd.write(byte(7));
  }
  //we use the allmost empty segment custom character to signify a empty part of the slider
  for (byte i = intensity; i < sliderLength; i ++) {
    lcd.setCursor(i + 1, 1);
    lcd.write(byte(6));
  }
  //the slider has 16 segments, 2 of witch are the - and +, and the rest are slider parts
}

void doAnyToContinuePannel() {// just a title with the same below text allways
  displayText(title, 0, 0, 12);
  displayAvabileKeys(1, 1, 1);

  displayText("   Do anything to continue.", 1, 0, 15);
}

void textInputPannel() {// the pannel used for the part of the game where we input the player's name
  byte firstEmptySpaces, lastEmptySpaces;

  displayText(title, 0, 0, 12);//basic stuff
  displayAvabileKeys(1, 1, 1);

  // using the formulas below we calculate how much blank spaces we have before our player name and after
  firstEmptySpaces = lcdColumns / 2 - cursorPosition - 1;
  lastEmptySpaces = lcdColumns / 2 - strlen(text) + cursorPosition + 1;// we want the cursor to remain statinary on the screen and the name to move, so we must calculate what letters should be 
                                                                       // appearing on the screen and how many spaces to the left or to the right to put

//we start to write
  lcd.setCursor(0, 1);

//in case the player name is longer than the avabile space
  if (firstEmptySpaces < 0) firstEmptySpaces = 0;
  if (lastEmptySpaces < 0 ) lastEmptySpaces = 0;

  for(byte i = 0; i < firstEmptySpaces; i ++)lcd.print(" ");//the first empty spaces

  for(byte i = firstEmptySpaces; i + lastEmptySpaces < lcdColumns; i ++)
    if (i - firstEmptySpaces == cursorPosition && !blinkState) lcd.write(byte(6));// void or not, if the current character is where the cursor is at then he is blinking using the allmost empty costum character
    else if (text[i - firstEmptySpaces] == '$') lcd.write(byte(7)); // $ is the null character in our decoding, so we want to show a "void" character (the custom allmost filled character)
    else lcd.print(text[i - firstEmptySpaces]);

  for(byte i = 0; i < lastEmptySpaces; i ++)lcd.print(" ");//the last empty spaces
}

void readingPannel() {// a simple pannel with just a title, text and the avabile keys
  displayText(title, 0, 0, 12);
  displayAvabileKeys(0, 1, 1);
  displayText(text, 1, 0, 15);
}

void inGamePannel() {//pannel dedicated to showing the level progress
  //we show the remaining time in the level
  lcd.setCursor(0, 0);
  lcd.print("time left: ");
  lcd.print(((levelDuration - (millis() - levelStart)) / 600000) % 6);
  lcd.print(((levelDuration - (millis() - levelStart)) / 60000) % 10);
  lcd.print(":");
  lcd.print(((levelDuration - (millis() - levelStart)) / 10000) % 6);
  lcd.print(((levelDuration - (millis() - levelStart)) / 1000) % 10);
  

  //we show the current score
  lcd.setCursor(0, 1);// we want the score to have the same length, so we put enough 0s before it ex.: 000100
  lcd.print(" Score:");
  score = score % 100000000;
  if (score < 10000000)lcd.print(0);
  if (score < 1000000)lcd.print(0);
  if (score < 100000)lcd.print(0);
  if (score < 10000)lcd.print(0);
  if (score < 1000)lcd.print(0);
  if (score < 100)lcd.print(0);
  if (score < 10)lcd.print(0);
  lcd.print(score);
  lcd.print(" ");
}

void afterLevelPannel() {// pannel dedicated to show the player that he passed the level, title, avabile keys with the score below it
  displayAvabileKeys(0, 0, 1);
  displayText(title, 0, 0, 12);

  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print("Score:");
  lastScore = lastScore % 100000000;// we want the score to have the same length, so we put enough 0s before it
  if (lastScore < 10000000)lcd.print(0);
  if (lastScore < 1000000)lcd.print(0);
  if (lastScore < 100000)lcd.print(0);
  if (lastScore < 10000)lcd.print(0);
  if (lastScore < 1000)lcd.print(0);
  if (lastScore < 100)lcd.print(0);
  if (lastScore < 10)lcd.print(0);
  lcd.print(lastScore);
  lcd.print(" ");
}

void highscorePannel() {// pannel dedicated to showing the highscore to the regular user, title, avabile keys on top, name and score on bottom
  displayText(title, 0, 0, 12);
  displayAvabileKeys(0, 1, 1);
  displayText(text, 1, 0, 6);

  lcd.setCursor(7, 1);
  lcd.print(" ");
  
  highscore = highscore % 100000000;// we want the score to have fixed length, so we put enough 0s before it
  if (highscore < 10000000)lcd.print(0);
  if (highscore < 1000000)lcd.print(0);
  if (highscore < 100000)lcd.print(0);
  if (highscore < 10000)lcd.print(0);
  if (highscore < 1000)lcd.print(0);
  if (highscore < 100)lcd.print(0);
  if (highscore < 10)lcd.print(0);
  lcd.print(highscore);
}

void losingHeartPannel() {//pannel dedicated to tell the player that he lost one heart, text and avabile keys on top, text, number and the heart symbol on bottom
  displayText(title, 0, 0, 12);
  displayAvabileKeys(0, 0, 1);
  displayText(text, 1, 0, 12);

  lcd.setCursor (13, 1);
  hearts = hearts % 100;
  if (hearts < 10)lcd.print(" ");// we want to have a fixed length
  lcd.print(hearts);
  lcd.write(byte(4));
}


void displayAvabileKeys(bool leftRight, bool upDown, bool pressButton) {// this is the function that displays the avabile keys in the top right corner
  //this shows up if we can use the joystick to move left or right
  lcd.setCursor(13, 0);
  if (leftRight) lcd.write(byte(0));
  else lcd.print(" ");

//this shows up if we can use the joystick to move up or down
  lcd.setCursor(14, 0);
  if (upDown) lcd.write(byte(1));
  else lcd.print(" ");
  
//this shows up if we can press the button
  lcd.setCursor(15, 0);
  if (pressButton) lcd.write(byte(2));
  else lcd.print(" ");
}

void displayText(const char text[], byte row, byte firstSegment, byte lastSegment) {//displays text on a certain part of the screen, if the text is larger than the area destinated for it, it gets scrolled
  lcd.setCursor(firstSegment, row);

  if (strlen(text) <= lastSegment - firstSegment + 1) {// if it fits it is ok, otherwise we scroll that text
    for (byte i = 0; i < strlen(text); i++)
      if (text[i] == '$') lcd.write(byte(7));// $ is the null character in our code so we show it using the allmost filled custom character
      else lcd.print(text[i]);
  }
  else if (scrolled) // if we have to print date on lcd because of the scroll in this loop (we scroll once in a while and update the display, see the scroll functions)
    for (byte i = firstSegment; i <= lastSegment; i ++)
      if(text[(i + scrolledAmmount) % strlen(text)] == '$') lcd.write(byte(7));// same as 5 rows, $ = NULL in the codification
      else lcd.print(text[(i + scrolledAmmount) % strlen(text)]);// we calculate what letters to show in the destinated area knowing how much letters were scrolled since the scrolling started
}

//when we start to scroll we initialize the scrolling parameters
void startScroll() { 
  startedToScroll = 1;
  lastScrolled = millis();
  scrolledAmmount = 0;
  scrolled = 1;// we also make the displayText function to start showing the text we have to scroll
}

//we stop scrolling 
void stopScroll() {
  startedToScroll = 0;
  scrolledAmmount = 0;
}

//we reset the scrolling back to the first step, usefull when we move trough a list of things that need to be scrolled (like the help section)
void resetScroll() {
  scrolledAmmount = 0;
  lastScrolled = millis();
  scrolled = 1;// we also make the displayText function to restart showing the text we have to scroll
}

void scrollLogic() {// once at a few time we scroll one more step
  if (startedToScroll & lastScrolled + scrollInterval < millis()) {
    lastScrolled = millis();
    scrolledAmmount ++;
    scrolled = 1;// here we signal to update the data to be showed on the lcd
  }
  else scrolled = 0;//if we have to wait, we dont update the screen yet
}


void blinkLogic() {
  //once in a while we change the blinkState
  if (lastBlink + blinkInterval < millis()) {
    lastBlink = millis();
    blinkState = !blinkState;
  }
}


// the brains of the program
//using the state variable he knows what sensors to listen to and what actions to take
//generally it moves from one state to another (using enterState and exitState), but each state has many more things beside that.
//so there is code that happnes only when you enter/exit a state, that is grouped in the enterState and exitState
void stateBehaviour() {
  if (state == 0) {//title screen
    scrollLogic();
    songLogic();

    if (newButtonPress || newLeft || newRight || newUp || newDown) {//listens for any kind of input
      newLeft = newRight = newUp = newDown = 0;// if we got a joystick token, we use it. for additional information see the sensors section
      //this is how we exit our current state and we enter a new one
      exitState(0);
      enterState(1);
    }
  }
  else if (state == 1) {//main menu
    scrollLogic();// check if any scrolling happened
    blinkLogic();// check if any blinking happened

    if (newDown) {// if the joystick is moved down we go to the next option
      moveSfx();//we make the moving joystick sound
      newLeft = newRight = newUp = newDown = 0;// we use the token
      // if there are any options left we go to the next one, we reset the scroll, so the user reads the next option from the first letter, and also we delete the residual image of the blinking arrow
      if (selectedIndex < mainMenuNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textWithArrowClear();
      }
    }
    if (newUp) {// if the joystick is moved up we go to the previous option
      moveSfx();//we make the moving joystick sound
      newLeft = newRight = newUp = newDown = 0;// we use the token
      // if there are any options left we go to the previous one, we reset the scroll, so the user reads the previous option from the first letter, and also we delete the residual image of the blinking arrow
      if (selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textWithArrowClear();
      }
    }

    if (newButtonPress) {// if we press the button
      clickSfx();//we make the press button sound

      exitState(1);// we exit our current state
      //depending of the current selected option we enter a new state
      if (selectedIndex == 0) enterState(20);
      else if (selectedIndex == 1) enterState(3);
      else if (selectedIndex == 2) enterState(4);
      else if (selectedIndex == 3) enterState(5);
      else if (selectedIndex == 4) enterState(6);
      else if (selectedIndex == 5) enterState(7);
    }
  }
  else if (state == 20) {//enter player name
    scrollLogic();
    blinkLogic();

//if we press the button we advance to the next state, picking the difficulty
    if (newButtonPress) {
      clickSfx();
      if (playerNameLength > 0 && playerName[playerNameLength - 1] == 0) playerNameLength --;//if the word ends with 2 * $ we don't save the last one
      
      exitState(20);
      enterState(21);
    }

//by pressing the joystick down or up we scroll trought the letters
//this happens like this: we want only the rightmost character to be able to scroll trough my void character ("$"), so it gets used when adding a new letter to the name, or removing the last letter
//so we want just the last character to be able to scroll trough the "$", the other characters aren't able to do it, they jump over that character, they can only scroll trought small and big letters and the " " character

    if (newDown) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (playerName[cursorPosition] == myAlphabetSize - 1) {// if we didn't reach the maximum length
        if (cursorPosition == playerNameLength - 1) playerName[cursorPosition] = 0;//we check if the current character is the last, null can only be the last
                                                                                   //character (or the last 2 in one case), so if the current letter is not the last one in the word, than we jump over the '$'
        else playerName[cursorPosition] = 1;
      }
      else playerName[cursorPosition] ++;// otherwise we scroll in growing order

      characterFromTextClear(cursorPositionLCDRelative);// we clear the current position between scrolls so we don't overlap characters
    }
    if (newUp) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (playerName[cursorPosition] == 0) playerName[cursorPosition] = myAlphabetSize - 1;// same as above, we try only to let the last character scroll trough "$", and make the other ones jump over it
      else if (cursorPosition < playerNameLength - 1 && playerName[cursorPosition] == 1) playerName[cursorPosition] = myAlphabetSize - 1;
      else playerName[cursorPosition] --;

      characterFromTextClear(cursorPositionLCDRelative);// we clear the current position between scrolls so we don't overlap characters
    }

    if (newLeft) {// here we want to move the cursor left inside the player name
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
// if we have 2 $ at the end and we need to delete the last one. the first one becomes the new tail of the array (our tail is represented by the character "$", so if the last character (the one that can 
// scroll trough it) choses the void character ("$") than when he moves from it, we delete that character)
      if (cursorPosition == playerNameLength - 1 && playerName[cursorPosition] == 0 && playerNameLength > 1) {
        playerNameLength --;
        cursorPosition --;
        textClear();
      }
      else if (cursorPosition > 0) {//if this is not the first letter, nor the last, that means we can safely move left without doing anything else
        cursorPosition --;
        textClear();
      }
    }
    if (newRight) {// here we want to move the cursor right inside the player name
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
//if we have 1 $ at the end of the word we can create another one, the old one becomes a void character (represented by the $ and the allmost full character on the lcd) that can be changed into a letter, but it will 
//never be kept nor shown in the highscores tab, its purpose is just to be the tail of the word and help adding new letters to the inputed word
      if (cursorPosition == playerNameLength - 1 && playerName[cursorPosition] != 0 && playerNameLength < maxPlayerName - 1) {//we can create one more letter
        playerNameLength ++;
        cursorPosition ++;
        playerName[playerNameLength] = 0;//(char)pgm_read_byte_near(myAlphabet + 0) = '$';
        textClear();
      }
      else if(cursorPosition < playerNameLength - 1) {//if the current character is not the last one than we can move freely to the right
        cursorPosition ++;
        textClear();
      }
    }

  }
  else if (state == 21) {// select difficulty, behaves allmost like the state 1 (main menu)
    scrollLogic();
    blinkLogic();

//if joystick is moved down we go to the next option
    if (newDown) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if(selectedIndex < difficultiesNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textWithArrowClear();
      }
    }

//if joystick is moved up we go to the previous option
    if (newUp) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if(selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textWithArrowClear();
      }
    }

//if the button is pressed we set the difficulty and we go to the state 23 (in game)
    if (newButtonPress) {
      clickSfx();
      difficulty = selectedIndex;
      level = 0;
      exitState(21);
      enterState(22);
    }

  }
  else if (state == 22) {// before level
    scrollLogic();

    if (matrixEmptyed && (newButtonPress || newLeft || newRight || newUp || newDown)) {// we wait for any input to start the level, also we wait for the matrix to be cleared
      matrixState = 0;// we change the status of the matrix to game display
      newLeft = newRight = newUp = newDown = 0;

      levelDuration = baseLevelDuration * (1 + level * 0.5f + difficulty * 0.2f);// here we calculate the duration of the level
      levelStart = millis();

      score = lastScore;// we set the current score as the last saved one

      for (byte i = 0; i < mapHight; i ++) matrix[i] = 0; // we empty the map of the previous tetromino

      ticksPerForcedStep = defaultTicksPerFrocedStep - 2 * difficulty - level;// here we calculate the starting speed of the level

//    we spawn the first tetromino
      tetrominoCount = 1;

      currentX = tetrominoSpawnPosition;
      currentY = 0;
      currentRotation = 0;
      currentPiece = rand() % tetrominoNumber;

// we go to the in game state
      exitState(22);
      enterState(23);
    }
  }
  else if (state == 23) {// in game
    if (levelStart + levelDuration < millis()) {// the purpose of the game is to survive the level, aka be able to place tetromino pieces when the level timer is out, so when the timer is out the level is passed
      score += baseLevelPointsValue * (difficulty * 0.7f + level * 0.5f + 1);// we add the points for passing the level
      //we advance to the next level and we save the score (because we finished the level)
      level ++;
      lastScore = score;
      //we go to the state that manages telling the player that he passed the level
      exitState(23);
      enterState(25);
    }
    else gameLoop();// if the timer is not out than we are simulating the game
  }
  else if (state == 24) {// this state tells the player that he lost (no more lifes left)
    scrollLogic();
//the only thing the player can do is skip this "popup" and go to the title screen at this point pecause the game is over for him
    if (newButtonPress) {
      clickSfx();
      exitState(24);

      EEPROM.get(lastHighscoreAddress, highscore);// we pull from EEPROM the smallest highscore and we check if the player has a bigger highscore than the ones stored in EEPROM
      
      if (lastScore > highscore) enterState(27);// if we have a new top highscore we instead go to the state that asks the player if he wants to save his score
      else {
        enterState(0);//back to main menu
        
        //we set the matrix to idle, showing the screensaver
        matrixIdle = 1;
        matrixState = 2;
        matrixEmptyed = 0;
        emptyX = 0;
        emptyY = 0;
      }
    }
  }
  else if (state == 25) {//level passed
    scrollLogic();

    if (newButtonPress) {// the player can only skip this state and go to the pre-level of the next level
      clickSfx();
      exitState(25);//exiting this state
      enterState(22);//going to the next pre-level
    }
  }
  else if (state == 26) {//-1 heart
    scrollLogic();

    if (newButtonPress) {// the player can only skip this state and go to the pre-lavel of the level he just lost a heart to
      clickSfx();
      exitState(26);
      enterState(22);
    }
  }
  else if (state == 27) {// new highscore
    scrollLogic();
    blinkLogic();
//this state handles the question "do you want to save your highscore?" if the score is bigger than the ones allready saved in EEPROm
    if (newLeft) {// to select the "yes" answer
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      lastArrowClear();
      selectedIndex = 0;
    }
    if (newRight) {// to select "no"
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      firstArrowClear();
      selectedIndex = 1;
    }

    if (newButtonPress) {//when the button is presssed, if "yes" was selected we compare the current score with all the top scores and we push it in EEPROM
      clickSfx();
      if (selectedIndex == 0) {
        //we pull from eeprom one by one the first and the second highscores and we compare it with the current score, and move our score in EEPROM on its deserving place
        EEPROM.get(secondHighscoreAddress, highscore);// we pull the second place highscore

        if (lastScore > highscore) { // we know allready that the score > lastHighscore (it is at least on the third place), so we compare it with the second place highscore
          EEPROM.get(biggestHighscoreAddress, highscore);

          if (lastScore > highscore) {
            //here we know that our score is the new first highscore so we move the second in the third place and the first in the second place and our new score in the first place, using the highscore and highscoreName buffers
            EEPROM.get(secondHighscoreAddress, highscore);
            EEPROM.put(lastHighscoreAddress, highscore);

            EEPROM.get(secondHighscoreNameAddress, highscoreName);
            EEPROM.put(lastHighscoreNameAddress, highscoreName);


            EEPROM.get(biggestHighscoreAddress, highscore);
            EEPROM.put(secondHighscoreAddress, highscore);

            EEPROM.get(biggestHighscoreNameAddress, highscoreName);
            EEPROM.put(secondHighscoreNameAddress, highscoreName);


            for (byte i = 0; i < playerNameLength; i ++) highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);// we decode the name like in the display funtion at state 20
            highscoreName[playerNameLength] = NULL;

            EEPROM.put(biggestHighscoreAddress, lastScore);
            EEPROM.put(biggestHighscoreNameAddress, highscoreName);
          }
          else {
            //here we know that our new score is the new second highscore so we move the second on the third place and the new score on the second place, using the buffers highscore and highscoreName
            EEPROM.get(secondHighscoreAddress, highscore);
            EEPROM.put(lastHighscoreAddress, highscore);

            EEPROM.get(secondHighscoreNameAddress, highscoreName);
            EEPROM.put(lastHighscoreNameAddress, highscoreName);


            for (byte i = 0; i < playerNameLength; i ++) highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);// we decode the name like in display function at state 20
            highscoreName[playerNameLength] = NULL;

            EEPROM.put(secondHighscoreAddress, lastScore);
            EEPROM.put(secondHighscoreAddress, highscoreName);
          }
        }
        else {
        //here we know that our new score is in the third place and we place it there
          for (byte i = 0; i < playerNameLength; i ++) highscoreName[i] = (char)pgm_read_byte_near(myAlphabet + playerName[i]);// we decode the name just like in display function at state 20
          highscoreName[playerNameLength] = NULL;

          EEPROM.put(lastHighscoreAddress, lastScore);
          EEPROM.put(lastHighscoreNameAddress, highscoreName);
        }
      }

      // we set the matrix to idle and we start the screen saver
      matrixIdle = 1;
      matrixState = 2;
      matrixEmptyed = 0;
      emptyX = 0;
      emptyY = 0;

      // after all of that we go back to title screen
      exitState(27);
      enterState(0);
    }
  }
  else if (state == 3) {// this state hadles the settings menu, it works the same as main menu
    scrollLogic();
    blinkLogic();

    if (newDown) {// if we move the joystick down we show the next option in the menu
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex < settingsNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textWithArrowClear();
      }
    }
    if (newUp) {// if we move the joystick up we show the previous option in the menu
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if(selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textWithArrowClear();
      }
    }
    if (newButtonPress) {// if we click we enter the selected option
      clickSfx();
      exitState(3);
      if (selectedIndex == 0) enterState(30);
      else if (selectedIndex == 1) enterState(31);
      else if (selectedIndex == 2) enterState(32);
      else if (selectedIndex == 3) enterState(33);
      else if (selectedIndex == 4) enterState(34);
      else if (selectedIndex == 5) enterState(1);// this is for going back to main menu
    }
  }
  else if (state == 30) {// lcd contrast
    if (newLeft) {// if we move left we lowe the contrast
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (lcdContrast > 0) {
        lcdContrast --;// this just lowers the ammount of segments, and below we change the contrast based in this out of the total number of segments in the slider
        analogWrite(lcdContrastPin, map(lcdContrast, 0, sliderLength, lcdMinContrast, lcdMaxContrast));// we map the lcd contrast to the limitation of the contrast given
      }
    }
    if (newRight) {// right to increase contrast
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (lcdContrast < sliderLength) {
        lcdContrast ++;
        analogWrite(lcdContrastPin, map(lcdContrast, 0, sliderLength, lcdMinContrast, lcdMaxContrast));
      }
    }

    //if we press the button we save in EEPROM the current value of lcdContrast
    if (newButtonPress) {
      clickSfx();
      EEPROM.update(lcdContrastAddress, lcdContrast);
      exitState(30);
      enterState(3);
    }
  }
  else if (state == 31) {//lcd brightness
    scrollLogic();
//this state is 99% the previous one, but it is used for the brightness instead of the contrast
    if (movedLeft) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (lcdBrightness > 0) {
        lcdBrightness --;
        analogWrite(lcdBrightnessPin, map(lcdBrightness, 0, sliderLength, lcdMinBrightness, lcdMaxBrightness));
      }
    }
    if (newRight) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (lcdBrightness < sliderLength) {
        lcdBrightness ++;
        analogWrite(lcdBrightnessPin, map(lcdBrightness, 0, sliderLength, lcdMinBrightness, lcdMaxBrightness));
      }
    }
    if (newButtonPress) {
      clickSfx();
      EEPROM.update(lcdBrightnessAddress, lcdBrightness);
      exitState(31);
      enterState(3);
    }
  }
  else if (state == 32) {//matrix brightness
    scrollLogic();
    //the same as the previous state, it is just used for matrix brightness
    if (newLeft) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (matrixBrightness > 0) {
        matrixBrightness --;
        lc.setIntensity(0, matrixBrightness);// matrix brightness takes values from 0 to 15 and my slider has values from 0 to 14, so no mapping needed, i just cut off the high end (the matrix is soo bright anyway)
      }
    }
    if (newRight) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (matrixBrightness < sliderLength) {
        matrixBrightness ++;
        lc.setIntensity(0, matrixBrightness);
      }
    }
    if (newButtonPress) {
      clickSfx();
      EEPROM.update(matrixBrightnessAddress, matrixBrightness);
      exitState(32);
      enterState(3);
    }
  }
  else if (state == 33) {// invert x axis
    scrollLogic();
    blinkLogic();
// a yes or no question, the same as in state 27
    if (newLeft) {//left for yes
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      lastArrowClear();
      selectedIndex = 0;
    }
    if (newRight) {//right for no
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      firstArrowClear();
      selectedIndex = 1;
    }

    if (newButtonPress) {// if we selected yes we invert the axis
      clickSfx();
      if (selectedIndex == 0) invertedXAxis = !invertedXAxis;
      exitState(33);
      enterState(3);
    }
  }
  else if (state == 34) {//invert y axis
    scrollLogic();
    blinkLogic();
//the same as the previous state, it is just for the y axis
    if (newLeft) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      lastArrowClear();
      selectedIndex = 0;
    }
    if (newRight) {
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      firstArrowClear();
      selectedIndex = 1;
    }

    if (newButtonPress) {
      clickSfx();
      if (selectedIndex == 0) invertedYAxis = !invertedYAxis;
      exitState(34);
      enterState(3);
    }
  }
  else if (state == 4) {// highscores
    scrollLogic();

    if (newDown) {// if we move down we scroll to the next highscore
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex < highscoresNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textClear();
      }
    }
    if (newUp) {// up for the previuous highscore
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textClear();
      }
    }
    if (newButtonPress) {// button press to return to the previous menu
      clickSfx();
      exitState(4);
      enterState(1);
    }
  }
  else if (state == 5) {// credits
    scrollLogic();
//just like the previous state
    if (newDown) {//down for next item
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex < creditsNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textClear();
      }
    }
    if (newUp) {// up for the previous item
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textClear();
      }
    }
    if (newButtonPress) {//button press to go back
      clickSfx();
      exitState(5);
      enterState(1);
    }
  }
  else if (state == 6) {//help
    scrollLogic();
//just like the previous state
    if (newDown) {//move down for the next item
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if (selectedIndex < helpNumber - 1) {
        selectedIndex ++;
        resetScroll();
        textClear();
      }
    }
    if (newUp) {//up for the previous item
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      if(selectedIndex > 0) {
        selectedIndex --;
        resetScroll();
        textClear();
      }
    }
    if (newButtonPress) {//button press to return
      clickSfx();
      exitState(6);
      enterState(1);
    }
  }
  else if (state == 7) {//back to title screen
    scrollLogic();
    blinkLogic();
    //a yes/no question, just like state 33
    if (newLeft) {//left for yes
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      lastArrowClear();
      selectedIndex = 0;
    }
    if (newRight) {//right for no
      moveSfx();
      newLeft = newRight = newUp = newDown = 0;
      firstArrowClear();
      selectedIndex = 1;
    }

    if (newButtonPress) {// if we selected yes we return to the title screen
      clickSfx();
      exitState(7);
      if (selectedIndex == 0) enterState(0);
      else enterState(1);
    }
  }
}

// this function is a helper for the stateBehaviour function, when we enter a certain state we want some things to happen (only once) so this is where this function
// comes in handy (for example we want to  start turning off the leds in the state 22, we start only once, if we start in each loop we never finish)
void enterState(byte newState) {
  lcd.clear();

  if (newState == 0) {//title screen
    state = 0;
    startScroll();
    startSong();
  }
  else if (newState == 1) {//main menu
    state = 1;
    startScroll();
    selectedIndex = lastMenuIndex;// we set the selected option to be the last selected one
  }
  else if (newState == 20) {//select player name
    state = 20;
    cursorPosition = 0;// we set the cursor to be at the first liter of the last introduced player name since opening of the device
    startScroll();
  }
  else if (newState == 21) {//select difficulty
    state = 21;
    startScroll();
    selectedIndex = 0;// we set the selected option to be the first one
  }
  else if (newState == 22) {//pre-game
  //we start cleaning any leds that are open in order to prepare for the start of the level
    matrixIdle = 0;
    matrixState = 2;
    matrixEmptyed = 0;

    state = 22;
    startScroll();
  }
  else if (newState == 23) {//in game
    state = 23;
  }
  else if (newState == 24) {//game over
    state = 24;
    startScroll();
  }
  else if (newState == 25) {//level passed
    state = 25;
    startScroll();
  }
  else if (newState == 26) {//-1 heart
    state = 26;
    startScroll();
  }
  else if (newState == 27) {// new highscore
    state = 27;
    startScroll();
    selectedIndex = 0;// we set the selected option to be the first one
  }
  else if (newState == 3) {//settings
    state = 3;
    startScroll();
    selectedIndex = lastSettingsIndex;// we set the selected option to be the last selected one when we were here
  }
  else if (newState == 30) {//lcd contrast
    state = 30;
  }
  else if (newState == 31) {//lcd brightness
    state = 31;
    startScroll();
  }
  else if (newState == 32) {//matrix brightness
    state = 32;
    startScroll();
  }
  else if (newState == 33) {// invert x axis
    state = 33;
    startScroll();
    selectedIndex = 0;// the default selected option is yes
  }
  else if (newState == 34) {//invert y axis
    state = 34;
    startScroll();
    selectedIndex = 0;//the default selected option is yes
  }
  else if (newState == 4) {// highscores
    state = 4;
    startScroll();
    selectedIndex = 0;//we start with the first highscore
  }
  else if (newState == 5) {//credits
    state = 5;
    startScroll();
    selectedIndex = 0;//we start with the first item
  }
  else if (newState == 6) {//help
    state = 6;
    startScroll();
    selectedIndex = 0;//we start with the first item
  }
  else if (newState == 7) {//back to title screen?
    state = 7;
    startScroll();
    selectedIndex = 0;// the default selected option is yes
  }
}

// a function that is allmost like the previous one, but it is used when we exit a state
void exitState(byte prevState) {
  if (prevState == 0) {//title screen
    stopScroll();
    stopSong();
  }
  else if (prevState == 1) {//main menu
    stopScroll();
    lastMenuIndex = selectedIndex;// we keep the last selected intex from the main menu 
  }
  else if (prevState == 20) {// select player name
    stopScroll();
  }
  else if (prevState == 21) {//select difficulty
    stopScroll();
    //we enter the pre level of a new game
    lastScore = 0;//the starting score

//  setting the stating number of hearts
    if (difficulty == 0) hearts = 5;//easy
    else if (difficulty == 1) hearts = 3;//medium
    else hearts = 1;//hardcore
  }
  else if (prevState == 22) {//pre game
    stopScroll();
  }
  else if (prevState == 23) {//in game
  }
  else if (prevState == 24) {//game over
    stopScroll();
  }
  else if (prevState == 25) {//level passed
    stopScroll();
    
    //we prepare to advance to the pre level of the next level so we increase the hearts based on difficulty
    if (difficulty == 0)hearts += 2;// easy
    else if (difficulty == 1) hearts += 1;//medium
  }
  else if (prevState == 26) {//-1 heart
    stopScroll();
  }
  else if (prevState == 27) {//new highscore
    stopScroll();
  }
  else if (prevState == 3) {//settings
    stopScroll();
    lastSettingsIndex = selectedIndex;// we keep the last selected item from the settings menu
  }
  else if (prevState == 30) {//lcd contrast
  }
  else if (prevState == 31) {//lcd brightness
    stopScroll();
  }
  else if (prevState == 32) {//matrix brightness
    stopScroll();
  }
  else if (prevState == 33) {//invert x axis
    stopScroll();
  }
  else if (prevState == 34) {//invert y axis
    stopScroll();
  }
  else if (prevState == 4) {//highscores
    stopScroll();
  }
  else if (prevState == 5) {//credits
    stopScroll();
  }
  else if (prevState == 6) {//help
    stopScroll();
  }
  else if (prevState == 7) {//back to title screen?
    stopScroll();
  }
}

//helper function used mainly in the blinking process, this function clears the segment on the lcd where the left arrow is (we clear just the arrow segment for a smoother display)
void firstArrowClear() {
  lcd.setCursor(0, 1);
  lcd.print(" ");
}

//same as the one above, for the right arrow (the left arrow is the one that is neart the "yes", and the right is for "no" from the yes/no question pannels)
void lastArrowClear() {
  lcd.setCursor(12, 1);
  lcd.print(" ");
}

//a function that has a purpose alike the previous two, this one clears the title (the first row, without the 3 last segments used for the current avabile keys)
void titleClear() {
  lcd.setCursor(0, 0);
  strcpy_P(title, blankTitle);
  lcd.print(title);
}

//same as the previous one, used to clear just the bottom row of the lcd
void textClear() {
  lcd.setCursor(0, 1);
  strcpy_P(text, blankText);
  lcd.print(text);
}

//used as the previous one, but to clear the second row of the lcd, besides the first segment
void textWithArrowClear() {
  lcd.setCursor(1, 1);
  strcpy_P(text, blankTextWithArrow);
  lcd.print(text);
}

//clears only a certain segment on the lcd
void characterFromTextClear(byte pos) {
  lcd.setCursor(pos, 1);
  lcd.print(" ");
}


// the main function that handles the sensors 
void sensorLogic() {
  // once at an interval we read the joystick sensors
  if (lastJoyRead + joyReadInterval < millis()) {
    updatePositions();
    lastJoyRead = millis();
  }

  newButtonPress = 0;// here is where the click token is deleted (if the token couldn't do anything in the loop he originated form, than it gets distroyed)
  //once at an interval we read the button sensor
  if (lastSWTime + sWInterval < millis()) {
    clickCheck();
    lastSWTime = millis();
  }
}

//helper function that checks the joystick sensors
void updatePositions() {
  //the reading of the sensors
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

//we keep the last triggers
  lastMovedLeft = movedLeft;
  lastMovedRight = movedRight;
  lastMovedUp = movedUp;
  lastMovedDown = movedDown;

// we now check of any read value is capable of being considered a trigger
// movedJoy is used to separate the inputs, so we need to return the joystick back to the starting position to read a new input
  movedLeft = movedRight = movedUp = movedDown = 0;

  if (xValue < minThreshold && !movedJoy) {// if we get an left trigger
    movedLeft = 1;// we save the trigger
    movedJoy = 1;
  }
  // if we got a left trigger, and the trigger is new (we didnt have last time a left trigger) and if the time since the last token was produced is longer than the production cooldown
  if (movedLeft && !lastMovedLeft && lastJoyTime + joyInterval < millis()) {
    lastJoyTime = millis();//we set the last token produce time
    newLeft = 1;// we made the new left move token
  }

// same as above, used for the right token
  if (xValue > maxThreshold && !movedJoy) {
    movedRight = 1;
    movedJoy = 1;
  }
  if (movedRight && !lastMovedRight && lastJoyTime + joyInterval < millis()) {
    lastJoyTime = millis();
    newRight = 1;
  }

// same as above, used for the down token
  if (yValue < minThreshold && !movedJoy) {
    movedDown = 1;
    movedJoy = 1;
  }
  if (movedDown && !lastMovedDown && lastJoyTime + joyInterval < millis()) {
    lastJoyTime = millis();
    newDown = 1;
  }

// same as above, used for the left token
  if (yValue > maxThreshold && !movedJoy) {
    movedUp = 1;
    movedJoy = 1;
  }
  if (movedUp && !lastMovedUp && lastJoyTime + joyInterval < millis()) {
    lastJoyTime = millis();
    newUp = 1;
  }

// if there is no trigger, that means the joystick is in the starting position, that means we can read for a new triggers
  if (yValue >= minThreshold && yValue <= maxThreshold && xValue >= minThreshold && xValue <= maxThreshold) movedJoy = 0;

// if we inverted the x axis we swap the token left with right and the other way around
  if (invertedXAxis) {
    if (newLeft && !newRight) {
      newLeft = 0;
      newRight = 1;
    }
    else if (!newLeft && newRight) {
      newLeft = 1;
      newRight = 0;
    }
  }

// if we inverted the y axis we swap the token down with up and the other way around
  if (invertedYAxis) {
    if (newUp && !newDown) {
      newUp = 0;
      newDown = 1;
    }
    else if (!newUp && newDown) {
      newUp = 1;
      newDown = 0;
    }
  }
}

//helper function that creates the button press tokens
void clickCheck() {
  lastSwState = swState;// we keep the last reading
  swState = digitalRead(sWPin);//we make a new reading
// if the current reading is a trigger, the last readng is not a trigger (aka the new reading is a new trigger) and enough time has passed than we create a new button press token
  if (swState == 0 && lastSwState == 1 && lastButtonTime + buttonPressInterval < millis()) {
    lastButtonTime = millis();
    newButtonPress = 1;
  }
}




void loadPreferences() {//pulling last settings from eeprom and applying them to the lcd and matrix
  lcdContrast = EEPROM.read(lcdContrastAddress) % (sliderLength + 1);//in case we pull out from eeprom a value bigger than the maximum number of segments of the slider, we reduce that to fit it
  analogWrite(lcdContrastPin, map(lcdContrast, 0, sliderLength, lcdMinContrast, lcdMaxContrast));

  lcdBrightness = EEPROM.read(lcdBrightnessAddress) % (sliderLength + 1);//in case we pull out from eeprom a value bigger than the maximum number of segments of the slider, we reduce that to fit it
  analogWrite(lcdBrightnessPin, map(lcdBrightness, 0, sliderLength, lcdMinBrightness, lcdMaxBrightness));

  matrixBrightness = EEPROM.read(matrixBrightnessAddress) % (sliderLength + 1);//in case we pull out from eeprom a value bigger than the maximum number of segments of the slider, we reduce that to fit it
  lc.setIntensity(deviceIndex, matrixBrightness);
}


// the function that handles the tetris theme
void songLogic() {
//if the song is being played and enough time has passed since we last played a new tone, than we play the next note
  if (startedSong) {
    if (lastNoteTime + lastNoteDuration < millis()) {
      int divider = pgm_read_word_near(melody + thisNote + 1);// we take the devider of the next note from the melody array
      int noteDuration;

      if (divider > 0) noteDuration = (wholenote) / divider; // negative divider means the note has a dot
      else if (divider < 0) {
        noteDuration = (wholenote) / abs(divider);// we calculate the note duration based on the duration of a whole note and based on the divider
        noteDuration *= 1.5;
      }
      // we save the current note starting time and duration
      lastNoteDuration = noteDuration;
      lastNoteTime = millis();

      // we play the tone (0.9 of the time, and 0.1 is left space between notes)
      noTone(buzzerPin);
      tone(buzzerPin, pgm_read_word_near(melody + thisNote), noteDuration * 0.9);

// we advance to the next note and divider
      if (thisNote == notes * 2) thisNote = 0;// if this was the last note, than we start the song all over again
      else thisNote += 2;
    }
  }
}

//function that starts the song
void startSong() {
  lastNoteTime = millis();
  thisNote = 0;
  lastNoteDuration = 0;
  startedSong = 1;
}

//function that stops the song and any playing sound
void stopSong() {
  startedSong = 0;
  noTone(buzzerPin);
}

//the function that simulates the game and handles its behaviour
void gameLoop() {
  frameMechanic();// here we calculate if we have a new tick(frame)
  //if we have a new frame, we count it, if we reach the required ammount we force move the current piece down by one row
   if (newTick) {
    tickCount ++;
    if (tickCount == ticksPerForcedStep) {
      forcedStep = 1;
      tickCount = 0;
    }
    else forcedStep = 0;

// if we placed enough tetromino we speed up the game
    if (tetrominoCount == piecesForSpeedup) {
      ticksPerForcedStep --;
      tetrominoCount = 0;
    }

// if we move the joystick left we first check if the piece fits in the new location (one column to the left), if it does we move it there
    if (newLeft) {
      if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY)) {
        moveSfx();
        currentX --;
        newLeft = newRight = newUp = newDown = 0;
      }
    }

// if we move the joystick right we first check if the piece fits in the new location (one column to the right), if it does we move it there
    if (newRight) {
      if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY)) {
        moveSfx();
        currentX ++;
        newLeft = newRight = newUp = newDown = 0;
      }
    }

// if we move the joystick down we first check if the piece fits in the new location (one row down), if it does we move it there
    if (newDown) {
      if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) {
        moveSfx();
        currentY ++;
        newLeft = newRight = newUp = newDown = 0;
      }
    }

// if we move the joystick up we first check if the piece, if rotated by 90 degrees, fits in her current position
    if (newUp) {
      if (doesPieceFit(currentPiece, currentRotation + 1, currentX, currentY)) { // when we press up the piece doesn't move (like the above), it changes the orientation
        moveSfx();
        currentRotation ++;
        newLeft = newRight = newUp = newDown = 0;
      }
    }

// here we force move the piece one row down, and also here we fix the pieces on the map
    if (forcedStep) {
      if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) currentY ++;// if the piece can move one row down without problem it is allright, we are doing nothing else besides that
      else {
          //if it can't go down, than it becomes stationary
          // for each filled tetromino [x][y] if it is true (we have a solid block there) we place it on the map as a permanent object
        for (byte px = 0; px < tetrominoSize; px ++)
          for (byte py = 0; py < tetrominoSize; py ++) {
            byte pi = rotate(px, py, currentRotation);// this function decodes from x y and rotation the index of the segment in the tetromino piece, for additional information see the tetromino codification section

// if the current block is used by the current tetromino piece than we place it on the map as an stationary object
          if(tetromino[currentPiece][pi])
            setValue(currentX + px, currentY + py, 1);
          }

// this is the part were we check for new formed lines after we fixed the last piece
        byte lineCounter = 0;
        for (byte py = 0; py < tetrominoSize; py ++)// we only check for the rows where our tetromino piece landed (it is useless to check all the old rows that have nothing new on them)
          if (currentY + py < mapHight) {// check if we are still within the map borders
            bool isFilled = 1;
              for (byte px = 0; px < matrixSize; px ++) 
                if (!getValue(px, currentY + py)) isFilled = 0;// if we have an open space than the line is not filled
            
            // if the current line is filled, we delete it, and increase the lineCounter
            if (isFilled) {
              for (byte k = currentY + py; k > 0; k --)// we copy the line above our current one from the matrix map
                matrix[k] = matrix[k - 1];
              matrix[0] = 0;// and we put a new empty line on top
              lineCounter ++;
            }
          }
// when we make at least a new filled line our score is increased, but if we make more lines at once then we get a bonus using the following formula:
        score += baseLinePointsValue * (1 + difficulty * 0.2f + level * 0.4f + (defaultTicksPerFrocedStep - ticksPerForcedStep) * lineCounter);//when the speed increases the score from filled lines increases too

        //create new piece
        tetrominoCount ++;
        currentX = tetrominoSpawnPosition;
        currentY = 0;
        currentRotation = 0;
        currentPiece = rand() % tetrominoNumber;

        if (!doesPieceFit(currentPiece, currentRotation, currentX, currentY)) {//if the new piece doesn't fit than -1heart
          exitState(23);
          if (hearts == 1) enterState(24);// if no hearts left than game over
          else {//otherwise just -1heart
            hearts --;
            enterState(26);
          }
        }
      }
    }
  }
  displayMatrix();//every game loop we display the matrix
}

//the function that calculates if this loop there is a new frame
void frameMechanic() {
  if (tickDuration + lastGameTick < millis()) {//once in a while we have a new frame(tick)
    lastGameTick = millis();
    newTick = 1;
  }
  else newTick = 0;
}

//function that handles the display of the game on the matrix
void displayMatrix() {//we update every row (not optimal)
  for (byte i = mapHight - matrixSize; i < mapHight; i ++)
    lc.setRow(deviceIndex, i - (mapHight - matrixSize), matrix[i]);//we display every row from the rows 4->11 of the map (counting from 0)
  
  if (doesPieceFit(currentPiece, currentRotation, currentX, currentY)) {// if the current piece fits than we displey it as well
    for (byte px = 0; px < tetrominoSize; px ++)
      for (byte py = 0; py < tetrominoSize; py ++) 
        if (currentY + py >= mapHight - matrixSize) {
          byte pi = rotate(px, py, currentRotation);//calculates the relative position in the tetromino vector, for additional information see the tetromino codification section

          if(tetromino[currentPiece][pi])// if the current block of tetromino piece is solid than we show it
            lc.setLed(deviceIndex, currentY + py - (mapHight - matrixSize), currentX + px, 1);
        }
  }
}

//function that checks if a given piece fits on a given position on the map (aka colision mechanic)
bool doesPieceFit(byte nTetromino, int nRotation, int nPosX, int nPosY) {
  for (byte px = 0; px < tetrominoSize; px ++)
    for (byte py = 0; py < tetrominoSize; py ++) {
      byte pi = rotate(px, py, nRotation);// the relative position in the tetromino vector (see tetromino codification for additional information)

      if (tetromino[nTetromino][pi] == 1) {// if the current block in the tetromino piece is solid
        if (nPosX + px < 0 || nPosX + px >= matrixSize || nPosY + py < 0 || nPosY + py >= mapHight) return 0;// if it is out of bounds than it doesn't fit on the map
        else if (getValue(nPosX + px, nPosY + py)) return 0;// if a solid block overlaps with a solid object of the map than the piece doesn't fit
      }
    }
  return 1;// if the piece passed all the checks than it fits
}

//function that gets a value from the map, 1 meaning solid object and 0 meaning empty space
bool getValue(byte px, byte py) {// gets value from the byte array (gets the value of the bit on a specific position)
  return (matrix[py] / p2[matrixSize - px - 1]) % 2;// the map is a array of bytes so we use powers of 2 to get a specific bit in each byte of the array (aka a position of the map)
}

//function that sets a value on the map (1 - solid, 0 - empty space)
void setValue(byte px, byte py, bool value) {// sets the value of a bit in the array of bytes (sets the value of a bit on a specific position)
  if (value == getValue(px, py)) return;// if the current value is allready there we do nothing
  if (value == 1) matrix[py] += p2[matrixSize - px - 1];//otherwise if we have to change a 0 for a 1 we use powers of 2 to add the required bit
  else matrix[py] -= p2[matrixSize - px - 1];// the same but we substract the same power when we change a 1 for a 0
}

//function that calculates the index i inside the aray of a tetromino, for more information see the tetromino decoding section
//just applying the formulas
byte rotate(byte px, byte py, int r) {
  if (r % 4 == 0) return py * tetrominoSize + px;//0
  else if (r % 4 == 1) return (tetrominoSize - 1) * tetrominoSize + py - (px * tetrominoSize);//90
  else if (r % 4 == 2) return tetrominoSize * tetrominoSize - 1 - (py * 4) - px;//180
  else return tetrominoSize - 1  - py + (px * tetrominoSize);//270
}

//function used to wipe data and overwrite the highscores and the settings stored in EEPROM
void eepromFirstWrite() {
  highscore = 0;
  strcpy(highscoreName, "Marian");
  EEPROM.put(biggestHighscoreAddress, highscore);
  EEPROM.put(biggestHighscoreNameAddress, highscoreName);
  highscore = 0;
  strcpy(highscoreName, "Ionut");
  EEPROM.put(secondHighscoreAddress, highscore);
  EEPROM.put(secondHighscoreNameAddress, highscoreName);
  highscore = 0;
  strcpy(highscoreName, "Edi");
  EEPROM.put(lastHighscoreAddress, highscore);
  EEPROM.put(lastHighscoreNameAddress, highscoreName);
//   lcdContrast = 10;
//   EEPROM.put(lcdContrastAddress, lcdContrast);
//   lcdBrightness = 12;
//   EEPROM.put(lcdBrightnessAddress, lcdBrightness);
//   matrixBrightness = 2;
//   EEPROM.put(matrixBrightnessAddress, matrixBrightness);
}

// this function handles the display on the matrix depending on the matrix state
void ledMatrixDisplay() {
  if (matrixState == 0) displayMatrix();// here if we are currently playing the game
  else if (matrixState == 1) fillMatrix();// if we are filling the matrix
  else emptyMatrix();// if we are emptying the matrix
}

//function that fills the matrix, step by step in each loop
void fillMatrix() {
  //if enough time has passed since the last led turn on we turn on another led, if we finished turning all the leds on we set matrixFilled to 1 and we reset x and y to 0, getting them ready for the next filling
  if (lastFilled + fillTime < millis()) {
    lastFilled = millis();
    lc.setLed(deviceIndex, fillX, fillY, 1);
  
    if (fillX == matrixSize - 1 && fillY == matrixSize - 1) {
      matrixFilled = 1;
      fillX = fillY = 0;
    }
    else {// row by row
      if (fillX == matrixSize - 1) {
        fillX = 0;
        fillY ++;
      }
      else fillX ++;
    }
  }
}

//function that empties the matrix, step by step in each loop
void emptyMatrix() {
  //if enough time has passed since the last led turn off we turn off another led, if we finished turning all the leds off we set matrixEmptyed to 1 and we reset x and y to 0, getting them ready for the next emptying
  if (lastEmptyed + emptyTime < millis()) {
    lastEmptyed = millis();
    lc.setLed(deviceIndex, emptyX, emptyY, 0);

    if (emptyX == matrixSize - 1 && emptyY == matrixSize - 1) {
      matrixEmptyed = 1;
      emptyX = emptyY = 0;
    }
    else {//row by row
      if (emptyX == matrixSize - 1) {
        emptyX = 0;
        emptyY ++;
      }
      else emptyX ++;
    }
  }
}

//this function handles the time when the matrix is idle, it alternates between turning all the leds on and turning all the leds off
void idleMatrix() {
  if (matrixIdle) {
      // if we finished filling we start emptying
    if (matrixState == 1 && matrixFilled) {
      matrixEmptyed = 0;
      matrixState = 2;
      emptyX = 0;
      emptyY = 0;
    }
    //if we finished emptying we start filling
    else if (matrixState == 2 && matrixEmptyed) {
      matrixFilled = 0;
      matrixState = 1;
      fillX = 0;
      fillY = 0;
    }
  }
}

//function that interrupts the theme song and plays a sfx that i chose for when moving the joystick
void moveSfx() {
  lastNoteDuration = sfxDuration;
  lastNoteTime = millis();
  noTone(buzzerPin);
  tone(buzzerPin, NOTE_C5, sfxDuration * 0.9);
}

//same as the above, but for when the button is pressed
void clickSfx() {
  lastNoteDuration = sfxDuration;
  lastNoteTime = millis();
  noTone(buzzerPin);
  tone(buzzerPin, NOTE_E5, sfxDuration * 0.9);
}

/*

===========================================================ADDITIONAL INFORMATION=================================================================================

0.Link Demo of the game: https://www.youtube.com/watch?v=IfsCczCO6Gw&ab_channel=MarianDimofte


1.The player namer codification
I store the player name in an array of type byte. Each letter in the array can be translated into a char using the vector myAlphabet as follows:
myAlphabet[playerName[index]] = decoded letter (char). Everywhere I decode the name I must use the function "pgm_read_byte" because I stored the
myAlphabet array in progamabile memory using PROGMEM. For additional information see: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

In addition in my coddification the "$" represents the tail of the name, (you can kinda say that strlen(playerName) = the position of the "$" but it isn't correct
because playerName is an array of bytes). I also use $ when the user adds/removes one letter. When you add another letter (at the end), by default it is 
set to be the void one ($) and you can change it after. Only the last letter can be set to be void by the user and if you do it and move your cursor from there,
the letter gets deleted (this is how the user shortens his name).


2.The tetromino codification
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


3.The states of the program
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


4.The matrix state
The matrix has 3 states: in game (0), filling(1), emptying(2). When in game the matrix shows the map and the current piece. When in filling the display the matrix
turns on the current position and advances to the next one. If the current position was the last one, we set the matrix filled variable to True. The same goes for
when emptying the display.

5.Sensors reading
The sensor are being read once at a certain interval. For the button we create a token when we detect a new press of the button (if a token was not created in the
past 0.1 seconds) and after that, depends of the state if that token gets used, otherwise at the next reading it gets destroyed.

For the joystick the principle is allmost the same, the only difference is that the token only gets destroyed after it was used ("newLeft = newRight = newDown = newUp = 0").

6.EEPROM mapping

//    address | stored data
//byte number | unsigned long / char[24] / byte


//   00 -> 03 | biggest highscore (unsigned long = 4 bytes)

//   04 -> 07 | second  highscore (-||-)

//   08 -> 11 | last highscore (-||-)


//   12 -> 35 | biggest highscore name (char[24] = 24 bytes)

//   36 -> 59 | second highscore name (-||-)

//   60 -> 83 | last highscore (-||-)


//   84 -> 84 | lcd contrast (1 byte)

//   85 -> 85 | lcd brightness (-||-)

//   86 -> 86 | matrix brightness (-||-)

*/