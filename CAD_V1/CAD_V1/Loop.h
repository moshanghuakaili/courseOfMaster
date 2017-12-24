#pragma once

class CFace;
class CHEdge;

class CLoop
{
public:
	CLoop();
	~CLoop();
public:
	CFace *l_face;
	CHEdge *l_hedge;
	CLoop *next;
	CLoop *prev;
};

