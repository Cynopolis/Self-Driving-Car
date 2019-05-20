import serial # PySerial Library

class Move:
    def __init__(self, comPort, speed, direction):
        self.comm = serial.Serial(comPort, 9600, timeout=.3)
        self.speed = speed
        self.direction = direction

    #speed is a floating point number between -1 and 1
    def setSpeed(self, speed):
        self.speed = speed
        if speed < 0:
            speed += 3
        speed += 14
        self.comm.write(('\r\n%f' % speed).encode())

    #direction is an integer between -45 and 45.
    def steer(self, direction):
        self.direction = direction
        round(direction / 45)
        if direction < 0:
            direction += 3
        direction += 10
        self.comm.write(('\r\n%d' % direction).encode())

    #todo have checkspeed return a float for the current speed
    def checkSpeed(self):
        for i in range(2):
            self.comm.write(('\r\n17').encode())
            try:
                str = self.comm.read(100).decode()
            except UnicodeDecodeError:
                str = "Recieve failed"
            print(str)

    # todo have getWheelPos return an int for the current wheel position
    def getWheelPos(self):
        self.comm.write(('\r\n13').encode())
        str = self.comm.readline()
        print(str)