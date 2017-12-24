#include "stdafx.h"
#include "Face.h"
#include "Solid.h"
#include "Loop.h"

CFace::CFace()
{
	f_solid = NULL;
	f_loop = NULL;
	next = NULL;
	prev = NULL;
	normal[0] = normal[1] = normal[2] = 0;
}


CFace::~CFace()
{
}
