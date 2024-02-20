// CColorControls.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CColorControls.h"


// CColorControls 대화 상자

IMPLEMENT_DYNAMIC(CColorControls, CDialogEx)

//CColorControls::CColorControls(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_COLOR_CONYROLS, pParent)

CColorControls::CColorControls()
	: CDialogEx(IDD_COLOR_CONYROLS) //부모 클래스의 생성자를 호출
{
}

CColorControls::CColorControls(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_COLOR_CONYROLS), myImg(Img), myBitmapInfo(bitmapInfo)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CColorControls::~CColorControls()	
{
}

void CColorControls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_IMAGE_CONTROL, m_imageControl);
	// DDX_Control 매크로는 컨트롤을 CWnd 클래스의 하위 클래스에 연결할 때 사용
}


BEGIN_MESSAGE_MAP(CColorControls, CDialogEx)
END_MESSAGE_MAP()


// CColorControls 메시지 처리기

BOOL CColorControls::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(350, 140, 1280, 720);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}