import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'build')))

from solar_sym import Body, System
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

AU = 1.5e11                 # Earth-Sun distance
Me = 5.972e24               # Earth mass
sec_per_day = 24 * 60 * 60  # Seconds per day
dt = 1 * sec_per_day
t_end = 365 * 11 * dt

# Solar system
solar_system = System([
    Body(name='Sun',     mass=333000*Me, position=[0, 0],       velocity=[0, 0]),
    Body(name='Mercury', mass=0.0553*Me, position=[0.4*AU, 0],  velocity=[0, 47000]),
    Body(name='Venus',   mass=0.815*Me,  position=[0.72*AU, 0], velocity=[0, 35000]),
    Body(name='Earth',   mass=Me,        position=[AU, 0],      velocity=[0, 29290]),
    Body(name='Mars',    mass=0.107*Me,  position=[1.5*AU, 0],  velocity=[0, 24000]),
    Body(name='Jupiter', mass=317.8*Me,  position=[5.2*AU, 0],  velocity=[0, 12440]),
])

# # Earth-Moon system
# earth_moon_system = System([
#     Body(name='Earth',   mass=Me,        position=[0, 0],       velocity=[0, 0]),
#     Body(name='Moon',    mass=0.0123*Me, position=[0.0025695*AU, 0], velocity=[0, 1000]),
# ])

# # Three-body problem
# three_body_system = System([
#     Body(name='A', mass=1, position=[-0.97000436, 0.24308753], velocity=[0.4662036850, 0.4323657300]),
#     Body(name='B', mass=1, position=[0, 0], velocity=[-0.93240737, -0.86473146]),
#     Body(name='C', mass=1, position=[0.97000436, -0.24308753], velocity=[0.4662036850, 0.4323657300]),
#     Body(name='Planet', mass=0.001, position=[-0.33, -0.3], velocity=[0, 0])
# ])
# three_body_system.G = 1
# dt = 0.001
# t_end = 10

system = solar_system

t = 0
while t < t_end:
    system.update_state(dt)
    t += dt

# Make copy
bodies = system.bodies

for body in bodies:
    body.cache_trajectory()

fig, ax = plt.subplots(figsize=(8,8))

artists = []
for body in bodies:

    path_body,     = ax.plot([], [], lw=1, c='black')
    position_body, = ax.plot([system.AU], [0], marker="o", markersize=4, markeredgecolor="blue", markerfacecolor="blue")
    text_body      = ax.text(system.AU, 0, body.name)
    artists.extend([path_body, position_body, text_body])

def update(i):
    for k in range(0, len(bodies)*3, 3):
        body = bodies[k//3]
        trajectory = np.array(body.cached_trajectory)[:i]
        artists[k].set_data(*trajectory.T)
        artists[k+1].set_data([body.cached_trajectory[i][0]], [body.cached_trajectory[i][1]])
        artists[k+2].set_position((body.cached_trajectory[i][0], body.cached_trajectory[i][1]))

    return artists

ax.set_xlim(-6*system.AU, 6*system.AU)
ax.set_ylim(-6*system.AU, 6*system.AU)
ax.grid()

anim = animation.FuncAnimation(fig, func=update, frames=len(bodies[0].cached_trajectory), interval=1, blit=True)
plt.show()
