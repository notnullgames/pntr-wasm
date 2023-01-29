// this is an example game in assemblyscript

let image: Image = {}

export function load(): void {
  log("Hello from tester.");
  image = load_image("/image.png")
  log(`Image loaded (${image.id}): ${image.width}x${image.height}`)
}

export function update(dt:u64): void {
  draw_rectangle(10, 10, 80, 200, RED)
  draw_rectangle(100, 10, 80, 200, GREEN)
  draw_rectangle(200, 10, 80, 200, BLUE)
  draw_pixel(300, 80, BLUE)
  draw_image(image, 200, 50)
}
