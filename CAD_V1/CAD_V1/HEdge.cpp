#include "stdafx.h"
#include "HEdge.h"
#include "Loop.h"
#include "Edge.h"
#include "Vertex.h"

CHEdge::CHEdge()
{
	he_loop = NULL;
	he_edge = NULL;
	he_v = NULL;
	next = NULL;
	prev = NULL;
}


CHEdge::~CHEdge()
{
}
