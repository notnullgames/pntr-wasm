// this is an example game in assemblyscript

let image: u8 = 0

export function load(): void {
  log("Hello from tester.");
  image = load_image("/image.png")
  log(`Image loaded (${image})`)
}

export function update(dt:u64): void {
  // clear_screen((u8(dt / 300000) % 2) == 0 ? DARKPURPLE : DARKGREEN)
  clear_screen(BLACK)
  draw_rectangle(10, 10, 80, 200, RED)
  draw_rectangle(100, 10, 80, 200, GREEN)
  draw_rectangle(200, 10, 80, 200, BLUE)
  draw_pixel(20, 20, BLUE)
  draw_image(image, 150, 50)
}

export function unload(): void {
  log("Ok, bye.");
}
