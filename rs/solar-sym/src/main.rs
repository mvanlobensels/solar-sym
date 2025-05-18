mod system;
use system::{Body, System};
use ndarray::{array};
use plotly::Plot;
use plotly::Scatter;
use plotly::common::Mode;

fn main() {
    const AU: f64 = 1.5e11;    // Earth-Sun distance
    const ME: f64 = 5.972e24;  // Earth mass

    let sec_per_day = 24. * 60. * 60.;  // Seconds per day
    let dt = 1.0 * sec_per_day;
    let t_end = 365. * 11. * dt;

    let mut solar_system: System = System::new(vec![
        Body::new(
            "Sun".to_string(),
            333000.0*ME,
            array![0.0, 0.0, 0.0],
            array![0.0, 0.0, 0.0]
        ),
        Body::new("Mercury".to_string(),
            0.0553*ME,
            array![0.4*AU,  0.0, 0.0],
            array![0.0, 47000.0, 0.0]
        ),
        Body::new("Venus".to_string(),
            0.815*ME,
            array![0.72*AU, 0.0, 0.0], 
            array![0.0, 35000.0, 0.0]
        ),
        Body::new("Earth".to_string(),
            ME,
            array![AU,0.0, 0.0],
            array![0.0, 29290.0, 0.0]
        ),
        Body::new("Mars".to_string(),
            0.107*ME,
            array![1.5*AU,  0.0, 0.0],
            array![0.0, 24000.0, 0.0]
        ),
        Body::new("Jupiter".to_string(),
            317.8*ME,
            array![5.2*AU,  0.0, 0.0], 
            array![0.0, 12440.0, 0.0]
        ), 
    ]);

    // First, run the simulation for the full duration
    let mut t = 0.0;
    while t < t_end {
        solar_system.step(dt);
        t += dt;
    }

    // Then render everything
    let mut plot = Plot::new();

    for body in &solar_system.bodies {
        // Extract the x and y coordinates from the trajectory
        let xs: Vec<f64> = body.trajectory.column(0).to_vec();
        let ys: Vec<f64> = body.trajectory.column(1).to_vec();
        
        let trace = Scatter::new(xs, ys)
            .mode(Mode::Lines)
            .name(body.name.clone());
        let trace = Scatter::new(vec![0, 1, 2], vec![2, 1, 0]);
        plot.add_trace(trace);
    }
    
    plot.show();

}
