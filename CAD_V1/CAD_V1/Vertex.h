#pragma once

class CHEdge;

class CVertex
{
public:
	CVertex();
	~CVertex();
public:
	int v_id;
	GLdouble v_coord[3];
	CHEdge *v_he;
	CVertex *next;
	CVertex *prev;
};

