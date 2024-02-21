// CBRIGHTNESSCTRL.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"


// CBRIGHTNESSCTRL 대화 상자

IMPLEMENT_DYNAMIC(CBRIGHTNESSCTRL, CDialogEx)

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL()
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1)
{} //기본생성자 ? 왱?

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CBRIGHTNESSCTRL::~CBRIGHTNESSCTRL()
{
}

void CBRIGHTNESSCTRL::DoDataExchange(CDataExchange* pDX)
{
	
	CDialogEx::DoDataExchange(pDX);
	MoveWindow(350,120,1280,720);

	GetDlgItem(IDC_CANVAS)->MoveWindow(1200, 700, 25, 25); //이미지 불러온창

	GetDlgItem(IDC_STATIC8)->MoveWindow(960, 140,25,25); // 밝기
	GetDlgItem(IDC_STATIC9)->MoveWindow(960, 260, 25,25);// 대비
	GetDlgItem(IDC_STATIC7)->MoveWindow(900, 710, 50,50); // 'SETTING'그룹창
	
	GetDlgItem(IDC_SPIN2)->MoveWindow(1195, 280, 20, 20);
	GetDlgItem(IDC_EDIT2)->MoveWindow(1175, 280, 20, 20);
	
	GetDlgItem(IDC_SPIN1)->MoveWindow(1195, 163, 20, 20);
	GetDlgItem(IDC_EDIT1)->MoveWindow(1175, 163, 20, 20);
	
	GetDlgItem(IDC_SLIDER1)->MoveWindow(960, 160, 180, 45);
	GetDlgItem(IDC_SLIDER2)->MoveWindow(960, 280, 180, 45);
	
	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);
}


BEGIN_MESSAGE_MAP(CBRIGHTNESSCTRL, CDialogEx)
//	ON_STN_ENABLE(IDC_CANVAS, &CBRIGHTNESSCTRL::OnStnEnableCanvas)
END_MESSAGE_MAP()
// CBRIGHTNESSCTRL 메시지 처리기

void CBRIGHTNESSCTRL::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo)
{

}