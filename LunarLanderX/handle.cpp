#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "handle.h"

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}

	
	// handling q or space key on start screen, other keys are ignored
void handleKeyPress_start(XInfo &xinfo, XEvent &event, int &start_huh) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

 	if ( i == 1) {
		

		// space means start the game
		if (text[0] == ' ') {
			start_huh = 1;		
			printf("Got key press -- Space\n");
		}

		// q is quitting
		else if (text[0] == 'q') {
			printf("Got key press -- q\n");
			error("Terminating normally.");
		}
		else 
			printf("Got key press -- %c\n", text[0]);
	}

}

void handleKeyPress_skip(XInfo &xinfo, XEvent &event, int &start_huh) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

 	if ( i == 1) {
		

		// space means start the game
		if (text[0] == 's') {
			start_huh = 1;		
			printf("Got key press -- 's'\n");
		}

		// q is quitting
		else if (text[0] == 'q') {
			printf("Got key press -- q\n");
			error("Terminating normally.");
		}
		else 
			printf("Got key press -- %c\n", text[0]);
	}

}


void handlePause(XInfo &xinfo, unsigned long &lastRepaint)
{
		int x = 0;
		XEvent event;
		//unsigned long lastRepaint = 0;
		int inside = 0;

		// print to the screen notify user the game is paused
		std::string text_pause("--Pause--");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
             		xinfo.current_w/2, xinfo.current_h/2, text_pause.c_str(), text_pause.length()  );
		std::string text_resume("Press Space to Resume");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
             		xinfo.current_w/2-30, xinfo.current_h/2 + 20, text_resume.c_str(), text_resume.length()  );

		while (x == 0) { // stay looping untill space is pressed
		
			if (XPending(xinfo.display) > 0) {
				XNextEvent( xinfo.display, &event );
				switch( event.type ) {
					case KeyPress:
						KeySym key;
						char text[BufferSize];
						int i = XLookupString((XKeyEvent *)&event,text,BufferSize,&key,NULL);		
						if ( i == 1) {
							if (text[0] == ' ') {
								printf("Resume game\n");
								x = 1;
							}
							// allows user to quit while pausing
							else if (text[0] == 'q') {
								error("Terminating normally.");
							}
						}
					break;

				} // switch
			}// if xpending
		} // while
	lastRepaint = now();
}

void handleKeyPress_option(XInfo &xinfo, XEvent &event,int &stage) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);

		
		if (text[0] == 'q') {
			error("Terminating normally.");
		}
	}
	
	// THIS REALLY IS NOT THE RIGHT WAY TO HANDLE THE KEYS...
	// but since time is running out, I will just make a very
	// easy one, will enhance later
	
	if (stage == 11) 
	{
		switch( key) {
    		case XK_Up:
    		case XK_Down:
			if( key == XK_Up ) 
			{
				printf("Got key press -- Up\n");
				stage = 13;
				return;
			}
			if( key == XK_Down ) 
			{
				printf("Got key press -- Down\n");
				stage = 12;
				return;
			}
		}
		if (text[0] == ' ')
		{
			printf("Got key press -- Space\n");
			stage = 111;
			return;
		}
	}
	
	if (stage == 12) 
	{
		switch( key) {
    		case XK_Up:
    		case XK_Down:
			if( key == XK_Up ) 
			{
				printf("Got key press -- Up\n");
				stage = 11;
				return;
			}
			if( key == XK_Down ) 
			{
				printf("Got key press -- Down\n");
				stage = 13;
				return;
			}
		}
		if (text[0] == ' ')
		{
			printf("Got key press -- Space\n");
			stage = 121;
			return;
		}
	}
	
	if (stage == 13) 
	{
		switch( key) {
    		case XK_Up:
    		case XK_Down:
			if( key == XK_Up ) 
			{
				printf("Got key press -- Up\n");
				stage = 12;
				return;
			}
			if( key == XK_Down ) 
			{
				printf("Got key press -- Down\n");
				stage = 11;
				return;
			}
		}
		
		if (text[0] == ' ')
		{
			printf("Got key press -- Space\n");
			stage = 131;
			return;
		}
	}
	
	if (stage == 131) 
	{
		
		if (text[0] == ' ')
		{
			printf("Got key press -- Space\n");
			stage = 11;
			return;
		}
	}
	if (stage == 121) 
	{
		
		if (text[0] == ' ')
		{
			printf("Got key press -- Space\n");
			stage = 11;
			return;
		}
	}
	
		printf("waiting for key...\n");
		return;

} // end option

void handleKeyPress(XInfo &xinfo, XEvent &event,Ship &ship, unsigned long &lastRepaint) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

switch( key) {
    case XK_Up:
    case XK_Down:
    case XK_Left:
    case XK_Right:
	
	if( key == XK_Up ) {
		printf("Got key press -- Up\n");
		//xinfo.y = xinfo.y - 5;
		//ship.move_ship(ship, ship.x, ship.y-5);
		ship.up(xinfo, ship);

	}
    else if( key == XK_Left ){
		printf("Got key press -- Left\n");
		//xinfo.x = xinfo.x - 5;
		//ship.x = ship.x -5;
		//ship.move_ship(ship, ship.x-5, ship.y);
		ship.left(xinfo, ship);
	}
    else if( key == XK_Right ){
		printf("Got key press -- Right\n");
		//xinfo.x = xinfo.x + 5;
		//ship.x = ship.x + 5;
		//ship.move_ship(ship,ship.x+5,  ship.y);
		ship.right(xinfo, ship);
	}
	
	else if( key == XK_Down ){
		printf("Got key press -- Down\n");
		//xinfo.x = xinfo.x + 5;
		//ship.x = ship.x + 5;
		//ship.move_ship(ship,ship.x,  ship.y+5);
		ship.down(xinfo, ship);
	}
}

	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);

		/*if (text[0] == 'a' ) {
			ship.move_ship(ship, ship.x-1, ship.y);
		}
		if (text[0] == 'd' ) {
			ship.move_ship(ship,ship.x+1,  ship.y);
		}
		if (text[0] == 'w' ) {
			ship.move_ship(ship, ship.x, ship.y-1);

		}
		// moving down for testing
		if (text[0] == 's' ) {
			ship.move_ship(ship,ship.x,  ship.y+1);

		}*/
		// now handle pause
		if (text[0] == ' ') {
			handlePause(xinfo, lastRepaint);
		} // if space

		if (text[0] == 'q') {
			error("Terminating normally.");
		}
	}

}

void handleTooSmall(XInfo &xinfo, XEvent event)
{
	int x = 0;
	XEvent event2;
	unsigned long lastRepaint = 0;
	int inside = 0;
		XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
		
		std::string text_pause("--Pause--");
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], xinfo.current_w/2, xinfo.current_h/2, text_pause.c_str(), text_pause.length()  );
		std::string text_resume("The window is too small, please resize the window");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], xinfo.current_w/2, xinfo.current_h/2 + 20, text_resume.c_str(), text_resume.length()  );

		while (x == 0) { // stay looping untill window size exceed 800*600
		
		
		// print to the screen notify user the game window is too small
		
		
			if (XPending(xinfo.display) > 0) {
				XNextEvent( xinfo.display, &event2 );
				switch( event2.type ) {
					case ConfigureNotify:
					{
						XConfigureEvent xce = event2.xconfigure;
						fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
						xinfo.current_w = xce.width;
						xinfo.current_h = xce.height;
						if (xce.width >= 800 && xce.height >= 600) 
						{
							printf("w > 800 and h > 600 resuming the game!!\n");
							x = 1;

						}
						else 
						{
							XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
		
							std::string text_pause("--Pause--");
							XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], xinfo.current_w/2, xinfo.current_h/2, text_pause.c_str(), text_pause.length()  );
							std::string text_resume("The window is too small");
							XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], xinfo.current_w/2-30, xinfo.current_h/2 + 20, text_resume.c_str(), text_resume.length()  );
							text_resume="Please resize to continue";
							XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], xinfo.current_w/2-35, xinfo.current_h/2 + 40, text_resume.c_str(), text_resume.length()  );
							
						
						}
						break;
					}
					case KeyPress:
					{
						KeySym key;
						char text[BufferSize];
						int i = XLookupString((XKeyEvent *)&event2,text,BufferSize,&key,NULL);		
						if ( i == 1) {
							if (text[0] == 'q') {
								error("Terminating normally.");
							}
						}
						break;
					}

				} // switch
			}// if xpending
		} // while

} // end handleTooSmall

void handleAnimation(XInfo &xinfo, Ship &ship, unsigned long t, unsigned long slot) {
	ship.move(xinfo, ship, t, slot);
}


void handleWin(XInfo xinfo, Ship ship)
{

	Pixmap ship_land[16];
	
	ship_land[0] = loadbitmap(xinfo, (char *)"bitmap/landing/landing1.xbm");

	ship_land[1] = loadbitmap(xinfo, (char *)"bitmap/landing/landing2.xbm");

    ship_land[2] = loadbitmap(xinfo, (char *)"bitmap/landing/landing3.xbm");

    ship_land[3] = loadbitmap(xinfo, (char *)"bitmap/landing/landing4.xbm");

    ship_land[4] = loadbitmap(xinfo, (char *)"bitmap/landing/landing5.xbm");

    ship_land[5] = loadbitmap(xinfo, (char *)"bitmap/landing/landing6.xbm");
    
    ship_land[6] = loadbitmap(xinfo, (char *)"bitmap/landing/landing7.xbm");

	ship_land[7] = loadbitmap(xinfo, (char *)"bitmap/landing/landing8.xbm");

    ship_land[8] = loadbitmap(xinfo, (char *)"bitmap/landing/landing9.xbm");

    ship_land[9] = loadbitmap(xinfo, (char *)"bitmap/landing/landing10.xbm");

    ship_land[10] = loadbitmap(xinfo, (char *)"bitmap/landing/landing11.xbm");

    ship_land[11] = loadbitmap(xinfo, (char *)"bitmap/landing/landing12.xbm");
    
    ship_land[12] = loadbitmap(xinfo, (char *)"bitmap/landing/landing13.xbm");

	ship_land[13] = loadbitmap(xinfo, (char *)"bitmap/landing/landing14.xbm");

    ship_land[14] = loadbitmap(xinfo, (char *)"bitmap/landing/landing15.xbm");

    ship_land[15] = loadbitmap(xinfo, (char *)"bitmap/landing/landing16.xbm");


	int i;
	int j;
	
	for (i = 0; i<=15; i++) {
		for (j = 0; j <30000; j ++)
		{
		XCopyPlane(xinfo.display, ship_land[i], xinfo.window, xinfo.gc[0],
                    0, 0, 20, 20, ship.x, ship.y, 1);
		}
		//usleep(1000000);
	
	
	}
	
	Pixmap win[5];
	
	win[0] = loadbitmap(xinfo, (char *)"bitmap/win/win0.xbm");
	win[1] = loadbitmap(xinfo, (char *)"bitmap/win/win1.xbm");
	win[2] = loadbitmap(xinfo, (char *)"bitmap/win/win3.xbm");
	win[3] = loadbitmap(xinfo, (char *)"bitmap/win/win4.xbm");
	win[4] = loadbitmap(xinfo, (char *)"bitmap/win/win5.xbm");
	
	
	for (i = 0; i<=4; i++) {
		for (j = 0; j <1000; j ++)
		{
			if (i>=4){
				j += 500;
				}
								
		XCopyPlane(xinfo.display, win[i], xinfo.window, xinfo.gc[0],
                    0, 0, 800, 600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2, 1);
		}
		//usleep(1000000);
	
	
	}
	
	/*
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w-800)/2, (xinfo.current_h-600)/2, 800, 600);
				std::string text("YOU WIN!!!");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1],
               			xinfo.current_w/2 - 10, xinfo.current_h/2 - 10, text.c_str(), text.length()  );
               			
               	text=("Press space to play again");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1],
                		xinfo.current_w/2 - 45, xinfo.current_h/2 + 20, text.c_str(), text.length()  );*/

}

void handleCrush(XInfo xinfo, Ship ship, int flag)
{
	int current_w;		// current size
	int current_h;
	
	Pixmap ship_crush[5];
	
	ship_crush[0] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush1.xbm");

	ship_crush[1] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush2.xbm");

    ship_crush[2] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush3.xbm");

    ship_crush[3] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush4.xbm");

    ship_crush[4] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush5.xbm");

    ship_crush[5] = loadbitmap(xinfo, (char *)"bitmap/crush/ship_crush6.xbm");

	int i;
	int j;
	
	for (i = 0; i<=5; i++) {
		for (j = 0; j <10000; j ++)
		{
		XCopyPlane(xinfo.display, ship_crush[i], xinfo.window, xinfo.gc[0],
                    0, 0, 20, 20, ship.x, ship.y, 1);
		}
		//usleep(1000000);
	
	
	}
	

	std::string text("YOUR SHIP CRUSHED!!!");
	XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
                		xinfo.current_w/2 - 10, xinfo.current_h/2 - 10, text.c_str(), text.length()  );
/*    
    if (flag == 0) 
	    text=("You let the ship crush on the mountain, you failed to save the planet... you should feel the guilt now, and you should definitely try again and beat the game");
	else if (flag == 1) 
	    text=("It was such a tragic... you tried so hard and get so far... but the speed was too fast...");

 	XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], xinfo.current_w/2 - 100, xinfo.current_h/2 + 20, text.c_str(), text.length()  );    
                	*/
    
    Pixmap end;
    
    if (flag == 0)
    {
    	Pixmap end = loadbitmap(xinfo, (char*) "bitmap/others/mountain.xbm");
			XCopyPlane(xinfo.display, end, xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
	}
	
	else if (flag == 1)
    {
    	Pixmap end = loadbitmap(xinfo, (char*) "bitmap/others/speed.xbm");
			XCopyPlane(xinfo.display, end, xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
	}
	
	text=("Press space to try again");
	XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1],
                		xinfo.current_w/2 - 65, xinfo.current_h/2 + 270, text.c_str(), text.length()  );
}


// update width and height when window is resized
void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	
	xinfo.current_w = xce.width;
	xinfo.current_h = xce.height;
		XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0,0,xinfo.current_w,xinfo.current_h);
	
	if (xce.width < 800 || xce.height < 600) 
	{	
		handleTooSmall(xinfo, event);
		//handlePause(xinfo);
	} // if window too small
	

} // end handle resize


