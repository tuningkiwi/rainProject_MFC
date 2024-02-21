// CButyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CButyDlg.h"


// CButyDlg 대화 상자

IMPLEMENT_DYNAMIC(CButyDlg, CDialogEx)

CButyDlg::CButyDlg()
	: CDialogEx(IDD_BUTY)
{

}

CButyDlg::CButyDlg(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_BUTY)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CButyDlg::~CButyDlg()
{
}

void CButyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CButyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 다이얼로그 창의 이름(캡션)을 변경합니다.
	SetWindowText(_T("뷰티"));

	// 다이얼로그 크기를 1280x720으로 설정합니다.
	CRect rectClient;
	rectClient.SetRect(0, 0, 1280, 720);
	AdjustWindowRectEx(&rectClient, GetStyle(), FALSE, GetExStyle());
	SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(), SWP_NOMOVE | SWP_NOZORDER);

	// 버튼의 새로운 위치를 설정합니다.
	CWnd* save = GetDlgItem(IDC_BUTTON1);
	CWnd* cancel = GetDlgItem(IDC_BUTTON2);

	if (save && cancel)
	{
		// 버튼1 위치 변경
		save->SetWindowPos(NULL, 200, 200, 100, 30, SWP_NOZORDER);

		// 버튼2 위치 변경
		cancel->SetWindowPos(NULL, 200, 300, 100, 30, SWP_NOZORDER);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CButyDlg, CDialogEx)
END_MESSAGE_MAP()


// CButyDlg 메시지 처리기
