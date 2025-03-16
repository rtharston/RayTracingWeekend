const std = @import("std");
const Color = @import("Color.zig");
const Vec3 = @import("Vec3.zig");
const Ray = @import("Ray.zig");
const ppm = @import("ppm.zig");

fn rayColor(r: Ray) ppm.Color {
    const unit_direction = r.dir.unit_vector();
    const a = 0.5 * (unit_direction.y + 1.0);
    return Color.white.mult(1.0 - a).add(Vec3.init(0.5, 0.7, 1.0).mult(a));
}

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    // Image
    const image_width = 640;
    const aspect_ratio = 16.0 / 9.0;
    const image_height: comptime_int = guaranteedImageHeight(image_width, aspect_ratio);

    // Viewport widths less than one are ok since they are real valued.
    const focal_length = 1.0;
    const viewport_height = 2.0;
    // use the actual ratio of image_width to image_height instead of the ideal ratio of aspect_ratio
    const viewport_width: comptime_int = @intFromFloat(viewport_height * @as(comptime_float, @floatFromInt(image_width)) / @as(comptime_float, @floatFromInt(image_height)));
    const camera_center = Ray.Point3.zero;

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const viewport_u = Vec3.init(viewport_width, 0.0, 0.0);
    const viewport_v = Vec3.init(0.0, -viewport_height, 0.0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const pixel_delta_u = viewport_u.divide(image_width);
    const pixel_delta_v = viewport_v.divide(image_height);

    // Calculate the location of the upper left pixel.
    const viewport_upper_left = camera_center.sub(Vec3.init(0.0, 0.0, focal_length)).sub(viewport_u.divide(2)).sub(viewport_v.divide(2));
    const pixel00_loc = viewport_upper_left.add((pixel_delta_u.add(pixel_delta_v).mult(0.5)));

    try stdout.print("P3\n{} {}\n255\n", .{ image_width, image_height });

    for (0..image_height) |j| {
        std.debug.print("\rScanlines remaining: {} ", .{image_height - j});
        for (0..image_width) |i| {
            const pixel_center = pixel00_loc.add(pixel_delta_u.mult(i)).add(pixel_delta_v.mult(j));
            const ray_dir = pixel_center.sub(camera_center);
            try ppm.writeColor(stdout, rayColor(Ray.init(camera_center, ray_dir)));
        }
    }

    try bw.flush(); // Don't forget to flush!
    std.debug.print("\rDone.                 \n", .{});
}

fn guaranteedImageHeight(image_width: comptime_int, aspect_ratio: comptime_float) comptime_int {
    const height: comptime_int = @intFromFloat(@as(comptime_float, @floatFromInt(image_width)) / aspect_ratio);
    // make sure the height is at least 1
    return if (height < 1) 1 else height;
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // Try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}

test "fuzz example" {
    const Context = struct {
        fn testOne(context: @This(), input: []const u8) anyerror!void {
            _ = context;
            // Try passing `--fuzz` to `zig build test` and see if it manages to fail this test case!
            try std.testing.expect(!std.mem.eql(u8, "canyoufindme", input));
        }
    };
    try std.testing.fuzz(Context{}, Context.testOne, .{});
}
