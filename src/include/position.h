#pragma once 

struct Position{
	int x; // onscreen x coordinate
	int y; // onscreen y coordinate
};
typedef struct Position Position;
typedef struct Position Velocity; /* here x and y acts as rectangular components of the velocity */

struct NearDet{
	int x;
	int y;
	unsigned idx;
	int dist;
};
typedef struct NearDet NearDet; /* nearest feature details; dist denotes the nearest distance b/w the feature */
