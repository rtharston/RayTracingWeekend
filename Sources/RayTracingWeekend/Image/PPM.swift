extension RTImage {
    func renderToPPM() -> String {
        var output = "P3\n\(width) \(height)\n255\n"

        for (i, pixel) in pixels.enumerated() {
            output += pixel.asPPMString
            // TODO: Consider using chunked from swift-algorithms
            if i % width == 0 {
                output += "\n"
            }
            // this is trusting that the pixel array is the right size
        }

        return output
    }
}

fileprivate extension Pixel {
    var asPPMString: String {
        "\(r) \(g) \(b) "
    }
}