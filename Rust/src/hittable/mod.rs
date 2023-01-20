pub mod sphere;
pub use sphere::Sphere;

use crate::ray::Ray;
use crate::vec3::{Point3, Vec3};

pub struct HitRecord {
    p: Point3,
    normal: Vec3,
    t: f64,
}

impl HitRecord {
    pub fn get_p(&self) -> &Point3 {
        &self.p
    }

    pub fn get_normal(&self) -> &Vec3 {
        &self.normal
    }

    pub fn get_t(&self) -> &f64 {
        &self.t
    }
}

pub trait Hittable {
    fn hit(&self, ray: &Ray, t_min: f64, t_max: f64) -> Option<HitRecord>;
    // fn hit(&self, ray: &Ray) -> Option<HitRecord>;
}
