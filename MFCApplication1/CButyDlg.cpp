// CButyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CButyDlg.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

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
	DDX_Control(pDX, IDC_PC_FT, picCtrl_FT);

}


BEGIN_MESSAGE_MAP(CButyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CButyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REVERT_FT, &CButyDlg::OnBnClickedRevertFt)
	ON_BN_CLICKED(IDCANCEL, &CButyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DETECT, &CButyDlg::OnBnClickedDetect)
	ON_BN_CLICKED(IDC_MERGE, &CButyDlg::OnBnClickedMerge)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CButyDlg 메시지 처리기
BOOL CButyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(350, 140, 1280, 720);

	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);
	GetDlgItem(IDC_REVERT_FT)->MoveWindow(1000, 720 - 220, 200, 45);
	GetDlgItem(IDC_DETECT)->MoveWindow(1000, 720 - 280, 200, 45);
	GetDlgItem(IDC_MERGE)->MoveWindow(1000, 720 - 400, 200, 45);

	// Slider control 초기화
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_EYE));
	if (pSlider)
	{
		pSlider->SetRange(1, 100); // 적절한 범위 설정
		pSlider->SetPos(50);       // 초기값 설정

		// Slider control의 위치 설정 (절대적인 수치 사용)
		pSlider->MoveWindow(1000, 720 - 280 - 55, 200, 45);  // 적절한 수치로 설정
	}

	// 초록색 원의 초기 크기를 50으로 설정
	eyeRadius = 50;

	//DrawImage(); dialog 호출시 oninitDiaog()뒤에 실행되는 메세지들에 의하여, 사진이 출력되지 않음 
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//적용 버튼을 누르면, 창을 종료하고, 부모창에도 변경된 이미지가 보여짐 
void CButyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//메세지 박스 적용되었습니다 알림 넣기 
	CDialogEx::OnOK();
}


//다이얼로그창에 사진 띄우기 
void CButyDlg::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) {
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
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, requestImg.cols, requestImg.rows, requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}


void CButyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent) {
	case 1:
		DrawImage(myImg, myBitmapInfo);//처음 로딩되는 이미지 

	}
	KillTimer(1);//처음 필터창을 켰을때, 사진을 띄우기 위한 용도라 바로 kill 
	CDialogEx::OnTimer(nIDEvent);
}

//윈도우 창이 DESTORY 갑자기 종료될때, 저장해야 하는 정보들 기능 수행
void CButyDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	//KillTimer(1);
	//저장 버튼 눌렀을 때, bmpinfo 변경되야 함 
	//취소 버튼 눌렀을 떄, bmpinfo 유지되야함. 

	// TODO: Add your message handler code here
}


//BITMAP 정보 구조체 데이터 생성 
void CButyDlg::CreateBitmapInfo(BITMAPINFO** btmInfo, int w, int h, int bpp) {
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

//되돌리기 기능 (원본 이미지로 돌아감) 
void CButyDlg::OnBnClickedRevertFt()
{
	// TODO: Add your control notification handler code here
	myImg = myOriginalImg.clone();
	DrawImage(myImg, myBitmapInfo);
	MessageBox(L"원본으로 돌아갑니다", L"알림", MB_OK);
}

//필터 창을 종료하고, 부모창 사진에는 변경이 안되어 있어야함. 
void CButyDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CButyDlg::OnBnClickedDetect()
{
	try
	{
		// Convert the Mat image to grayscale for eye detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Copy the current image to myOriginalImg (assuming myOriginalImg is initialized)
		myOriginalImg = myImg.clone();

		// Load pre-trained eye cascade classifier
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// Detect eyes in the image
		std::vector<Rect> eyes;
		eyeCascade.detectMultiScale(grayImage, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Loop over detected eyes
		for (size_t i = 0; i < eyes.size(); i++)
		{
			Point eyeCenter(eyes[i].x + eyes[i].width / 2, eyes[i].y + eyes[i].height / 2);
			int radius = cvRound(eyeRadius);
			circle(myImg, eyeCenter, radius, Scalar(0, 255, 0), 2);
		}

		// Redraw the image with eye detection
		DrawImage(myImg, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(CString("Error during eye detection: ") + ex.what());
	}
}

void CButyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_EYE)
	{
		// 눈의 크기를 slider 값으로 설정
		eyeRadius = static_cast<int>(SendDlgItemMessage(IDC_SLIDER_EYE, TBM_GETPOS, 0, 0));

		// 다시 그리기
		DrawImage(myImg, myBitmapInfo);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CButyDlg::OnBnClickedMerge()
{
	try
	{
		// Convert the Mat image to grayscale for face detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained face cascade classifier
		CascadeClassifier faceCascade;
		faceCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_default.xml");

		// Load the image to be overlaid (headpin.jpg)
		Mat overlayImage = imread("C:\\Users\\EMBEDDED\\source\\repos\\intel1\\cpp\\images\\STAR01.jpg");

		// Detect faces in the image
		std::vector<Rect> faces;
		faceCascade.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Loop over detected faces
		for (size_t i = 0; i < faces.size(); i++)
		{
			Point faceCenter(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			int radius = cvRound(faces[i].width / 2.0);

			// Draw the green circle
			circle(myImg, faceCenter, radius, Scalar(0, 255, 0), 2);

			// Define the region of interest (ROI) where the overlay will be applied
			Rect roiRect(faceCenter.x - radius, faces[i].y, 2 * radius, radius);
			Mat roi = myImg(roiRect);

			// Resize overlay image to match the ROI size
			resize(overlayImage, overlayImage, roi.size());

			// Blend the overlay image with the ROI using addWeighted
			addWeighted(roi, 0.2, overlayImage, 1.0, 0.0, roi);
		}

		// Redraw the image with face detection and overlay
		DrawImage(myImg, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(CString("Error during face detection: ") + ex.what());
	}
}