use crate::{
    ray::Ray,
    vec3::{Point3, Vec3},
};

use super::{HitRecord, Hittable};

pub struct Sphere {
    center: Point3,
    radius: f64,
    // radius_squared: f64,
}

impl Sphere {
    pub const fn new(center: Point3, radius: f64) -> Self {
        Sphere {
            center,
            radius,
            // it would be nice to only calculate this once, but right now
            // const is more valuable.
            // radius_squared: radius * radius,
        }
    }

    pub fn get_center(&self) -> &Point3 {
        &self.center
    }

    pub fn get_radius(&self) -> &f64 {
        &self.radius
    }
}

impl Hittable for Sphere {
    fn hit(&self, ray: &Ray, t_min: f64, t_max: f64) -> Option<HitRecord> {
        let oc = ray.get_origin() - &self.center;
        let a = ray.get_direction().length_squared();
        let half_b = Vec3::dot(&oc, ray.get_direction());
        // let c = oc.length_squared() - self.radius_squared;
        let c = oc.length_squared() - self.radius * self.radius;

        let discriminant = half_b * half_b - a * c;
        if discriminant < 0.0 {
            return None;
        }

        let sqrtd = discriminant.sqrt();

        // Find the nearest root that lies in the acceptable range.
        let root: f64;
        let root_minus = (-half_b - sqrtd) / a;
        if root_minus < t_min || root_minus > t_max {
            let root_plus = (-half_b + sqrtd) / a;
            if root_plus < t_min || root_plus > t_max {
                return None;
            } else {
                root = root_plus;
            }
        } else {
            root = root_minus;
        }

        let p = ray.at(root);
        Some(HitRecord::new(
            p,
            (&p - &self.center) / self.radius,
            root,
            ray.get_direction(),
        ))
    }
}
