import board
import digitalio
import time

#Setup pins
ELHack = digitalio.DigitalInOut(board.D0)
ELHack.direction = digitalio.Direction.OUTPUT

ELSpace = digitalio.DigitalInOut(board.D1)
ELSpace.direction = digitalio.Direction.OUTPUT

sensor = digitalio.DigitalInOut(board.D2)
sensor.direction = digitalio.Direction.INPUT

#Monitoring for movement
while True:
    if sensor.value == True:
        #monotonic increments in seconds on the Trinket
        tstart = time.monotonic()

        ELHack.value = True
        ELSpace.value = True

        #On for 10mins or until next motion triggers
        while time.monotonic() - tstart < (10*60):
            time.sleep(1)

    #Turn off if no motion or keep on if still movement
    ELHack.value = sensor.value
    ELSpace.value = ELHack.value