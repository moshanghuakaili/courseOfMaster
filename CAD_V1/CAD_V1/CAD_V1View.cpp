
// CAD_V1View.cpp : implementation of the CCAD_V1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CAD_V1.h"
#endif

#include "CAD_V1Doc.h"
#include "CAD_V1View.h"
#include "SweepDialog.h"
#include "Alert.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCAD_V1View

IMPLEMENT_DYNCREATE(CCAD_V1View, CView)

BEGIN_MESSAGE_MAP(CCAD_V1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TOOL_ADDFACE, &CCAD_V1View::OnToolAddface)
	ON_COMMAND(ID_TOOL_SWEEP, &CCAD_V1View::OnToolSweep)
	ON_COMMAND(ID_VIEW_FRONT, &CCAD_V1View::OnViewFront)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_WIRE, &CCAD_V1View::OnViewWire)
	ON_COMMAND(ID_VIEW_SURFACE, &CCAD_V1View::OnViewSurface)
END_MESSAGE_MAP()

// CCAD_V1View construction/destruction

CCAD_V1View::CCAD_V1View()
{
	// TODO: add construction code here
	lookAtPos_X = 0;
	lookAtPos_Y = 0;
	lookAtPos_Z = 10;
	for (int a = 0; a < 100; a++) {
		l_No[a] = 0;
	}
	draw_Face = FALSE;
	right_Button = FALSE;
	is_surface = TRUE;
	v_Head = new CVertex();
	tessobj = NULL;

}

CCAD_V1View::~CCAD_V1View()
{
}

BOOL CCAD_V1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCAD_V1View drawing

void CCAD_V1View::OnDraw(CDC* /*pDC*/)
{
	CCAD_V1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScence();

	glFinish();
	// Now Swap the buffers
	SwapBuffers(m_pDC->GetSafeHdc());
	// TODO: add draw code for native data here
}


// CCAD_V1View printing

BOOL CCAD_V1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCAD_V1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCAD_V1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

BOOL CCAD_V1View::SetupPixelFormat(void) {
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), //pfd结构大小
		1, //版本号
		PFD_DRAW_TO_WINDOW | //支持在窗口中绘图
		PFD_SUPPORT_OPENGL | //支持OPENGL
		PFD_DOUBLEBUFFER, //双缓存模式
		PFD_TYPE_RGBA, //RGBA颜色模式
		24, //24位颜色深度
		0,0,0,0,0,0, //忽略颜色位
		0, //没有非透明度缓存
		0, //忽略移位位
		0, //无累计缓存
		0,0,0,0, //忽略累计位
		32, //32位深度缓存
		0, //无模板缓存
		0, //无辅助模板
		PFD_MAIN_PLANE, //主层
		0, //保留
		0,0,0 //忽略层 可见性 损毁掩模
	};
	int pixelFormat;
	//为设备描述表的到最匹配的像素格式
	if ((pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0) {
		MessageBox(_T("choose pixel format failed!"));
		return FALSE;
	}
	//设置最匹配的像素格式为当前的像素格式
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd) == FALSE) {
		MessageBox(_T("set pixel format failed!"));
		return FALSE;
	}
	return TRUE;
}
BOOL CCAD_V1View::InitializeOpenGL(void) {
	m_pDC = new CClientDC(this);
	if (m_pDC == NULL)
	{
		MessageBox(_T("Error Obtaining DC"));
		return FALSE;
	}
	//设置当前的绘图像素格式
	if (!SetupPixelFormat()) {
		return FALSE;
	}
	//创建绘图描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hRC == NULL) {
		return FALSE;
	}
	//使绘图描述表为当前调用线程的当前绘图描述表
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE) {
		return FALSE;
	}

	tessobj = gluNewTess();
	// Register the callback function
	gluTessCallback(tessobj, GLU_TESS_VERTEX, (void(__stdcall *)(void))vertexCallback);
	gluTessCallback(tessobj, GLU_TESS_BEGIN, (void(__stdcall *)(void))beginCallback);
	gluTessCallback(tessobj, GLU_TESS_END, endCallback);
	gluTessCallback(tessobj, GLU_TESS_ERROR, (void(__stdcall *)(void))errorCallback);
	gluTessCallback(tessobj, GLU_TESS_COMBINE, (void(__stdcall *)(void))combineCallback);
	// Set the winding rule
	gluTessProperty(tessobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

	return TRUE;
}

void CCAD_V1View::RenderScence(void) {
	
	//ini
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, 1.0f, 1.0f, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lookAtPos_X, lookAtPos_Y, lookAtPos_Z, 0, 0, 0, 0, 1, 0);

	glEnable(GL_LIGHTING);   //开关:使用光
	glEnable(GL_LIGHT0);     //打开0#灯
	glEnable(GL_DEPTH_TEST); //打开深度测试
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
	GLfloat light_ambient[] = { 1, 1, 1, 1.0 };   
	GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_specular[] = { 0.2f, 0.2f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//draw sweep
	CCAD_V1Doc *doc = GetDocument();
	CSolid *solid;
	solid = doc->solid;
	CFace *pFace;
	CLoop *pLoop;
	CHEdge *pHalfEdge, *pHeHead;

	GLfloat		draw_ambient[] = { 1.0, 1.0, 1.0 },
		mat_ambient[] = { 0.5, 0.5, 0.5 },
		mat_specular[] = { 1, 1, 1, 1.0 },
		shininess[] = { 30.0 };

	// draw coordinate
	GLfloat x_ambient[] = { 1.0, 0.0, 0.0 },
	y_ambient[] = { 0.0, 1.0, 0.0 },
	z_ambient[] = { 0.0, 0.0, 1.0 };
	glPushMatrix();
	glBegin(GL_LINES);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, x_ambient);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(2.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, y_ambient);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 2.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, z_ambient);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 2.0);
	glEnd();
	glPopMatrix();

	if (is_surface) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glEnable(GL_NORMALIZE);
		while (solid) {
			pFace = solid->s_face;
			while (pFace)
			{
				gluTessBeginPolygon(tessobj, NULL);
				glNormal3dv(pFace->normal);
				pLoop = pFace->f_loop;
				while (pLoop)
				{
					gluTessBeginContour(tessobj);
					pHalfEdge = pHeHead = pLoop->l_hedge;
					while (pHalfEdge)
					{
						gluTessVertex(tessobj, pHalfEdge->he_v->v_coord, pHalfEdge->he_v->v_coord);
						pHalfEdge = pHalfEdge->next;
						if (pHalfEdge == pHeHead)
							break;
					}
					gluTessEndContour(tessobj);
					pLoop = pLoop->next;
				}

				gluTessEndPolygon(tessobj);
				pFace = pFace->next;
			}
			solid = solid->next;
		}
	}
	else {
		GLfloat line_ambient[] = { 0.0, 0.5, 0.5 };
		glPushMatrix();
		glBegin(GL_LINES);
		while (solid) {
			pFace = solid->s_face;
			while (pFace)
			{
				pLoop = pFace->f_loop;
				while (pLoop)
				{
					pHalfEdge = pHeHead = pLoop->l_hedge;
					while (pHalfEdge)
					{
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, line_ambient);
						glVertex3f(pHalfEdge->he_v->v_coord[0], pHalfEdge->he_v->v_coord[1], pHalfEdge->he_v->v_coord[2]); //点P1,左下角  
						glVertex3f(pHalfEdge->next->he_v->v_coord[0], pHalfEdge->next->he_v->v_coord[1], pHalfEdge->next->he_v->v_coord[2]);
						pHalfEdge = pHalfEdge->next;
						if (pHalfEdge == pHeHead)
							break;
					}
					pLoop = pLoop->next;
				}
				pFace = pFace->next;
			}
			solid = solid->next;
		}
		glEnd();
		glPopMatrix();
	}

	if (draw_Face) {
		if (v_Head->v_id != -1) {
			glPushMatrix();
			glBegin(GL_LINES);
			CVertex *v_tmp_head = v_Head;
			CVertex *v_tmp = v_Head;
			int count = 0;
			int l_count = 0;
			while (v_tmp->next) {
				if (count == l_No[l_count] - 1) {
					//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, draw_ambient);
					glVertex3d(v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2]);
					glVertex3d(v_tmp_head->v_coord[0], v_tmp_head->v_coord[1], v_tmp_head->v_coord[2]);
					v_tmp_head = v_tmp->next;
					l_count++;
				}
				else {
					//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, draw_ambient);
					glVertex3d(v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2]);
					glVertex3d(v_tmp->next->v_coord[0], v_tmp->next->v_coord[1], v_tmp->next->v_coord[2]);
				}
				count++;
				v_tmp = v_tmp->next;
			}
			glEnd();
			glPopMatrix();
		}
	}
}
// CCAD_V1View diagnostics

#ifdef _DEBUG
void CCAD_V1View::AssertValid() const
{
	CView::AssertValid();
}

void CCAD_V1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCAD_V1Doc* CCAD_V1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCAD_V1Doc)));
	return (CCAD_V1Doc*)m_pDocument;
}
#endif //_DEBUG


// CCAD_V1View message handlers


int CCAD_V1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//OnToolSweep();
	if (InitializeOpenGL()) {
		return 0;
	}
	return 0;
}


void CCAD_V1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	//_cprintf("%d,%d\n",cx,cy);
	glViewport(0, 0, cx, cy);
}


void CCAD_V1View::OnDestroy()
{
	CView::OnDestroy();
	// TODO: Add your message handler code here
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}
	if (::wglDeleteContext(m_hRC) == FALSE)
	{
		MessageBox(_T("Could not delete RC"));
	}
	if (m_pDC)
	{
		delete m_pDC;
	}
	m_pDC = NULL;
}

BOOL CCAD_V1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	lookAtPos_Z = lookAtPos_Z + zDelta / 100;
	InvalidateRect(NULL, FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CCAD_V1View::OnToolAddface()
{
	Alert alertdlg;
	if (alertdlg.DoModal() == IDOK)
	{
		draw_Face = TRUE;
		OnViewFront();
	}
	
	// TODO: Add your command handler code here
}


void CCAD_V1View::OnToolSweep()
{
	
	draw_Face = FALSE;
	lookAtPos_X = -10;
	lookAtPos_Y = 5;
	lookAtPos_Z = 8;
	//double dis = 1;
	//CSweepDlg	dlg;
	/*CVertex *a= new CVertex();
	CVertex *b = new CVertex();
	CVertex *c = new CVertex();
	CVertex *d = new CVertex();
	CVertex *a1 = new CVertex();
	CVertex *b1 = new CVertex();
	CVertex *c1 = new CVertex();
	CVertex *d1 = new CVertex();
	CVertex *a2 = new CVertex();
	CVertex *b2 = new CVertex();
	CVertex *c2 = new CVertex();
	CVertex *d2 = new CVertex();

	a->v_coord[2] = b->v_coord[2] = c->v_coord[2] = d->v_coord[2] = 0;
	a->v_coord[0] = b->v_coord[0] = a->v_coord[1] = d->v_coord[1] = -1;
	c->v_coord[0] = d->v_coord[0] = b->v_coord[1] = c->v_coord[1] = 1;
	a->v_id = b->v_id = c->v_id = d->v_id = 1;

	a1->v_coord[2] = b1->v_coord[2] = c1->v_coord[2] = d1->v_coord[2] = 0;
	a1->v_coord[0] = b1->v_coord[0] = -0.5;
	a1->v_coord[1] = d1->v_coord[1] = 0;
	c1->v_coord[0] = d1->v_coord[0] = 0;
	b1->v_coord[1] = c1->v_coord[1] = 0.5;
	a1->v_id = b1->v_id = c1->v_id = d1->v_id = 1;

	a2->v_coord[2] = b2->v_coord[2] = c2->v_coord[2] = d2->v_coord[2] = 0;
	a2->v_coord[0] = b2->v_coord[0] = 0.1;
	a2->v_coord[1] = d2->v_coord[1] = -0.5;
	c2->v_coord[0] = d2->v_coord[0] = 0.5;
	b2->v_coord[1] = c2->v_coord[1] = 0;
	a2->v_id = b2->v_id = c2->v_id = d2->v_id = 1;

	b->prev = a;
	a->next = c->prev = b;
	b->next = d->prev = c;
	c->next = a1->prev = d;
	b1->prev = d->next = a1;
	a1->next = c1->prev = b1;
	b1->next = d1->prev = c1;
	c1->next = d1;
	b2->prev = d1->next = a2;
	a2->next = c2->prev = b2;
	b2->next = d2->prev = c2;
	c2->next = d2;
	a->prev = d2->next = NULL;
	v_Head = a;
	
	
	l_No[0] = 4;
	l_No[1] = 8;
	l_No[2] = 12;
	double vector[3] = { 0,0,1 };*/
	SweepDialog	dlg;
	dlg.sweep_vx = 0;
	dlg.sweep_vy = 0;
	dlg.sweep_vz = 0;

	double vector[3];
	if (dlg.DoModal() == IDOK)
	{
		vector[0] = dlg.sweep_vx;
		vector[1] = dlg.sweep_vy;
		vector[2] = dlg.sweep_vz;

		CCAD_V1Doc* pDoc = GetDocument();
		pDoc->sweep(v_Head, l_No, vector);
		InvalidateRect(NULL, FALSE);
	}
	// TODO: Add your command handler code here
}


void CCAD_V1View::OnViewFront()
{
	// TODO: Add your command handler code here
	lookAtPos_X = 0;
	lookAtPos_Y = 0;
	lookAtPos_Z = 10;
	InvalidateRect(NULL, FALSE);
}


void CCAD_V1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLdouble wx, wy, wz;

	right_Button = FALSE;
	button_Point = point;

	SetCapture();
	if (draw_Face == TRUE) {
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		gluUnProject((GLdouble)point.x, (GLdouble)(viewport[3] - (GLint)point.y - 1), 0.0,
			modelview, projection, viewport, &wx, &wy, &wz);
		/*_cprintf("point:%d,%d\n", point.x, point.y);
		_cprintf("viewport:%d,%d,%d,%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);*/
		//_cprintf("w:%f,%f,%f\n", wx, wy, wz);
		if (v_Head->v_id == -1) {
			v_Head->v_coord[0] = wx * (lookAtPos_Z);
			v_Head->v_coord[1] = wy * (lookAtPos_Z);
			v_Head->v_coord[2] = 0;
			//_cprintf("v_Head:%f,%f,%f,%d\n", v_Head->v_coord[0], v_Head->v_coord[1], v_Head->v_coord[2], v_Head->v_id);
		}
		else {
			int count = 0;
			CVertex *v_tmp = v_Head;
			CVertex *v_new = new CVertex();
			while (v_tmp->next) {
				v_tmp = v_tmp->next;
				count++;
			}
			v_new->v_coord[0] = wx * (lookAtPos_Z) ;
			v_new->v_coord[1] = wy * (lookAtPos_Z) ;
			v_new->v_coord[2] = 0;
			v_new->prev = v_tmp;
			v_tmp->next = v_new;
			//_cprintf("not_v_Head:%f,%f,%f,%d\n", v_new->v_coord[0], v_new->v_coord[1], v_new->v_coord[2], v_new->v_id);
		}
		InvalidateRect(NULL, FALSE);
	}
	
	CView::OnLButtonDown(nFlags, point);
}


void CCAD_V1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CVertex *v_tmp = v_Head;
	int count = 0;
	while (v_tmp->next) {
		v_tmp = v_tmp->next;
		count++;
	}
	v_tmp->v_id = count;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CCAD_V1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLdouble wx, wy, wz;

	button_Point = point;
	if (draw_Face == TRUE ) {
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		gluUnProject((GLdouble)point.x, (GLdouble)(viewport[3] - (GLint)point.y - 1), 0.0,
			modelview, projection, viewport, &wx, &wy, &wz);
		/*_cprintf("point:%d,%d\n", point.x, point.y);
		_cprintf("viewport:%d,%d,%d,%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);*/
		//_cprintf("w:%f,%f,%f\n", wx, wy, wz);
		CVertex *v_tmp = v_Head;
		int count = 0;
		while (v_tmp->next) {
			v_tmp = v_tmp->next;
			count++;
		}
		v_tmp->v_coord[0] = wx * (lookAtPos_Z);
		v_tmp->v_coord[1] = wy * (lookAtPos_Z);
		v_tmp->v_coord[2] = 0;
		InvalidateRect(NULL, FALSE);
	}
	//_cprintf("OnMouseMove:%d\n", draw_Face);
	CView::OnMouseMove(nFlags, point);
}


void CCAD_V1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (draw_Face == TRUE) {
		right_Button = TRUE;
		CVertex *v_tmp = v_Head;
		int count = 0;
		while (v_tmp->next) {
			//_cprintf("%d,", v_tmp->v_id);
			v_tmp = v_tmp->next;
			count++;
			
		}
		for (int l_no_count = 0; l_no_count < 100; l_no_count++) {
			if (l_No[l_no_count] == 0) {
				l_No[l_no_count] = count;
				//_cprintf(",%d\n", l_no_count);
				break;
			}
		}
		InvalidateRect(NULL, FALSE);
	}
	//_cprintf("%d", l_No[0]);
	/*CVertex *v_tmp = v_Head;
	while (v_tmp->next) {
		if (v_tmp) {
			_cprintf("v:%f,%f,%f\n", v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2]);
		}
		v_tmp = v_tmp->next;
	}
	CVertex *v_tmp_head = v_Head;
	v_tmp = v_Head;
	int count = 0;
	int l_count = 0;
	while (v_tmp->next) {
		if (count == l_No[l_count] - 1) {
			_cprintf("%f,%f,%f+++++%f,%f,%f\n", v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2], v_tmp_head->v_coord[0], v_tmp_head->v_coord[1], v_tmp_head->v_coord[2]);
			v_tmp_head = v_tmp->next;
			l_count++;
		}
		else {
			_cprintf("%f,%f,%f+++++%f,%f,%f\n", v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2], v_tmp->next->v_coord[0], v_tmp->next->v_coord[1], v_tmp->next->v_coord[2]);
		}
		count++;
		v_tmp = v_tmp->next;
	}*/
	CView::OnRButtonDown(nFlags, point);
}

void CCAD_V1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	if (nChar == VK_UP) {
		_cprintf("123");
		lookAtPos_Y = lookAtPos_Y + 1;
	}
	if (nChar == VK_DOWN) {
		lookAtPos_Y = lookAtPos_Y - 1;
	}
	if (nChar == VK_LEFT) {
		lookAtPos_X = lookAtPos_X + 1;
	}
	if (nChar == VK_RIGHT) {
		lookAtPos_X = lookAtPos_X - 1;
	}
	InvalidateRect(NULL, FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CALLBACK beginCallback(GLenum type)
{
	//_cprintf("begin");
	glBegin(type);
}

void CALLBACK endCallback(void)
{
	//_cprintf("end");
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte	*estring;

	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}

void CALLBACK vertexCallback(GLvoid	*vertex)
{
	//_cprintf("vertexCallback");
	glVertex3dv((GLdouble *)vertex);
}

void CALLBACK combineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	int	i;
	_cprintf("combineCallback\n");
	vertex = (GLdouble *)malloc(6 * sizeof(GLdouble));
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	for (i = 3; i < 6; i++)
		vertex[i] = weight[0] * vertex_data[0][i]
		+ weight[1] * vertex_data[1][i]
		+ weight[2] * vertex_data[2][i]
		+ weight[3] * vertex_data[3][i];
	*dataOut = vertex;
}


void CCAD_V1View::OnViewWire()
{
	is_surface = FALSE;
	InvalidateRect(NULL, FALSE);
	// TODO: Add your command handler code here
}


void CCAD_V1View::OnViewSurface()
{
	is_surface = TRUE;
	InvalidateRect(NULL, FALSE);
	// TODO: Add your command handler code here
}
