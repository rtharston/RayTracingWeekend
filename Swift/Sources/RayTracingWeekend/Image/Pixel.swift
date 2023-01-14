typealias Pixel = GenericPixel<UInt8>

struct GenericPixel<Size: UnsignedInteger> {
    let r: Size
    let g: Size
    let b: Size
    let a: Size

}

extension GenericPixel where Size == UInt8 {
    init(red: UInt8, green: UInt8, blue: UInt8, alpha: UInt8 = 255) {
        r = red
        g = green
        b = blue
        a = alpha
    }
}
