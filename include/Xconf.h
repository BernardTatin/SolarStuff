/* 
 * File:   private-Xhelper.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 4 mars 2016, 18:36
 */

#ifndef PRIVATE_XHELPER_H
#define	PRIVATE_XHELPER_H

typedef struct {
	Display* display;
	int screen;
	Window root_window;
	Window win;
	GC gc;
	XGCValues gr_values; 
	GC gr_context;
} TSXconfig;

extern TSXconfig xconf_main;

bool xconf_open(const int x, const int y, const int width, const int height);

static inline GC xconf_init_gc(void) {
	xconf_main.gc = DefaultGC(xconf_main.display, xconf_main.screen);
	xconf_main.gc = xconf_main.gr_context;
	return xconf_main.gc;
}

static inline GC xconf_gc(void) {
	return xconf_main.gc;
}

static inline void xconf_close(void) {
    XDestroyWindow(xconf_main.display, xconf_main.win);
    XCloseDisplay(xconf_main.display);
}
#endif	/* PRIVATE_XHELPER_H */

