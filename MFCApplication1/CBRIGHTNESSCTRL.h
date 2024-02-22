#pragma once
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"
#include "opencv2/opencv.hpp"
using namespace cv;
// CBRIGHTNESSCTRL 대화 상자

class CBRIGHTNESSCTRL : public CDialogEx
{
	DECLARE_DYNAMIC(CBRIGHTNESSCTRL)

public:
	CBRIGHTNESSCTRL();   // 기본 생성자
	CBRIGHTNESSCTRL(Mat Img, BITMAPINFO* bitmapInfo);
	virtual ~CBRIGHTNESSCTRL();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_BRIGHTNESSCTRL_BTN_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);
	
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	virtual HRESULT get_accChild(VARIANT varChild, IDispatch** ppdispChild);
};
