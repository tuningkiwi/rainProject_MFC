#pragma once
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"

// CBRIGHTNESSCTRL 대화 상자

class CBRIGHTNESSCTRL : public CDialogEx
{
	DECLARE_DYNAMIC(CBRIGHTNESSCTRL)

public:
	CBRIGHTNESSCTRL(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBRIGHTNESSCTRL();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_BRIGHTNESSCTRL_BTN_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
