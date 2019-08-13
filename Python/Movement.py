import serial  # PySerial Library

class Move:
    def __init__(self, comPort, speed, direction):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        self.speed = speed
        self.direction = direction
        # wait until arduino is booted
        ans = ''
        while ans != '0':
            ans = self.comm.readline().decode().strip()
            #print(ans)

    # speed is a floating point number between -1 and 1
    def setSpeed(self, speed):
        self.speed = speed
        speed += 15

        while(self.checkSpeed() != self.speed):
            self.comm.write(('\r\n%f' % speed).encode())

    # direction is an integer between -30 and 30.
    def setWheelPos(self, direction):
        self.direction = direction
        direction = round(direction / 30)
        if direction < 0:
            direction += 3
        direction += 10
        while float(self.direction != self.checkWheelPos()):
            self.comm.write(('\r\n%d' % direction).encode())

    # gets wheel velocity
    def checkSpeed(self):
        try:
            ans = ''
            while ans == '':
                self.comm.write('\r\n17').encode()
                ans = self.comm.readline().decode().rstrip()
                #print(ans)

            ans = float(ans)
            return ans
        except UnicodeDecodeError:
            return -2

    # gets steering angle. Returns -2 if there's an error.
    def checkWheelPos(self):
        try:
            ans = ''
            while ans == '':
                self.comm.write(('\r\n13').encode())
                ans = self.comm.readline().decode().strip()
            ans = int(ans)
            return ans
        except UnicodeDecodeError:
            return -2