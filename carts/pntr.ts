// This is the auto-imported header for pntr WASM games, written in typescript

@unmanaged
export class Color {
  r: u8
  g: u8
  b: u8
  a: u8
}

@unmanaged
export class Image {
  id: u8
  height: u8
  width: u8
}

export const LIGHTGRAY :Color = { r: 200, g: 200, b: 200, a: 255 }
export const GRAY      :Color = { r: 130, g: 130, b: 130, a: 255 }
export const DARKGRAY  :Color = { r: 80,  g:  80, b:  80, a: 255 }
export const YELLOW    :Color = { r: 253, g: 249, b: 0,   a: 255 }
export const GOLD      :Color = { r: 255, g: 203, b: 0,   a: 255 }
export const ORANGE    :Color = { r: 255, g: 161, b: 0,   a: 255 }
export const PINK      :Color = { r: 255, g: 109, b: 194, a: 255 }
export const RED       :Color = { r: 230, g:  41, b:  55, a: 255 }
export const MAROON    :Color = { r: 190, g:  33, b:  55, a: 255 }
export const GREEN     :Color = { r: 0,   g: 228, b: 48,  a: 255 }
export const LIME      :Color = { r: 0,   g: 158, b: 47,  a: 255 }
export const DARKGREEN :Color = { r: 0,   g: 117, b: 44,  a: 255 }
export const SKYBLUE   :Color = { r: 102, g: 191, b: 255, a: 255 }
export const BLUE      :Color = { r: 0,   g: 121, b: 241, a: 255 }
export const DARKBLUE  :Color = { r: 0,   g:  82, b: 172, a: 255 }
export const PURPLE    :Color = { r: 200, g: 122, b: 255, a: 255 }
export const VIOLET    :Color = { r: 135, g:  60, b: 190, a: 255 }
export const DARKPURPLE:Color = { r: 112, g:  31, b: 126, a: 255 }
export const BEIGE     :Color = { r: 211, g: 176, b: 131, a: 255 }
export const BROWN     :Color = { r: 127, g: 106, b:  79, a: 255 }
export const DARKBROWN :Color = { r: 76,  g:  63, b:  47, a: 255 }
export const WHITE     :Color = { r: 255, g: 255, b: 255, a: 255 }
export const BLACK     :Color = { r: 0,   g:   0, b:   0, a: 255 }
export const BLANK     :Color = { r: 0,   g:   0, b:   0, a: 0   }
export const MAGENTA   :Color = { r: 255, g:   0, b: 255, a: 255 }
export const RAYWHITE  :Color = { r: 245, g: 245, b: 245, a: 255 }


// Log a string
@external("env", "null0_log")
declare function _log(text: ArrayBuffer): void
export function log(text: string): void {
  return _log(String.UTF8.encode(text, true))
}

// Fatal error - call this from your code on a fatal runtime error, similar to assemblyscript's abort(), but it's utf8
@external("env", "null0_fatal")
declare function _fatal(message: ArrayBuffer, filename: ArrayBuffer, lineNumber: i32, columnNumber: i32): void
export function fatal(message: string, filename: string, lineNumber: i32, columnNumber: i32): void {
  return _fatal(String.UTF8.encode(message, true), String.UTF8.encode(filename, true), lineNumber, columnNumber)
}

// Load an image
@external("env", "null0_load_image")
declare function _load_image(filename: ArrayBuffer): Image
export function load_image(filename: string): Image {
  return _load_image(String.UTF8.encode(filename, true))
}

// Draw a rectangle on the screen
@external("env", "null0_draw_rectangle")
export declare function draw_rectangle(x:i32, y:i32, width:i32, height:i32, color:Color): void

// Draw a pixel on the screen
@external("env", "null0_draw_pixel")
export declare function draw_pixel(x:i32, y:i32, color:Color): void

// Draw an image on the screen
@external("env", "null0_draw_image")
export declare function draw_image(image: Image, x:i32, y:i32): void

