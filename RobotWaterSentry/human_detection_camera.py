#!/usr/bin/env python3
# Based on the work of Google Inc. 2017
# https://github.com/google/aiyprojects-raspbian/blob/aiyprojects/src/aiy/vision/models/

"""Camera inference human detection demo code.

Runs continuous human detection on the VisionBonnet 

Example:
human_detection_camera.py 

Ref:
https://github.com/google/aiyprojects-raspbian/blob/aiyprojects/src/aiy/vision/models/object_detection.py

"""

from aiy.vision.inference import CameraInference
from aiy.vision.models import object_detection

from aiy.vision.leds import Leds
from aiy.vision.leds import Pattern
from aiy.vision.pins import PIN_A

from picamera import PiCamera

import time

RED = (0xFF, 0x00, 0x00)

leds = Leds()

def squirt (position):
    angle = math.degrees(math.atan((position/2.9) * 2.5))
    bus.write_byte_data(slaveAddress , 2, angle )
    time.sleep(0.1)
   
    while bus.read_word_data (slaveAddress, 1) != 0:
        time.sleep(0.1)

    leds.update(Leds.rgb_on(RED))
    solenoid.on()
    time.sleep(1)
    solenoid.off()
    leds.update(Leds.rgb_pattern(RED))

def main():

    print('Human detection')

    # Turn on the LED so we know the box is ready
    leds.pattern = Pattern.breathe(1000)
    leds.update(Leds.rgb_pattern(RED))

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    with PiCamera() as camera:
        # Forced sensor mode, 1640x1232, full FoV. See:
        # https://picamera.readthedocs.io/en/release-1.13/fov.html#sensor-modes
        # This is the resolution inference run on.
        camera.sensor_mode = 4
        # Set camera to match
        camera.resolution = (1640, 1232)
        camera.framerate = 30

        with CameraInference(object_detection.model()) as inference:
            for i, result in enumerate(inference.run()):
                for i, obj in enumerate(object_detection.get_objects(result, 0.3)):
                    if  obj.score > 0.7 and obj.kind == 1:   # Person
                        print('Human detected #%d: %s' % (i, str(obj)))
                        x, y, width, height = obj.bounding_box
                        squirt((x + (width/2) - (1640 / 2 )) / 1640)


if __name__ == '__main__':
    main()
