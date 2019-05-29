import serial
from time import sleep

class IMU:
    def __init__(self, comPort):
        self.comm = serial.Serial(comPort, 9600, timeout=.1)
        self.GPSIsReady = False
        self.roll = 0
        self.pitch = 0
        self.yaw = 0
        self.XAccel = 0
        self.YAccel = 0
        self.heading = 0
        self.lat = 0
        self.lng = 0
        self.altitude = 0
        self.speed = 0
        self.satellites = 0
        sleep(1)

    def getGyro(self):
        ans = ''
        while ans != '10':
            self.comm.write(('\r\n10').encode())
            ans = self.comm.readline().decode().strip()
        ans = self.comm.readline().decode().strip()
        ans = list(map(float, ans.split(' ')))
        self.roll = ans[0]
        self.pitch = ans[1]
        self.yaw = ans[2]
        return (self.roll, self.pitch, self.yaw)

    def getAccel(self):
        ans = ''
        while ans != '11':
            self.comm.write(('\r\n11').encode())
            ans = self.comm.readline().decode().strip()
        ans = self.comm.readline().decode().strip()
        ans = list(map(float, ans.split(' ')))
        self.XAccel = ans[0]
        self.YAccel = ans[1]
        return (self.XAccel, self.YAccel)

    def getHeading(self):
        ans = ''
        while ans != '12':
            self.comm.write(('\r\n12').encode())
            ans = self.comm.readline().decode().strip()
        ans = self.comm.readline().decode().strip()
        self.heading = (float)(ans)
        return self.heading

    def checkGPSConnection(self):
        ans = ''
        while ans != '15':
            self.comm.write(('\r\n15').encode())
            ans = self.comm.readline().decode().strip()
        ans = self.comm.readline().decode().strip()
        self.satellites = (int)(ans)
        return self.satellites

    def getGPSLocation(self):
        ans = ''
        if self.checkGPSConnection() != 0:
            while ans != '13':
                self.comm.write(('\r\n13').encode())
                ans = self.comm.readline().decode().strip()
            ans = self.comm.readline().decode().strip()
            ans = list(map(float, ans.split(' ')))
            self.lat = ans[0]
            self.lng = ans[1]
            self.altitude = ans[2]
            return (self.lat, self.lng, self.altitude)
        else:
            return (-1, -1, -1)

    def getGPSSpeed(self):
        ans = ''
        if self.checkGPSConnection() != 0:
            while ans != '13':
                self.comm.write(('\r\n13').encode())
                ans = self.comm.readline().decode().strip()
            ans = self.comm.readline().decode().strip()
            self.speed = (float)(ans)
            return self.speed
        else:
            return -500

    def getRawGPSData(self):
        ans = ''
        while ans != '16':
            self.comm.write(('\r\n16').encode())
            ans = self.comm.readline().decode().strip()
        ans = self.comm.readline().decode().strip()
        return ans


