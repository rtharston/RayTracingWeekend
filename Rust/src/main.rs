use std::fs::File;
use std::io::Write;

mod image;
use image::{Image, Pixel};
mod output;
mod vec3;
use vec3::{Color, Point3, Vec3};
mod ray;
use ray::Ray;
mod hittable;
use hittable::{Hittable, Sphere};

// I want this to be in main (and selectable) and passed in to the generator
const ASPECT_RATIO: f64 = 16.0 / 9.0;

fn ray_traced_image(width: usize, height: usize) -> Image {
    const VIEWPORT_HEIGHT: f64 = 2.0;
    const VIEWPORT_WIDTH: f64 = ASPECT_RATIO * VIEWPORT_HEIGHT;
    const FOCAL_LENGTH: f64 = 1.0;
    const ORIGIN: Point3 = Point3::new_zero();
    const HORIZONTAL: Vec3 = Vec3::new(VIEWPORT_WIDTH, 0.0, 0.0);
    const VERTICAL: Vec3 = Vec3::new(0.0, VIEWPORT_HEIGHT, 0.0);
    // I'd like this to be const, but that doesn't work with my sub and div impls
    let lower_left_corner: Vec3 =
        ORIGIN - HORIZONTAL / 2.0 - VERTICAL / 2.0 - Vec3::new(0.0, 0.0, FOCAL_LENGTH);

    let world: Vec<Sphere> = vec![
        Sphere::new(Point3::new(0.0, 0.0, -1.0), 0.5),
        Sphere::new(Point3::new(0.0, -100.5, -1.0), 100.0),
    ];

    let mut pixels = Vec::<Pixel>::with_capacity(width * height);

    let mut progress_counter = 0;
    for h in (0..height).rev() {
        if progress_counter % 10 == 0 && h != 0 {
            eprintln!("Scanlines remaining: {}", h);
        }
        progress_counter += 1;

        for w in 0..width {
            let u = (w as f64) / (width - 1) as f64;
            let v = (h as f64) / (height - 1) as f64;
            let ray = Ray::new(
                ORIGIN,
                lower_left_corner + u * HORIZONTAL + v * VERTICAL - ORIGIN,
            );

            let color = |ray: Ray| -> Color {
                if let Some(hit) = world.hit(&ray, 0.0, f64::INFINITY) {
                    let n = hit.get_normal();
                    // Return a visualization of the sphere's normal map
                    return 0.5 * Color::new(n.get_x() + 1.0, n.get_y() + 1.0, n.get_z() + 1.0);
                }

                let unit_direction = ray.get_direction().unit_vector();
                let t = 0.5 * (unit_direction.get_y() + 1.0);
                (1.0 - t) * Color::new(1.0, 1.0, 1.0) + t * Color::new(0.5, 0.7, 1.0)
            };

            pixels.push(Pixel::from_color(color(ray)))
        }
    }
    eprintln!("Render complete.");
    Image::new(pixels, width, height)
}

enum OutputOption {
    File(File),
    STDOUT,
}

impl Image {
    fn save(self, output_option: OutputOption) {
        eprintln!("Saving image.");
        let image_str = self.ppm_string();
        match output_option {
            OutputOption::File(mut file) => match file.write_all(image_str.as_bytes()) {
                Err(error) => eprintln!("Failed to save image: {}", error),
                Ok(_) => eprintln!("Done."),
            },
            OutputOption::STDOUT => print!("{}", image_str),
        };
    }
}

fn print_usage() {
    println!(
        "usage:
ray_tracing_weekend <path>|--stdout
    Generate a ray traced image
    <path>      path to the output file
    --stdout    print image data to stdout
    "
    );
}

fn main() {
    let args: Vec<String> = std::env::args().collect();

    let output_option = match args.len() {
        2 => {
            let cmd = &args[1];
            match &cmd[..] {
                "--stdout" => OutputOption::STDOUT,
                // TODO: Check if valid path
                _ => match File::create(cmd) {
                    Ok(file) => OutputOption::File(file),
                    Err(error) => {
                        eprintln!("couldn't create file '{}'", cmd);
                        eprintln!("ERROR: {}\n", error);

                        print_usage();
                        std::process::exit(-2)
                    }
                },
            }
        }
        _ => {
            print_usage();
            std::process::exit(-1);
        }
    };

    // TODO: get output parameters from arguments
    // const WIDTH: usize = 2560;
    const WIDTH: usize = 400;
    const HEIGHT: usize = (WIDTH as f64 / ASPECT_RATIO) as usize;

    ray_traced_image(WIDTH, HEIGHT).save(output_option);
}
