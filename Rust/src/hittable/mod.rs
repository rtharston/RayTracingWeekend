pub mod sphere;
pub use sphere::Sphere;

use crate::ray::Ray;
use crate::vec3::{Point3, Vec3};

#[derive(Clone, Copy)]
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

// Question: The original C++ code made a custom list that had an internal vector of shared pointers
// so the hittable objects could be shared around; do I need to handle RC<Hittable>?

impl<H: Hittable> Hittable for std::vec::Vec<H> {
    fn hit(&self, ray: &Ray, t_min: f64, t_max: f64) -> Option<HitRecord> {
        let mut temp_rec: Option<HitRecord> = None;
        let mut closest_so_far = t_max;

        for hittable_object in self.iter() {
            let hit_record = hittable_object.hit(ray, t_min, closest_so_far);
            if let Some(record) = hit_record {
                closest_so_far = record.t;
                temp_rec = hit_record;
            }
        }

        return temp_rec;
    }
}
