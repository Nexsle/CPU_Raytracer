#pragma once
#include "rtweekend.h"

class interval
{
public:
	double min, max;

	interval() : min(+infinity), max(-infinity) {}

	interval(double min, double max) : min(min), max(max) {}

	double Size() const
	{
		return max - min;
	}

	bool Contains(double x) const 
	{
		return min <= x && x <= max;
	}
	bool Surounds(double x) const		
	{
		return min < x && x < max;
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
