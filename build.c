#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <process.h>

#include <sys/stat.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __clang__
#  define CC "clang"
#elif _MSC_VER
#  define CC "cl"
#else
#  define CC "cc"
#endif

static void usage() {
  fprintf(stderr, "just call 'build' without arguments\n");
}

static int run(char ** args) {
  assert(args && args[0]);

  if (0 == _spawnvp(_P_WAIT, args[0], (const char * const *)args)) {
    return 0;
  }

  fprintf(stderr, "failed to run child process: %s\n", args[0]);
  return 1;
}

static int compile(char * name) {
  char src[1024];
  sprintf(src, "%s.c", name);

  char exe[1024];
  sprintf(exe, "%s.exe", name);

  char * args[] = { CC, "-Wall", "-g", "-o", exe, src, "-luser32", 0 };
  if (run(args)) return 1;

  fprintf(stderr, "%s\n", src);
  return 0;
}

int main(int argc, char ** argv) {
  if (argc != 1) return (usage(), 1);

  if (compile("hello-winmain")) return 1;
  if (compile("hello-main")) return 1;
  if (compile("wndproc")) return 1;

  return 0;
}
