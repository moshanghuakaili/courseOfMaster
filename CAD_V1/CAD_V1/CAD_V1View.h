
// CAD_V1View.h : interface of the CCAD_V1View class
//

#pragma once
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

class CCAD_V1View : public CView
{
protected: // create from serialization only
	CCAD_V1View();
	DECLARE_DYNCREATE(CCAD_V1View)

// Attributes
// openGL
public:
	HGLRC m_hRC; //Rendering Context
	CClientDC* m_pDC; //Device Context
	BOOL InitializeOpenGL(void); //初始化 OpenGL
	BOOL SetupPixelFormat(void); //设置像素格式
	void RenderScence(void);
public:
	CCAD_V1Doc* GetDocument() const;


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCAD_V1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	GLfloat lookAtPos_X;
	GLfloat lookAtPos_Y;
	GLfloat lookAtPos_Z;
	BOOL draw_Face;
	BOOL right_Button;
	BOOL is_surface;
	CPoint button_Point;

	CVertex *v_Head;
	//CVertex *v_Tmp;
	int l_No[100];
	GLUtesselator* tessobj;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnToolAddface();
	afx_msg void OnToolSweep();
	afx_msg void OnViewFront();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewWire();
	afx_msg void OnViewSurface();
};
void CALLBACK vertexCallback(GLvoid* vertex);
void CALLBACK beginCallback(GLenum type);
void CALLBACK endCallback();
void CALLBACK errorCallback(GLenum errorCode);
void CALLBACK combineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut);

#ifndef _DEBUG  // debug version in CAD_V1View.cpp
inline CCAD_V1Doc* CCAD_V1View::GetDocument() const
   { return reinterpret_cast<CCAD_V1Doc*>(m_pDocument); }
#endif

