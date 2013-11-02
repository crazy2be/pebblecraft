#include <pebble.h>

static Window *window;

GRect bounds;

typedef struct GContext {
  GBitmap dest_bitmap;
} MyGContext;

uint16_t frame = 0;

static void render_opengl(Layer* layer, GContext *ctx) {
  MyGContext* c = (MyGContext*)ctx;
  uint32_t* data = (uint32_t*)c->dest_bitmap.addr;

  int16_t w = bounds.size.w/32 + 1;
  int16_t h = bounds.size.h;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      data[y*w + x] = (uint32_t)y;
    }
  }
  frame++;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d %d", w, h, frame);
  layer_mark_dirty(layer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  layer_set_update_proc(window_layer, render_opengl);
}

static void window_unload(Window *window) {}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
