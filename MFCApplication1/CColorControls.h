#pragma once
#include "afxdialogex.h"


// CColorControls 대화 상자

class CColorControls : public CDialogEx
{
	DECLARE_DYNAMIC(CColorControls)

public:
	CColorControls(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColorControls();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_CONYROLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
