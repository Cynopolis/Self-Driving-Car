import serial # PySerial Library
import time

class Move:
    def __init__(self, comPort, speed, direction):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        self.speed = speed
        self.direction = direction
        # give time for the arduino to boot
        time.sleep(1)

    #speed is a floating point number between -1 and 1
    def setSpeed(self, speed):
        self.speed = speed
        if speed < 0:
            speed += 3
        speed += 14

        while(float(self.checkSpeed()) != self.speed):
            self.comm.write(('\r\n%f' % speed).encode())

    #direction is an integer between -45 and 45.
    def setWheelPos(self, direction):
        self.direction = direction
        direction = round(direction / 45)
        if direction < 0:
            direction += 3
        direction += 10
        while(float(self.direction != self.checkWheelPos())):
            self.comm.write(('\r\n%d' % direction).encode())

    def checkSpeed(self):
        try:
            ans = ''
            while ans == '':
                self.comm.write(('\r\n17').encode())
                ans = self.comm.read(100).decode().strip()
            float(ans)
            return ans
        except UnicodeDecodeError:
            return -2

    def checkWheelPos(self):
        try:
            ans = ''
            while ans == '':
                self.comm.write(('\r\n13').encode())
                ans = self.comm.read(100).decode().strip()
            ans = int(ans)
            return ans
        except UnicodeDecodeError:
            return -2