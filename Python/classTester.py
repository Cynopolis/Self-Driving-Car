import Movement
import Sonar
import Positioning

move = Movement.Move('COM5', 0, 0)
move.setSpeed(-1)
print(move.checkSpeed())
