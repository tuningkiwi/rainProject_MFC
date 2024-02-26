﻿// CButyDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CButyDlg.h"
#include <chrono>
#include <ctime>

// openCV 관련 헤더 파일
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// Windows GDI+ 라이브러리 헤더파일
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
	myImg = Img;                                                  // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;                                    // 이미지 Bitmap 정보 가져오기
	bmpHistory.push_back(Img);                                    // 이미지 매트릭스 정보 추가
	bmpInfoHistory.push_back(bitmapInfo);                         // 이미지 Bitmap 정보 추가
}

CButyDlg::~CButyDlg()
{
}

void CButyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PC_FT, picCtrl_FT);                      // pictual control 제어
	DDX_Control(pDX, IDC_SLIDER_EYE, m_eyeSizeSliderCtrl);        // slider control 제어
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

	// 대화 상자 및 버튼 크기 및 위치 설정
	MoveWindow(350, 140, 1280, 720);

	//현재 window 크기 출력
	CRect m_rectCurHist;
	this->GetWindowRect(m_rectCurHist);// right:창의 너비 bottm: 창의 높이

	int btnlocLeft = int(m_rectCurHist.right * 5 / 6);
	int btnWidth = int(m_rectCurHist.right * 1 / 6);

	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);
	GetDlgItem(IDC_REVERT_FT)->MoveWindow(1000, 720 - 220, 200, 45);
	GetDlgItem(IDC_DETECT)->MoveWindow(1000, 720 - 280, 200, 45);
	GetDlgItem(IDC_MERGE)->MoveWindow(1000, 720 - 400, 200, 45);
	GetDlgItem(IDC_SLIDER_EYE)->MoveWindow(1200, 720 - 340, 200, 45);
	GetDlgItem(IDC_IMAGECALL)->MoveWindow(1000, 720 - 460, 200, 45);
	GetDlgItem(IDC_VIDEOCALL)->MoveWindow(1000, 720 - 520, 200, 45);
	GetDlgItem(IDC_PC_FT)->MoveWindow(0, 0, btnlocLeft, m_rectCurHist.bottom);

	// Slider control 초기화
	m_eyeSizeSliderCtrl.SetRange(1, 10);                      // 슬라이더 범위 설정
	m_eyeSizeSliderCtrl.SetPos(1);                             // 초기 슬라이더 값 설정(눈 크기 값)

	// 슬라이더를 위한 이벤트 핸들러 등록
	m_eyeSizeSliderCtrl.SetBuddy(GetDlgItem(IDC_SLIDER_EYE));  // 슬라이더와 연결된 Buddy 컨트롤 설정

	return TRUE;  
}

// 저장 버튼
void CButyDlg::OnBnClickedOk()
{
	KillTimer(2);

	CDialogEx::OnOK();
}

// 이미지 그리기 함수
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
	if (nIDEvent == 0) { 
		DrawImage(bmpHistory.back(), bmpInfoHistory.back());
		KillTimer(0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 부모창 이미지 불러오기
void CButyDlg::OnBnClickedImageCall()
{
	DrawImage(myImg, myBitmapInfo);
}

// 부모창 동영상 불러오기
void CButyDlg::OnBnClickedVideoCall()
{
	DrawImage(myImg, myBitmapInfo);
}

//윈도우 창이 DESTORY 갑자기 종료될때, 저장해야 하는 정보들 기능 수행
void CButyDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
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

	(*btmInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);      //구조체에 필요한 바이트 수
	(*btmInfo)->bmiHeader.biPlanes = 1;                           // 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	(*btmInfo)->bmiHeader.biBitCount = bpp;                       //픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	(*btmInfo)->bmiHeader.biCompression = BI_RGB;                 //압축되지 않은 RGB.
	(*btmInfo)->bmiHeader.biSizeImage = 0;                        //이미지의 크기(바이트)를 지정합니다.
	(*btmInfo)->bmiHeader.biXPelsPerMeter = 0;                    //비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	(*btmInfo)->bmiHeader.biYPelsPerMeter = 0;                    //비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	(*btmInfo)->bmiHeader.biClrUsed = 0;                          //비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	(*btmInfo)->bmiHeader.biClrImportant = 0;                     //비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

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
	(*btmInfo)->bmiHeader.biHeight = -h;                           //음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}

//되돌리기 기능 (원본 이미지로 돌아감) 
void CButyDlg::OnBnClickedRevertFt()
{
	myImg = myOriginalImg.clone();                                 // 원본 이미지 가져오기
	DrawImage(myImg, myBitmapInfo);                                // 원본 이미지 그리기
	MessageBox(L"원본으로 돌아갑니다", L"알림", MB_OK);
}

// 취소 버튼, 이미지 변경 X
void CButyDlg::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}

// 얼굴에서 눈을 검출하기 위한 함수, 슬라이더와 연결됨
void CButyDlg::DetectEyesAndDrawRectangles()
{
	// 현재 슬라이더 설정 값 가져옴
	int eyeSizeFactor = m_eyeSizeSliderCtrl.GetPos();

	try
	{
		// Mat 이미지를 눈 검출을 위하여 grayscale로 바꿈
		Mat grayImage;
		cvtColor(myImg, grayImage, COLOR_BGR2GRAY);

		// 눈 검출 모델 불러옴
		CascadeClassifier eyeCascade;
		eyeCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");

		// 눈 검출 수행
		std::vector<Rect> detectedEyes;
		eyeCascade.detectMultiScale(grayImage, detectedEyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// 원본 이미지 복사
		Mat imgWithEyes = myImg.clone();

		// 눈 검출 Loop
		for (size_t i = 0; i < detectedEyes.size(); ++i)
		{
			// 눈 검출을 위한 구역 설정
			Mat eyeCropped = myImg(detectedEyes[i]);

			// 슬라이더 값에 따른 눈 크기 조절
			Mat resizedEye;
			resize(eyeCropped, resizedEye, Size(detectedEyes[i].width * eyeSizeFactor, detectedEyes[i].height * eyeSizeFactor));

			// 원본 이미지에 크기가 변경된 이미지 삽입을 위한 기존 눈 위치 계산
			int pasteX = detectedEyes[i].x - (resizedEye.cols - detectedEyes[i].width) / 2;
			int pasteY = detectedEyes[i].y - (resizedEye.rows - detectedEyes[i].height) / 2;

			// 원본 이미지에 크기가 변경된 이미지 삽입을 위한 기존 눈 위치 계산
			pasteX = std::max(pasteX, 0);
			pasteY = std::max(pasteY, 0);

			// 크기가 조정된 눈을 붙일 원본 영상의 관심 영역(ROI)
			Rect pasteRect(pasteX, pasteY, resizedEye.cols, resizedEye.rows);

			// 크기 조정된 눈 영상과 지정된 ROI의 원본 영상 사본을 혼합
			addWeighted(imgWithEyes(pasteRect), 0.1, resizedEye, 0.9, 0, imgWithEyes(pasteRect));
		}
		// 합성된 이미지 다시 그리기
		DrawImage(imgWithEyes, myBitmapInfo);
	}
	catch (const Exception& ex)
	{
		AfxMessageBox(CString("Error during eye detection: ") + ex.what());
	}
}

// 슬라이더에 눈 검출 함수 연결
void CButyDlg::OnSliderEyeSizeChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Handle the eye size change event here
	DetectEyesAndDrawRectangles();
	*pResult = 0;
}

// 눈 검출 수행 버튼
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
		// 파일 선택 창을 통해 이미지 선택
		CFileDialog dlg(TRUE, _T("*.jpg;*.png;*.bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
			_T("Image Files (*.jpg; *.png; *.bmp)|*.jpg;*.png;*.bmp|All Files (*.*)|*.*||"));

		if (dlg.DoModal() == IDOK)
		{
			CString selectedImagePath = dlg.GetPathName();

			// CString을 std::string으로 변환
			CT2CA pszConvertedAnsiString(selectedImagePath);
			std::string imagePath(pszConvertedAnsiString);

			// 이미지 로드
			Mat overlayImage = imread(imagePath);
			if (overlayImage.empty())
			{
				AfxMessageBox(_T("Error loading overlay image."));
				return;
			}

			// 원본 이미지 저장
			myOriginalImg = myImg.clone();
			myImgAfterChange = myImg.clone();

			// Mat 이미지를 눈 검출을 위하여 grayscale로 바꿈
			Mat grayImage;
			cvtColor(myImgAfterChange, grayImage, COLOR_BGR2GRAY);

			// 얼굴 검출 모델 불러옴
			CascadeClassifier faceCascade;
			if (!faceCascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_default.xml"))
			{
				AfxMessageBox(_T("Error loading face cascade classifier."));
				return;
			}

			// 얼굴 검출 수행
			std::vector<Rect> faces;
			faceCascade.detectMultiScale(grayImage, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

			// 검출된 얼굴에 대한 루프
			for (size_t i = 0; i < faces.size(); i++)
			{
				// 녹색 사각형 그리기
				rectangle(myImgAfterChange, faces[i], Scalar(0, 255, 0), 2);

				// 오버레이가 적용될 영역(ROI) 정의
				Rect roiRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);

				// 얼굴 영역 위에 이미지를 오버레이할 위치 계산
				int overlayX = faces[i].x + faces[i].width / 2 - 100;
				int overlayY = faces[i].y - 50;

				// 오버레이 위치가 이미지의 경계 내에 있는지 확인
				overlayX = max(overlayX, 0);
				overlayY = max(overlayY, 0);

				// 오버레이 이미지의 크기 계산
				int newWidth = 200;
				int newHeight = 50;

				// 오버레이 이미지 크기 조절
				resize(overlayImage, overlayImage, Size(newWidth, newHeight));

				// 오버레이 영역이 이미지의 경계 내에 있는지 확인
				if (overlayX + overlayImage.cols <= myImg.cols && overlayY + overlayImage.rows <= myImg.rows)
				{
					// 얼굴 영역 위에 이미지를 오버레이
					overlayImage.copyTo(myImgAfterChange(Rect(overlayX, overlayY, overlayImage.cols, overlayImage.rows)));
				}
			}

			// 얼굴 검출과 오버레이가 적용된 이미지를 다시 그리기
			DrawImage(myImgAfterChange, myBitmapInfo);
		}
	}
	catch (const Exception& ex)
	{
		AfxMessageBox(CString("Error during face detection: ") + ex.what());
	}
}