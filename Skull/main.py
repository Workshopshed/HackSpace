import time
import board
import touchio
from digitalio import DigitalInOut, Direction

led1 = DigitalInOut(board.D0)
led1.direction = Direction.OUTPUT
led2 = DigitalInOut(board.D2)
led2.direction = Direction.OUTPUT

touch_pad = board.A0 
touch = touchio.TouchIn(touch_pad)

def detected():
    for i in range(3):
            led1.value = True
            led2.value = True
            time.sleep(0.5)
            led1.value = False
            led2.value = False
            time.sleep(0.2)

detected()

while True:
   if touch.value:
       detected()
   time.sleep(0.05)