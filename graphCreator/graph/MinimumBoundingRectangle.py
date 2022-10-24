class MinimumBoundingRectangle:  # Class to represent a MBR

    
    def __init__(self, x, y):
        self.lower_x = x
        self.lower_y = y
        self.upper_x = x
        self.upper_y = y
        self.pointsInside = [[x,y]]    

    def append(self, x, y):
        if self.lower_x > x:
            self.lower_x = x
            updated = True

        if self.lower_y > y:
            self.lower_y = y
            updated = True

        if self.upper_x <x:
            self.upper_x = x
            updated = True

        if self.upper_y < y:
            self.upper_y = y
            updated = True

        self.pointsInside.append([x,y])

        return self

    def __str__(self):
        return ("(" + self.lower_x + ", " + self.lower_y + ", " + self.upper_x + ", " + self.upper_y + " )")
    
    def createLineFormat(self, node):
        line = str(node) + "\t" + str(self.lower_x) + "\t" + str(self.lower_y) + "\t" + str(self.upper_x) + "\t" + str(self.upper_y)
        if (len(self.pointsInside) > 1):
            for point in self.pointsInside:
                line += "\t" + str(point[0]) + "\t" + str(point[1]) 

        line += "\n"
        return line

