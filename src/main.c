#include "main.h"
#include "bridge/bridge.h"
#include "client/client.h"
#include <stdio.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

int main(int argc, char *argv[]) {
  printf("Program name is: %s\n", argv[0]);
  enum Mode mode = BOTH;

  if (argc >= 2) {
// Program modes, not in web
#ifndef __EMSCRIPTEN__
    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "--server") != 0) {
        printf("Running in server only mode.\n");
        mode = SERVER;
      } else if (strcmp(argv[i], "--client") != 0) {
        printf("Running in client only mode.\n");
        mode = CLIENT;
      } else {
        printf("Running in both mode.\n");
        mode = BOTH; // Technically not needed but... wth
      }
    }
#endif
  }

  // Force client on web
#ifdef __EMSCRIPTEN__
  // In web we just support client mode.
  printf("Running in client mode due to web env.\n");
  mode = CLIENT;
#endif

  switch (mode) {
  case CLIENT:
    InitBridge(REMOTE);
    InitClient();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(ClientMainloop, 0, 1);
#endif

    CloseClient();
    CloseBridge();
    break;

  case SERVER:
    /* code */
    break;

  case BOTH:
    InitBridge(LOCAL);
    /* code */
    CloseBridge();
    break;

  default:
    break;
  }
  return 0;
}