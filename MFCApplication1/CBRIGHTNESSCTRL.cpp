// CBRIGHTNESSCTRL.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"


// CBRIGHTNESSCTRL 대화 상자

IMPLEMENT_DYNAMIC(CBRIGHTNESSCTRL, CDialogEx)

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1, pParent)
{

}

CBRIGHTNESSCTRL::~CBRIGHTNESSCTRL()
{
}

void CBRIGHTNESSCTRL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBRIGHTNESSCTRL, CDialogEx)
END_MESSAGE_MAP()


// CBRIGHTNESSCTRL 메시지 처리기
