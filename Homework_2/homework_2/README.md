#Homework 2 Semaphore
<br>
https://drive.google.com/file/d/1GEbm5Rj8y8VNhvnDo4DRF-zrUhmYGQZX/view?usp=share_link

### Homework 2

I created a simple function which will take the input from the button and i am using it as a Pull up button with a debounce function which will filter if it a real press or not . Also  i created a function for each state which will display the current color and also it will send the signal for the buzzer

**Components**:  5 LEDs, 1 button, 1 buzzer, resistors and wires (per logic)

**General  description:**Building  the  traffic  lights  for  a  crosswalk.   Youwill use 2 LEDs to represent the traffic lights for people (red and green)and 3 LEDs to represent the traffic lights for cars (red, yellow and green).See the states it needs to go through.  If anything is not clear, ask.  Also,see the uploaded video (the intervals are different, but the states flow isthe same).  It’s a traffic lights system for people and cars - don’t overthinkit.

#### Task requirements
  - **State 1**:(default, reinstated after state 4 ends):  green light for cars,red  light  for  people,  no  sounds.   Duration:  indefinite,  changed  bypressing the button.
  - **State 2**: (initiated by counting down 8 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration:  3 seconds.
  - **State 3**: (initiated after state 2 ends):  red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.
  - **State 4**: initiated after state 3 ends):  red for cars,blinking greenfor people and a beeping sound from the buzzer,  at a constant in-terval,  faster than the beeping in state 3.  This state should last 4seconds.
