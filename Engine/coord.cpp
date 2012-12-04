////////////////////////////////////////////////////////////////////////////////
// Filename: coord.cpp
////////////////////////////////////////////////////////////////////////////////
#include "coord.h"

Coord::Coord() :
	x(0), y(0), z(0)
{
}


Coord::Coord(const Coord& other)
	: x(other.x), y(other.y), z(other.z)
{
}


Coord::~Coord()
{
}

Coord::Coord(float new_x, float new_y, float new_z) :
	x(new_x), y(new_y), z(new_z)
{
}

Coord& Coord::operator= (const Coord& other)
{
	Coord new_coord(other);
	return new_coord;
}

Coord& Coord::operator= (const float uniform_value)
{
	Coord new_coord(uniform_value, uniform_value, uniform_value);
	return new_coord;
}