import numpy as np
from typing import List

class CelestialBody:

  G = 6.6743e-11 # m3 kg-1 s-2

  def __init__(self, name: str, mass: float, position: List[float], satellites=[]):
    self._name = name
    self._mass = mass
    
    self.r = position[0]
    self.theta = position[1]

    self._satellites = satellites
    
