#pragma once


// CImageSeamView

class CImageSeamView : public CWnd
{
	DECLARE_DYNAMIC(CImageSeamView)

public:
	CImageSeamView();
	virtual ~CImageSeamView();

	void ResetScrollRange();

protected:
	DECLARE_MESSAGE_MAP()

	void PaintImageSeam(CDC &dc);

	const static int blockSideLength = 32;
	const static int fontHeight = 16;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
};


