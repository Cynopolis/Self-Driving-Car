import Movement
import Sonar

sonar = Sonar.Sonar('COM8')
sonar.doSweep()
dat = sonar.updateFrame()
print("updated")
for data in dat:
    print(data)