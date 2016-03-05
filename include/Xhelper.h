/* 
 * File:   Xhelper.h
 * Author: Bernard TATIN <bernard dot tatin at outlook dot org>
 *
 * Created on 3 mars 2016, 23:38
 */

#ifndef XHELPER_H
#define	XHELPER_H

int XhDrawString(const int x, const int y, char *format, ...);

static inline void XhFillRectangle(const int x, const int y, const int width, const int height) {
	XFillRectangle(xconf_main.display, xconf_main.win, xconf_main.gc, x, y, width, height);
}
#endif	/* XHELPER_H */
