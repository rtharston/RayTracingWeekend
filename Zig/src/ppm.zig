pub const Color = @import("Vec3.zig");

pub const ColorU8 = struct {
    r: u8,
    g: u8,
    b: u8,
    const black: ColorU8 = .{ 0, 0, 0 };
    const red: ColorU8 = .{ 255, 0, 0 };
    const green: ColorU8 = .{ 0, 255, 0 };
    const blue: ColorU8 = .{ 0, 0, 255 };
    const white: ColorU8 = .{ 255, 255, 255 };

    pub fn init(r: u8, g: u8, b: u8) ColorU8 {
        return ColorU8{
            .r = r,
            .g = g,
            .b = b,
        };
    }

    pub fn fromColor(color: Color) ColorU8 {
        return ColorU8{
            .r = @intFromFloat(255.99 * color.x),
            .g = @intFromFloat(255.99 * color.y),
            .b = @intFromFloat(255.99 * color.z),
        };
    }
};

pub fn writeColor(writer: anytype, color: Color) !void {
    try writer.print("{} {} {}\n", ColorU8.fromColor(color));
}
