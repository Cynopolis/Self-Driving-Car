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
        speed += 15
        ans = ''
        while ans != ('%f' % speed):
            self.comm.write(('\r\n%f' % speed).encode())
            ans = self.comm.readline().decode().strip()

    #direction is an integer between -30 and 30.
    def setWheelPos(self, direction):
        self.direction = direction
        direction = direction / 30
        direction += 11
        ans = ''
        # keep sending messages to the arduino until it acknowledges you
        while ans != ('%f' % direction):
            self.comm.write(('\r\n%d' % direction).encode())
            ans = self.comm.readline().decode().strip()

    def checkSpeed(self):
        try:
            ans = ''
            while ans != '17':
                self.comm.write(('\r\n17').encode())
                ans = self.comm.readline().decode().strip()
            float(ans)
            return ans
        except UnicodeDecodeError:
            return -2

    def checkWheelPos(self):
        try:
            ans = ''
            while ans != '13':
                self.comm.write(('\r\n13').encode())
                ans = self.comm.readline().decode().strip()
            ans = int(ans)
            return ans
        except UnicodeDecodeError:
            return -2

    #this class is currently untested because at the time of writting I didn't have an arduino. Try it out and see if it works
    def writeToArduino(self, message):
        str = '' + message
        ans = ''
        while ans != message:
            self.comm.write(('\r\n'+message).encode())
            ans = self.comm.readline().decode().strip()
        return ans