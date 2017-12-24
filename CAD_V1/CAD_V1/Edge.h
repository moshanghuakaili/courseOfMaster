#pragma once

class CHEdge;

class CEdge
{
public:
	CEdge();
	~CEdge();
public:
	int e_id;
	CEdge *next;
	CEdge *prev;
	CHEdge *he1;
	CHEdge *he2;
};

