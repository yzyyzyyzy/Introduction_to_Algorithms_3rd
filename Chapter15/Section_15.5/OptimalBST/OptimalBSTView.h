#pragma once


#include <string>
#include <sstream>
#include "BinaryTree.h"

using namespace std;


// COptimalBSTView

class COptimalBSTView : public CWnd
{
	DECLARE_DYNAMIC(COptimalBSTView)

public:
	COptimalBSTView();
	virtual ~COptimalBSTView();

	void ResetScrollRange();

protected:
	DECLARE_MESSAGE_MAP()

private:
	static const int m_TreeNodeWidth = 30;
	static const int m_TreeNodeHeight = 50;
	static const int m_FontHeight = 18;
	template <typename T> CSize GetPaintSize(const BinaryTree<T> &bt);
	template <typename T> void Paint(const BinaryTree<T> &bt);
	template <typename T> void PaintNode(CDC *pDC, const BinaryTreeNode<T> *subtree, int x, int y, int step);

public:
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


template <typename T>
CSize COptimalBSTView::GetPaintSize(const BinaryTree<T> &bt)
{
	int h = bt.get_height();
	int w = 0;

	if(h != -1)
		w = (int)(pow(2.0, h) * m_TreeNodeWidth);

	int dw1 = w;
	int dw2 = w;

	const BinaryTreeNode<T> *p = bt.get_root();
	while(p != NULL)
	{
		dw1 >>= 1;
		p = p->left;
	}

	const BinaryTreeNode<T> *q = bt.get_root();
	while(q != NULL)
	{
		dw2 >>= 1;
		q = q->right;
	}

	dw1 -= (m_TreeNodeWidth/2);
	dw2 -= (m_TreeNodeWidth/2);

	return CSize(w-dw1-dw2, (h+1)*m_TreeNodeHeight);
}

template <typename T>
void COptimalBSTView::Paint(const BinaryTree<T> &bt)
{
	int dx = GetScrollPos(SB_HORZ);
	int dy = GetScrollPos(SB_VERT);

	int h = bt.get_height();
	int w = 0;

	if(h != -1)
		w = (int)(pow(2.0, h) * m_TreeNodeWidth);

	int dw1 = w;
	int dw2 = w;

	const BinaryTreeNode<T> *p = bt.get_root();
	while(p != NULL)
	{ 
		dw1 >>= 1;
		p = p->left;
	
	}

	const BinaryTreeNode<T> *q = bt.get_root();
	while(q != NULL)
	{
		dw2 >>= 1;
		q = q->right;
	}

	dw1 -= (m_TreeNodeWidth/2);
	dw2 -= (m_TreeNodeWidth/2);

	CRect wnd_rect;
	GetClientRect(wnd_rect);
	CRect paint_rect(0, 0, w-dw1-dw2, (h+1)*m_TreeNodeHeight);
	paint_rect -= CSize(dx, dy);

	CClientDC dc(this);
	dc.ExcludeClipRect(paint_rect);
	dc.FillSolidRect(wnd_rect, RGB(180, 180, 180));
	CRgn rgn;
	rgn.CreateRectRgn(wnd_rect.left, wnd_rect.top, wnd_rect.right, wnd_rect.bottom);
	dc.SelectClipRgn(&rgn, RGN_COPY);
	dc.FillSolidRect(paint_rect, RGB(255, 255, 255));

	if(bt.get_root() != NULL)
	{
		CFont font;
		font.CreateFont(m_FontHeight, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, CString("Courier New"));
		CFont *pOldFont = dc.SelectObject(&font);
		int oldTextAlign = dc.SetTextAlign(TA_BOTTOM|TA_CENTER);

		PaintNode(&dc, bt.get_root(), w/2-dw1-dx, m_TreeNodeHeight/2-dy, w/2);

		dc.SetTextAlign(oldTextAlign);
		dc.SelectObject(pOldFont);
	}
}


template <typename T>
void COptimalBSTView::PaintNode(CDC *pDC, const BinaryTreeNode<T> *subtree, int x, int y, int step)
{
	CString str;
	int cx, cy;
	step >>= 1;

	ostringstream is;
	is << subtree->data;
	string str1(is.str());

	str = str1.c_str();
	pDC->TextOut(x, y+m_FontHeight/2, str);

	if(subtree->left != NULL)
	{
		cx = x-step;
		cy = y+m_TreeNodeHeight;
		PaintNode(pDC, subtree->left, cx, cy, step);
		pDC->MoveTo(x-2, y+m_FontHeight/2+2);
		pDC->LineTo(cx, cy-m_FontHeight/2-2);
	}

	if(subtree->right != NULL)
	{
		cx = x+step;
		cy = y+m_TreeNodeHeight;
		PaintNode(pDC, subtree->right, cx, cy, step);
		pDC->MoveTo(x+2, y+m_FontHeight/2+2);
		pDC->LineTo(cx, cy-m_FontHeight/2-2);
	}
}
