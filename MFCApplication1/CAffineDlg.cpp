// CAffineDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CAffineDlg.h"


// CAffineDlg 대화 상자

IMPLEMENT_DYNAMIC(CAffineDlg, CDialogEx)

CAffineDlg::CAffineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Affine_it, pParent)
{

}

CAffineDlg::CAffineDlg(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_Affine_it)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CAffineDlg::~CAffineDlg()
{
}

void CAffineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAffineDlg, CDialogEx)
END_MESSAGE_MAP()


// CAffineDlg 메시지 처리기
BOOL CAffineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}