#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "handle.h"
#include "story.h"
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>



void opening(XInfo &xinfo)
{
	char *filename[31];
	filename[0] = (char *)"bitmap/open/open0.xbm";
	filename[1] = (char *)"bitmap/open/open1.xbm";
	filename[2] = (char *)"bitmap/open/open2.xbm";
	filename[3] = (char *)"bitmap/open/open3.xbm";
	filename[4] = (char *)"bitmap/open/open4.xbm";
	filename[5] = (char *)"bitmap/open/open5.xbm";
	filename[6] = (char *)"bitmap/open/open6.xbm";
	filename[7] = (char *)"bitmap/open/open7.xbm";
	filename[8] = (char *)"bitmap/open/open8.xbm";
	filename[9] = (char *)"bitmap/open/open9.xbm";
	filename[10] = (char *)"bitmap/open/open10.xbm";
	filename[11] = (char *)"bitmap/open/open11.xbm";
	filename[12] = (char *)"bitmap/open/open12.xbm";
	filename[13] = (char *)"bitmap/open/open13.xbm";
	filename[14] = (char *)"bitmap/open/open14.xbm";
	filename[15] = (char *)"bitmap/open/open15.xbm";
	filename[16] = (char *)"bitmap/open/open16.xbm";
	filename[17] = (char *)"bitmap/open/open17.xbm";
	filename[18] = (char *)"bitmap/open/open18.xbm";
	filename[19] = (char *)"bitmap/open/open19.xbm";
	filename[20] = (char *)"bitmap/open/open20.xbm";
	filename[21] = (char *)"bitmap/open/open21.xbm";
	filename[22] = (char *)"bitmap/open/open22.xbm";
	filename[23] = (char *)"bitmap/open/open23.xbm";
	filename[24] = (char *)"bitmap/open/open24.xbm";
	filename[25] = (char *)"bitmap/open/open25.xbm";
	filename[26] = (char *)"bitmap/open/open26.xbm";
	filename[27] = (char *)"bitmap/open/open27.xbm";
	filename[28] = (char *)"bitmap/open/open28.xbm";
	filename[29] = (char *)"bitmap/open/open29.xbm";
	filename[30] = (char *)"bitmap/open/open30.xbm";
	filename[31] = (char *)"bitmap/open/open31.xbm";
	
	Pixmap opening[31];
	int FPS = 30;
	XEvent event;
	unsigned long lastRepaint = 0;
	int frame = 0;
	int start_huh = 0;
	int time = 60000000;
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
	
	while( start_huh == 0) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress_skip(xinfo, event, start_huh);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;	
				
			}
		}
		
		// animation timing
		unsigned long end = now();
		if (frame == 31) 
		{
			//printf("set start to 1\n");
			start_huh = 1;
		}
		if (end - lastRepaint > time/FPS)
		{
			if (frame >= 16) 
				time = 90000000;
			//printf("frame is %d\n", frame);
			opening[frame] = loadbitmap(xinfo, filename[frame]);
			XCopyPlane(xinfo.display, opening[frame], xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
			
			std::string text1("PRESS 's' TO SKIP");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+350, (xinfo.current_h-600)/2+550, text1.c_str(), text1.length()  );
			
			frame++;
			lastRepaint = now();
		}


		// give the system time to do other things
		if (XPending(xinfo.display) == 0) {
			usleep(time/FPS - (end - lastRepaint));
		}
	} //end while
}




