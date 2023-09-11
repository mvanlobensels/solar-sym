import numpy as np

class CelestialBody:

  G = 6.6743e-11 # m3 kg-1 s-2

  def __init__(self):
    self._M = None
    
    self.r = None
    self.theta = None

    self.satellites = []
    
