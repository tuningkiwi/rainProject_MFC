﻿﻿// CButyDlg.cpp: 구현 파일
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
	GetDlgItem(IDC_IMAGECALL)->MoveWindow(1000, 720 - 460, 200, 45);
	GetDlgItem(IDC_VIDEOCALL)->MoveWindow(1000, 720 - 520, 200, 45);

	// Slider control 초기화
	CSliderCtrl* pSlider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_EYE));
	if (pSlider)
	{
		pSlider->SetRange(1, 100); // 적절한 범위 설정
		pSlider->SetPos(50);       // 초기값 설정

		// Slider control의 위치 설정 (절대적인 수치 사용)
		pSlider->MoveWindow(1000, 720 - 280 - 55, 200, 45);  // 적절한 수치로 설정
	}

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
/*
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
*/

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
	
}


void CButyDlg::OnBnClickedImageCall()
{
	DrawImage(myImg, myBitmapInfo);
}

void CButyDlg::OnBnClickedVideoCall()
{
	OnTimer(1);
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

/* 백업
void CButyDlg::OnBnClickedDetect()
{
	try
	{
		// Copy the current image to myOriginalImg (assuming myOriginalImg is initialized)
		myOriginalImg = myImg.clone();

		// Convert the Mat image to grayscale for eye detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained eye cascade classifier
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// Detect eyes in the image
		std::vector<Rect> eyes;
		eyeCascade.detectMultiScale(grayImage, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Loop over detected eyes and draw rectangles
		for (size_t i = 0; i < eyes.size(); i++)
		{
			rectangle(myImg, eyes[i], Scalar(0, 255, 0), 2);
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
	try
	{
		// Copy the current image to myOriginalImg (assuming myOriginalImg is initialized)
		myOriginalImg = myImg.clone();

		// Convert the Mat image to grayscale for eye detection
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// Load pre-trained eye cascade classifier
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// Detect eyes in the image
		m_detectedEyes.clear();  // Clear existing data before detecting again
		eyeCascade.detectMultiScale(grayImage, m_detectedEyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Create a mask to keep only the regions inside the green rectangles
		Mat mask = Mat::zeros(myImg.size(), CV_8UC1);

		// Loop over detected eyes and draw rectangles on the mask
		for (size_t i = 0; i < m_detectedEyes.size(); i++)
		{
			// Get the current eye rectangle
			Rect currentEyeRect = m_detectedEyes[i];

			// Crop the region of interest (ROI) from the original image using currentEyeRect
			Mat eyeROI = myOriginalImg(currentEyeRect);

			// Double the size of the ROI
			Mat enlargedEyeROI;
			resize(eyeROI, enlargedEyeROI, Size(currentEyeRect.width, currentEyeRect.height) * 2);

			// Check if the enlarged eye ROI fits within the original image
			if (currentEyeRect.x - currentEyeRect.width / 2 >= 0 &&
				currentEyeRect.y - currentEyeRect.height / 2 >= 0 &&
				currentEyeRect.x + currentEyeRect.width / 2 + currentEyeRect.width < myOriginalImg.cols &&
				currentEyeRect.y + currentEyeRect.height / 2 + currentEyeRect.height < myOriginalImg.rows)
			{
				// Replace the region in the original image with the enlarged ROI
				try
				{
					enlargedEyeROI.copyTo(myOriginalImg(currentEyeRect));
				}
				catch (const Exception& ex)
				{
					// Handle the exception (e.g., display an error message)
					AfxMessageBox(CString("Error copying enlarged eye ROI to original image: ") + ex.what());
				}
			}
			else
			{
				// Handle the case where the enlarged ROI does not fit within the original image
				//AfxMessageBox(CString("Enlarged eye ROI exceeds the boundaries of the original image.") + ex.what());
			}
		}

		// Use the mask to keep only the regions inside the green rectangles
		Mat result;
		myOriginalImg.copyTo(result, mask);

		// Redraw the image with eye detection
		DrawImage(result, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(CString("Error during eye detection: ") + ex.what());
	}
}

void CButyDlg::AdjustEyeSizeUsingSlider()
{
	try
	{
		// Get the current slider position for adjusting eye size
		int eyeSizeSliderValue = m_eyeSizeSliderCtrl.GetPos();
		TRACE("eyeSizeSliderValue: %d\n", eyeSizeSliderValue);

		// Create a new Mat to store the adjusted image
		Mat adjustedImage = myImg.clone();

		// Loop over detected eye regions and adjust sizes
		for (size_t i = 0; i < m_detectedEyes.size(); i++)
		{
			// Define the region of interest (ROI) for each detected eye
			Rect eyeROI = m_detectedEyes[i];

			// Extract the eye region from the original image
			Mat eyeImage = myOriginalImg(eyeROI).clone();

			// Dynamic resizing based on the slider value
			double resizeFactor = 1.0 + 0.01 * eyeSizeSliderValue; // Example: 1.0 to 2.0 based on the slider

			// Resize the extracted eye region dynamically
			resize(eyeImage, eyeImage, Size(), resizeFactor, resizeFactor);

			// Optionally, save the adjusted eye region to a new bitmap (Mat)
			Mat adjustedEyeBitmap = eyeImage.clone();

			// Perform additional operations on the adjustedEyeBitmap if needed

			// Draw the adjusted eye region back onto the new Mat
			adjustedEyeBitmap.copyTo(adjustedImage(eyeROI));
		}

		// Redraw the image with adjusted eye sizes
		DrawImage(adjustedImage, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		// Handle the exception (e.g., display an error message)
		CString errorMessage;
		errorMessage.Format(_T("Error during eye size adjustment: %s"), ex.what());
		AfxMessageBox(errorMessage);

		// Print exception information to the Output window
		TRACE("Exception during eye size adjustment: %s\n", ex.what());
	}

}

void CButyDlg::OnBnClickedDetect()
{
	DetectEyesAndDrawRectangles();

}

void CButyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_EYE)
	{
		AdjustEyeSizeUsingSlider();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
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
