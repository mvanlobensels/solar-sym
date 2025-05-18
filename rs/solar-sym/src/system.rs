use ndarray::{Array1, Array2, Axis, concatenate};


/// Represents a celestial body with a name, mass, position, velocity, and trajectory.
#[derive(Clone)]
pub struct Body {
    pub name: String,
    mass: f64,
    pub position: Array1<f64>,
    pub velocity: Array1<f64>,
    pub trajectory: Array2<f64>
}

impl Body {
    /// Constructs a new `Body` instance.
    ///
    /// # Arguments
    ///
    /// * `name` - A `String` representing the name of the body.
    /// * `mass` - A `f64` representing the mass of the body.
    /// * `position` - An `Array1<f64>` representing the position of the body.
    /// * `velocity` - An `Array1<f64>` representing the velocity of the body.
    ///
    /// # Returns
    ///
    /// A `Body` instance with the specified name, mass, position, and velocity.
    pub fn new(name: String, mass: f64, position: Array1<f64>, velocity: Array1<f64>) -> Self {
        Body {
            name, 
            mass, 
            position: position.clone(),
            velocity,
            trajectory: position.into_shape((1, 3)).unwrap()
        }
    }
}

pub struct System {
    pub bodies: Vec<Body>
}
impl System {
    pub const G: f64 = 6.67e-11;  // Gravitational constant

    /// Creates a new `System` with the given vector of `Body` instances.
    ///
    /// # Arguments
    ///
    /// * `bodies` - A vector containing `Body` instances that make up the system.
    ///
    /// # Returns
    ///
    /// A `System` instance containing the specified bodies.
    pub fn new(bodies: Vec<Body>) -> Self {
        System {
            bodies
        }
    }

    pub fn get_body(&self, name: &str) -> Option<&Body> {
        self.bodies.iter().find(|body| body.name == name)
    }

    pub fn step(&mut self, dt: f64) {
        let bodies_clone = self.bodies.clone();  // Clone the bodies for the inner loop

        for body in self.bodies.iter_mut() {
            let mut f = Array1::from_elem(3, 0.0);

            for other in &bodies_clone {
                if body.name != other.name {
                    f = f + Self::gravitational_force(body, other)
                }
            }

            let acceleration = &f / body.mass;
            body.velocity = &body.velocity + &(&acceleration * dt);
            body.position = &body.position + &(&body.velocity * dt);
            body.trajectory = concatenate(Axis(0), &[
                body.trajectory.view(),
                body.position.view().into_shape((1,3)).unwrap()
            ]).unwrap();
        }
    }

    /// Calculates the gravitational force between two bodies using Newton's law of universal gravitation
    /// 
    /// F = -G * (m₁m₂/r²) * r̂
    /// 
    /// where:
    /// - G is the gravitational constant
    /// - m₁, m₂ are the masses of the two bodies
    /// - r is the displacement vector between the bodies
    /// - r̂ is the unit vector in the direction of r
    ///
    /// # Arguments
    /// * `body1` - Reference to the first body
    /// * `body2` - Reference to the second body
    ///
    /// # Returns
    /// * `Array1<f64>` - The gravitational force vector (in Newtons) acting on body1 due to body2
    fn gravitational_force(body1: &Body, body2: &Body) -> Array1<f64> {
        let r = &body1.position - &body2.position;
        -Self::G * body1.mass * body2.mass * &r / r.mapv(|r| r.powi(2)).sum().powf(1.5)
    }
}

