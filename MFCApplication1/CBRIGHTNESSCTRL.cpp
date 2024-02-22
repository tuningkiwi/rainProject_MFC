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
{} //기본생성자 ?

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CBRIGHTNESSCTRL::~CBRIGHTNESSCTRL()
{}
BOOL CBRIGHTNESSCTRL::OnInitDialog()
{
	CDialog::OnInitDialog(); 
	MoveWindow(350, 120, 1280, 720);

	GetDlgItem(IDC_CANVAS)->MoveWindow(1200, 700, 25, 25); //이미지 불러온창

	GetDlgItem(IDC_STATIC8)->MoveWindow(960, 140, 25, 25); // 밝기
	GetDlgItem(IDC_STATIC9)->MoveWindow(960, 260, 25, 25);// 대비
	GetDlgItem(IDC_STATIC7)->MoveWindow(960, 710, 50, 50); // 'SETTING'그룹창

	GetDlgItem(IDC_SPIN2)->MoveWindow(1195, 280, 20, 20);
	GetDlgItem(IDC_EDIT2)->MoveWindow(1175, 280, 20, 20);

	GetDlgItem(IDC_SPIN1)->MoveWindow(1195, 163, 20, 20);
	GetDlgItem(IDC_EDIT1)->MoveWindow(1175, 163, 20, 20);

	GetDlgItem(IDC_SLIDER1)->MoveWindow(960, 160, 180, 45);
	GetDlgItem(IDC_SLIDER2)->MoveWindow(960, 280, 180, 45);

	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);


	SetTimer(1, 80, NULL);
	return TRUE; // 포커스 설정을 위한 기본값 반환
}
void CBRIGHTNESSCTRL::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	

}


BEGIN_MESSAGE_MAP(CBRIGHTNESSCTRL, CDialogEx)
//	ON_STN_ENABLE(IDC_CANVAS, &CBRIGHTNESSCTRL::OnStnEnableCanvas)
END_MESSAGE_MAP()
// CBRIGHTNESSCTRL 메시지 처리기

void CBRIGHTNESSCTRL::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) // 매트릭스 판때기 , 비트맵 판때기
{
	KillTimer(1);
	CRect canvas; // 먼저 메인에서 불러온 이미지를 이어서 받을 사각형 과(의)객체 'canvas' 생성
	this->GetClientRect(canvas); //사각형 객체에='canvas' [사각형의 x, y 좌표설정이되는 = (0, 0) 함수'GetClientRect']
	int wx = int(canvas.right * 5 / 6);
	int wy = canvas.bottom;

	//불러올 사진 cols 가져오기.

	CClientDC dc(GetDlgItem(IDC_CANVAS));
	CRect rect;// 이미지를 넣을 사각형 
	if (requestImg.cols > wx) {
		
		int resize_h = cvRound((wx * requestImg.rows) / requestImg.cols);
		int y = cvRound((wy - resize_h) / 2);
	
		GetDlgItem(IDC_CANVAS)->MoveWindow(0, y, requestImg.cols, resize_h);
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2);
		int y = cvRound((wy - requestImg.rows) / 2);
		GetDlgItem(IDC_CANVAS)->MoveWindow(x, y, requestImg.cols, requestImg.rows);
	}
	GetDlgItem(IDC_CANVAS)->GetClientRect(canvas);
	SetStretchBltMode(dc.GetSafeHdc(),COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, requestImg.cols, requestImg.rows,
		requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
	
}

void CBRIGHTNESSCTRL::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
			DrawImage(myImg ,myBitmapInfo);
	}
	KillTimer(1);
	CDialogEx::OnTimer(nIDEvent);
	
}

void CBRIGHTNESSCTRL::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CBRIGHTNESSCTRL::CreateBitmapInfo(BITMAPINFO** btmInfo, int w, int h, int bpp) {
	if (*btmInfo != NULL) //기존 비트맵 정보 초기화 
	{
		delete* btmInfo;
		*btmInfo = NULL; //기존 BITMAP 정보 삭제 
	}

	if (bpp == 8) //1채널 
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	(*btmInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	(*btmInfo)->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	(*btmInfo)->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	(*btmInfo)->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	(*btmInfo)->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	(*btmInfo)->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	(*btmInfo)->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	(*btmInfo)->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	(*btmInfo)->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			(*btmInfo)->bmiColors[i].rgbBlue = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbGreen = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbRed = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbReserved = 0;
		}
	}

	(*btmInfo)->bmiHeader.biWidth = w;
	(*btmInfo)->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}