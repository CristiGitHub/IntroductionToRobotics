const int buttonPin = 2;
const int buzzPin = 11;
const int carRedColorPin = 7;
const int carYellowColorPin = 3;
const int carGreenColorPin = 4;
const int passerRedColorPin = 5;
const int passerGreenColorPin = 6;



const int debounceOffset = 1000;
const int colorChangeOffset = 8000;
const int colorTimer[] = {2000,4000,14000,18000};

const int soundCrossingTimer = 500;
const int blinkCrossingTimer = 500;
const int blinkingStartingPhase = 3000;


bool isChangingColor = false;
unsigned long long lastInterruptionCallTime = 0;
unsigned long long lastColorChangeTime = 0;
int buttonState =HIGH;
int curentCarColor = 1;
bool lastTone = false;
unsigned long long lastToneTimer = 0;
unsigned long long fromLastBlinkCrossingTimer = 0;
bool lastBlinkPhase = false;
bool blinkerSwitcher = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(buttonPin,INPUT_PULLUP);
carColorChanger();
attachInterrupt(digitalPinToInterrupt(buttonPin),blink, CHANGE);
}

void loop() {
  if(isChangingColor){
  colorChangerTimer();
  carColorChanger();
  safeCrossingSound();
  }
  if(buttonState == LOW && !isChangingColor)
{
  lastColorChangeTime = millis();
  buttonState = HIGH;
  isChangingColor = true;
}

}


void carColorChanger(){

if(curentCarColor == 3){
  digitalWrite( carRedColorPin,HIGH);
  if(millis()-lastColorChangeTime + blinkingStartingPhase>=colorTimer[2]){
    blinkingCrossing();
  }
  else{
  digitalWrite( passerGreenColorPin , HIGH);
  }
  digitalWrite( passerRedColorPin , LOW);
  digitalWrite( carGreenColorPin,LOW);
  digitalWrite(carYellowColorPin,LOW);
}
if(curentCarColor == 2){
  digitalWrite( carRedColorPin,LOW);
  digitalWrite( carGreenColorPin,LOW);
  digitalWrite(carYellowColorPin,HIGH);
}
if(curentCarColor == 1){
  digitalWrite(carYellowColorPin,LOW);
  digitalWrite( carRedColorPin,LOW);
  digitalWrite( carGreenColorPin,HIGH);
  digitalWrite( passerGreenColorPin , LOW);
  digitalWrite( passerRedColorPin , HIGH);

}
}

void colorChangerTimer(){
  unsigned long long currentCarColorTimer=millis() - lastColorChangeTime;

if( currentCarColorTimer > colorTimer[0] && currentCarColorTimer <= colorTimer[1] && isChangingColor){
  curentCarColor = 2;
}
if( currentCarColorTimer > colorTimer[1] && currentCarColorTimer <= colorTimer[2] && isChangingColor){
  curentCarColor = 3;
}
if( currentCarColorTimer > colorTimer[2] && currentCarColorTimer <= colorTimer[3] && isChangingColor){
  curentCarColor = 1;
}
if( currentCarColorTimer > colorTimer[3] ){
    isChangingColor=false;
}

}


void safeCrossingSound(){
  if(millis()-lastColorChangeTime > colorTimer[1] && millis()-lastColorChangeTime  <= colorTimer[2] && isChangingColor){
  if(!lastTone){
    lastToneTimer = millis();
    lastTone=!lastTone;
    noTone(buzzPin);
  }
    if(lastTone && millis() - lastToneTimer > soundCrossingTimer){
    tone(buzzPin,330);  
    lastTone=!lastTone;
    }
  }
  else{
    noTone(buzzPin);
  }
}
void blinkingCrossing(){
  if(millis()-lastColorChangeTime > colorTimer[1] && millis()-lastColorChangeTime  <= colorTimer[2] && isChangingColor)
    {
      if(!lastBlinkPhase){
      fromLastBlinkCrossingTimer = millis();
      lastBlinkPhase =!lastBlinkPhase;
      blinkerSwitcher= !blinkerSwitcher;    
    }
    if(lastBlinkPhase && millis() - fromLastBlinkCrossingTimer > blinkCrossingTimer){
      digitalWrite( passerGreenColorPin , blinkerSwitcher);
      lastBlinkPhase =!lastBlinkPhase;
    }
  }
  else{
      digitalWrite( passerGreenColorPin , LOW);
  }


}

int debounceDelay = 30;
unsigned long long lastDebounceTime = 0;
bool lastChangableState = true;
bool lastStableState = true;
bool readButtonCurrentState = false;
void blink(){
  if(millis()-lastInterruptionCallTime > debounceOffset && !isChangingColor){
  buttonState=!buttonState;
  lastInterruptionCallTime= millis();}


  readButtonCurrentState = digitalRead(buttonPin);
  if (readButtonCurrentState != lastChangableState) {
    lastDebounceTime = millis();
    lastChangableState = readButtonCurrentState;
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lastStableState == LOW && readButtonCurrentState == HIGH){
      buttonState=!buttonState;
    }
    lastStableState = readButtonCurrentState;
  }
}
