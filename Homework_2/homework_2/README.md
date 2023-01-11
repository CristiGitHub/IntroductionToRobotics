#Homework 2 Semaphore
<br>
https://drive.google.com/file/d/1GEbm5Rj8y8VNhvnDo4DRF-zrUhmYGQZX/view?usp=share_link

### Homework 2

**Components**:  5 LEDs, 1 button, 1 buzzer, resistors and wires (per logic)

#### Task requirements
  - **State 1**:(default, reinstated after state 4 ends):  green light for cars,red  light  for  people,  no  sounds.   Duration:  indefinite,  changed  bypressing the button.
  - **State 2**: (initiated by counting down 8 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration:  3 seconds.
  - **State 3**: (initiated after state 2 ends):  red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.
  - **State 4**: initiated after state 3 ends):  red for cars,blinking greenfor people and a beeping sound from the buzzer,  at a constant in-terval,  faster than the beeping in state 3.  This state should last 4seconds.
