#include "stdafx.h"
#include "Solid.h"
#include "Edge.h"
#include "Face.h"

CSolid::CSolid()
{
	s_face = NULL;
	s_edge = NULL;
	s_v_head = NULL;
	next = NULL;
	prev = NULL;
}


CSolid::~CSolid()
{
}
