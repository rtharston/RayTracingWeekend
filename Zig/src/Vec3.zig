const std = @import("std");
const expect = std.testing.expect;

x: f64,
y: f64,
z: f64,
pub const zero: @This() = .init(0.0, 0.0, 0.0);

pub fn init(x: f64, y: f64, z: f64) @This() {
    return @This(){
        .x = x,
        .y = y,
        .z = z,
    };
}

pub fn negate(self: @This()) @This() {
    return .{
        .x = -self.x,
        .y = -self.y,
        .z = -self.z,
    };
}

// TODO: consider adding second version that assigns to self
pub fn add(self: @This(), other: @This()) @This() {
    return .{
        .x = self.x + other.x,
        .y = self.y + other.y,
        .z = self.z + other.z,
    };
}

// TODO: consider adding second version that assigns to self
pub fn sub(self: @This(), other: @This()) @This() {
    return .{
        .x = self.x - other.x,
        .y = self.y - other.y,
        .z = self.z - other.z,
    };
}

// TODO: consider adding second version that assigns to self
pub fn mult(self: @This(), other: anytype) @This() {
    if (@TypeOf(other) == f64 or @TypeOf(other) == comptime_float) {
        return .{
            .x = self.x * other,
            .y = self.y * other,
            .z = self.z * other,
        };
    } else if (@TypeOf(other) == i32 or @TypeOf(other) == i64 or @TypeOf(other) == comptime_int or @TypeOf(other) == usize) {
        return .{
            .x = self.x * @as(f64, @floatFromInt(other)),
            .y = self.y * @as(f64, @floatFromInt(other)),
            .z = self.z * @as(f64, @floatFromInt(other)),
        };
    } else if (@TypeOf(other) == @This()) {
        return .{
            .x = self.x * other.x,
            .y = self.y * other.y,
            .z = self.z * other.z,
        };
    } else {
        unreachable;
    }
}

// TODO: consider adding second version that assigns to self
pub fn divide(self: @This(), other: f64) @This() {
    return mult(self, 1 / other);
}

pub fn length(self: @This()) f64 {
    return std.math.sqrt(self.length_squared());
}

pub fn length_squared(self: @This()) f64 {
    return self.x * self.x + self.y * self.y + self.z * self.z;
}

pub fn dot(self: @This(), other: @This()) f64 {
    return self.x * other.x + self.y * other.y + self.z * other.z;
}

pub fn cross(self: @This(), other: @This()) @This() {
    return .{
        .x = self.y * other.z - self.z * other.y,
        .y = self.z * other.x - self.x * other.z,
        .z = self.x * other.y - self.y * other.x,
    };
}

pub fn unit_vector(v: @This()) @This() {
    return v.divide(v.length());
}

pub fn eql(self: @This(), other: @This()) bool {
    return self.x == other.x and self.y == other.y and self.z == other.z;
}

test "dot product" {
    const v1 = @This().init(1.0, 0.0, 0.0);
    const v2 = @This().init(0.0, 1.0, 0.0);
    try expect(v1.dot(v2) == 0.0);
}

test "mult" {
    const v1 = @This().init(1.0, 0.0, 2.0);
    const v2 = @This().init(0.5, 1.0, 0.0);
    try expect(v1.mult(v2).eql(@This().init(0.5, 0.0, 0.0)));

    try expect(v1.mult(0.5).eql(@This().init(0.5, 0.0, 1.0)));
}

test "length" {
    const v1 = @This().init(1.0, 0.0, 0.0);
    try expect(v1.length() == 1.0);
    const v2 = @This().init(1.0, 1.0, 0.0);
    try expect(v2.length() == std.math.sqrt(2.0));
}

test "unit_vector" {
    const v1 = @This().init(1.0, 0.0, 0.0);
    try expect(v1.unit_vector().eql(v1));
    const v2 = @This().init(std.math.sqrt(2.0), std.math.sqrt(2.0), 0.0);
    try expect(v2.unit_vector().eql(@This().init(std.math.sqrt(2.0) / 2.0, std.math.sqrt(2.0) / 2.0, 0.0)));
}
