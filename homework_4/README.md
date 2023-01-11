# Homework 4


![Alt text](IMG_0042.JPG?raw=true "Circuit Board")

https://www.youtube.com/shorts/OvbDtjponro



### Homework #4

-**Components**:a  joystick,  a  4  digit  7-segment  display,  a  74hc595  shift register


-**General Description**: Use the joystick to move through the 4 digit 7segment displays digits, press the button to lock in on the current digitand use the other axis to increment or decrement the number.  Keep thebutton pressed to reset all the digit values and the current position to thefirst digit in the first state.


Use the joystick to move through the 4 digit 7segment displays digits, press the button to lock in on the current digitand use the other axis to increment or decrement the number.  Keep thebutton pressed to reset all the digit values and the current position to thefirst digit in the first state.
- **First state**:you can use a joystick axis to cycle through the 4 digits;using the other axis does nothing.  A blinking decimal point showsthe current digit position.  When pressing the button, you lock in onthe selected digit and enter the second state.
- **Second state**:in this state, the decimal point stays always on, nolonger blinking and you can no longer use the axis to cycle throughthe  4  digits.   Instead,  using  the  other  axis,  you  can  increment  ondecrement  the  number  on  the  current  digit  IN  HEX  (aka  from  0to F, as in the lab).  Pressing the button again returns you to theprevious state.  Also, keep in mind that when changing the number,you  must  increment  it  for  each  joystick  movement  -  it  should  notwork continuosly increment if you keep the joystick in one position(aka with joyMoved).
- **Reset**:  toggled by long pressing the buttononly in the first state.When resetting, all the digits go back to 0 and the current positionis set to the first (rightmost) digit, in the first state.
