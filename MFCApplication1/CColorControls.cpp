// CColorControls.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CColorControls.h"


// CColorControls 대화 상자

IMPLEMENT_DYNAMIC(CColorControls, CDialogEx)

CColorControls::CColorControls(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLOR_CONYROLS, pParent)
{

}

CColorControls::~CColorControls()
{
}

void CColorControls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColorControls, CDialogEx)
END_MESSAGE_MAP()


// CColorControls 메시지 처리기
