#pragma once

class CSolid;
class CLoop;

class CFace
{
public:
	CFace();
	~CFace();
public:
	CSolid *f_solid;
	CLoop *f_loop;
	CFace *next;
	CFace *prev;
	GLdouble normal[3];
};

