import Movement

car = Movement.Move('COM4', 0, 0)
car.setSpeed(0.25)
print(car.checkSpeed())
car.setWheelPos(0)
print(car.checkWheelPos())