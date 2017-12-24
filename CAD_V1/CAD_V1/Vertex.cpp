#include "stdafx.h"
#include "Vertex.h"
#include "HEdge.h"

CVertex::CVertex()
{
	v_id = -1;
	v_coord[0] = v_coord[1] = v_coord[2] = 0;
	v_he = NULL;
	next = NULL;
	prev = NULL;
}


CVertex::~CVertex()
{
}
