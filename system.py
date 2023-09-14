import numpy as np
from typing import List, Tuple
import matplotlib.pyplot as plt
from matplotlib import animation

class Body:
    """Celestial body class.

    Args:
        name (str): Body name
        mass (float): Body mass (kg)
        position (Tuple[float, float]): Starting position [x (m), y (m)]
        velocity (Tuple[float, float]): Starting velocity [v_x (m), v_y (m)]
    """

    def __init__(self, name: str, mass: float, position: Tuple[float, float], velocity: Tuple[float, float]) -> None:
        self._name = name
        self._mass = mass

        self.position = np.array(position, dtype=float)
        self.velocity = np.array(velocity, dtype=float)
        self.trajectory = []


class System:
    """Celestial system class.

        Args:
            bodies (List[Body]): List of celestial bodies (e.g. stars, planets, moons)
    """

    G = 6.67e-11    # Gravitational constant
    AU = 1.5e11     # Earth-Sun distance

    def __init__(self, bodies: List[Body]) -> None:
        self._bodies = bodies

    def update_state(self, dt: float):
        """Calculate and update state of each body in the system using numerical integration.

        Args:
            dt (float): time step (s)
        """
        for body in self._bodies:
            F = 0

            for other in self._bodies:
                if id(body) != id(other):

                    r = body.position - other.position
                    F += -self.G * body._mass * other._mass * r / np.sum(r**2)**1.5

            body.acceleration = F / body._mass
            body.velocity += body.acceleration * dt
            body.position += body.velocity * dt
            body.trajectory.append(body.position.tolist())

    def plot(self) -> None:
        """Plot evolution of the system.
        """
        fig, ax = plt.subplots(figsize=(8,8))

        artists = []
        for body in self._bodies:

            path_body,     = ax.plot([], [], '-g', lw=1, c='black')
            position_body, = ax.plot([self.AU], [0], marker="o", markersize=4, markeredgecolor="blue", markerfacecolor="blue")
            text_body      = ax.text(self.AU, 0, body._name)
            artists.extend([path_body, position_body, text_body])

        def update(i):
            for k in range(0, len(self._bodies)*3, 3):
                body = self._bodies[k//3]
                trajectory = np.array(body.trajectory)[:i]
                # artists[k].set_data(*list(zip(*trajectory)))
                artists[k].set_data(*trajectory.T)
                artists[k+1].set_data(*body.trajectory[i])
                artists[k+2].set_position((body.trajectory[i][0], body.trajectory[i][1]))

            return artists

        ax.set_xlim(-6*self.AU, 6*self.AU)
        ax.set_ylim(-6*self.AU, 6*self.AU)
        ax.grid()

        anim = animation.FuncAnimation(fig, func=update, frames=len(self._bodies[0].trajectory), interval=1, blit=True)
        plt.show()