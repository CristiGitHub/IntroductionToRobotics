// declare all the joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int segSize = 8;

bool buttonState = false; 
unsigned long long lastDebounceTime = 0;

bool readButtonCurrentState = false;
int index = 0;

unsigned long long lastTimerFromBlink = 0;
const int blinkOffset = 500;
const int resetTimePress = 2000;
bool blinkState = true;
// modify if you have common anode
bool commonAnode = false; 

int debounceDelay = 30;
bool lastChangableState = true;
bool lastStableState = true;
bool commandInUse = false;

const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
byte swState = LOW;

byte lastSwState = LOW;
int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;

int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int nextPosition = 0;
int currentSegmentPosition = pinA;

int segmentsNeighboars[segSize][5]={
  {0   ,  pinG, pinF, pinB ,0},
  {pinA,  pinG, pinF, 0    ,0},
  {pinG,  pinD, pinE, pinDP , 0},
  {pinG,  0   , pinE, pinC , 0},
  {pinG,  pinD, 0,    pinC , 0},
  {pinA,  pinG, 0,    pinB , 0},
  {pinA,  pinD, 0,  0 , 0},
  {0 , 0 , pinC , 0 , 0}
};

void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode == true) {
    state = !state;
  }
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pinSW),blink, CHANGE);
}

void loop() {
  nextPosition = -1;
//Serial.println(currentSegmentPosition);
for(int i = 0 ; i < segSize ; ++i){
  if(i+4 !=currentSegmentPosition){
    digitalWrite(i+4,segmentsNeighboars[i][4]);
  }
}
blinkSegmentSelector();
xValueInput();
yValueInput();
if(nextPosition!=-1){
  if(segmentsNeighboars[currentSegmentPosition-4][nextPosition]>0){
  currentSegmentPosition=segmentsNeighboars[currentSegmentPosition-4][nextPosition];
  }
}
} 




void blinkSegmentSelector(){
  if(millis()-lastTimerFromBlink>blinkOffset){
  digitalWrite(currentSegmentPosition,blinkState);
  blinkState=!blinkState;
  lastTimerFromBlink=millis();
  }
}

void xValueInput(){
  xValue = analogRead(pinX);
  if ( minThreshold <= xValue && xValue <= maxThreshold) {
    joyMovedX = false;
  }

  if (xValue > maxThreshold && joyMovedX == false) {
    nextPosition=0;
    joyMovedX = true;
  }

  if (xValue < minThreshold && joyMovedX == false) {
    nextPosition=1;
    joyMovedX = true;
  }
}

void yValueInput(){
  yValue = analogRead(pinY);
  if ( minThreshold <= yValue && yValue <= maxThreshold) {
    joyMovedY = false;
  }

  if (yValue > maxThreshold && joyMovedY == false) {
    nextPosition=3;
    joyMovedY = true;
  }

  if (yValue < minThreshold && joyMovedY == false) {
   nextPosition=2;
    joyMovedY = true;
  }
}


void lightUpSegmnet(){
  Serial.println("salut");
  segmentsNeighboars[currentSegmentPosition-4][4]=(segmentsNeighboars[currentSegmentPosition-4][4]+1)%2;
}

void resetLightState(){
  for (int i=0; i<segSize; i++) {
    digitalWrite(i+4,0);
    segmentsNeighboars[i][4]=0;
  }
}


void blink(){
  commandInUse = false;
  readButtonCurrentState = digitalRead(pinSW);
  if (readButtonCurrentState != lastChangableState) {
     if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lastStableState == LOW && readButtonCurrentState == HIGH){
      if(millis() -lastDebounceTime >  resetTimePress){
        resetLightState();
      }
      else
      {
        lightUpSegmnet();
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
        lightUpSegmnet();
      }
    }
    lastStableState = readButtonCurrentState;
  }
}

