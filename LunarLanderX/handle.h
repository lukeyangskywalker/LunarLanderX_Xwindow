#ifndef _HANDLE_H_
#define _HANDLE_H_

#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "loadbitmap.h"
#include "mist.h"

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
using namespace std;
const int BufferSize = 10;

void error(string str );
void handleKeyPress_start(XInfo &xinfo, XEvent &event, int &start_huh);
void handleKeyPress_skip(XInfo &xinfo, XEvent &event, int &start_huh);
void handlePause(XInfo &xinfo, unsigned long &lastRepaint);
void handleKeyPress_option(XInfo &xinfo, XEvent &event,int &stage);
void handleKeyPress(XInfo &xinfo, XEvent &event,Ship &ship, unsigned long &lastRepaint);
void handleTooSmall(XInfo &xinfo, XEvent event);
void handleAnimation(XInfo &xinfo, Ship &ship, unsigned long t, unsigned long slot);
void handleWin(XInfo xinfo, Ship ship);
void handleCrush(XInfo xinfo, Ship ship, int flag);
void handleResize(XInfo &xinfo, XEvent &event);

#endif /* _HANDLE_H_ */
