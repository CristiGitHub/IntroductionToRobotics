#include "LedControl.h"  //  need the library
#include <LiquidCrystal.h>
#include <EEPROM.h>
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;  // A0 - analog pin connected to X output
const int pinY = A1;  // A1 - analog pin connected to Y output

const int xPin = A0;
const int yPin = A1;
const int lcdBrightnessPin = 3;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
byte matrixBrightness = 2;

byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;

int debounceDelay = 30;
bool lastChangableState = true;
bool lastStableState = true;
bool commandInUse = false;
unsigned long long lastDebounceTime = 0;
const int resetTimePress = 1000;
bool blinkSwitcherDp = false;
unsigned long long lastBlinkDp = 0;
const int blinkDelayDp = 300;

const int minThreshold = 200;
const int maxThreshold = 600;

const byte moveInterval = 100;
unsigned long long lastMoved = 0;
int lastMeniuOption = -1;

const byte matrixSize = 8;
bool matrixChanged = true;

bool isFoodEaten =true;

byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

int currentMenuPosition = 0;
int currentPhase = -3;
bool backMenuPressed = false;
const int maxValue = 16;
const int minValue = 0;
int currentSettings[] = { 0, 0, 0, 0 };
int settingAdjustmentVariable = 0;
int lastDisplayedScore=-2;
int currentScore =-1;

void setup() {
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);    
    pinMode(pinSW, INPUT_PULLUP);
  pinMode(lcdBrightnessPin , OUTPUT);                
  lcd.begin(16, 2);
  analogWrite(lcdBrightnessPin, 150);
  attachInterrupt(digitalPinToInterrupt(pinSW), blink, CHANGE);
  //  lcd.print("hello, world!");
  matrix[xPos][yPos] = 1;
}
void loop() {
  // TODO: update positions in an interval
  if (currentPhase != 5) {
    //Serial.println(currentMenuPosition);
    displayCurrentMenu();
  }
  // lcd.setCursor(0,1);
  // print the number of seconds since reset:
  //lcd.print(millis() /1000);
  if (currentPhase == 5) {
    
    if(isFoodEaten){
      generateFood();
      currentScore+=1;
      isFoodEaten=false;
    }

    if(currentScore!=lastDisplayedScore){
      displayCurrentScore();
      lastDisplayedScore=currentScore;
    }

    if (millis() - lastMoved > moveInterval) {
      updatePositions();  // calculare stare
    }

    if (matrixChanged == true) {
      updateMatrix();
      matrixChanged = false;
    }
  }
  // TODO: update matrix when broadcasted that it changed
}

byte customChar[] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};

const String welcomeScreen[] = {"Welcome to my", "Snake"};
const String howToUseContinueScreen[] = {"Short click", "for continue"};
const String howToUseBackScreen[]={"Long click","for back"};
const String menuOptionsString[] = { "New Game", "Score Board", "Settings", "About", "How to play" };
const String settingsMenu[] = { "Enter Name", "Starting Level", "LCD contrast", "LCD brightness", "Matrix brightness", "Sounds" };
String scorBoardMenu[] = { "eu 1000", "eu 10", "eu 9", "eu 8", "eu 7", "eu 6" };
const String aboutText[] = { "Salut eu sunt", "Lefter Ioan", "Cristian si", "asta e Snake-ul", "meu enjoy" };
const String howToPlayText[] = { "Use the Joytick", "To Move and", "try to collect", "the food", "to incress the", "size" };
const int sizeWelcomeScreen =2;
const int sizeHowToUseContinueScreen = 2;
const int sizeHowToUseBackScreen = 2;
const int sizeMenuOption = 5;
const int sizeSettingsMenu = 6;
const int sizeScorBoardMenu = 6;
const int sizeAbout = 5;
const int sizeHowToPlay = 6;

bool inSettingsAdjustment = false;
bool navigationMoved = false;

int genRow=0;
int genCol=0;

void displayCurrentScore(){

  lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.createChar(0, customChar);
    lcd.print("  Current Score:");
    lcd.setCursor(0, 1);
    lcd.print("    ");
    lcd.print(currentScore);
}

void displayCurrentMenu() {
   if (currentPhase == -3) {
    menuDisplay(welcomeScreen, sizeWelcomeScreen);
  }
   if (currentPhase == -2) {
    menuDisplay(howToUseContinueScreen, sizeHowToUseContinueScreen);
  }
   if (currentPhase == -1) {
    menuDisplay(howToUseBackScreen, sizeHowToUseBackScreen);
  }
  if (currentPhase == 0) {
    menuDisplay(menuOptionsString, sizeMenuOption);
  }
  if (currentPhase == 1) {
    menuDisplay(scorBoardMenu, sizeScorBoardMenu);
  }
  if (currentPhase == 2) {
    if (!inSettingsAdjustment) {
      menuDisplay(settingsMenu, sizeSettingsMenu);
    } else {
      Serial.println(currentSettings[0]);
      genericSettings();
    }
  }
  if (currentPhase == 3) {
    menuDisplay(aboutText, sizeAbout);
  }
  if (currentPhase == 4) {
    menuDisplay(howToPlayText, sizeHowToPlay);
  }
}


void menuDisplay(String options[], int size) {
  navigateFunction(size);
  if (lastMeniuOption != currentMenuPosition || backMenuPressed) {
    lcd.clear();
    backMenuPressed = false;
    lcd.setCursor(0, 0);
    //lcd.createChar(0, customChar);
    lcd.print('>' + options[currentMenuPosition]);
    lastMeniuOption = currentMenuPosition;
    if (currentMenuPosition + 1 < size) {
      lcd.setCursor(0, 1);
      lcd.print(' ' + options[currentMenuPosition + 1]);
    }
  }
}


void genericSettings() {
 navigateFunction(maxValue);
  if (lastMeniuOption != currentMenuPosition ) {
    lcd.clear();
    backMenuPressed = false;
    lcd.setCursor(0, 0);
    lcd.print("> ");
  lcd.print(currentMenuPosition);
    lastMeniuOption = currentMenuPosition;
    if (currentMenuPosition - 1 >=0 ) {
      lcd.setCursor(0, 1);
      lcd.print(currentMenuPosition - 1);
    }
  }
}

void updateSettings(){
  analogWrite(lcdBrightnessPin,map(currentSettings[2],0,16,70,250));
  lc.setIntensity(0, currentSettings[3]);  
}

// theoretical example
void generateFood() {
  genRow = random(matrixSize);
  genCol = random(matrixSize);
  matrix[genRow][genCol] = 1;
  updateMatrix();
  matrixChanged = true;
}

void updateMatrix() {
  // TODO: update matrix using byte matrix (refresh function)
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      lc.setLed(0, i, j, matrix[i][j]);
    }
  }
}


// void readStringArray(String *array, int size, int eepromAddress){
// for(int i = 0; i < size; i++){
// //Read from EEPROM
// array[i] = EEPROM.readString(eepromAddress);
// //Update EEPROM address
// eepromAddress += array[i].length() + 1; //+1 for the null terminator
// }
// }
// //Write to EEPROM
// void writeStringArray(String *array, int size, int eepromAddress){
// for(int i = 0; i < size; i++){
// EEPROM.writeString(eepromAddress, array[i]);
// //Update EEPROM address
// eepromAddress += array[i].length() + 1; //+1 for the null terminator
// }
// }


void updatePositions() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);


 if(genCol== yPos && genRow==xPos){
    matrix[genRow][genCol] = 0;
    matrixChanged = true;
    isFoodEaten=true;
  }

  xLastPos = xPos;
  yLastPos = yPos;

if (xValue < minThreshold) {
    if (xPos > 0) {
      xPos--;
    } else {
      xPos = 7;
    }
  }

  if (xValue > maxThreshold) {
    if(xPos < matrixSize - 1)
    {xPos++;}
    else{

      xPos = 0;
    }
  }

  if (yValue > maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } else {
      yPos = 0;
    }
  }

  if (yValue < minThreshold) {
    if (yPos > 0) {
      yPos--;
    } else {
      yPos = 7;
    }
  }

  if (xLastPos != xPos || yLastPos != yPos) {
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
    matrixChanged = true;
    lastMoved = millis();
  }
}

void navigateFunction(int size) {
  int yValue = analogRead(yPin);

  if (yValue > maxThreshold && !navigationMoved) {
    navigationMoved = !navigationMoved;
    if (currentMenuPosition < size - 1) {
      currentMenuPosition++;
      lcd.clear();
    } else {
      currentMenuPosition = 0;
      lcd.clear();
    }
  } else {
    if (yValue < minThreshold && !navigationMoved) {
      navigationMoved = !navigationMoved;
      if (currentMenuPosition > 0) {
        currentMenuPosition--;
        lcd.clear();

      } else {
        currentMenuPosition = size - 1;
        lcd.clear();
      }
    } else {
      if (navigationMoved && yValue > minThreshold && yValue < maxThreshold) {
        navigationMoved = !navigationMoved;
      }
    }
  }
}

void clickMenu() {
  if (currentPhase == -3 ) {
    currentPhase = -2;
    currentMenuPosition = 0;
    lastMeniuOption=-1;
  }
  else
  if (currentPhase == -2 ) {
    currentPhase = -1;
    currentMenuPosition = 0;
    lastMeniuOption=-1;
  }
  else
 if (currentPhase == -1 ) {
    currentPhase = 0;
    currentMenuPosition = 0;
    lastMeniuOption=-1;
  }
  else
  if (currentPhase == 0 && currentMenuPosition == 0) {
    currentPhase = 5;
    updateMatrix();
    currentMenuPosition = 0;
  }
  else
  if (currentPhase == 0 && currentMenuPosition == 1) {
    currentPhase = currentMenuPosition;
    currentMenuPosition = 0;
  }
  else
  if (currentPhase == 0 && currentMenuPosition == 2) {
    currentPhase = currentMenuPosition;
    currentMenuPosition = 0;
  }
  else
  if (currentPhase == 0 && currentMenuPosition == 3) {
    currentPhase = currentMenuPosition;
    currentMenuPosition = 0;
  }
  else
  if (currentPhase == 0 && currentMenuPosition == 4) {
    currentPhase = currentMenuPosition;
    currentMenuPosition = 0;
  }
  else
  if (currentPhase == 2 && currentMenuPosition >= 1 && currentMenuPosition <= 4 && !inSettingsAdjustment) {
    settingAdjustmentVariable = currentMenuPosition - 1;
    currentMenuPosition = currentSettings[currentMenuPosition - 1];
    inSettingsAdjustment = true;
    lcd.clear();
  }
  else
 { if (currentPhase == 2 && inSettingsAdjustment) {
    currentSettings[settingAdjustmentVariable] = currentMenuPosition;
    updateSettings();
  }}
  displayCurrentMenu();
}

void backPress() {
  if(currentPhase<=0 && currentPhase >-3){
  currentPhase+=-1;
  currentMenuPosition = 0;
  }
  if (currentPhase == 1 || currentPhase == 2 || currentPhase == 3 || currentPhase == 4) {
    if (inSettingsAdjustment && currentPhase == 2) {
      currentMenuPosition = 0;
      inSettingsAdjustment = false;
      lcd.clear();
    } else {
      currentPhase = 0;
      currentMenuPosition = 0;
    }
  }
  if(currentPhase==5){
    currentPhase=0;
    currentMenuPosition=0;
  lc.clearDisplay(0);    
  }
  backMenuPressed = true;
  displayCurrentMenu();
}

void blink() {
  commandInUse = false;
  bool readButtonCurrentState = digitalRead(pinSW);
  if (readButtonCurrentState != lastChangableState) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (lastStableState == LOW && readButtonCurrentState == HIGH) {
        if (millis() - lastDebounceTime > resetTimePress) {
          backPress();
        } else {
          clickMenu();
        }
        commandInUse = true;
      }
      lastStableState = readButtonCurrentState;
    }
    lastDebounceTime = millis();
    lastChangableState = readButtonCurrentState;
  }

  if ((millis() - lastDebounceTime) > debounceDelay && !commandInUse) {
    if (lastStableState == LOW && readButtonCurrentState == HIGH) {
      if (millis() - lastDebounceTime > resetTimePress) {
        backPress();
      } else {
        clickMenu();
      }
    }
    lastStableState = readButtonCurrentState;
  }
}