#pragma once

class CFace;
class CEdge;
class CVertex;

class CSolid
{
public:
	CSolid();
	~CSolid();

public:
	CFace *s_face;
	CEdge *s_edge;
	CVertex *s_v_head;
	CSolid *prev;
	CSolid *next;
};

