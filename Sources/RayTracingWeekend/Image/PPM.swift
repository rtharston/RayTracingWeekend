extension RTImage {
    func renderToPPM() -> String {
        "P3\n\(width) \(height)\n255\n" + pixels.map(\.asPPMString).joined(separator: "\n")
    }
}

fileprivate extension Pixel {
    var asPPMString: String {
        "\(r) \(g) \(b)"
    }
}