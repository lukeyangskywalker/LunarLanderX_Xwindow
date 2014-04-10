#ifndef _MIST_H_
#define _MIST_H_


/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>


struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
	int current_w;		// current size
	int current_h;

	
};

// get microseconds
unsigned long now();

struct Ship {
	// variables to keep track ship's speed
	float v0; // speed at t0
	float vt; // speed at t1
	
	float old_v0; // store previous v0
	float vx; // speed on x axis
	
	float slot1; // store delta t
	
	float a; // acceleration
	
	// upper left coord of the ship
	float x;
	float y;
	
	// upper right coord
	float urx;
	float ury;
	
	// lower right of the ship
	float lrx;
	float lry;
	
	// lower left of the ship
	float llx;
	float lly;
	
	// center coord of the ship
	float cx;
	float cy;
	
	float ship_size;
	
	int fuel;
	// shows thruster direction, 0 if not using, 1 is using thruster
	// 0 - up, 1 - down, 2 - left, 3 - right
	int thruster[4];
	
	double ship_slop[4];
	double mountain_slop[4];
	int zone_index[4]; // stores which zone the ship is in

	void init(XInfo &xinfo, Ship &ship, int x, int y, int fuel);
	
	void move(XInfo &xinfo, Ship &ship, unsigned long t, unsigned long slot);
	
		
		void up(XInfo &xinfo, Ship &ship) ;
		void down(XInfo &xinfo, Ship &ship) ;
		void left(XInfo &xinfo, Ship &ship);
		void right(XInfo &xinfo, Ship &ship) ;
		
	
	
	void move_ship(XInfo xinfo, Ship &ship, int x, int y);
	void draw (XInfo xinfo, Ship &ship) ;


};


struct Terrain {

	// need to work on .. now background is constant...
	// pre-set landing coordnate
	int land_x1[2];
	int land_y[2] ;
	int land_x2[2] ;
	// array index  :  0  1  2  3  4  5  6 ...
	// array element: x0 y0 x1 y1 x2 y2 x3 ...
	int coord[80];
	int total_index;
	int border[80];
	int land_index[2];
	int land_pos[2];
	int total_landing_zone;
	
	int total_zone;
	
	int draw_flag;
	
	void init(Terrain &terrain);
	
	void draw (XInfo xinfo, Terrain &terrain) ;

};



struct C_Status {
	int x;
	void draw(XInfo xinfo, C_Status current_status, Ship ship, Terrain terrain);
	
}; // end C_Status;

int crush_huh (Ship &ship, Terrain &terrain);

#endif /* _MIST_H_ */
