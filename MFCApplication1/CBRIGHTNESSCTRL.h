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
	BOOL colorToGray();
	BOOL GrayToColor();
	Mat matChangImg;
	Mat matChangImg2;
	BITMAPINFO* BitChangeImg;
	BITMAPINFO* BitChangeImg2;
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	Mat backupImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnDestroy();
	
// virtual HRESULT get_accChild(VARIANT varChild, IDispatch** ppdispChild);
	
	afx_msg void OnBnClickedButton3();
	CSliderCtrl m_slider;
	CEdit m_edit;
	CSpinButtonCtrl m_spin;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
};
