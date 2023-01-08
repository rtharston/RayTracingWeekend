import Foundation

struct RTImage {
    // TODO: Use generics to create an array of the same sized height of pixels
    let pixels: [Pixel]
    let width: Int
    let height: Int
    
    public init(pixels: [Pixel], width: Int, height: Int) {
        self.pixels = pixels
        self.height = height
        self.width = width
        
        // TODO: Add a check if the pixel array is the right size, or get the width and height from array
    }
}
