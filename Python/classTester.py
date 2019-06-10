import Movement
import Sonar
import Positioning
from time import sleep
car = Movement.Move("COM3", 0, 0)

print(car.checkWheelPos())
sleep(3)
car.callibrateWheel()

