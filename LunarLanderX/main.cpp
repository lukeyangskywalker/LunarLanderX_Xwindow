/*
 * Luke's first xwindow game
 */
#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mist.h"
#include "loadbitmap.h"
#include "handle.h"
#include "story.h"
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
const int Border = 10;
//const int BufferSize = 10;
const int FPS = 30;

void repaint (XInfo xinfo, Ship &ship, Terrain &terrain, C_Status current_status) {
	// wipe background
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0,0,xinfo.current_w,xinfo.current_h);
	XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2, (xinfo.current_h-600)/2, 800, 600);
	ship.draw(xinfo, ship);
	terrain.draw(xinfo, terrain);
	current_status.draw(xinfo, current_status, ship, terrain);

}

/*
 * Initialize X and create a window
 */
//void initX(int argc, char *argv[],XInfo &xInfo) {
void initX(XInfo &xInfo) {
	//unsigned long white, black;

	//cout<<"starting main...\n";
   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	xInfo.screen = DefaultScreen(xInfo.display);


	xInfo.width = 800;
	xInfo.height = 600;
	xInfo.current_h = 600;
	xInfo.current_w = 800;
	
	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		10, 10,			// upper left corner location
		xInfo.width, xInfo.height,	// size of the window
		2,						// width of window's border
		BlackPixel(xInfo.display, xInfo.screen), 
		WhitePixel(xInfo.display, xInfo.screen)); 	// window background colour

	/* 
	 * Create Graphics Contexts
	 */

	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);


	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask | 
		KeyReleaseMask |
		PointerMotionMask | 
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask);  // for resize events

	XMapRaised(xInfo.display, xInfo.window);  
	XFlush(xInfo.display); 
	sleep(1);	// let server get set up before sending drawing commands
}



// opening screen animation. only two key works here:
// q to quit and space to play the game

void eventLoop_start(XInfo &xinfo) {

	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;
	int start_huh = 0;
	//XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
	
	char *filename[2];
	filename[0] = (char *)"bitmap/others/start.xbm";
	filename[1] = (char *)"bitmap/others/start1.xbm";
	Pixmap start[2];
	start[0] = loadbitmap(xinfo, filename[0]);
	start[1] = loadbitmap(xinfo, filename[1]);
	int frame = 0;
	while( start_huh == 0 ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress_start(xinfo, event, start_huh);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;	
				
			}
		}
		
		// animation timing
		unsigned long end = now();
	
		
			                  
                  
        if (end - lastRepaint > 1000000/10)
		{
			XCopyPlane(xinfo.display, start[frame], xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
			
			/*std::string text1("PRESS SPACE TO SKIP");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+350, (xinfo.current_h-600)/2+550, text1.c_str(), text1.length()  );
			*/
			if (frame == 1)
				frame = 0;
			else 
				frame = 1;
			lastRepaint = now();
		
			std::string text("Press q to quit");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1],
                  10, 10, text.c_str(), text.length()  );  
		}
	/*	if (end%2 == 0) {
			std::string text1("PRESS SPACE TO START");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], 350, 290, text1.c_str(), text1.length()  );
			lastRepaint = now();
		}
		else if (end%2 == 1) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 350, 270, 130, 50);

			lastRepaint = now();
		}*/

		// give the system time to do other things
		if (XPending(xinfo.display) == 0) {
			usleep(1000000/10 - (end - lastRepaint));
		}
	}
}

void eventLoop_option(XInfo &xinfo) {

	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;
	int option = 0;
	//XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 0, 0, 800, 600);
	
	char *filename[2];
	filename[0] = (char *)"bitmap/others/start.xbm";
	//filename[1] = (char *)"bitmap/others/start1.xbm";
	Pixmap start[2];
	start[0] = loadbitmap(xinfo, filename[0]);
	Pixmap pointer = loadbitmap(xinfo, (char *)"bitmap/others/ship_normal.xbm");
	Pixmap howtoplay= loadbitmap(xinfo, (char *) "bitmap/others/keyboard.xbm");
	Pixmap about = loadbitmap(xinfo, (char *) "bitmap/others/about.xbm");
	//start[1] = loadbitmap(xinfo, filename[1]);
	int frame = 0;
	int stage = 11;
	// stage 11 : pointer to start
	// stage 12 : pointer to option
	// stage 13 : pointer to about 
	/*
	option:
	0 = back to start screen
	1 = 
	
	*/
	while( stage != 111 ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress_option(xinfo, event, stage);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;	
				
			}
		}
		
		// animation timing
		unsigned long end = now();
	
		
			                  
                  
        if (end - lastRepaint > 1000000/FPS)
		{
			if (stage == 11 ||stage == 12 ||stage == 13)
			{
				XCopyPlane(xinfo.display, start[0], xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
			
				std::string text1("NEW GAME");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+350, (xinfo.current_h-600)/2+450, text1.c_str(), text1.length()  );
			
				text1=("HOW TO PLAY");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+350, (xinfo.current_h-600)/2+480, text1.c_str(), text1.length()  );
				
				text1=("ABOUT");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+350, (xinfo.current_h-600)/2+510, text1.c_str(), text1.length()  );
			
				if (stage ==11) 
					XCopyPlane(xinfo.display, pointer, xinfo.window, xinfo.gc[1], 0, 0, 20,20,(xinfo.current_w-800)/2+320, (xinfo.current_h-600)/2+435,1);
				else if (stage ==12) 
					XCopyPlane(xinfo.display, pointer, xinfo.window, xinfo.gc[1], 0, 0, 20,20,(xinfo.current_w-800)/2+320, (xinfo.current_h-600)/2+465,1);
				else if (stage ==13) 
					XCopyPlane(xinfo.display, pointer, xinfo.window, xinfo.gc[1], 0, 0, 20,20,(xinfo.current_w-800)/2+320, (xinfo.current_h-600)/2+495,1);
			
			std::string text="Use arrow keys to move, space to confirm";
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+290, (xinfo.current_h-600)/2+570, text.c_str(), text.length()  );
			
			}
			else if (stage == 131) //about page
			{
				XCopyPlane(xinfo.display, about, xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
			
				std::string text="press space to go back";
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+300, (xinfo.current_h-600)/2+570, text.c_str(), text.length()  );
			}
			
			else if (stage == 121) //about page
			{
				XCopyPlane(xinfo.display, howtoplay, xinfo.window, xinfo.gc[0],
                    0, 0, 800,600, (xinfo.current_w-800)/2, (xinfo.current_h-600)/2,1);
			
				std::string text="press space to go back";
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], (xinfo.current_w-800)/2+300, (xinfo.current_h-600)/2+570, text.c_str(), text.length()  );
			}
			
			lastRepaint = now();
		
			std::string text("Press q to quit");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1],
                  10, 10, text.c_str(), text.length()  );  
                  
                  
            
		}
	/*	if (end%2 == 0) {
			std::string text1("PRESS SPACE TO START");
			XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], 350, 290, text1.c_str(), text1.length()  );
			lastRepaint = now();
		}
		else if (end%2 == 1) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], 350, 270, 130, 50);

			lastRepaint = now();
		}*/

		// give the system time to do other things
		if (XPending(xinfo.display) == 0) {
			usleep(1000000/10 - (end - lastRepaint));
		}
	}
}

void eventLoop(XInfo &xinfo, Ship &ship, Terrain &terrain, C_Status &current_status) {
	// Add stuff to paint to the display list
/*
	dList.push_front(&ball);
	dList.push_front(&left_eye);
	dList.push_front(&right_eye);
	*/
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;
	
	int time_flag = 1; // at the beginning of game, flag = 1
	unsigned long t = 0;
	unsigned long slot = 0;
	
	int index = 1;
	while( true ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case ButtonPress:
					//handleButtonPress(xinfo, event);
					cout<<"Button Press\n";					
					break;
				case KeyPress:
				{
/*				    XEvent new_event;
         			XPeekEvent(xinfo.display, &new_event);
         			if (new_event.type == KeyPress && new_event.xkey.keycode != event.xkey.keycode)
         			{
         				handleKeyPress(xinfo, new_event, ship);
         				printf("got different key\n");
         			
         			}
   			
*/					handleKeyPress(xinfo, event, ship, lastRepaint);
					//cout<<"Key Press\n";	
					break;
				}
				case KeyRelease:
					cout<<"Got key release\n";
					break;	
				/*case KeyRelease:
				{
     				unsigned short newkey = 0;

    				if (XEventsQueued(xinfo.display, QueuedAfterReading))
       				{
         				XEvent new_event;
         				XPeekEvent(xinfo.display, &new_event);

         				if (new_event.type == KeyPress )
         				//&& new_event.xkey.keycode != event.xkey.keycode)
           				{
           					//handleKeyPress(xinfo, new_event, ship);
           					printf("key press different key\n");
             				//fprintf (stdout, "key #%ld was retriggered.\n", (long) XLookupKeysym (&new_event.xkey, 0));

            	 			// delete retriggered KeyPress event
             				//XNextEvent (xinfo.display, &event);
             				newkey = 1;
           				}
       				}

    				if (newkey==0)
       					printf ("same key repeating\n");
   				
   					break;
   				}*/
 
				case MotionNotify:
					//handleMotion(xinfo, event, inside);
					//cout<<"Motion Notify\n";	
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					//cout<<"config notify\n";	
					break;	
			}
		}
		
		// animation timing
		unsigned long end = now();
		if (time_flag == 1) 
		{
			t = 0;
		}
		
		else 
		{
			t = t + end - lastRepaint;
			slot = end - lastRepaint; 	
		}
		
		if (end - lastRepaint > 1000000/FPS || time_flag == 1) {
			time_flag = 0;
			
			handleAnimation(xinfo, ship, t, slot);
			
			if ((ship.x >= terrain.land_x1[0] && ship.lrx <= terrain.land_x2[0] && ship.lry >= terrain.land_y[0]) || (ship.x >= terrain.land_x1[1] && ship.lrx <= terrain.land_x2[1] && ship.lry >= terrain.land_y[1]))
			{
				if (ship.v0 <= 25)
				{
				handleWin(xinfo, ship);
               	break;
               	}
               	else
               	{
               		//printf("Too fast!!\n");
               		handleCrush(xinfo, ship, 1);
               		break;
               	}
				
			}
			
		
			
			// test if ship crashes
  			if (crush_huh(ship, terrain))
  			{
  			
  				handleCrush(xinfo, ship, 0);
  				/*//XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[1], 0, 0, 800, 600);
				std::string text("YOUR SHIP CRUSHED!!!");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
                		390, 290, text.c_str(), text.length()  );
                	
                text=("Press space to try again");
				XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
                		375, 320, text.c_str(), text.length()  );*/
                break;
  			}
  			
			else {
			//xinfo.y+=index;
			//ship.y+=index;
			//crush_huh(ship, terrain);
			repaint(xinfo, ship, terrain, current_status);
			int a = 1;
			int b = 0;	
			}
			lastRepaint = now();
		}
		
		
		
		// give the system time to do other things
		if (XPending(xinfo.display) == 0) {
			usleep(1000000/FPS - (end - lastRepaint));
		}
	}
}

void new_game(XInfo xinfo);
void eventLoop_end(XInfo &xinfo) {

	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;
	int start_huh = 0;
	while( start_huh == 0 ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress_start(xinfo, event, start_huh);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;	
				
			}
		}
		
		// animation timing
		unsigned long end = now();
	}
	new_game(xinfo);
}


void new_game(XInfo xInfo) {

	
	Ship ship;
	Terrain terrain;
	C_Status current_status;
	
	ship.init(xInfo, ship, 430, 40, 500);
	terrain.init(terrain);

	// go to the start screen, press space to play the game
	printf("current screen size is %d x %d\n", xInfo.current_w, xInfo.current_h);
	eventLoop_start(xInfo);
	
	// option screen
	eventLoop_option(xInfo);
	
	//XFillRectangle(xInfo.display, xInfo.window, xInfo.gc[0], 50, 50, 20, 20);
	eventLoop(xInfo, ship, terrain, current_status);     
	
	// go to the game over screen and press space to play again (jump back to eventLoop)
	eventLoop_end(xInfo);
	
	XCloseDisplay(xInfo.display);

}
int main () {
	XInfo xInfo;
	initX(xInfo);
	opening(xInfo);
	new_game(xInfo);
	
}
