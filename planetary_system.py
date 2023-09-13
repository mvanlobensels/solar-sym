import numpy as np
from typing import List
import matplotlib.pyplot as plt
from matplotlib import animation

class CelestialBody:

    def __init__(self, name: str, mass: float, position, velocity) -> None:
        self._name = name
        self._mass = mass

        self.position = np.array(position, dtype=float)
        self.velocity = np.array(velocity, dtype=float)
        self.path = []


class PlanetarySystem:

    G = 6.67e-11    # Gravitational constant
    AU = 1.5e11     # Earth-Sun distance

    def __init__(self, bodies: List[CelestialBody]) -> None:
        self.bodies = bodies

    def update_state(self, dt: float):
        for body in self.bodies:
            F = 0

            for other in self.bodies:
                if id(body) != id(other):

                    r = body.position - other.position
                    F += -self.G * body._mass * other._mass * r / np.sum(r**2)**1.5

            body.acceleration = F / body._mass
            body.velocity += body.acceleration * dt
            body.position += body.velocity * dt
            body.path.append(body.position.tolist())

    def plot(self):
        fig, ax = plt.subplots(figsize=(8,8))

        artists = []
        for body in self.bodies:

            path_body,     = ax.plot([], [], '-g', lw=1, c='black')
            position_body, = ax.plot([self.AU], [0], marker="o", markersize=4, markeredgecolor="blue", markerfacecolor="blue")
            text_body      = ax.text(self.AU, 0, body._name)
            artists.extend([path_body, position_body, text_body])

        def update(i):
            for k in range(0, len(self.bodies)*3, 3):
                body = self.bodies[k//3]
                artists[k].set_data(*list(zip(*body.path)))
                artists[k+1].set_data(*body.path[i])
                artists[k+2].set_position((body.path[i][0], body.path[i][1]))

            return artists

        ax.set_xlim(-6*self.AU, 6*self.AU)
        ax.set_ylim(-6*self.AU, 6*self.AU)
        ax.grid()

        anim = animation.FuncAnimation(fig, func=update, frames=len(self.bodies[0].path), interval=1, blit=True)
        plt.show()