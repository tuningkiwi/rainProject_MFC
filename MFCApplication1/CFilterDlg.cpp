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
	DDX_Control(pDX, IDC_PC_FT, picCtrl_FT);
	DDX_Control(pDX, IDC_EMBOSS_FT, embossFT);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
//	ON_WM_GETMINMAXINFO()
ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
//ON_BN_CLICKED(IDC_IMGLOAD_FT, &CFilterDlg::OnBnClickedImgloadFt)
ON_WM_TIMER()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_EMBOSS_FT, &CFilterDlg::OnBnClickedEmbossFt)
END_MESSAGE_MAP()


// CFilterDlg 메시지 처리기


BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(350, 140, 1280, 720);
	//picCtrl_FT.MoveWindow(0,0,1000,720);
	GetDlgItem(IDOK)->MoveWindow(1000,720-160, 200, 45);
	GetDlgItem(IDCANCEL)->MoveWindow(1000,720-100, 200, 45);
	embossFT.MoveWindow(1000, 40, 200, 45);
	
	//DrawImage(); dialog 호출시 oninitDiaog()뒤에 실행되는 메세지들에 의하여, 사진이 출력되지 않음 
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 
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


void CFilterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//메세지 박스 적용되었습니다 알림 넣기 
	CDialogEx::OnOK();
}


//다이얼로그창에 사진 띄우기 
void CFilterDlg::DrawImage(Mat requestImg) {
	KillTimer(1); 

	//필터창 크기
	CRect rect;
	this->GetClientRect(&rect);
	//CRect ftWnd(0,0,1280,720);// four-integers are left, top, right, and bottom
	// picctrl 너비, 높이 조정 
	int wx = int(rect.right * 5 / 6);
	int wy = rect.bottom;

	//불러올 사진 cols 가져오기.
	CClientDC dc(GetDlgItem(IDC_PC_FT)); 
	//CRect rect;// 이미지를 넣을 사각형 
	if (requestImg.cols > wx) {
		//cols: 1080 = rows : wid;
		int resize_h = cvRound((wx * requestImg.rows) / requestImg.cols);
		int y = cvRound((wy - resize_h) / 2);
		//if (y < 0) { //구현예정. 
		//	//float ratio =  m_matImage.rows / m_matImage.cols;
		//	y = 0;
		//	
		//}
		GetDlgItem(IDC_PC_FT)->MoveWindow(0, y, requestImg.cols, resize_h);
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2);
		int y = cvRound((wy - requestImg.rows) / 2);
		GetDlgItem(IDC_PC_FT)->MoveWindow(x, y, requestImg.cols, requestImg.rows);
	}


	//GetClientRect(left, top, right, bottom ) 클라이언트 영역의 좌표
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//불러올 이미지 사진에 따라 조정된 Picture Ctrl 크기 
	GetDlgItem(IDC_PC_FT)->GetClientRect(&rect);

	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	// 대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	// 함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, requestImg.cols, requestImg.rows, requestImg.data, myBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

//void CFilterDlg::OnBnClickedImgloadFt()
//{
	// TODO: Add your control notification handler code here
//	DrawImage();
//}


void CFilterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent) {
	case 1:
		DrawImage(myImg);//처음 로딩되는 이미지 
	}
	KillTimer(1);//처음 필터창을 켰을때, 사진을 띄우기 위한 용도라 바로 kill 
	CDialogEx::OnTimer(nIDEvent);
}


void CFilterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	//KillTimer(1);
	//저장 버튼 눌렀을 때, bmpinfo 변경되야 함 
	//취소 버튼 눌렀을 떄, bmpinfo 유지되야함. 

	// TODO: Add your message handler code here
}

//진흙필터 :1채널 필터링 : grayscale이미지로 변환 필요 
void CFilterDlg::OnBnClickedEmbossFt()//1채널 필터링 
{
	// TODO: Add your control notification handler code here
	colorToGray();//color 사진의 경우 gray로 변경. 

	float data[] = { -1,-1,0,-1,0,1,0,1,1 };
	Mat emboss(3, 3, CV_32FC1, data);

	Mat dst;
	filter2D(myImgAfterChange, dst, -1, emboss, Point(-1, -1), 128);
	myImgAfterChange = dst;
	DrawImage(myImgAfterChange);
}

BOOL CFilterDlg::colorToGray()
{
	// TODO: Add your implementation code here.
	//현재 채널 정보를 확인해서 gray가 아닐 경우
	if (myImg.channels() != 1) {
		cvtColor(myImg, myImgAfterChange, COLOR_BGR2GRAY);
		CreateBitmapInfo(myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
		return true; 
	}
	return false; 
}


//BITMAP 정보 구조체 데이터 생성 
void CFilterDlg::CreateBitmapInfo(int w, int h, int bpp) {
	if (myBitmapInfo != NULL) //기존 비트맵 정보 초기화 
	{
		delete myBitmapInfo;
		myBitmapInfo = NULL; //기존 BITMAP 정보 삭제 
	}

	if (bpp == 8) //1채널 
		myBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		myBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	myBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	myBitmapInfo->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	myBitmapInfo->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	myBitmapInfo->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	myBitmapInfo->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	myBitmapInfo->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	myBitmapInfo->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	myBitmapInfo->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	myBitmapInfo->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			myBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			myBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			myBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			myBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	myBitmapInfo->bmiHeader.biWidth = w;
	myBitmapInfo->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}
