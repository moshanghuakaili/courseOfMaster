
// CAD_V1Doc.cpp : implementation of the CCAD_V1Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CAD_V1.h"
#endif

#include "CAD_V1Doc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCAD_V1Doc

IMPLEMENT_DYNCREATE(CCAD_V1Doc, CDocument)

BEGIN_MESSAGE_MAP(CCAD_V1Doc, CDocument)
END_MESSAGE_MAP()


// CCAD_V1Doc construction/destruction

CCAD_V1Doc::CCAD_V1Doc()
{
	// TODO: add one-time construction code here
	solid = NULL;
}

CCAD_V1Doc::~CCAD_V1Doc()
{
}

BOOL CCAD_V1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCAD_V1Doc serialization

void CCAD_V1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCAD_V1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCAD_V1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCAD_V1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCAD_V1Doc diagnostics

#ifdef _DEBUG
void CCAD_V1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCAD_V1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

CSolid* CCAD_V1Doc::mvfs(GLdouble *v_coord) {
	CSolid *new_solid = new CSolid();
	CFace *new_face = new CFace();
	CLoop *new_lp = new CLoop();
	CVertex	*new_v = new CVertex();
	
	_cprintf("mvfs:\n");
	//new vertex
	new_v->v_coord[0] = v_coord[0];
	new_v->v_coord[1] = v_coord[1];
	new_v->v_coord[2] = v_coord[2];

	//new face
	new_face->f_solid = new_solid;
	new_face->f_loop = new_lp;

	//new loop
	new_lp->l_face = new_face;

	//new_solid
	new_solid->s_face=new_face;
	new_solid->s_v_head = new_v;

	return new_solid;
}
CHEdge* CCAD_V1Doc::mev(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp) {
	CVertex *v1;
	CHEdge *he_tmp;
	CVertex *new_v2 = new CVertex();
	CHEdge *new_he1 = new CHEdge();
	CHEdge *new_he2 = new CHEdge();
	CEdge *new_edge = new CEdge();
	
	CSolid *s = lp->l_face->f_solid;
	_cprintf("meV:\n");
	_cprintf("v1_coord:%f,%f,%f++++++++v2_coord:%f,%f,%f\n", v1_coord[0], v1_coord[1], v1_coord[2], v2_coord[0], v2_coord[1], v2_coord[2]);
	v1 = s->s_v_head;
	while (v1) {
		if (JudgeSameV(v1->v_coord, v1_coord)) {
			break;
		}
		v1 = v1->next;
	}
	if (v1) {

		//v1->v_he = new_he1;
		
		//new vertex
		new_v2->v_coord[0] = v2_coord[0];
		new_v2->v_coord[1] = v2_coord[1];
		new_v2->v_coord[2] = v2_coord[2];
		//new_v2->v_he = new_he2;
		
		//new halfedge
		new_he1->he_v = v1;
		new_he2->he_v = new_v2;
		new_he1->next = new_he2;
		new_he2->prev = new_he1;
		new_he1->he_edge = new_edge;
		new_he2->he_edge = new_edge;
		new_he1->he_loop = lp;
		new_he2->he_loop = lp;
		new_he1->he_adj = new_he2;
		new_he2->he_adj = new_he1;

		he_tmp = lp->l_hedge;
		if (!he_tmp) {
			lp->l_hedge = new_he1;
			new_he1->prev = new_he2;
			new_he2->next = new_he1;
		}
		else {
			for (he_tmp = lp->l_hedge; !JudgeSameV(v1_coord, he_tmp->next->he_v->v_coord); he_tmp = he_tmp->next) {}
			new_he1->prev = he_tmp;
			new_he2->next = he_tmp->next;
			he_tmp->next->prev = new_he2;
			he_tmp->next = new_he1;
		}
		/*if (!lp->l_hedge) {
			lp->l_hedge = new_he1;
			new_he1->prev = new_he2;
			new_he2->next = new_he1;
		}
		else {
			v1->v_he->prev->next = new_he1;
			new_he2->next = v1->v_he;
		}*/
		//new edge
		new_edge->he1 = new_he1;
		new_edge->he2 = new_he2;

		if (s->s_edge) {
			s->s_edge->prev = new_edge;
			new_edge->next = s->s_edge;
			s->s_edge = new_edge;
		}
		else {
			s->s_edge = new_edge;
		}

		if (s->s_v_head) {
			s->s_v_head->prev = new_v2;
			new_v2->next = s->s_v_head;
			s->s_v_head = new_v2;
		}
		else {
			s->s_v_head = new_v2;
		}
		CHEdge *heee = s->s_face->f_loop->l_hedge;
		/*_cprintf("loop:");
		for (int count = 0; count < 8; count++) {
			_cprintf(" %f,%f,%f\n+++", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}*/
	}
	return  new_he1;
}
CLoop* CCAD_V1Doc::mef(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp) {
	CVertex *v1, *v2;
	CSolid *s = lp->l_face->f_solid;
	CFace *new_face = new CFace();
	CLoop *new_loop = new CLoop();
	CEdge *new_edge = new CEdge();
	CHEdge *new_he1 = new CHEdge();
	CHEdge *new_he2 = new CHEdge();
	CHEdge *he1_tmp, *he2_tmp;
	_cprintf("meF:\n");
	_cprintf("v1_coord:%f,%f,%f++++++++v2_coord:%f,%f,%f\n", v1_coord[0], v1_coord[1], v1_coord[2], v2_coord[0], v2_coord[1], v2_coord[2]);
	v1 = s->s_v_head;
	while (v1) {
		if (JudgeSameV(v1->v_coord, v1_coord)) {
			break;
		}
		v1 = v1->next;
	}
	v2 = s->s_v_head;
	while (v2) {
		if (JudgeSameV(v2->v_coord, v2_coord)) {
			break;
		}
		v2 = v2->next;
	}

	for (he1_tmp = lp->l_hedge; !JudgeSameV(v1_coord, he1_tmp->next->he_v->v_coord); he1_tmp = he1_tmp->next) {}
	for (he2_tmp = lp->l_hedge; !JudgeSameV(v2_coord, he2_tmp->next->he_v->v_coord); he2_tmp = he2_tmp->next) {}

	new_he1->he_v = v1;
	new_he2->he_v = v2;
	new_he1->he_edge = new_edge;
	new_he2->he_edge = new_edge;
	new_edge->he1 = new_he1;
	new_edge->he2 = new_he2;
	new_he1->he_adj = new_he2;
	new_he2->he_adj = new_he1;

	//desperate two loop
	new_he1->next = he2_tmp->next;
	new_he1->prev = he1_tmp;
	new_he2->next = he1_tmp->next;
	new_he2->prev = he2_tmp;
	he2_tmp->next->prev = new_he1;
	he1_tmp->next->prev = new_he2;
	he1_tmp->next = new_he1;
	he2_tmp->next = new_he2;

	//one loop ->lp .one loop->new loop
	new_he2->he_loop = lp;
	lp->l_hedge = new_he2;
	new_he1->he_loop = new_loop;
	new_loop->l_hedge = new_he1;
	for (he1_tmp = new_he1->next; !JudgeSameV(v1_coord, he1_tmp->he_v->v_coord); he1_tmp = he1_tmp->next) {
		he1_tmp->he_loop = new_loop;
	}
	for (he2_tmp = new_he2->next; !JudgeSameV(v2_coord, he2_tmp->he_v->v_coord); he2_tmp = he2_tmp->next) {
		he2_tmp->he_loop = lp;
	}
	
	//new face
	new_loop->l_face = new_face;
	new_face->f_loop = new_loop;
	new_face->f_solid = s;
	
	if (s->s_edge) {
		s->s_edge->prev = new_edge;
		new_edge->next = s->s_edge;
		s->s_edge = new_edge;
	}
	else {
		s->s_edge = new_edge;
	}

	if (s->s_face) {
		s->s_face->prev = new_face;
		new_face->next = s->s_face;
		s->s_face = new_face;
	}
	else {
		s->s_face = new_face;
	}
	return  new_loop;
}
CLoop* CCAD_V1Doc::kemr(GLdouble *v1_coord, GLdouble *v2_coord, CLoop *lp) {
	_cprintf("kemr:\n");
	_cprintf("v1_coord:%f,%f,%f++++++++v2_coord:%f,%f,%f\n", v1_coord[0], v1_coord[1], v1_coord[2], v2_coord[0], v2_coord[1], v2_coord[2]);
	
	CVertex *v1, *v2;
	CSolid *s = lp->l_face->f_solid;
	CLoop *new_loop = new CLoop();
	CLoop *lp_tmp;
	CEdge *edge_tmp;
	CHEdge *he1_tmp, *he2_tmp;

	he1_tmp = lp->l_hedge;
	while (he1_tmp) {
		if (JudgeSameV(v1_coord, he1_tmp->he_v->v_coord) && JudgeSameV(v2_coord, he1_tmp->next->he_v->v_coord)) {
			break;
		}
		he1_tmp = he1_tmp->next;
	}
	he2_tmp = he1_tmp->he_adj;
	
	//make v1 in the new loop
	he2_tmp->next->he_loop = new_loop;
	new_loop->l_hedge = he2_tmp->next;
	lp->l_hedge = he1_tmp->next;

	he1_tmp->prev->next = he2_tmp->next;
	he2_tmp->next->prev = he1_tmp->prev;
	he2_tmp->prev->next = he1_tmp->next;
	he1_tmp->next->prev = he2_tmp->prev;
	he1_tmp->prev = he1_tmp->next = NULL;
	he2_tmp->prev = he2_tmp->next = NULL;
	
	//new loop
	for (he1_tmp = new_loop->l_hedge->next; !JudgeSameV(v1_coord, he1_tmp->he_v->v_coord); he1_tmp = he1_tmp->next) {
		he1_tmp->he_loop = new_loop;
	}
	//delete edge
	/*edge_tmp = s->s_edge;
	while (edge_tmp) {
		if (edge_tmp->he1 == he1_tmp) {
			break;
		}
		edge_tmp = edge_tmp->next;
	}
	edge_tmp->prev->next = edge_tmp->next;
	edge_tmp->next->prev = edge_tmp->prev;
	edge_tmp->next = edge_tmp->prev = NULL;*/

	new_loop->next = lp->next;
	new_loop->prev = lp;
	if (lp->next) {
		lp->next->prev = new_loop;
	}
	lp->next = new_loop;
	new_loop->l_face = lp->l_face;

	/*if (new_loop) {
		_cprintf("what\n");
		_cprintf("he1_tmp:%f,%f,%f\n", new_loop->l_hedge->he_v->v_coord[0], new_loop->l_hedge->he_v->v_coord[1], new_loop->l_hedge->he_v->v_coord[2]);
	}*/
	//delete he1_tmp;
	//delete he2_tmp;
	return  new_loop;
}
void CCAD_V1Doc::kfmrh(CLoop *lp_in, CLoop *lp_out) {
	CLoop *lp_tmp;
	CFace *f_tmp;
	_cprintf("kfmrh:\n");
	
	lp_out->l_face->f_solid->s_face = lp_in->l_face->next;
	lp_tmp = lp_out->l_face->f_loop;
	while (lp_tmp->next) {
		lp_tmp = lp_tmp->next;
	}
	lp_tmp->next = lp_in;
	lp_in->prev = lp_tmp;

	//delete face
	f_tmp = lp_in->l_face;
	if (f_tmp->prev) {
		f_tmp->prev->next = f_tmp->next;
	}
	if (f_tmp->next) {
		f_tmp->next->prev = f_tmp->prev;
	}
	f_tmp->prev = f_tmp->next = NULL;
	
	lp_in->l_face = lp_out->l_face;
	
	delete f_tmp;
}
void CCAD_V1Doc::sweep(CVertex *v_Head, int *l_No, double *vector) {
	// make the first face
	// here we resume that the first loop is the out one, and others are inner loops
	_cprintf("sweep:\n");
	CVertex *v_tmp = v_Head;
	CVertex *v_tmp_h = v_Head;
	CSolid *s;
	s = mvfs(v_tmp_h->v_coord);
	//_cprintf("s-v:%f,%f,%f\n", s->s_v_head->v_coord[0], s->s_v_head->v_coord[1], s->s_v_head->v_coord[2]);
	int l_count = 0;
	for(l_count=0; l_No[l_count] != 0; l_count++){}
	int v_count = 1;
	int l_no_count = 0;
	v_tmp = v_Head;
	// make the loops, the first one is the outer one;
	for (v_count; v_count < l_No[l_no_count]; v_count++) {
		mev(v_tmp->v_coord, v_tmp->next->v_coord, s->s_face->f_loop);
		v_tmp = v_tmp->next;
	}
	mef(v_tmp->v_coord, v_tmp_h->v_coord, s->s_face->f_loop);
	l_no_count++;
	
	//the first vertex is at the last of list
	CVertex *v_tmp_last;
	for (v_tmp_last = s->s_v_head; v_tmp_last->next != NULL; v_tmp_last = v_tmp_last->next) {}
	CFace *f_tmp_last;
	for (f_tmp_last = s->s_face; f_tmp_last->next != NULL; f_tmp_last = f_tmp_last->next) {}
	
	if (l_No[l_no_count] > 0) {
		v_tmp = v_tmp->next;
		v_tmp_h = v_tmp;
		// make the inner loops
		//_cprintf("v_count:%d,l_No[%d]:%d\n", v_count, l_no_count, l_No[l_no_count]);//4-1
		for (l_no_count; l_No[l_no_count] != 0; l_no_count++) {
			_cprintf("FORIN1___v_count:%d,l_No[%d]:%d\n", v_count, l_no_count, l_No[l_no_count]);
			mev(v_tmp_last->v_coord, v_tmp_h->v_coord, f_tmp_last->prev->f_loop);
			v_count++;
			for (v_count; v_count < l_No[l_no_count]; v_count++) {
				mev(v_tmp->v_coord, v_tmp->next->v_coord, f_tmp_last->prev->f_loop);
				v_tmp = v_tmp->next;
			}
			mef(v_tmp->v_coord, v_tmp_h->v_coord, f_tmp_last->prev->f_loop);// to make the loop whose e will be killed be the first loop of the face
			kemr(v_tmp_h->v_coord, v_tmp_last->v_coord, f_tmp_last->prev->f_loop);
			
			v_tmp = v_tmp->next;
			v_tmp_h = v_tmp;
			
		}
	}

	_cprintf("--------------start sweep--------------\n");
	v_tmp = v_Head;
	v_count = 1;
	l_no_count = 0;
	double new_coord[3], new_coord_h[3], new_coord_prev[3];
	new_coord[0] = new_coord_h[0] = new_coord_prev[0] = v_tmp->v_coord[0] + vector[0];
	new_coord[1] = new_coord_h[1] = new_coord_prev[1] = v_tmp->v_coord[1] + vector[1];
	new_coord[2] = new_coord_h[2] = new_coord_prev[2] = v_tmp->v_coord[2] + vector[2];
	mev(v_tmp->v_coord, new_coord, f_tmp_last->f_loop);

	v_tmp = v_tmp->next;
	for (v_count; v_count < l_No[l_no_count]; v_count++) {
		new_coord[0] = v_tmp->v_coord[0] + vector[0];
		new_coord[1] = v_tmp->v_coord[1] + vector[1];
		new_coord[2] = v_tmp->v_coord[2] + vector[2];
		_cprintf("v_tmp: %f,%f,%f\n+++", v_tmp->v_coord[0], v_tmp->v_coord[1], v_tmp->v_coord[2]);
		mev(v_tmp->v_coord, new_coord, f_tmp_last->f_loop);
		mef(new_coord_prev, new_coord, f_tmp_last->f_loop);
		new_coord_prev[0] = new_coord[0];
		new_coord_prev[1] = new_coord[1];
		new_coord_prev[2] = new_coord[2];
		v_tmp = v_tmp->next;
	}
	mef(new_coord_h, new_coord, f_tmp_last->f_loop);
	
	l_no_count = 1;
	v_tmp_h = v_tmp;
	_cprintf("---------------sweep_inner--------------\n");
	for (l_no_count; l_No[l_no_count] != 0; l_no_count++) {
		new_coord[0] = new_coord_h[0] = new_coord_prev[0] = v_tmp->v_coord[0] + vector[0];
		new_coord[1] = new_coord_h[1] = new_coord_prev[1] = v_tmp->v_coord[1] + vector[1];
		new_coord[2] = new_coord_h[2] = new_coord_prev[2] = v_tmp->v_coord[2] + vector[2];
		
		CLoop *tmp_lp = f_tmp_last->prev->f_loop;
		_cprintf("---------------sweep_inner---%d-----------\n", l_no_count);
		
		CHEdge *h = tmp_lp->l_hedge;
		
		for (tmp_lp; tmp_lp->next != NULL; tmp_lp = tmp_lp->next) {}
		
		for (int i = l_no_count; i > 1; i--) {
			_cprintf("------%d\n",l_no_count);
			tmp_lp = tmp_lp->prev;
		}
		tmp_lp = tmp_lp->l_hedge->he_adj->he_loop;

		mev(v_tmp_h->v_coord, new_coord, tmp_lp);
		v_tmp = v_tmp->next;
		v_count++;
		
		for (v_count; v_count < l_No[l_no_count]; v_count++) {
			new_coord[0] = v_tmp->v_coord[0] + vector[0];
			new_coord[1] = v_tmp->v_coord[1] + vector[1];
			new_coord[2] = v_tmp->v_coord[2] + vector[2];
			mev(v_tmp->v_coord, new_coord, tmp_lp);
			mef(new_coord, new_coord_prev, tmp_lp);
			new_coord_prev[0] = new_coord[0];
			new_coord_prev[1] = new_coord[1];
			new_coord_prev[2] = new_coord[2];
			v_tmp = v_tmp->next;
		}
		
		mef(new_coord,new_coord_h, tmp_lp);// to make the loop whose e will be killed be the first loop of the face
		///kfmrh的大圈是错的
		CFace *f_out = f_tmp_last;
		//_cprintf("l_count:%d\n", l_count);
		for (int i = l_No[0]+l_count-1; i > 0; i--) {
			f_out = f_out->prev;
		}
		kfmrh(s->s_face->f_loop, f_out->prev->f_loop);
		v_tmp_h = v_tmp;
	}
	_cprintf("---------------test--------------\n");

	int countf =1, countl = 1;
	for (CFace *f = s->s_face; f; f = f->next) {
		countl = 1;
		for (CLoop *l = f->f_loop; l; l = l->next) {
			_cprintf("face%d->loop%d:\n", countf, countl);
			CHEdge *h = l->l_hedge;
			_cprintf("%f,%f,%f\n",h->he_v->v_coord[0], h->he_v->v_coord[1], h->he_v->v_coord[2]); 
			_cprintf("%f,%f,%f\n", h->next->he_v->v_coord[0], h->he_v->v_coord[1], h->next->he_v->v_coord[2]);
			_cprintf("%f,%f,%f\n", h->next->next->he_v->v_coord[0], h->next->next->he_v->v_coord[1], h->next->next->he_v->v_coord[2]);
			_cprintf("%f,%f,%f\n", h->next->next->next->he_v->v_coord[0], h->next->next->next->he_v->v_coord[1], h->next->next->next->he_v->v_coord[2]);
			countl++;
		}
		countf++;
	}
	
	
	/*int count1 = 1;
	CFace *pFace;
	if (s->s_face->f_loop->l_hedge) {
		_cprintf("exist");
	}
	/*for (pFace = s->s_face; pFace; pFace = pFace->next) {
		CHEdge *h = pFace->f_loop->l_hedge;
	}*/
	CalcNormal(s);
	this->solid = s;
	/*{
		CHEdge *heee = s->s_face->f_loop->l_hedge;
		_cprintf("face1->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
		heee = s->s_face->next->f_loop->l_hedge;
		_cprintf("face2->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
		heee = s->s_face->next->next->f_loop->l_hedge;
		_cprintf("face3->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
		heee = s->s_face->next->next->next->f_loop->l_hedge;
		_cprintf("face4->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
		heee = s->s_face->next->next->next->next->f_loop->l_hedge;
		_cprintf("face5->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
		heee = s->s_face->next->next->next->next->next->f_loop->l_hedge;
		_cprintf("face6->loop:");
		for (int count = 0; count < 6; count++) {
			_cprintf(" %f,%f,%f\n", heee->he_v->v_coord[0], heee->he_v->v_coord[1], heee->he_v->v_coord[2]);
			heee = heee->next;
		}
	}*/
}
BOOL CCAD_V1Doc::JudgeSameV(GLdouble *v1_coord, GLdouble *v2_coord) {
	if (v1_coord[0] == v2_coord[0] && v1_coord[1] == v2_coord[1] && v1_coord[2] == v2_coord[2]) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void CCAD_V1Doc::CalcNormal(CSolid	*pSolid)
{
	CFace		*pFace;
	CHEdge	*pEdge, *pHeHead;
	CVertex		*pVert1, *pVert2, *pVert3;
	GLdouble	sub1[3], sub2[3], normal[3];
	GLdouble	sum_norm;

	pFace = pSolid->s_face;
	while (pFace)
	{
		pHeHead = pEdge = pFace->f_loop->l_hedge;
		pVert3 = pEdge->he_v;
		pVert2 = pEdge->next->he_v;
		pVert1 = pEdge->next->next->he_v;
		sub1[0] = pVert1->v_coord[0] - pVert2->v_coord[0];
		sub1[1] = pVert1->v_coord[1] - pVert2->v_coord[1];
		sub1[2] = pVert1->v_coord[2] - pVert2->v_coord[2];
		sub2[0] = pVert2->v_coord[0] - pVert3->v_coord[0];
		sub2[1] = pVert2->v_coord[1] - pVert3->v_coord[1];
		sub2[2] = pVert2->v_coord[2] - pVert3->v_coord[2];
		normal[0] = sub1[1] * sub2[2] - sub2[1] * sub1[2];
		normal[1] = sub1[2] * sub2[0] - sub2[2] * sub1[0];
		normal[2] = sub1[0] * sub2[1] - sub2[0] * sub1[1];
		sum_norm = normal[0] + normal[1] + normal[2];
		sum_norm = fabs(sum_norm);
		normal[0] = normal[0] / sum_norm;
		normal[1] = normal[1] / sum_norm;
		normal[2] = normal[2] / sum_norm;
		pFace->normal[0] = -normal[0];
		pFace->normal[1] = -normal[1];
		pFace->normal[2] = -normal[2];
		pFace = pFace->next;
	}
}
// CCAD_V1Doc commands
