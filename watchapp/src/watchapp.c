#include <pebble.h>
#include "dither.h"
#include "miniGL/mine.h"

static Window *window;
static Layer *render_layer;

GRect bounds;

uint16_t frame = 0;
uint8_t* model_buf = NULL;

extern uint8_t framebuffer[144*144 / 2];
static void register_timer(void* data);
uint32_t frame_count = 0;

static void render_opengl(Layer* layer, GContext *ctx) {
  GBitmap* bitmap = (GBitmap*)ctx;
  uint32_t* buf = (uint32_t*)bitmap->addr;

  gl_drawframe(model_buf);

  floyd_steinberg_dither(framebuffer, (uint8_t*)buf, -1);
//   buf[0] = frame_count++;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Rendered frame %ud", (unsigned int)frame_count);
  frame_count++;
}

static void register_timer(void* data) {
  app_timer_register(15, register_timer, data);
  layer_mark_dirty(render_layer);
}

static void window_load(Window *window) {
  Layer* window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  render_layer = layer_create(bounds);
  layer_set_update_proc(render_layer, render_opengl);
  layer_add_child(window_layer, render_layer);
  register_timer(NULL);
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

  ResHandle handle = resource_get_handle(RESOURCE_ID_MODEL);
  size_t len = resource_size(handle);
  uint8_t* buf = malloc(len);
  resource_load(handle, buf, len);
  model_buf = buf;

  gl_init();
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
