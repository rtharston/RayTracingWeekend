#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include "sphere.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
  // TODO: change this back to hittable
  std::vector<shared_ptr<sphere>> objects;

  hittable_list() noexcept {}
  hittable_list(const shared_ptr<sphere> object) noexcept { add(object); }

  void clear() { objects.clear(); }

  void add(const shared_ptr<sphere> object) noexcept {
    objects.push_back(object);
  }
  
  bool hit(const ray& r, const interval ray_t, hit_record& rec) const noexcept override {
    // TODO: make this configurable depending on AVX support (hard coded for now because this is for my CPU)
    constexpr auto max_obj_count = 4;

    std::array<hit_record, max_obj_count> recs;
    // hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    for (int i = 0; i < objects.size();) {
      int obj_count = 0;
      if ((i + max_obj_count) < objects.size() )
        obj_count = max_obj_count;
      else {
        obj_count = objects.size() - i;
      }

      const double a = r.direction().length_squared();
      const auto results = hit_spheres_avx2(&objects[i], r, a, interval(ray_t.min, closest_so_far), recs, obj_count);

      for (int j = 0; j < obj_count; ++j) {
        // multiple objects may be hit in the same test, so we can't blindly record all hits; e.g. results[4] might be farther than results[2], so don't record results[4]
        if (results[j] && recs[j].t < closest_so_far) {
          hit_anything = true;
          closest_so_far = recs[j].t;
          rec = recs[j];
        }
      }
      i += obj_count;
    }

    return hit_anything;
  }
};

#endif // HITTABLE_LIST_H