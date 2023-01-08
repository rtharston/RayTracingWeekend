protocol Vec3 {
    var x: Double { get }
    var y: Double { get }
    var z: Double { get }

    init(x: Double, y: Double, z: Double)
}

struct Point3: Vec3 {
    let x: Double
    let y: Double
    let z: Double
}

struct Color: Vec3 {
    let r: Double
    let g: Double
    let b: Double

    internal init(r: Double, g: Double, b: Double) {
        self.r = r
        self.g = g
        self.b = b
    }

    internal init(x: Double, y: Double, z: Double) {
        self.r = x
        self.g = y
        self.b = z
    }

    var x: Double { get { r } }
    var y: Double { get { g } }
    var z: Double { get { b } }
}

extension Vec3 {
    static prefix func -(lhs: Self) -> Self {
        Self(x: -lhs.x, y: -lhs.y, z: -lhs.z)
    }

    // TODO: See if this is necessary
    // subscript(i: Int) -> Double {
    //     switch i {
    //     case 0: return x
    //     case 1: return y
    //     case 2: return z
    //     default: return Double.nan
    //     }
    // }

    static func +=(lhs: inout Self, rhs: Self) -> Self {
        Self(x: lhs.x + rhs.x, y: lhs.y + rhs.y, z: lhs.z + rhs.z)
    }

    static func *=(lhs: inout Vec3, rhs: Double) -> Self {
        Self(x: lhs.x * rhs, y: lhs.y * rhs, z: lhs.z * rhs)
    }

    static func /=(lhs: inout Vec3, rhs: Double) -> Self {
        Self(x: lhs.x / rhs, y: lhs.y / rhs, z: lhs.z / rhs)
    }

    var length: Double {
        lengthSquared.squareRoot()
    }

    var lengthSquared: Double {
        x * x + y * y + z * z
    }
}
