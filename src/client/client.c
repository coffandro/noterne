#include "client.h"
#include "gui.h"
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
  Rectangle draw_rect = {
      .x = 0, .y = 0, .width = screenWidth, .height = screenHeight};

  // Draw
  BeginDrawing();

  ClearBackground(RAYWHITE);

  Rectangle top = rect_cut_top(&draw_rect, 48);

  if (GuiButton(rect_cut_left(&top, 48), "X")) {
    printf("Hi!\n");
  }

  if (GuiButton(rect_cut_left(&top, 48), "||")) {
    printf("Hi!\n");
  }

  if (GuiButton(rect_cut_left(&top, 48), "-")) {
    printf("Hi!\n");
  }

  DrawRectangleRec(top, BLUE);

  DrawRectangle(draw_rect.x, draw_rect.y, draw_rect.width, draw_rect.height,
                ORANGE);

  EndDrawing();
}

void CloseClient() { CloseWindow(); }