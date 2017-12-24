#pragma once

class CLoop;
class CEdge;
class CVertex;

class CHEdge
{
public:
	CHEdge();
	~CHEdge();
public:
	CLoop *he_loop;
	CEdge *he_edge;
	CHEdge *he_adj;
	CVertex *he_v;
	CHEdge *next;
	CHEdge *prev;
};

