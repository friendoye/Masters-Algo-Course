
class Ocean():
    def __init__(self, width, height, ocean_objects):
        self.width = width
        self.height = height
        self.ocean_objects = ocean_objects

    def check_cell(self, x, y):
        if x < 0 or x >= self.width:
            return Obstacle()
        elif y < 0 or y >= self.height:
            return Obstacle()
        else:
            obj = self.ocean_objects[y][x]
            return obj if (obj is not None) else OceanObject()

class OceanObject():
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def make_turn(self, ocean):
        pass

class Mammal(OceanObject):
    def __init__(self, x, y, increasing_interval):
        super(Mammal, self).__init__(x, y)
        self.increasing_interval = increasing_interval

    def make_turn(self, ocean):
        pass

class Predator(Mammal):
    def __init__(self, x, y, increasing_interval, time_before_die):
        super(Predator, self).__init__(x, y, increasing_interval)
        self.time_before_die = time_before_die

class Prey(Mammal):
    def __init__(self, x, y, increasing_interval):
        super(Prey, self).__init__(x, y, increasing_interval)

class Obstacle(OceanObject):
    def __init__(self, x = 0, y = 0):
        super(Obstacle, self).__init__(x, y)


if __name__ == "__main__":
