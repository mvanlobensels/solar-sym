import numpy as np
from planetary_system import CelestialBody, PlanetarySystem

G = 6.67e-11                # Gravitational constant
AU = 1.5e11                 # Earth-Sun distance
Me = 5.972e24               # Earth mass
sec_per_day = 24 * 60 * 60  # Seconds per day
dt = 0.01 * sec_per_day
t_end = 365 * 11 * dt

# Solar system
solar_system = PlanetarySystem([
    CelestialBody(name='Sun',     mass=333000*Me, position=[0, 0],       velocity=[0, 0]),
    CelestialBody(name='Mercury', mass=0.0553*Me, position=[0.4*AU, 0],  velocity=[0, 47000]),
    CelestialBody(name='Venus',   mass=0.815*Me,  position=[0.72*AU, 0], velocity=[0, 35000]),
    CelestialBody(name='Earth',   mass=Me,        position=[AU, 0],      velocity=[0, 29290]),
    # CelestialBody(name='Moon',    mass=0.0123*Me, position=[1.002707*AU, 0], velocity=[0, 29290+1000]),
    CelestialBody(name='Mars',    mass=0.107*Me,  position=[1.5*AU, 0],  velocity=[0, 24000]),
    CelestialBody(name='Jupiter', mass=317.8*Me,  position=[5.2*AU, 0],  velocity=[0, 12440])
])

# earth_moon_system = PlanetarySystem([
#     CelestialBody(name='Earth',   mass=Me,        position=[0, 0],       velocity=[0, 0]),
#     CelestialBody(name='Moon',    mass=0.0123*Me, position=[0.0025695*AU, 0], velocity=[0, 1000]),
# ])

for t in range(0, t_end, dt):
    solar_system.update_state(dt)

solar_system.plot()