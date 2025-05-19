import RPi.GPIO as GPIO
import time


# GPIO Pin Setup RED = 17 GREEN = 4 BLUE = 27
RED = 17
GREEN = 22
BLUE = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(RED, GPIO.OUT)
GPIO.setup(GREEN, GPIO.OUT)
GPIO.setup(BLUE, GPIO.OUT)

# Function to set RGB color
def set_color(r, g, b):
    GPIO.output(RED, r)
    GPIO.output(GREEN, g)
    GPIO.output(BLUE, b)

try:
    while True:
        set_color(1, 0, 0)  # Red ON
        time.sleep(1)

        set_color(0, 1, 0)  # Green ON
        time.sleep(1)

        set_color(0, 0, 1)  # Blue ON
        time.sleep(1)

        set_color(1, 1, 1)  # weiss ON
        time.sleep(1)

        set_color(1, 1, 0)  # gelb ON
        time.sleep(1)
        set_color(0, 1, 1)  # cyan ON
        time.sleep(1)
        set_color(1, 0, 1)  # pink ON
        time.sleep(1)

        set_color(0, 0, 0)  # 9 OFF
        time.sleep(1)

except KeyboardInterrupt:
    GPIO.cleanup()
