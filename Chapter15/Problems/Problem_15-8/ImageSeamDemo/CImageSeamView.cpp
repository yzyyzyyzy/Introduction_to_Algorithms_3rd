// CImageSeamView.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageSeamDemo.h"
#include "CImageSeamView.h"
#include "ImageSeamDemoDlg.h"
#include "ImageSeam.h"


// CImageSeamView

IMPLEMENT_DYNAMIC(CImageSeamView, CWnd)

CImageSeamView::CImageSeamView()
{

}

CImageSeamView::~CImageSeamView()
{
}


BEGIN_MESSAGE_MAP(CImageSeamView, CWnd)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CImageSeamView::ResetScrollRange()
{
	int paintTotalWidth = 0;
	int paintTotalHeight = 0;

	CImageSeamDemoDlg *pDlg = (CImageSeamDemoDlg*)GetParent();

	if (pDlg != NULL)
	{
		paintTotalWidth = pDlg->GetImageSeam().GetWidth() * blockSideLength + 80;
		paintTotalHeight = pDlg->GetImageSeam().GetHeight() * blockSideLength + 20;
	}

	SCROLLINFO scroll_info;
	scroll_info.cbSize = sizeof(SCROLLINFO);

	CRect client;
	GetClientRect(client);
	scroll_info.fMask = SIF_RANGE | SIF_PAGE;
	scroll_info.nMin = 0;
	scroll_info.nMax = paintTotalWidth;
	scroll_info.nPage = client.Width();
	SetScrollInfo(SB_HORZ, &scroll_info);

	GetClientRect(client);
	scroll_info.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
	scroll_info.nMin = 0;
	scroll_info.nMax = paintTotalHeight;
	scroll_info.nPage = client.Height();
	SetScrollInfo(SB_VERT, &scroll_info);
}

void CImageSeamView::PaintImageSeam(CDC &dc)
{
	CRect wndRect;
	GetClientRect(wndRect);

	CRgn rgn;
	rgn.CreateRectRgn(wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
	dc.SelectClipRgn(&rgn, RGN_COPY);
	dc.FillSolidRect(wndRect, RGB(255, 255, 255));

	CImageSeamDemoDlg *pDlg = (CImageSeamDemoDlg*)GetParent();
	if (pDlg == NULL) return;

	int dx = GetScrollPos(SB_HORZ);
	int dy = GetScrollPos(SB_VERT);

	int paintTotalWidth = pDlg->GetImageSeam().GetWidth() * blockSideLength;
	int paintTotalHeight = pDlg->GetImageSeam().GetHeight() * blockSideLength;

	int rowBase = 10 - dy;
	int columnBase = wndRect.Width() > paintTotalWidth+80 ? (wndRect.Width()-paintTotalWidth)/2-dx : 40-dx;

	dc.MoveTo(columnBase, rowBase);
	dc.LineTo(columnBase+paintTotalWidth, rowBase);

	dc.MoveTo(columnBase, rowBase);
	dc.LineTo(columnBase, rowBase+paintTotalHeight);

	int rowCoordinate = rowBase;
	int columnCoordinate = columnBase;

	for (int i = 0; i < pDlg->GetImageSeam().GetHeight(); ++i)
	{
		rowCoordinate += blockSideLength;
		dc.MoveTo(columnBase, rowCoordinate);
		dc.LineTo(columnBase+paintTotalWidth+1, rowCoordinate);
	}

	for (int j = 0; j < pDlg->GetImageSeam().GetWidth(); ++j)
	{
		columnCoordinate += blockSideLength;
		dc.MoveTo(columnCoordinate, rowBase);
		dc.LineTo(columnCoordinate, rowBase+paintTotalHeight+1);
	}

	const int *pSeamChain = pDlg->GetImageSeam().GetSeamChainBuffer();

	if (pSeamChain != NULL)
	{
		rowCoordinate = rowBase;
		CBrush brush(RGB(192, 192, 192));

		for (int i = 0; i < pDlg->GetImageSeam().GetHeight(); ++i)
		{
			columnCoordinate = columnBase + pSeamChain[i]*blockSideLength;
			CRect rect(columnCoordinate+1, rowCoordinate+1, columnCoordinate+blockSideLength, rowCoordinate+blockSideLength);
			dc.FillRect(rect, &brush);
			rowCoordinate += blockSideLength;
		}
	}

	const unsigned char *pDisruption = pDlg->GetImageSeam().GetDisruptionBuffer();

	if (pDisruption != NULL)
	{
		CFont font;
		font.CreateFont(fontHeight, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
						OUT_TT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY,
						VARIABLE_PITCH | FF_ROMAN, CString("Courier New"));
		CFont *pOldFont = dc.SelectObject(&font);
		int oldTextAlign = dc.SetTextAlign(TA_TOP|TA_CENTER);
		int oldBkMode = dc.SetBkMode(TRANSPARENT);

		rowCoordinate = rowBase + blockSideLength/2 - fontHeight/2;

		for (int i = 0; i < pDlg->GetImageSeam().GetHeight(); ++i)
		{
			columnCoordinate = columnBase + blockSideLength/2;

			for (int j = 0; j < pDlg->GetImageSeam().GetWidth(); ++j)
			{
				CString str;
				str.Format("%d", static_cast<int>(*pDisruption++));
				dc.TextOut(columnCoordinate, rowCoordinate, str);
				columnCoordinate += blockSideLength;
			}

			rowCoordinate += blockSideLength;
		}

		dc.SelectObject(pOldFont);
		dc.SetTextAlign(oldTextAlign);
		dc.SetBkMode(oldBkMode);
	}
}


// CImageSeamView 消息处理程序

BOOL CImageSeamView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONDBLCLK)
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CImageSeamView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ResetScrollRange();
}

void CImageSeamView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x = GetScrollPos(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LEFT:
		x = 0;
		break;

	case SB_RIGHT:
		x = GetScrollLimit(SB_HORZ) - 1;
		break;

	case SB_LINELEFT:
		x -= 1;
		break;

	case SB_LINERIGHT:
		x += 1;
		break;

	case SB_PAGELEFT:
		x -= 10;
		break;

	case SB_PAGERIGHT:
		x += 10;
		break;

	case SB_THUMBTRACK:
		x = nPos;
		break;

	default:
		break;
	}

	int x_max = GetScrollLimit(SB_HORZ);
	if (x > x_max) x = x_max - 1;
	if (x < 0) x = 0;

	SetScrollPos(SB_HORZ, x);
	Invalidate();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageSeamView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int y = GetScrollPos(SB_VERT);

	switch (nSBCode)
	{
	case SB_TOP:
		y = 0;
		break;

	case SB_BOTTOM:
		y = GetScrollLimit(SB_VERT) - 1;
		break;

	case SB_LINEUP:
		y -= 1;
		break;

	case SB_LINEDOWN:
		y += 1;
		break;

	case SB_PAGEUP:
		y -= 10;
		break;

	case SB_PAGEDOWN:
		y += 10;
		break;

	case SB_THUMBTRACK:
		y = nPos;
		break;

	default:
		break;
	}

	int y_max = GetScrollLimit(SB_VERT);
	if (y > y_max) y = y_max - 1;
	if (y < 0) y = 0;

	SetScrollPos(SB_VERT, y);
	Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImageSeamView::OnPaint()
{
	static bool first_paint = 1;

	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()

	if (first_paint == 1)
	{
		first_paint = 0;
		ShowScrollBar(SB_BOTH);
		SCROLLINFO scroll_info;
		scroll_info.fMask = SIF_DISABLENOSCROLL;
		SetScrollInfo(SB_HORZ, &scroll_info);
		SetScrollInfo(SB_VERT, &scroll_info);
		//EnableScrollBarCtrl(SB_HORZ);
		//EnableScrollBarCtrl(SB_VERT);

		ResetScrollRange();
	}

	PaintImageSeam(dc);
}
