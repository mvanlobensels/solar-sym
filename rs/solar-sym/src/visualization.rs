extern crate kiss3d;

use kiss3d::nalgebra::{Vector3, UnitQuaternion, Point3};
use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::camera::ArcBall;

pub struct Visualizer {
    window: Window,
    camera: ArcBall,
}

impl Visualizer {
    pub fn new() {
        let mut window = Window::new("Kiss3d: cube");
        let mut c      = window.add_cube(1.0, 1.0, 1.0);
    
        c.set_color(1.0, 0.0, 0.0);
    
        window.set_light(Light::StickToCamera);
    
        let rot = UnitQuaternion::from_axis_angle(&Vector3::y_axis(), 0.014);
    
        while window.render() {
            c.prepend_to_local_rotation(&rot);
        }
    }

    pub fn render(&mut self, system: &System) {
        self.window.set_light(Light::StickToCamera);

        while self.window.render() {
            // Draw each body and its trajectory
            for body in &system.bodies {
                // Draw the current position
                let mut sphere = self.window.add_sphere(0.1);
                let pos = Point3::new(
                    body.position[0] as f32 / 1e11,  // Scale down by 10^11 to make it visible
                    body.position[1] as f32 / 1e11,
                    body.position[2] as f32 / 1e11
                );
                sphere.set_local_translation(pos);

                // Draw the trajectory
                for i in 0..body.trajectory.nrows()-1 {
                    let start = Point3::new(
                        body.trajectory[[i, 0]] as f32 / 1e11,
                        body.trajectory[[i, 1]] as f32 / 1e11,
                        body.trajectory[[i, 2]] as f32 / 1e11
                    );
                    let end = Point3::new(
                        body.trajectory[[i+1, 0]] as f32 / 1e11,
                        body.trajectory[[i+1, 1]] as f32 / 1e11,
                        body.trajectory[[i+1, 2]] as f32 / 1e11
                    );
                    self.window.draw_line(&start, &end, &Point3::new(1.0, 1.0, 1.0));
                }
            }
        }
    }
} 