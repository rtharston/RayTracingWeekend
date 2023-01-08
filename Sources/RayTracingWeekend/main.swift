func imageHelloWorld(width: Int, height: Int) -> RTImage {
    let pixels = [Pixel](unsafeUninitializedCapacity: width * height) { buffer, initializedCount in
        for h in 0..<height {
            let rowIndex = height - h - 1

            for w in 0..<width {
                let red = UInt8(Double(w) / Double(width - 1) * 255)
                let green = UInt8(Double(h) / Double(height - 1) * 255)

                buffer[w + (rowIndex * width)] = Pixel(red: red, green: green, blue: 63)
                initializedCount += 1
            }
        }
    }
    return RTImage(pixels: pixels, width: width, height: height)
}

let width = 256
let height = 256

let pic = imageHelloWorld(width: width, height: height)

print(pic.renderToPPM())