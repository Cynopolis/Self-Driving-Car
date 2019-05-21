import serial # PySerial Library
import time

class Move:
    def __init__(self, comPort, speed, direction):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        self.speed = speed
        self.direction = direction
        # give time for the arduino to boot
        time.sleep(1)
        # flush the serial buffer
        self.comm.write(('\r\n0').encode())
        self.comm.read(100).decode().strip()

    #speed is a floating point number between -1 and 1
    def setSpeed(self, speed):
        self.speed = speed
        if speed < 0:
            speed += 3
        speed += 14

        #for i in range(2):
        self.comm.write(('\r\n%f' % speed).encode())
        str = self.comm.read(100).decode().strip()
        print(str)

    #direction is an integer between -45 and 45.
    def setWheelPos(self, direction):
        self.direction = direction
        round(direction / 45)
        if direction < 0:
            direction += 3
        direction += 10
        self.comm.write(('\r\n%d' % direction).encode())

    def checkSpeed(self):
        self.comm.write(('\r\n17').encode())
        try:
            ans = float(self.comm.read(100).decode().strip())
            return ans
        except UnicodeDecodeError:
            return -2

    def checkWheelPos(self):
        self.comm.write(('\r\n13').encode())
        try:
            ans = self.comm.read(100).decode().strip()
            print(ans)
            ans = int(ans)
            return ans
        except UnicodeDecodeError:
            return -2