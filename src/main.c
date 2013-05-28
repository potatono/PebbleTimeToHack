#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x8A, 0x95, 0x4D, 0xC9, 0xE2, 0xA1, 0x4D, 0x7E, 0xA6, 0x43, 0xB5, 0x67, 0x75, 0xE8, 0xC8, 0xC3 }
PBL_APP_INFO(MY_UUID,
             "Time To Hack", "Justin Day",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define BIT(x,y)     ((x & (1 << y)) >> y)
#define TIME_STR_BUFFER_BYTES 6

const int POINTS[20] = { 21,45, 53,19, 93,19, 122,45, 
						10,83, 21,122, 53,148, 93,148, 122,122, 134,83
					   };

Window window;
Layer layer;
GPoint point;
TextLayer textlayer;
char str[TIME_STR_BUFFER_BYTES];

void layer_callback(Layer *me, GContext *ctx) {
	(void)me;
	
	PblTm t;
	get_time(&t);
	unsigned short hour = t.tm_hour % 12;
	unsigned short min = t.tm_min;	

	for (int i=0; i<4; i++) {
		point = GPoint(POINTS[i*2], POINTS[i*2+1]);
		graphics_context_set_fill_color(ctx, GColorBlack);
		graphics_fill_circle(ctx, point, 10);
	
		if (!BIT(hour,(3-i))) {
			graphics_context_set_fill_color(ctx, GColorWhite);
			graphics_fill_circle(ctx, point, 8);
		}
	}
	
	for (int i=0; i<6; i++) {
		point = GPoint(POINTS[i*2+8], POINTS[i*2+9]);
		graphics_context_set_fill_color(ctx, GColorBlack);
		graphics_fill_circle(ctx, point, 10);
	
		if (!BIT(min,(5-i))) {
			graphics_context_set_fill_color(ctx, GColorWhite);
			graphics_fill_circle(ctx, point, 8);
		}
	}
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
	(void)ctx;
	(void)event;
	
	string_format_time(str, TIME_STR_BUFFER_BYTES, "%I:%M", event->tick_time);
	text_layer_set_text(&textlayer, str);

	layer_mark_dirty(&layer);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

	window_init(&window, "Hello World");
	window_stack_push(&window, true /* Animated */);
	
	text_layer_init(&textlayer, GRect(0,64,144,40));
	strcpy(str, "");
	text_layer_set_font(&textlayer, fonts_get_system_font(FONT_KEY_GOTHAM_34_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(&textlayer, GTextAlignmentCenter);
	text_layer_set_text(&textlayer, str);
	layer_add_child(&window.layer, &textlayer.layer);
	
	layer_init(&layer, window.layer.frame);
	layer.update_proc = &layer_callback;
	layer_add_child(&window.layer, &layer);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	  .tick_info = {
		  .tick_handler = &handle_tick,
		  .tick_units = MINUTE_UNIT
	  }
  };
  app_event_loop(params, &handlers);
}
