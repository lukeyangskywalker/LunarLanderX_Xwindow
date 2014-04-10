#include <iostream>
#include <sstream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>		/* sleep(), etc.  */
#include "mist.h"
#include "loadbitmap.h"
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//using namespace std;


// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void Ship::init(XInfo &xinfo,Ship &ship, int x, int y, int fuel)
	{
		ship.ship_size = 18;
		ship.x = (xinfo.current_w-800)/2 + x;
		ship.y = (xinfo.current_h-600)/2 + y;
		ship.lrx = x + ship.ship_size;
		ship.lry = y + ship.ship_size;
		ship.llx = x;
		ship.lly = y + ship.ship_size;
		ship.urx = x + ship.ship_size;
		ship.ury = y;
		
		ship.cx = (ship.x + lrx)/2;
		ship.cy = (ship.y + lry)/2;
		
		ship.v0 = 0;
		ship.a = 1.5;
		ship.vx = 0;
		ship.fuel = fuel;
	
	}
void Ship::move(XInfo &xinfo, Ship &ship, unsigned long t, unsigned long slot) 
	{
			//tt = (float)t/1000000;
			ship.slot1 = (float)slot/1000000;
			ship.vt = v0 + ship.a * ship.slot1;	
			
			ship.y = ship.y + (ship.vt*ship.vt - ship.v0*ship.v0)/2*ship.a;
			//ship.y = ship.y + (ship.vt^1.5 - ship.v0^1.5)/2*ship.a;
			
			ship.x = ship.x + ship.vx;			

			//printf("v0 = %f, vt = %f, slot = %f, x = %f, y = %f\n", ship.v0, ship.vt, ship.slot1, ship.x, ship.y);
			
			if (ship.old_v0 > ship.v0) // goind up
			{
			//printf("going up\n");
				if (ship.vt >=9.5 && ship.vt <= 12.80) {
					ship.vt -= 11;
					}
			
			
			}
			else if (ship.old_v0 <= ship.v0) // going down
			{
			//printf("going down\n");
				if (ship.vt >=0.100 && ship.vt <= 2.0) {
					ship.vt += 12;
					}
			
			}
			ship.old_v0 =ship.v0;
			ship.v0 = ship.vt;		
						
			ship.move_ship(xinfo, ship, ship.x, ship.y);
	} // end move
		
		void Ship::up(XInfo &xinfo, Ship &ship) {
			if (ship.fuel >0)
			{
			ship.fuel -= 1;
			//ship.y -= 5;
			ship.v0 = ship.v0 - 0.2;
			ship.thruster[0] = 1;
			//printf("v0 = %f \n", v0);
			}
			else
			 ship.fuel = 0;
		}
		void Ship::down(XInfo &xinfo, Ship &ship) {
		if (ship.fuel >0)
			{
			ship.fuel -= 1;
			//ship.y += 5;
			ship.v0 = ship.v0 + 0.3;
			ship.thruster[1] = 1;
			//printf("v0 = %f \n", v0);
			}
		else 
			ship.fuel = 0;
		}
		void Ship::left(XInfo &xinfo, Ship &ship) {
		if (ship.fuel >0)
			{
			ship.fuel -= 1;
			//ship.x -= 5;
			//tt = (float)slot/1000000;		
			ship.vx = ship.vx - 0.1;	
			ship.thruster[2] = 1;		
			//x = x - vx;
			}
		else
			 ship.fuel = 0;
		}
		void Ship::right(XInfo &xinfo, Ship &ship) {
		if (ship.fuel >0)
			{
			ship.fuel -= 1;
			//ship.x += 5;
			ship.vx = ship.vx + 0.1;
			ship.thruster[3] = 1;
			}
		else
			 ship.fuel = 0;
		}
		
	
	
void Ship::move_ship(XInfo xinfo, Ship &ship, int x, int y)
	{
	//printf("current w is %d, h is %d x is %d y is %d\n", xinfo.current_w, xinfo.current_h, x, y);
		//ship.x = (xinfo.current_w-800)/2 + x;
		//ship.y = (xinfo.current_h-600)/2 + y;
		ship.x = x;
		ship.y = y;
		ship.lrx = x + ship.ship_size;
		ship.lry = y + ship.ship_size;
		ship.llx = x;
		ship.lly = y + ship.ship_size;
		ship.urx = x + ship.ship_size;
		ship.ury = y;
		
		ship.cx = (ship.x + lrx)/2;
		ship.cy = (ship.y + lry)/2;
	}
	
void Ship::draw (XInfo xinfo, Ship &ship) {
	
	Pixmap ship_normal = loadbitmap(xinfo, (char *)"bitmap/others/ship_normal.xbm");
		

		
		if (ship.thruster[0] == 1) // going up
		{
			XCopyPlane(xinfo.display, ship_normal, xinfo.window, xinfo.gc[0],
                    0, 0, 20, 20, ship.x, ship.y, 1);
			if (now()%3 == 0)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.llx+6, ship.lly+2 , ship.cx, ship.cy+18);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx, ship.cy+18, ship.lrx-6, ship.lry+1 );
			}
			else if (now()%3 == 1)
			{
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.llx+6, ship.lly+2, ship.cx, ship.cy+23);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx, ship.cy+23, ship.lrx-6, ship.lry+1);
			
			}
			else if (now()%3 == 2)
			{
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.llx+6, ship.lly+2, ship.cx, ship.cy+28);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx, ship.cy+28, ship.lrx-6, ship.lry+1);
			
			}
		
		} // if thruster up
		
		else if (ship.thruster[2] == 1)  // if going left
		{
		
			Pixmap ship_left = loadbitmap(xinfo, (char *)"bitmap/move/left1.xbm");
			XCopyPlane(xinfo.display, ship_left, xinfo.window, xinfo.gc[0],
                    0, 0, 20, 20, ship.x, ship.y, 1);
                    			
			if (now()%3 == 0)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.urx+2, ship.ury+10 , ship.cx +18, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx+18, ship.cy+3, ship.lrx+2, ship.lry - 6 );
			}
			
			else if (now()%3 == 1)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.urx+2, ship.ury+10 , ship.cx +23, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx+23, ship.cy+3, ship.lrx+2, ship.lry - 6 );
			}
			
			else if (now()%3 == 2)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.urx+2, ship.ury+10 , ship.cx +28, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx+28, ship.cy+3, ship.lrx+2, ship.lry - 6 );
			}
		
		} // if going left
		
		else if (ship.thruster[3] == 1) //going right
		{
		
			Pixmap ship_right = loadbitmap(xinfo, (char *)"bitmap/move/right1.xbm");
			XCopyPlane(xinfo.display, ship_right, xinfo.window, xinfo.gc[0],
                    0, 0, 20, 20, ship.x, ship.y, 1);
                    
			if (now()%3 == 0)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.x-2, ship.y+10 , ship.cx -18, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx-18, ship.cy+3, ship.llx-2, ship.lly - 6 );
			}
			else if (now()%3 == 1)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.x-2, ship.y+10 , ship.cx -23, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx-23, ship.cy+3, ship.llx-2, ship.lly - 6 );
			}
			
			else if (now()%3 == 2)
			{		
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.x-2, ship.y+10 , ship.cx -28, ship.cy+3);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], ship.cx-28, ship.cy+3, ship.llx-2, ship.lly - 6 );
			}
		
		} // if going right
		
		else 
		{
			
		
			XCopyPlane(xinfo.display, ship_normal, xinfo.window, xinfo.gc[0],
                    0, 0,
                    20, 20,
                    ship.x, ship.y,
                    1);
		
		}
		
		// clear all thrusters
		int i;
		for (i = 0; i < 4; i ++)
		{
			ship.thruster[i] = 0;
		}
	}


	
void Terrain::init(Terrain &terrain)
	{
		terrain.draw_flag = 0;
		// setting a seed for rand()
		srand(time(NULL));
		int x;
		for (x=0;x<80;x++)
		{
			terrain.coord[x] = 999;
		}
		
		for (x=0;x<80;x++)
		{
			terrain.border[x] = 999;
		}
		
		terrain.total_landing_zone = 2;
		// generate the landing place
		// now, only two landing zone
		// x1[0] < x1[1]
		terrain.land_x1[0] = rand() % (390-20+1) + 20;
		terrain.land_x2[0] = land_x1[0] + 40;
		terrain.land_y[0] = rand() % (590-400+1) + 400;
		
		terrain.land_x1[1] = rand() % (760-450+1) + 450;
		terrain.land_x2[1] = land_x1[1] + 40;
		terrain.land_y[1] = rand() % (590-500+1) + 500;
		
		//printf (" landing zone 0 is : %d ~ %d, zone 1 is : %d ~ %d\n",terrain.land_x1[0],terrain.land_x2[0],terrain.land_x1[1], terrain.land_x2[1]);
		
		terrain.total_zone = 16;
		terrain.total_index = (terrain.total_zone+1) * 2;
		
		int zone_size = 800/terrain.total_zone; // size for each zone
												// will be used to rand x
		
		int low_low = 590;
		int low_high = 500;
		int high_low = 490;
		int high_high = 350;

		//int prev_y = rand() % (high_high - low_low +1 ) + low_low;
		int prev_y = 500;
		int i;
		//printf("total index is : %d\n", terrain.total_index);
		for (i =0; i <= terrain.total_index+1 ; i = i++) 
		{
			if ( i == 0)
			{
				terrain.coord[0] = 0; // x0
				continue;
			}

			
			if (i%2 == 0) // even means x
			{
				int low = (i/2) * zone_size;
				int high = low + zone_size - (zone_size/10);
				terrain.coord[i] = rand() % (high-low+1) + low;
			
			
				// check if the x-coord has a collosion with the landing x
				//printf("terrain coord is %d, land_x1 is %d, land_x2 is %d\n", terrain.coord[i], terrain.land_x1[0], terrain.land_x2[0]);
				while ((terrain.coord[i] >= terrain.land_x1[0] && terrain.coord[i] <= terrain.land_x2[0]) || (terrain.coord[i] >= terrain.land_x1[1] && terrain.coord[i] <= terrain.land_x2[1]))
				{
				//	printf("re-rand terrain coord...\n");
					terrain.coord[i] = rand() % (high-low+1) + low;
				//	printf("new terrain coord is %d\n", terrain.coord[i]);
				}
			

			}
			else // odd means y
			{
				if (prev_y >= 560) 
				{				
					prev_y -= 40;
				}

				int high = prev_y + 40;
				int low = prev_y - 40;
	
			
				terrain.coord[i] = rand() % (high - low+1) + low;
				prev_y = terrain.coord[i];
			
			}
			//printf("i is : %d, number is : %d\n", i , terrain.coord[i]);
		}

		/*for (i=0;i<45;i++)
		{
			if (i%2 == 0) 
			printf("index %d is: %d ", i, terrain.coord[i]);
			else 
			printf("index %d is: %d\n", i, terrain.coord[i]);
		}*/
		
		///////////////////////////////////////////////////////////////
		// now make the terrain.border/////////////////////////////////
		///////////////////////////////////////////////////////////////
		
		terrain.draw_flag = 1;
		//terrain.land_pos[terrain.total_landing_zone];
		//int x;
		
		for (i=0; i <= terrain.total_index+1; i++)
		{
			terrain.border[i] = terrain.coord[i];
		
		}
		
		// insert two landing zone (eight coord into terrain.border)
		for (x = 0; x < 2; x ++)
		{
			// first find the index to insert
			for (i = 0; i <= terrain.total_index+1; i= i+2) 
			{
				
				if (terrain.border[i] < terrain.land_x1[x] && terrain.land_x2[x] < terrain.border[i+2]) 
				{
					land_pos[x] = i+2;
					terrain.land_index[x] = i+2;
					
					// found the spot, now replace and push everything else back
					int j;
					int tempx1 ;
					int tempy1 ;
					int tempx2 ;
					int tempy2 ;
						
					int insertx1 = terrain.land_x1[x];
					int inserty1 = terrain.land_y[x];
					int insertx2 = terrain.land_x2[x];
					int inserty2 = terrain.land_y[x];
					//(terrain.total_zone+6)*2-4
					for (j = i+2; j < 70; j = j +4)
					{
						//printf("pushing index : %d\n", j);
						tempx1 = terrain.border[j];
						tempy1 = terrain.border[j+1];
						tempx2 = terrain.border[j+2];
						tempy2 = terrain.border[j+3];
											
						terrain.border[j] = insertx1;
						terrain.border[j+1] = inserty1;
						terrain.border[j+2] = insertx2;
						terrain.border[j+3] = inserty2;
					
						insertx1 = tempx1;
						inserty1 = tempy1;
						insertx2 = tempx2;
						inserty2 = tempy2;
					
					
	
					}
					i = i +4; // i can advance by 4 since i+2 is the newly added landing coord
					
					break;
				}
			}
		}
		
		/*
		for (i=0;i<80;i++)
		{
			if (i%2 == 0) 
			printf("index %d is: %d ", i, terrain.border[i]);
			else 
			printf("index %d is: %d\n", i, terrain.border[i]);
		}
		printf("landing pos 0 is %d, 1 is: %d\n",land_pos[0], land_pos[1]);
		*/
		
		
	} // end init
	
void Terrain::draw (XInfo xinfo, Terrain &terrain) 
	{
		int i;		
		int j = 0;
		//i <=terrain.total_index+(terrain.total_landing_zone * 2)+2
		for (i = 0; i < 70; i=i+2) 
		{
			int next_x = terrain.border[i+2];
			int next_y = terrain.border[i+3];
			//printf("now at index %d, x: %d, y : %d\n", i, terrain.border[i], terrain.border[i+1]);
			int land_pos_index;
			
			if (i+2 == terrain.land_index[0] || i+2 == terrain.land_index[1]) 
			{
				
				if (i+2 == terrain.land_index[0])
					land_pos_index = 0;
				else if ( i+2 == terrain.land_index[1])
					land_pos_index = 1;
				
				//printf("drawing landing zone %d, index is %d\n:", land_pos_index, i);
				// link to the landing zone
				//printf("linking %d:%d to %d:%d\n", terrain.border[i], terrain.border[i+1], terrain.land_x1[land_pos_index], terrain.land_y[land_pos_index]);
				
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], terrain.border[i],terrain.border[i+1], terrain.land_x1[land_pos_index], terrain.land_y[land_pos_index]);
				
				// draw the landing zone
				//printf("drawing %d:%d, length %d, height %d\n", terrain.land_x1[land_pos_index], terrain.land_y[land_pos_index],  terrain.land_x2[land_pos_index]-terrain.land_x1[land_pos_index], 5);
								
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], terrain.land_x1[land_pos_index], terrain.land_y[land_pos_index], terrain.land_x2[land_pos_index]-terrain.land_x1[land_pos_index], 3);
								
				
				next_x = terrain.border[i+6];
				next_y = terrain.border[i+7];
				// link to the next mountain
				//printf("linking %d:%d to %d:%d\n", terrain.land_x2[land_pos_index], terrain.land_y[land_pos_index],  next_x, next_y);
				XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], terrain.land_x2[land_pos_index],terrain.land_y[land_pos_index], next_x, next_y);
			/*
				// set land_x1 and land_y
				terrain.border[j] = terrain.coord[i];
				terrain.border[j+1] = terrain.coord[i+1];
				
				j = j + 2;
				
				terrain.border[j] = terrain.land_x1;
				terrain.border[j+1] = terrain.land_y;
				terrain.land_index = j;
				
				// set land_x2 and land_y
				j = j + 2;
				
				terrain.border[j] = terrain.land_x2;
				terrain.border[j+1] = terrain.land_y;*/
				i = i + 4;
			}
			else 
			{
			XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], terrain.border[i],terrain.border[i+1], next_x, next_y);
			//terrain.border[j] = terrain.coord[i];
			//terrain.border[j+1] = terrain.coord[i+1];
			}
		//j = j +2;
		}

/*
		for (i=0;i<=14;i++)
		{
			printf("index %d is: %d\n", i, terrain.border[i]);
		}
*/

	} //terrain.draw

	void C_Status::draw(XInfo xinfo, C_Status current_status, Ship ship, Terrain terrain) 
	{
		
		std::string text("Press q to quit");
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w-800)/2 + 10, (xinfo.current_h-600)/2 + 20, text.c_str(), text.length()  );  
                  
        // getting ship coord info and display on top right           
        std::string text1 = "ship position X : ";
        std::string text2 = " Y : ";
        std::ostringstream oss;
		oss << text1 << ship.x<< text2 << ship.y;
		text1 = oss.str();     
		//std::string text1(pos_info);
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w-800)/2 + 600, (xinfo.current_h-600)/2 + 20, text1.c_str(), text1.length()  );
		
		
		
		// draw the fuel
		text= "FUEL LEVEL";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w/2)-10, (xinfo.current_h-600)/2 + 20, text.c_str(), text.length()  );  
		
		XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w/2)-35, (xinfo.current_h-600)/2 + 30, 100, 10);
		XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], (xinfo.current_w/2)-35, (xinfo.current_h-600)/2 + 30, ship.fuel/5, 10);
		
				/*
		// display which zone the ship is in
		std::ostringstream oss3;
		oss3<< "Ship.ul is in zone : "<<ship.zone_index[0]<< " Ship.ur is in zone : "<<ship.zone_index[1]<< " Ship.ll is in zone : "<<ship.zone_index[2]<< " Ship.lr is in zone : "<<ship.zone_index[3];
		//" Ship slop is : "<<ship.ship_slop<<" Mountain slop is : "<<ship.mountain_slop;
		text1 = oss3.str();   
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[1], 50, 35, text1.c_str(), text1.length()  );
		*/
		
	}


	// if ship make contact to the mountain (exculding landing zone)
	// return 1 indicate a crash
	// otherwise return 0
int crush_huh (Ship &ship, Terrain &terrain) 
	{
		if (terrain.draw_flag == 0)
			return 0;
		// convert to math x-y coord
		int y = 600-ship.y;
		// int zone_index;
		// now check which zone the ship is in
		
		// we check the ship by checking its four cornor coords
		// if any of the four cornors touches the mountain, the 
		// ship crushes
		// set crush_flag = 1 if any of the four cornors touches
		// the mountain
		int crush_flag=0;

		// 0 - upper left; 1 - upper right; 2 - lower left; 3 - lower right
		int x_coord[4];
		x_coord[0] = ship.x;
		x_coord[1] = ship.urx;
		x_coord[2] = ship.llx;
		x_coord[3] = ship.lrx;
		
		int y_coord[4];
		y_coord[0] = ship.y;
		y_coord[1] = ship.ury;
		y_coord[2] = ship.lly;
		y_coord[3] = ship.lry;
	
		//int ship_zone_index
		int i;
		for (i = 0; i <4; i++)
		{
			// first check which zone the coord is in
			int max = (terrain.total_zone +1 + (terrain.total_landing_zone *2)) * 2;
			int j;
			for (j = 0; j < max; j = j +2)
			{
			//printf("j = %d, %d < %d && %d > %d\n", j, terrain.border[j],x_coord[i],terrain.border[j+2],x_coord[i] );
				if (terrain.border[j]< x_coord[i] && terrain.border[j+2] > x_coord[i]) 
				{
					ship.zone_index[i] = j;
					//printf("shipzone index %d is : %d\n", i, ship.zone_index[i]);
					break;
				}
			}	
		
		
			// now check if each cornor touches the mountain
			
			// if the point is already in the landing zone, it will never crush
			// continue to check next point	
			if (ship.zone_index[i] == terrain.land_index[0] || ship.zone_index[i] == terrain.land_index[1])
			{
				continue;
			}
		
		
			// now calculate if the ship crushes
			// terrain.border[ship.zone_index+1] - ship.lly = ((600-ship.lly) - (600 - terrain.border[ship.zone_index+1])) 
			double delta_y = terrain.border[ship.zone_index[i]+1] - terrain.border[ship.zone_index[i]+3];
			double delta_x = terrain.border[ship.zone_index[i]+2]- terrain.border[ship.zone_index[i]];
			double mountain_slop = delta_y/delta_x;

			delta_y = terrain.border[ship.zone_index[i]+1] - y_coord[i];
			delta_x = x_coord[i] - terrain.border[ship.zone_index[i]];
			double ship_slop = delta_y/delta_x;
		
		
			ship.ship_slop[i] = ship_slop;
			ship.mountain_slop[i] = mountain_slop;
			
			//printf("ship slop is %f, terrain slop is %f\n", ship.ship_slop[i], ship.mountain_slop[i]);
			
			if (ship.ship_slop[i] < ship.mountain_slop[i]) 
			{
				//printf("conor %d CRUSHed!!\n", i);
				return 1;
			}
		
		
		} // end for

		
		// for every point , check if the point is in
		// lower left and lower right cornor
		// if so, check for y, if the point is above
		// the lower left/lower right y, then its a crush
		
		int max = (terrain.total_zone +1 + (terrain.total_landing_zone *2)) * 2;
		int j;
		for (j = 0; j < max; j = j +2)
		{
			if (ship.llx < terrain.border[j] && terrain.border[j] < ship.lrx)
			{
				if (ship.lly >= terrain.border[j+1])
				{
					//printf("CRUSHED at point x: %d, y: %d\n", terrain.border[j], terrain.border[j+1]);
					return 1;
				}
			
			} 
		} 

		
		
		return 0;
	} // end crush_huh
	

