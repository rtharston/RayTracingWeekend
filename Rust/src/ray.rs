use crate::vec3::{Point3, Vec3};

pub struct Ray {
    origin: Point3,
    direction: Vec3,
}

impl Ray {
    pub fn new(origin: Point3, direction: Vec3) -> Self {
        Ray { origin, direction }
    }

    pub fn new_zero() -> Self {
        Ray {
            origin: Point3::new_zero(),
            direction: Vec3::new_zero(),
        }
    }

    pub fn get_origin(&self) -> &Point3 {
        &self.origin
    }

    pub fn get_direction(&self) -> &Vec3 {
        &self.direction
    }

    pub fn at(&self, t: f64) -> Point3 {
        &self.origin + &(&self.direction * t)
    }

    pub fn hit_sphere(&self, center: &Point3, radius: f64) -> bool {
        let oc = &self.origin - center;
        let a = Vec3::dot(&self.direction, &self.direction);
        let b = 2.0 * Vec3::dot(&oc, &self.direction);
        let c = Vec3::dot(&oc, &oc) - radius * radius;
        let discriminant = b * b - 4.0 * a * c;
        discriminant > 0.0
    }
}
