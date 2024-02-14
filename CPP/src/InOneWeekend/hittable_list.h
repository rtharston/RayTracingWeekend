#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
  std::vector<shared_ptr<hittable>> objects;

  hittable_list() noexcept {}
  hittable_list(const shared_ptr<hittable> object) noexcept { add(object); }

  void clear() { objects.clear(); }

  void add(const shared_ptr<hittable> object) noexcept {
    objects.push_back(object);
  }
  
  bool hit(const ray& r, const double ray_tmin, const double ray_tmax, hit_record& rec) const noexcept override {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;

    for (const auto& object : objects) {
      if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }
};

#endif // HITTABLE_LIST_H