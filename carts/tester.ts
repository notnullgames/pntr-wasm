// this is an example game in assemblyscript

let image: u8 = 0
let ri: u8 = 0

export function load(): void {
  log("Hello from tester.")

  image = load_image("/image.png")
  log(`Image loaded (${image})`)

  // another way to make a rectangle
  ri = gen_image_color(20, 20, RED)
  log(`red image (${ri})`)
  draw_image_on_image(image, ri, 10, 10)
}

export function update(dt:u64): void {
  clear_screen((u8(dt / 300000) % 2) == 0 ? DARKPURPLE : DARKGREEN)
  // clear_screen(BLACK)
  draw_rectangle(10, 10, 80, 200, RED)
  draw_rectangle(100, 10, 80, 200, GREEN)
  draw_rectangle(200, 10, 80, 200, BLUE)
  draw_pixel(20, 20, BLUE)
  draw_image(image, 150, 50)
  draw_circle(20, 20, 20, BLUE)
}

export function unload(): void {
  log("Ok, bye.");
}
