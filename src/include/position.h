#pragma once 

struct Position{
	float x; // onscreen x coordinate
	float y; // onscreen y coordinate
};
typedef struct Position Position;

struct Velocity{
	float x; // onscreen x coordinate
	float y; // onscreen y coordinate
};
typedef struct Velocity Velocity; /* int for position because position needs to be countable pixel point on screen */

struct NearDet{
	int x;
	int y;
	unsigned idx;
	int dist;
};
typedef struct NearDet NearDet; /* nearest feature details; dist denotes the nearest distance b/w the feature */
