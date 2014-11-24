#ifndef _SIMULATION_H
#define _SIMULATION_H

struct WorldPos {
	WorldPos(double x, double y, double z, double angle);

	double x, y, z, angle;
};

#endif