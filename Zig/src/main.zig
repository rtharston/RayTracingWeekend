const std = @import("std");
const Vec3 = @import("Vec3.zig");
const ppm = @import("ppm.zig");

pub fn main() !void {
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    // Image
    const image_width = 1024;
    const image_height = 1024;

    try stdout.print("P3\n{} {}\n255\n", .{ image_width, image_height });

    for (0..image_height) |j| {
        std.debug.print("\rScanlines remaining: {} ", .{image_height - j});
        for (0..image_width) |i| {
            try ppm.writeColor(stdout, .{
                .x = @as(f64, @floatFromInt(i)) / (image_width - 1),
                .y = @as(f64, @floatFromInt(j)) / (image_width - 1),
                .z = 0.0,
            });
        }
    }

    try bw.flush(); // Don't forget to flush!
    std.debug.print("\rDone.                 \n", .{});
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
