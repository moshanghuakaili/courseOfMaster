
// CAD_V1Doc.h : interface of the CCAD_V1Doc class
//


#pragma once

#include "Solid.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Loop.h"
#include "HEdge.h"

class CCAD_V1Doc : public CDocument
{
protected: // create from serialization only
	CCAD_V1Doc();
	DECLARE_DYNCREATE(CCAD_V1Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CCAD_V1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	CSolid	*solid;

public:
	CSolid* mvfs(GLdouble *v_coord);
	CHEdge* mev(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp);
	CLoop* mef(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp);
	CLoop* kemr(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp);
	void kfmrh(CLoop *lp_out, CLoop *lp);
	void sweep(CVertex *v_Head, int *l_No, double *vector);
	BOOL JudgeSameV(GLdouble *v1_coord, GLdouble *v2_coord);
	void CalcNormal(CSolid	*pSolid);
#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
