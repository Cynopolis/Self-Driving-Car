import serial # PySerial Library
import time

class Move:
    def __init__(self, comPort, speed, direction):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        self.speed = speed
        self.direction = direction
        # give time for the arduino to boot
        ans = ''
        while ans != 'ready':
            ans = self.comm.readline().decode().strip()
            time.sleep(0.1)
            print(ans)

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
        while ans != 'ready':
            ans = self.comm.readline().decode().strip()
            time.sleep(0.1)

    def checkSpeed(self):
        try:
            ans = ''
            while ans != '17':
                self.comm.write(('\r\n17').encode())
                ans = self.comm.readline().decode().strip()
            ans = self.comm.readline().decode().strip()
            float(ans)
            return ans
        except UnicodeDecodeError:
            return -2

    def checkWheelPos(self):
        ans = ''
        while ans != '13':
            self.comm.write(('\r\n13').encode())
            ans = self.comm.read(2).decode().strip()
        ans = self.comm.readline().decode().strip()
        ans = float(ans)
        return ans

    #this class is currently untested because at the time of writting I didn't have an arduino. Try it out and see if it works
    def writeToArduino(self, message):
        ans = ''
        message = str(message)
        while ans != message:
            self.comm.write(('\r\n'+message).encode())
            ans = self.comm.readline().decode().strip()
            print(ans)
        ans = self.comm.readline().decode().strip()
        return ans

    def callibrateWheel(self):
        print(self.writeToArduino(1))
        ans = ''
        while ans != 'ready':
            ans = self.comm.readline().decode().strip()
            time.sleep(0.1)
            print(ans)