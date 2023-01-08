import Foundation

func printErr(_ message: String) {
    guard let messageData = "\(message)\n".data(using: .utf8) else { return }

    FileHandle.standardError.write(messageData)
}

func imageHelloWorld(width: Int, height: Int) -> RTImage {
    let printProgress = { (lines: Int) in printErr("Scanlines remaining: \(lines)") }

    let pixels = [Pixel](unsafeUninitializedCapacity: width * height) { buffer, initializedCount in
        printProgress(height)

        for h in (0..<height).reversed() {
            if h % 10 == 0, h != 0 {
                printProgress(h)
            }

            let rowIndex = height - h - 1

            for w in 0..<width {
                let red = UInt8(Double(w) / Double(width - 1) * 255)
                let green = UInt8(Double(h) / Double(height - 1) * 255)

                buffer[w + (rowIndex * width)] = Pixel(red: red, green: green, blue: 63)
                initializedCount += 1
            }
        }
    }
    printErr("Done")
    return RTImage(pixels: pixels, width: width, height: height)
}

let width = 256
let height = 256

let pic = imageHelloWorld(width: width, height: height)

print(pic.renderToPPM())