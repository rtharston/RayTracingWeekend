const Vec3 = @import("Vec3.zig");
pub const Point3 = Vec3;
const expect = @import("std").testing.expect;

orig: Point3,
dir: Vec3,
const zero: @This() = .init(.zero, .zero);

pub fn init(orig: Point3, dir: Vec3) @This() {
    return @This(){
        .orig = orig,
        .dir = dir,
    };
}

pub fn at(self: @This(), t: f64) Point3 {
    return self.orig.add(self.dir.mult(t));
}

test "at" {
    const r = @This().init(.zero, .init(1.0, 2.0, 0.0));
    try expect(r.at(0.4).eql(.init(0.4, 0.8, 0.0)));

    try expect(zero.at(1.0).eql(.zero));

    const r2 = @This().init(.init(1.0, 0.8, 0.0), .init(1.0, 2.0, 0.0));
    try expect(r2.at(1.0).eql(.init(2.0, 2.8, 0.0)));

    // try expect(v1.mult(0.5).eql(@This().init(0.5, 0.0, 1.0)));
}
