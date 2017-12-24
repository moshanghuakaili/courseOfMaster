#include "stdafx.h"
#include "Edge.h"
#include "HEdge.h"

CEdge::CEdge()
{
	e_id = -1;
	next = NULL;
	prev = NULL;
	he1 = NULL;
	he2 = NULL;
}


CEdge::~CEdge()
{
}
