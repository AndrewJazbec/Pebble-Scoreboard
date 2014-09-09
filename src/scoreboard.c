#include "pebble.h"

#define NUM_TEAM1_PKEY 1
#define NUM_TEAM2_PKEY 2
  
#define NUM_TEAM1_DEFAULT 0
#define NUM_TEAM2_DEFAULT 0
  
static Window *window;

static TextLayer *team1_text_layer;
static TextLayer *team2_text_layer;

static int team1 = NUM_TEAM1_DEFAULT;
static int team2 = NUM_TEAM2_DEFAULT;

static void update_text1() {
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "Team1: %u", team1);
  text_layer_set_text(team1_text_layer, body_text);
}

static void update_text2() {
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "Team2: %u", team2);
  text_layer_set_text(team2_text_layer, body_text);
}


static void increment1_click_handler(ClickRecognizerRef recognizer, void *context) {
  team1++;
  update_text1();
}

static void increment2_click_handler(ClickRecognizerRef recognizer, void *context) {
  team2++;
  update_text2();
}

//Long Click Press
void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  team1 = 0;
  team2 = 0;

  update_text1();
  update_text2();
}

//Long Click Rlease
void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //No contents needed
}
  

//Click Config
static void click_config_provider(void *context) {
  
  //Button click delay 50ms
  const uint16_t repeat_interval_ms = 50;
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, (ClickHandler) increment1_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, (ClickHandler) increment2_click_handler);
  
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  
}

//Window Load
static void window_load(Window *me) {
  
  //Layers
  Layer *layer = window_get_root_layer(me);
  const int16_t width = layer_get_frame(layer).size.w - ACTION_BAR_WIDTH - 3;
  
  team1_text_layer = text_layer_create(GRect(8, 30, width, 60));
  text_layer_set_font(team1_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(team1_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(team1_text_layer));
  
  team2_text_layer = text_layer_create(GRect(8, 90, width, 60));
  text_layer_set_font(team2_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(team2_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(team2_text_layer));
  
  //Udate text
  update_text1();
  update_text2();
}

static void window_unload(Window *window) {
  
  text_layer_destroy(team1_text_layer);
  text_layer_destroy(team2_text_layer);
}

static void init(void) {
 
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
team1 = persist_exists(NUM_TEAM1_PKEY) ? persist_read_int(NUM_TEAM1_PKEY) : NUM_TEAM1_DEFAULT;
team2 = persist_exists(NUM_TEAM2_PKEY) ? persist_read_int(NUM_TEAM2_PKEY) : NUM_TEAM2_DEFAULT;
  window_stack_push(window, true /* Animated */);
  
}

static void deinit(void) {
  
  persist_write_int(NUM_TEAM1_PKEY, team1);
  persist_write_int(NUM_TEAM2_PKEY, team2);

  window_destroy(window);
  
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}