import Movement
import Sonar
import Positioning

imu = Positioning.IMU('COM8')

while True:
    nmea = imu.getRawGPSData()
    if(nmea != ""):
        print(nmea)