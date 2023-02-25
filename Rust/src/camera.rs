use crate::vec3::{Point3, Vec3};

struct Camera {
    origin: Point3,
    lower_left_corner: Point3,
    horizontal: Vec3,
    vertical: Vec3
}

impl Camera {
    pub fn new() -> Camera {
        let aspect_ratio = 16.0 / 9.0;
        let viewport_height = 2.0;
        let viewport_width = aspect_ratio * viewport_height;
        let focal_length = 1.0;

        let h = Vec3::new(viewport_width, 0.0, 0.0);
        let v = Vec3::new(0.0, viewport_height, 0.0);


        Camera {
            origin: Point3::new_zero(),
            lower_left_corner: origin - horizontal / 2.0 - vertical / 2.0 - vec3(0.0, 0.0, focal_length),
            horizontal: h,
            vertical: v
        }
    }
}