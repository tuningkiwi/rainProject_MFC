﻿// CButyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CButyDlg.h"
#include <chrono>
#include <ctime>

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
	bmpHistory.push_back(Img);
	bmpInfoHistory.push_back(bitmapInfo);
}

CButyDlg::~CButyDlg()
{
}

void CButyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PC_FT, picCtrl_FT);
	DDX_Control(pDX, IDC_SLIDER_EYE, m_eyeSizeSliderCtrl);
}


BEGIN_MESSAGE_MAP(CButyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CButyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REVERT_FT, &CButyDlg::OnBnClickedRevertFt)
	ON_BN_CLICKED(IDCANCEL, &CButyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DETECT, &CButyDlg::OnBnClickedDetect)
	ON_BN_CLICKED(IDC_MERGE, &CButyDlg::OnBnClickedMerge)
	ON_BN_CLICKED(IDC_IMAGECALL, &CButyDlg::OnBnClickedImageCall)
	ON_BN_CLICKED(IDC_VIDEOCALL, &CButyDlg::OnBnClickedVideoCall)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EYE, &CButyDlg::OnSliderEyeSizeChanged)
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
	GetDlgItem(IDC_SLIDER_EYE)->MoveWindow(1200, 720 - 340, 200, 45);
	GetDlgItem(IDC_IMAGECALL)->MoveWindow(1000, 720 - 460, 200, 45);
	GetDlgItem(IDC_VIDEOCALL)->MoveWindow(1000, 720 - 520, 200, 45);

	// Slider control 초기화
	// Initialize the slider control for eye size
	m_eyeSizeSliderCtrl.SetRange(0.1, 4);  // Set your desired range
	m_eyeSizeSliderCtrl.SetPos(2);  // Set the initial eye size
	m_eyeSizeSliderCtrl.SetTicFreq(1);
	m_eyeSizeSliderCtrl.SetPageSize(1);

	// Register the event handler for the slider
	m_eyeSizeSliderCtrl.SetBuddy(GetDlgItem(IDC_SLIDER_EYE));  // IDC_STATIC_EYE_SIZE is an example static control to display the current eye size
	m_eyeSizeSliderCtrl.SetRangeMin(1);
	m_eyeSizeSliderCtrl.SetRangeMax(10);  // Set the same range as above

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//적용 버튼을 누르면, 창을 종료하고, 부모창에도 변경된 이미지가 보여짐 
void CButyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//메세지 박스 적용되었습니다 알림 넣기 

	KillTimer(2);
	CDialogEx::OnOK();
}


void CButyDlg::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) {
	KillTimer(1);

	// 현재 다이얼로그의 크기 가져오기
	CRect rect;
	this->GetClientRect(&rect);

	// 원본 이미지 크기와 다이얼로그 크기 비교
	bool isImageLarger = (requestImg.cols > rect.Width()) || (requestImg.rows > rect.Height());

	// 이미지의 비율 계산
	double imgAspectRatio = static_cast<double>(requestImg.cols) / requestImg.rows;
	int displayedWidth = requestImg.cols;
	int displayedHeight = requestImg.rows;

	// 이미지가 다이얼로그 창보다 큰 경우에만 크기 조절
	if (isImageLarger) {
		displayedHeight = rect.Height() * 4 / 5;
		displayedWidth = cvRound(displayedHeight * imgAspectRatio);

		// 이미지의 너비가 다이얼로그 너비보다 큰 경우에만 크기 조절
		if (displayedWidth > rect.Width()) {
			displayedWidth = rect.Width() * 5 / 6;
			displayedHeight = cvRound(displayedWidth / imgAspectRatio);
		}
	}

	// 이미지가 화면 중앙에 위치하도록 계산
	int x = (rect.Width() - displayedWidth) * 1 / 6;  // 다이얼로그 가로의 5/6에 해당하는 부분 중간에 위치
	int y = (rect.Height() - displayedHeight) * 3 / 5;  // 다이얼로그 세로의 5/6에 해당하는 부분 중간에 위치

	// Picture Control 위치 및 크기 조정
	GetDlgItem(IDC_PC_FT)->MoveWindow(x, y, displayedWidth, displayedHeight);

	// 화면에 이미지 그리기
	CClientDC dc(GetDlgItem(IDC_PC_FT));
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, displayedWidth, displayedHeight, 0, 0, requestImg.cols, requestImg.rows, requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}


void CButyDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0) {//0번타이머

		//처음 다이얼로그 창을 띄울 때 onInitDialog()에서 drawimage가 이후 
		// 자동 수행되는 메시지 함수에의해서 출력이 안되서 
		//onInitDialog()에 타이머로 걸어놓음  
		DrawImage(bmpHistory.back(), bmpInfoHistory.back());//처음 로딩되는 이미지 
		KillTimer(0);
	}

	// Handle other timer events or call the base class
	CDialogEx::OnTimer(nIDEvent);
}


void CButyDlg::OnBnClickedImageCall()
{
	DrawImage(myImg, myBitmapInfo);
}

void CButyDlg::OnBnClickedVideoCall()
{
	DrawImage(myImg, myBitmapInfo);
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
/*
void CButyDlg::DetectEyesAndDrawRectangles()
{
	// Get the current position of the slider
	int eyeSizeFactor = m_eyeSizeSliderCtrl.GetPos();

	try
	{
		// Convert the Mat image to grayscale for eye detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained eye cascade classifier
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// Detect eyes in the image
		std::vector<Rect> detectedEyes;
		eyeCascade.detectMultiScale(grayImage, detectedEyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Loop over detected eyes
		for (size_t i = 0; i < detectedEyes.size(); ++i)
		{
			// Crop the region of interest (ROI) from the original image using eyeRect
			Mat eyeCropped = myImg(detectedEyes[i]);

			// Resize the cropped eye image based on the slider value
			Mat resizedEye;
			resize(eyeCropped, resizedEye, Size(detectedEyes[i].width * eyeSizeFactor, detectedEyes[i].height * eyeSizeFactor));

			// Calculate the position to paste the resized eye back into the original image
			int pasteX = detectedEyes[i].x - (resizedEye.cols - detectedEyes[i].width) / 2;
			int pasteY = detectedEyes[i].y - (resizedEye.rows - detectedEyes[i].height) / 2;

			// Ensure that the pasted region is within the bounds of the original image
			pasteX = std::max(pasteX, 0);
			pasteY = std::max(pasteY, 0);

			// Region of interest (ROI) in the original image to paste the resized eye
			Rect pasteRect(pasteX, pasteY, resizedEye.cols, resizedEye.rows);

			// Blend the resized eye image with the original image in the specified ROI
			addWeighted(myImg(pasteRect), 0.1, resizedEye, 0.9, 0, myImg(pasteRect));
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
*/
void CButyDlg::DetectEyesAndDrawRectangles()
{
	// Get the current position of the slider
	int eyeSizeFactor = m_eyeSizeSliderCtrl.GetPos();

	try
	{
		// Convert the Mat image to grayscale for eye detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained eye cascade classifier
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// Detect eyes in the image
		std::vector<Rect> detectedEyes;
		eyeCascade.detectMultiScale(grayImage, detectedEyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Create a copy of the original image to draw on
		Mat imgWithEyes = myImg.clone();

		// Loop over detected eyes
		for (size_t i = 0; i < detectedEyes.size(); ++i)
		{
			// Crop the region of interest (ROI) from the original image using eyeRect
			Mat eyeCropped = myImg(detectedEyes[i]);

			// Resize the cropped eye image based on the slider value
			Mat resizedEye;
			resize(eyeCropped, resizedEye, Size(detectedEyes[i].width * eyeSizeFactor, detectedEyes[i].height * eyeSizeFactor));

			// Calculate the position to paste the resized eye back into the original image
			int pasteX = detectedEyes[i].x - (resizedEye.cols - detectedEyes[i].width) / 2;
			int pasteY = detectedEyes[i].y - (resizedEye.rows - detectedEyes[i].height) / 2;

			// Ensure that the pasted region is within the bounds of the original image
			pasteX = std::max(pasteX, 0);
			pasteY = std::max(pasteY, 0);

			// Region of interest (ROI) in the original image to paste the resized eye
			Rect pasteRect(pasteX, pasteY, resizedEye.cols, resizedEye.rows);

			// Blend the resized eye image with the copy of the original image in the specified ROI
			addWeighted(imgWithEyes(pasteRect), 0.1, resizedEye, 0.9, 0, imgWithEyes(pasteRect));
		}
		// Redraw the image with eye detection
		DrawImage(imgWithEyes, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(CString("Error during eye detection: ") + ex.what());
	}
}

void CButyDlg::OnSliderEyeSizeChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Handle the eye size change event here
	DetectEyesAndDrawRectangles();
	*pResult = 0;
}

void CButyDlg::OnBnClickedDetect()
{
	DetectEyesAndDrawRectangles();

}

void CButyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
}


void CButyDlg::OnBnClickedMerge()
{
	try
	{
		// Copy the current image to myOriginalImg (assuming myOriginalImg is initialized)
		myOriginalImg = myImg.clone();

		// Convert the Mat image to grayscale for face detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained face cascade classifier
		CascadeClassifier faceCascade;
		if (!faceCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_default.xml"))
		{
			AfxMessageBox(_T("Error loading face cascade classifier."));
			return;
		}

		//Detect faces in the image
		std::vector<Rect> faces;
		faceCascade.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Load the image to be overlaid
		Mat overlayImage = imread("C:\\Users\\김경태\\Downloads\\headpin02.jpg");
		if (overlayImage.empty())
		{
			AfxMessageBox(_T("Error loading overlay image."));
			return;
		}

		// Loop over detected faces
		for (size_t i = 0; i < faces.size(); i++)
		{
			// Draw the green rectangle
			rectangle(myImg, faces[i], Scalar(0, 255, 0), 2);

			// Define the region of interest (ROI) where the overlay will be applied
			Rect roiRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);

			// Calculate the position to overlay the image on top of the face region
			int overlayX = faces[i].x + faces[i].width / 5 - overlayImage.cols / 1;
			int overlayY = faces[i].y + faces[i].height / 4 - overlayImage.rows / 1;

			// Ensure that the overlay position is within the bounds of the image
			overlayX = max(overlayX, 0);
			overlayY = max(overlayY, 0);

			// Calculate the size of the overlay image based on the face size
			int newWidth = 400;
			int newHeight = 50;

			// Resize the overlay image
			resize(overlayImage, overlayImage, Size(newWidth, newHeight));

			// Ensure that the overlay region is within the bounds of the image
			if (overlayX + overlayImage.cols <= myImg.cols && overlayY + overlayImage.rows <= myImg.rows)
			{
				//Overlay the image on top of the face region
				overlayImage.copyTo(myImg(Rect(overlayX, overlayY, overlayImage.cols, overlayImage.rows)));
			}
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
