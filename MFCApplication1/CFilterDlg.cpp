// CFilterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CFilterDlg.h"


// CFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg()
	: CDialogEx(IDD_DIALOG1)
{

}

CFilterDlg::CFilterDlg(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_DIALOG1)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo; 
}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
//	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CFilterDlg 메시지 처리기


BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//INT_PTR CFilterDlg::DoModal()
//{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

//	return CDialogEx::DoModal();
//}


//void CFilterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	lpMMI->ptMinTrackSize = CPoint(1080,720);
//	lpMMI->ptMaxTrackSize = CPoint(1080,720);
//	CFilterDlg::OnGetMinMaxInfo(lpMMI);
	//CDialogEx::OnGetMinMaxInfo(lpMMI);
//}
