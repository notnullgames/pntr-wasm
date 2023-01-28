This will load games made with [pntr](https://github.com/RobLoach/pntr), compiled to wasm, in libretro or standalone.

The idea is you can write your game in any language you like, and load it in a safe sandbox. Games are just zip files with `main.wasm` as an entry-point, and any assets you need.

### building

```
cmake -B build .
make -C build

# run standalone
./build/pntr-host build/tester.pntr

# run in libretro, in linux, with retroarch in your path
retroArch -L build/pntr-host-libretro.so build/tester.pntr

# run in libretro, in windiows, with retroarch in your path
retroArch -L build/pntr-host-libretro.dll build/tester.pntr

# run in libretro, on mac
/Applications/RetroArch.app/Contents/MacOS/RetroArch -L build/pntr-host-libretro.dylib build/tester.pntr
```