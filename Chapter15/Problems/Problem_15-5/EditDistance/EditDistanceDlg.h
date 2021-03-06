
// EditDistanceDlg.h: 头文件
//

#pragma once


#include "EditDistanceAlgo.h"
#include "TransferView.h"


// CEditDistanceDlg 对话框
class CEditDistanceDlg : public CDialogEx
{
// 构造
public:
	CEditDistanceDlg(CWnd* pParent = nullptr);	// 标准构造函数

	//const CString& GetOriginalString() const { return m_OriginalString; }
	//const CString& GetTargetString() const { return m_TargetString; }
	const EditDistance& GetEditDistance() const { return m_EditDistance; }

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITDISTANCE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	EditDistance m_EditDistance;
	CTransferView m_TransferView;
	CFont m_EditFont;
	CString m_OriginalString;
	CString m_TargetString;
public:
	afx_msg void OnEditDistance();
	afx_msg void OnClear();
	double m_CopyCost;
	double m_ReplaceCost;
	double m_DeleteCost;
	double m_InsertCost;
	double m_TwiddleCost;
	double m_KillCost;
};
