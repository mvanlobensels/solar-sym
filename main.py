import numpy as np
from planetary_system import CelestialBody
import matplotlib.pyplot as plt
from matplotlib import animation

G = 6.67e-11                # Gravitational constant
AU = 1.5e11                 # Earth-Sun distance
Ms = 2e30                   # Sun mass
Me = 5.972e24               # Earth mass
sec_per_day = 24 * 60 * 60  # Seconds per day
dt = 1 * sec_per_day
t_end = 365 * 5 * dt

sun   = CelestialBody(name='Sun', mass=333000*Me, position=[0, 0], velocity=[0, 0])
# earth = CelestialBody(name='Earth', mass=Me, position=[AU, 0], velocity=[0, 29290])
earth = CelestialBody(name='Jupiter', mass=317.8*Me, position=[5.2*AU, 0], velocity=[0, 12440])

for t in range(0, t_end, dt):

    r = earth.position - sun.position
    F = -G * sun._mass * earth._mass * r / np.sum(r**2)**1.5

    earth.acceleration = F / earth._mass
    earth.velocity += earth.acceleration * dt
    earth.position += earth.velocity * dt
    earth.path.append(earth.position.tolist())

    sun.acceleration = F / sun._mass
    sun.velocity += sun.acceleration * dt
    sun.position += sun.velocity * dt
    sun.path.append(sun.position.tolist())


fig, ax = plt.subplots(figsize=(6,6))

path_earth,     = ax.plot([], [], '-g', lw=1, c='black')
position_earth, = ax.plot([AU], [0], marker="o", markersize=4, markeredgecolor="blue", markerfacecolor="blue")
text_earth      = ax.text(AU, 0, earth._name)

path_sun,       = ax.plot([0], [0], marker="o", markersize=7, markeredgecolor="yellow", markerfacecolor="yellow")
text_sun        = ax.text(0, 0, sun._name)

def update(i):
    path_earth.set_data(*list(zip(*earth.path)))
    position_earth.set_data(*earth.path[i])
    text_earth.set_position((earth.path[i][0], earth.path[i][1]))

    path_sun.set_data(*sun.path[i])
    text_sun.set_position((sun.path[i][0], sun.path[i][1]))

    return path_earth, path_sun, position_earth, text_earth, text_sun

ax.set_xlim(-6*AU, 6*AU)
ax.set_ylim(-6*AU, 6*AU)
ax.grid()

anim = animation.FuncAnimation(fig, func=update, frames=len(earth.path), interval=1, blit=True)
plt.show()
