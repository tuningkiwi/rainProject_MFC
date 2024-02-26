#pragma once

#include "afxdialogex.h"
#include "opencv2/opencv.hpp"  // openCV 라이브러리 추가

using namespace cv;
using namespace std;

// CButyDlg 대화 상자

class CButyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CButyDlg)

public:
	CButyDlg();  // 표준 생성자입니다.
	CButyDlg(Mat Img, BITMAPINFO* bitmapInfo);  // 매개변수를 받아들이는 다른 생성자를 선언
	virtual ~CButyDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);
	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);

	Mat myImg;                                  // 이미지 정보를 담고 있는 객체.
	Mat myOriginalImg;                          // 원본 이미지를 저장하는 변수 추가
	BITMAPINFO* myBitmapInfo;                   // Bitmap 정보를 담고 있는 구조체.
	CStatic picCtrl_FT;                         // pictual control 을 제어하기 위한 변수 선언
	CSliderCtrl m_eyeSizeSliderCtrl;            // 슬라이더로 눈 크기 제어를 하기 위한 슬라이더 변수 선언
	vector<Mat> bmpHistory;                     // 부모 창과 자식 창의 이미지 정보 전달을 위한 변수 선언
	vector<BITMAPINFO*> bmpInfoHistory;         // 부모 창과 자식 창의 이미지 정보 전달을 위한 변수 선언
	
	// 사용안하는 것 같은 변수
	CImage m_displayImage;
	Mat myImgAfterChange;
	BITMAPINFO* myBmpInfoAfterChange;
	vector<Rect> m_detectedEyes;

	afx_msg void OnBnClickedOk();               // 저장 버튼
	afx_msg void OnTimer(UINT_PTR nIDEvent);    // 타이머 함수
	afx_msg void OnDestroy();                   
	afx_msg void OnBnClickedRevertFt();         // 되돌리기 버튼
	afx_msg void OnBnClickedCancel();           // 취소 버튼 
	afx_msg void OnBnClickedDetect();           // 눈 인식 버튼
	afx_msg void OnBnClickedMerge();            // 얼굴 합성 버튼
	afx_msg void OnBnClickedImageCall();        // 부모 창에서 이미지 불러오기 버튼
	afx_msg void OnBnClickedVideoCall();        // 부모 창에서 동영상 불러오기 버튼
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void DetectEyesAndDrawRectangles(); // 눈 인식 하는 함수
	afx_msg void OnSliderEyeSizeChanged(NMHDR* pNMHDR, LRESULT* pResult);  // 슬라이더 호출 함수

	// 동영상 불러오기 함수 및 변수

};
