# Homework 3  7 -Segment Display

https://drive.google.com/file/d/1e6mbJDclXZRgu890jJ8H6zs3UoJFygpE/view?usp=share_link

![Alt text](media/poza.jpeg?raw=true "Circuit Board")

**Components**:  1  7-segment  display,  1  joystick,  resistors  and  wires  (perlogic)

General description:You will use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segmentsshould be natural (meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
### The system has the following states:
**State 1**:  1(default,  but  also  initiated  after  a  button  press  in  State2):  Current  position  blinking.   Can  use  the  joystick  to  move  fromone  position  to  neighbors.   Short  pressing  the  button  toggles  state2.  Long pressing the button in state 1 resets the entire display byturning all the segments OFF and moving the current position to thedecimal point.<br>
**State 2**:  2(initiated  after  a  button  press  in  State  1):   The  currentsegment  stops  blinking,  adopting  the  state  of  the  segment  beforeselection (ON or OFF). Toggling the X (or Y, you chose) axis shouldchange  the  segment  state  from  ON  to  OFF  or  from  OFF  to  ON.Clicking the joystick should save the segment state and exit back tostate 1.
