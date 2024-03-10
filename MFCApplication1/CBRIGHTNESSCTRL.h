﻿#pragma once
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

	//	void HandleBrightnessChange(NMHDR* pNMHDR, LRESULT* pResult, 
	//		int& edit_val, CSliderCtrl& slider, CEdit& edit, BITMAPINFO* BitChangeImg);

	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);
	BOOL colorToGray();
	BOOL GrayToColor();

	BITMAPINFO* BitChangeImg;
	BITMAPINFO* BitChangeImg2;

	BITMAPINFO* BitChangeResultImg;
	Mat resultImg;



	Mat myImg;// 이미지 정보를 담고 있는 객체.
	Mat backupImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.

	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();



	// virtual HRESULT get_accChild(VARIANT varChild, IDispatch** ppdispChild);

	afx_msg void OnBnClickedButton3();
	CSliderCtrl m_slider;
	CEdit m_edit;
	CSpinButtonCtrl m_spin;

	CSliderCtrl m_slider2;
	CEdit m_edit2;
	CSpinButtonCtrl m_spin2;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	//afx_msg void OnDeltaposSpin1_DH(NMHDR* pNMHDR, LRESULT* pResult);
	int m_edit_val;
	int m_edit_val2;
	afx_msg void mspin_updown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void mspin_updown2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton4();
	//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
