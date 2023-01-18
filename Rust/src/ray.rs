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

    pub fn sphere_normal(&self, center: &Point3, radius: f64) -> Option<f64> {
        let oc = &self.origin - center;
        let a = self.direction.length_squared();
        let half_b = Vec3::dot(&oc, &self.direction);
        let c = oc.length_squared() - radius * radius;
        let discriminant = half_b * half_b - a * c;
        if discriminant < 0.0 {
            None
        } else {
            Some((-half_b - discriminant.sqrt()) / a)
        }
    }

    pub fn hit_sphere(&self, center: &Point3, radius: f64) -> bool {
        self.sphere_normal(center, radius).is_some()
    }
}
