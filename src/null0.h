// this is the host-side header for null0

// TODO: should we do argc/argv for carts?

// TODO: check which of these I am actually using
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PNTR_IMPLEMENTATION
#include "pntr.h"

#include "m3_env.h"
#include "physfs.h"
#include "wasm3.h"

enum Null0CartType {
  Null0CartTypeInvalid,
  Null0CartTypeDir,
  Null0CartTypeZip,
  Null0CartTypeWasm,
};

struct Null0Image {
  uint8_t id;
  uint8_t height;
  uint8_t width;
};
typedef struct Null0Image Null0Image;

static M3Environment* env;
static M3Runtime* runtime;
static M3Module* module;
static M3Function* cart_load;
static M3Function* cart_update;
struct timespec startTime;
struct timespec nowTime;

bool FileExistsInPhysFS(const char* fileName) {
  PHYSFS_Stat stat;
  if (PHYSFS_stat(fileName, &stat) == 0) {
    return false;
  }
  return stat.filetype == PHYSFS_FILETYPE_REGULAR;
}

// Log a string
static m3ApiRawFunction(null0_log) {
  m3ApiGetArgMem(const char*, message);
  printf("%s\n", message);
  m3ApiSuccess();
}

// Fatal error - call this from your code on a fatal runtime error, similar to assemblyscript's abort(), but it's utf8
static m3ApiRawFunction(null0_fatal) {
  m3ApiGetArgMem(const char*, message);
  m3ApiGetArgMem(const char*, fileName);
  m3ApiGetArg(uint16_t, lineNumber);
  m3ApiGetArg(uint16_t, columnNumber);
  fprintf(stderr, "%s at %s:%d:%d\n", message, fileName, lineNumber, columnNumber);
  exit(1);
}

static m3ApiRawFunction(null0_draw_image) {
  m3ApiSuccess();
}
static m3ApiRawFunction(null0_draw_pixel) {
  m3ApiSuccess();
}
static m3ApiRawFunction(null0_draw_rectangle) {
  m3ApiSuccess();
}
static m3ApiRawFunction(null0_load_image) {
  m3ApiSuccess();
}

// all wasm3 functions return same sort of error-pattern, so this wraps that
static void null0_check_wasm3(M3Result result) {
  if (result) {
    M3ErrorInfo info;
    m3_GetErrorInfo(runtime, &info);
    fprintf(stderr, "%s - %s\n", result, info.message);
    exit(1);
  }
}

// this checks the general state of the runtime, to make sure there are no errors lingering
static void null0_check_wasm3_is_ok() {
  M3ErrorInfo error;
  m3_GetErrorInfo(runtime, &error);
  if (error.result) {
    fprintf(stderr, "%s - %s\n", error.result, error.message);
    exit(1);
  }
}

// given a filename and some bytes (at least 4) this will tell you what type the cart is (dir/wasm/zip/invalid)
enum Null0CartType null0_get_cart_type(char* filename, u8* bytes, u32 byteLength) {
  DIR* dirptr;
  if (access(filename, F_OK) != -1) {
    if ((dirptr = opendir(filename)) != NULL) {
      return Null0CartTypeDir;
    } else {
      // printf("%d %d %d %d\n", bytes[0], bytes[1], bytes[2], bytes[3]);

      if (bytes[0] == 80 && bytes[1] == 75 && bytes[2] == 3 && bytes[3] == 4) {
        return Null0CartTypeZip;
      }
      if (bytes[0] == 0 && bytes[1] == 97 && bytes[2] == 115 && bytes[3] == 109) {
        return Null0CartTypeWasm;
      }
      return Null0CartTypeInvalid;
    }
  } else {
    return Null0CartTypeInvalid;
  }
}

// run this in your game-loop
void null0_update_cart() {
  clock_gettime(CLOCK_MONOTONIC_RAW, &nowTime);
  uint64_t delta = (nowTime.tv_sec - startTime.tv_sec) * 1000000 + (nowTime.tv_nsec - startTime.tv_nsec) / 1000;
  if (cart_update) {
    null0_check_wasm3(m3_CallV(cart_update, delta));
  }
}

// given a filename, byte-array and length of cart file, this will load up wasm environment for it
int null0_load_cart_wasm(char* filename, u8* wasmBuffer, u32 byteLength) {
  enum Null0CartType t = null0_get_cart_type(filename, wasmBuffer, byteLength);

  if (t == Null0CartTypeInvalid) {
    fprintf(stderr, "invalid cart.\n");
    return 1;
  }

  // TODO: init neesd a string of "who are you?", not sure if this is right way to do that
  PHYSFS_init(filename);
  env = m3_NewEnvironment();
  runtime = m3_NewRuntime(env, 1024 * 1024, NULL);

  if (t == Null0CartTypeDir || t == Null0CartTypeZip) {
    PHYSFS_mount(filename, NULL, 0);

    if (!FileExistsInPhysFS("main.wasm")) {
      fprintf(stderr, "No main.wasm.\n");
      return 1;
    }

    PHYSFS_File* wasmFile = PHYSFS_openRead("main.wasm");
    PHYSFS_uint64 wasmLen = PHYSFS_fileLength(wasmFile);
    u8* wasmBufferP[wasmLen];
    PHYSFS_sint64 bytesRead = PHYSFS_readBytes(wasmFile, wasmBufferP, wasmLen);
    PHYSFS_close(wasmFile);

    if (bytesRead == -1) {
      fprintf(stderr, "Could not read main.wasm.\n");
      return 1;
    }

    null0_check_wasm3(m3_ParseModule(env, &module, (u8*)wasmBufferP, wasmLen));
  } else if (t == Null0CartTypeWasm) {
    null0_check_wasm3(m3_ParseModule(env, &module, wasmBuffer, byteLength));
  }

  null0_check_wasm3(m3_LoadModule(runtime, module));

  // add the writable dir overlay to root
  PHYSFS_mount(PHYSFS_getWriteDir(), NULL, 0);

  // IMPORTS (to wasm)
  m3_LinkRawFunction(module, "env", "null0_log", "v(*)", &null0_log);
  m3_LinkRawFunction(module, "env", "null0_fatal", "v(**ii)", &null0_fatal);

  m3_LinkRawFunction(module, "env", "null0_draw_image", "v(*ii)", &null0_draw_image);
  m3_LinkRawFunction(module, "env", "null0_draw_pixel", "v(ii*)", &null0_draw_pixel);
  m3_LinkRawFunction(module, "env", "null0_draw_rectangle", "v(iiii*)", &null0_draw_rectangle);
  m3_LinkRawFunction(module, "env", "null0_load_image", "*(*)", &null0_load_image);

  null0_check_wasm3_is_ok();

  // EXPORTS (from wasm)
  m3_FindFunction(&cart_load, runtime, "load");
  m3_FindFunction(&cart_update, runtime, "update");

  null0_check_wasm3_is_ok();

  clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);

  if (cart_load) {
    null0_check_wasm3(m3_CallV(cart_load));
  } else {
    fprintf(stderr, "no load() in cart.\n");
  }

  return 0;
}

// given a filename, this will load up wasm environment for it
int null0_load_cart(char* filename) {
  u8* wasm = NULL;
  u32 fsize = 0;

  FILE* f = fopen(filename, "rb");
  if (!f) {
    fprintf(stderr, "Cannot open file.\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  wasm = (u8*)malloc(fsize);
  if (!wasm) {
    fprintf(stderr, "cannot allocate memory for wasm binary\n");
    return 1;
  }

  if (fread(wasm, 1, fsize, f) != fsize) {
    fprintf(stderr, "cannot read file\n");
    return 1;
  }
  fclose(f);
  f = NULL;

  return null0_load_cart_wasm(filename, wasm, fsize);
}