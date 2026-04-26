#include "client.h"
#include "raylib.h"
#include <stdio.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#define WEB_EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define WEB_EXPORT
#endif

int screenWidth = 800;
int screenHeight = 450;

void InitClient() {
  InitWindow(screenWidth, screenHeight, "Noteren");
  SetTargetFPS(60);
}

WEB_EXPORT void set_dimensions(int w, int h) {
  screenWidth = w;
  screenHeight = h;
  SetWindowSize(screenWidth, screenHeight);
}

void ClientMainloop() {
  if (WindowShouldClose()) {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop(); /* this should "kill" the app. */
#else
    exit(0);
#endif
  }

  // Update
  Rectangle draw_rect = {.x = 10, .y = 10, .width = 100, .height = 100};

  // Draw
  BeginDrawing();

  ClearBackground(RAYWHITE);

  if (GuiButton(draw_rect, "AAAAAAA")) {
    printf("Hi!\n");
  }

  EndDrawing();
}

void CloseClient() { CloseWindow(); }