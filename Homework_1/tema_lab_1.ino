const int redDial = A0;
const int blueDial = A1;
const int greenDial = A2;
const int redWritePin = 9;
const int blueWritePin = 10;
const int greenWritePin = 11;
const int writeOffset = 10;
const int lowPassFilter = 20;

int redValue=0;
int greenValue=0;
int blueValue=0;
bool isLowPassFilterTriggerd = false;

int lastRedValue = 1023;
int lastBlueValue =1023;
int lastGreenValue = 1023;
int lastSumValue = 3069;
int currentSumValue =0;



void setup() {
    Serial.begin(9600);
    pinMode(redDial,INPUT);
    pinMode(blueDial,INPUT);
    pinMode(greenDial,INPUT);
    pinMode(redWritePin,OUTPUT);
    pinMode(blueWritePin,OUTPUT);
    pinMode(greenWritePin,OUTPUT);
    loadLastPreset();
}
// change the voltage for each color if the differce between the current one and the last one is bigger than an offset
//
void loop() {
  redValue=analogRead(redDial);
  blueValue=analogRead(blueDial);
  greenValue=analogRead(greenDial);
  lowPassFileter();
  currentSumValue= redValue + blueValue + greenValue;
    if(abs(currentSumValue-lastSumValue)>=writeOffset || isLowPassFilterTriggerd )
    {
      if(abs(lastRedValue-redValue) >=writeOffset || isLowPassFilterTriggerd ){
          analogWrite(redWritePin,toVoltage(redValue));
          lastRedValue = redValue;
      }
      if(abs(lastBlueValue-blueValue) >= writeOffset || isLowPassFilterTriggerd ){
          analogWrite(blueWritePin,toVoltage(blueValue));  
          lastBlueValue = blueValue;
      }
      if(abs(lastGreenValue-greenValue) >= writeOffset || isLowPassFilterTriggerd ){
          analogWrite(greenWritePin,toVoltage(greenValue));  
          lastGreenValue = greenValue;
      }
      isLowPassFilterTriggerd = false;
      lastSumValue = lastGreenValue + lastBlueValue + lastRedValue;
    }
}


// Loads the last Color preset 
void loadLastPreset(){
  lastRedValue = redValue=analogRead(redDial);
  lastBlueValue = blueValue=analogRead(blueDial);
  lastGreenValue = greenValue=analogRead(greenDial);
  lastSumValue = lastGreenValue + lastBlueValue + lastRedValue;
  analogWrite(redWritePin,toVoltage(redValue));
  analogWrite(blueWritePin,toVoltage(blueValue));  
  analogWrite(greenWritePin,toVoltage(greenValue));  
}

void lowPassFileter()
{
  if( redValue < lowPassFilter && redValue != 0 ){
    redValue = 0;
    isLowPassFilterTriggerd = true;
  }
   if( blueValue < lowPassFilter && blueValue != 0 ){
    blueValue = 0;
    isLowPassFilterTriggerd = true;
  }  
   if( greenValue < lowPassFilter && greenValue != 0 ){
    greenValue = 0;
    isLowPassFilterTriggerd = true;
  }
}

int toVoltage(int value) {
  int result = 0.0;
  result = map(value, 0 , 1023 , 0 , 255);
  return result;
}
