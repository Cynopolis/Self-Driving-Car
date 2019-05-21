import serial
import time

class Sonar:
    def __init__(self, comPort):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        time.sleep(1)

    # get's current data then tells the sonar to do a sweep
    def updateFrame(self):
        ans = ''
        # continually requests data until it's told the command has been recieved.
        while ans == '':
            self.comm.write(('\r\n10').encode())
            ans = self.comm.readline().decode().strip()
        # gets the list of distances in a string
        ans = self.comm.readline().decode().strip()
        # converts the string to an array of floats
        return map(float, ans.split(' '))

    #tells sonar to do a sweep without getting data
    def doSweep(self):
        ans = ''
        while ans == '':
            self.comm.write(('\r\n11').encode())
            ans = self.comm.read().decode().strip()

    #gets the data without doing a sweep
    def getData(self):
        ans = ''
        #continually requasts data until it's told the command has been recieved
        while ans == '':
            self.comm.write(('\r\n12').encode())
            ans = self.comm.readline().decode().strip()
        # gets the list of distances in a string
        ans = self.comm.readline().decode().strip()
        # converts the string to an array of floats
        return map(float, ans.split(' '))