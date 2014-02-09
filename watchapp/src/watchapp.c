#include <pebble.h>
#include "dither.h"
#include "miniGL/mine.h"

static Window *window;
static Layer *render_layer;
static bool wireframe = false;


uint16_t frame = 0;
uint8_t* model_buf = NULL;

extern uint8_t framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT / 2];
static void register_timer(void* data);

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  wireframe = !wireframe;
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}


static void render_opengl(Layer* layer, GContext *ctx) {
  GBitmap* bitmap = (GBitmap*)ctx;
  uint32_t* buf = (uint32_t*)bitmap->addr;
//  static uint16_t framecount = 0;

  gl_drawframe(model_buf, wireframe);

  floyd_steinberg_dither(framebuffer, (uint8_t*)buf, -1);
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "Rendered frame %ud", (unsigned int)frame_count);
//  frame_count++;
}

static void register_timer(void* data) {
  app_timer_register(60, register_timer, data);
  layer_mark_dirty(render_layer);
}

static void window_load(Window *window) {
  Layer* window_layer = window_get_root_layer(window);
  render_layer = layer_create(layer_get_bounds(window_layer));
  layer_set_update_proc(render_layer, render_opengl);
  layer_add_child(window_layer, render_layer);
  register_timer(NULL);
}

static void window_unload(Window *window) {}

static void init(void) {
  light_enable(true);  // Leave the backlight on
  window = window_create();
  //window_set_fullscreen(window, true);
  window_set_background_color(window, GColorBlack);
  window_set_click_config_provider(window, click_config_provider);
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

  app_event_loop();
  deinit();
}
