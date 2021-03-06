#pragma once


// CTransferView

class CTransferView : public CWnd
{
	DECLARE_DYNAMIC(CTransferView)

public:
	CTransferView();
	virtual ~CTransferView();

	void ResetScrollRange();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	static const int columnBlank = 60;
	static const int rowBlank = 8;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


