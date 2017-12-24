#include "stdafx.h"
#include "Loop.h"
#include "Face.h"
#include "HEdge.h"

CLoop::CLoop()
{
	l_face = NULL;
	l_hedge = NULL;
	next = NULL;
	prev = NULL;
}


CLoop::~CLoop()
{
}
