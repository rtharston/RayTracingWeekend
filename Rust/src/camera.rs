use crate::vec3::{Point3, Vec3};

struct Camera {
    origin: Point3,
    lower_left_corner: Point3,
    horizontal: Vec3,
    vertical: Vec3
}

impl Camera {
    pub fn new() -> Camera {
        // TODO: Consider making the aspect ratio selectable
        const ASPECT_RATIO: f64 = 16.0 / 9.0;
        const VIEWPORT_HEIGHT: f64 = 2.0;
        const VIEWPORT_WIDTH: f64 = ASPECT_RATIO * VIEWPORT_HEIGHT;
        const FOCAL_LENGTH: f64 = 1.0;
        const ORIGIN: Point3 = Point3::new_zero();
        const HORIZONTAL: Vec3 = Vec3::new(VIEWPORT_WIDTH, 0.0, 0.0);
        const VERTICAL: Vec3 = Vec3::new(0.0, VIEWPORT_HEIGHT, 0.0);

        Camera {
            origin: ORIGIN,
            lower_left_corner: ORIGIN - HORIZONTAL / 2.0 - VERTICAL / 2.0 - Vec3::new(0.0, 0.0, FOCAL_LENGTH),
            horizontal: HORIZONTAL,
            vertical: VERTICAL
        }
    }
}