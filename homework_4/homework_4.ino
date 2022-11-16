// Aprindere secventiala cifre
//DS= [D]ata [S]torage - data
//STCP= [ST]orage [C]lock [P]in latch
//SHCP= [SH]ift register [C]lock [P]in clock

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

bool joyMovedX = false;
bool joyMovedY = false;

int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

int debounceDelay = 30;
bool lastChangableState = true;
bool lastStableState = true;
bool commandInUse = false;
unsigned long long lastDebounceTime = 0;
const int resetTimePress = 3000;
bool blinkSwitcherDp = false;
unsigned long long lastBlinkDp = 0;
const int blinkDelayDp = 300;

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

int numbersToDisplay[]{
  0 , 0 , 0 , 0
};
int curentPosition = 0;
int numberToDisplaySize = 4;
const int displayCount = 4;

byte registers[regSize];
const int encodingsNumber = 16;



byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); //
  pinMode(dataPin, OUTPUT); //ds

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW),blink, CHANGE);
  Serial.begin(9600);
}

bool changeStateActive = false;
int currentDigit = 0;


void loop() {
if(changeStateActive){
  changeNumber();
yValueInput();
}
else{
  xValueInput(); 
   writeNumber();
}
}

void xValueInput(){
  int xValue = analogRead(pinX);
  if ( minThreshold <= xValue && xValue <= maxThreshold) {
    joyMovedX = false;
  }

  if (xValue > maxThreshold && joyMovedX == false) {
    if(curentPosition-1>-1){
    curentPosition+=-1;
   }
    joyMovedX = true;
  }

  if (xValue < minThreshold && joyMovedX == false) {
 if(curentPosition+1<4){
    curentPosition+=1;
   }
    joyMovedX = true;
  }
}

void yValueInput(){
 int yValue = analogRead(pinY);
  if ( minThreshold <= yValue && yValue <= maxThreshold) {
    joyMovedY = false;
  }

  if (yValue > maxThreshold && joyMovedY == false) {
    if(numbersToDisplay[curentPosition]+1 <16){
      numbersToDisplay[curentPosition]+=1;
      Serial.println("+1");
    }
    joyMovedY = true;
  }

  if (yValue < minThreshold && joyMovedY == false) {
    if(numbersToDisplay[curentPosition]-1 >-1){
      numbersToDisplay[curentPosition]+=-1;
      Serial.println("-1");
    }
    joyMovedY = true;
  }
}


void changeNumber(){
  activateDisplay(curentPosition);
  writeReg(byteEncodings[numbersToDisplay[curentPosition]]);
}

void writeNumber() {
  for(int i = 0 ; i < numberToDisplaySize ; ++i)
    {
     activateDisplay(i);
     if(curentPosition == i){
        writeReg(byteEncodings[numbersToDisplay[i]]+blinkSwitcherDp);
        if(millis()-lastBlinkDp>blinkDelayDp){
          blinkSwitcherDp=!blinkSwitcherDp;
          lastBlinkDp=millis();
        }
     }
     else
     {
       writeReg(byteEncodings[numbersToDisplay[i]]);
     }
     delay(5);
    
   }
}

void activateDisplay(int displayNumber) {

  for (auto display:displayDigits) {
    digitalWrite(display, HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeReg(byte encoding) {
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void resetLightState(){
  for (int i=0; i<numberToDisplaySize; i++) {
    numbersToDisplay[i]=0;
    curentPosition = 0;
  }
}

void blink(){
  commandInUse = false;
  bool readButtonCurrentState = digitalRead(pinSW);
  if (readButtonCurrentState != lastChangableState) {
     if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lastStableState == LOW && readButtonCurrentState == HIGH){
      if(millis() -lastDebounceTime >  resetTimePress){
        resetLightState();
      }
      else
      {
        changeStateActive=!changeStateActive;
      }
      commandInUse =true;
    }
    lastStableState = readButtonCurrentState;
  }
    lastDebounceTime = millis();
    lastChangableState = readButtonCurrentState;
  }

      if ((millis() - lastDebounceTime) > debounceDelay && !commandInUse) {
    if (lastStableState == LOW && readButtonCurrentState == HIGH){
      if(millis() -lastDebounceTime >  resetTimePress){
        resetLightState();
      }
      else
      {
        changeStateActive=!changeStateActive;
      }
    }
    lastStableState = readButtonCurrentState;
  }
}
