 #include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>		/* sleep(), etc.  */
#include "loadbitmap.h"
#include "mist.h"

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>


 Pixmap loadbitmap(XInfo xinfo, char *filename)
 {
    /* this variable will contain the ID of the newly created pixmap.    */
    Pixmap bitmap;
    /* these variables will contain the dimensions of the loaded bitmap. */
    unsigned int bitmap_width, bitmap_height;
    /* these variables will contain the location of the hotspot of the   */
    /* loaded bitmap.                                                    */
    int hotspot_x, hotspot_y;

    /* load the bitmap found in the file "icon.bmp", create a pixmap     */
    /* containing its data in the server, and put its ID in the 'bitmap' */
    /* variable.                                    
                         */
                         
    //char *filename = "ship_normal.xbm";
    int rc = XReadBitmapFile(xinfo.display, xinfo.window,
                            filename,
                             &bitmap_width, &bitmap_height,
                             &bitmap,
                             &hotspot_x, &hotspot_y);
    /* check for failure or success. */
    switch (rc) {
        case BitmapOpenFailed:
            fprintf(stderr, "XReadBitmapFile - could not open file '%s'.\n", filename);
	    exit(1);
            break;
        case BitmapFileInvalid:
            fprintf(stderr,
                    "XReadBitmapFile - file '%s' doesn't contain a valid bitmap.\n",
                    filename);
	    exit(1);
            break;
        case BitmapNoMemory:
            fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
	    exit(1);
            break;
    }

    /* start drawing the given pixmap on to our window. */
    return bitmap;
  } // end bitmapread
