pub mod sphere;
pub use sphere::Sphere;

use crate::ray::Ray;
use crate::vec3::{Point3, Vec3};

pub struct HitRecord {
    p: Point3,
    normal: Vec3,
    t: f64,
    front_face: bool,
}

impl HitRecord {
    pub fn new(p: Point3, outward_normal: Vec3, t: f64, ray_direction: &Vec3) -> Self {
        let front_face = Vec3::dot(&outward_normal, ray_direction) < 0.0;
        let normal = if front_face {
            outward_normal
        } else {
            -outward_normal
        };

        HitRecord {
            p,
            normal,
            t,
            front_face,
        }
    }

    pub fn get_p(&self) -> &Point3 {
        &self.p
    }

    pub fn get_normal(&self) -> &Vec3 {
        &self.normal
    }

    pub fn get_t(&self) -> &f64 {
        &self.t
    }

    pub fn get_front_face(&self) -> bool {
        self.front_face
    }
}

pub trait Hittable {
    fn hit(&self, ray: &Ray, t_min: f64, t_max: f64) -> Option<HitRecord>;
    // fn hit(&self, ray: &Ray) -> Option<HitRecord>;
}
