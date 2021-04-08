from UILC_general import *


class Morena:
    self.L = "Linear"
    self.R = "Rectangle"

    def __init__(self, led, nled, case):
        self.case = case
        self.led = led
        self.nled = nled
        self.dm = self.dm()
    
    def dm(self):
        if self.case == self.L:
            print()
        elif self.case == self.R:
            print()

    
    def array(self,height):
    
    def intensity(self, target):
    
    def grad_intensity(self, target, axis=0):
        if axis ==0: #x axis
        elif axis == 1: #y axis
        elif axis ==2: #xy gradient
        else:
            return "err"
