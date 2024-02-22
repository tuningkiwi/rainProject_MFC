#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;


// CButyDlg 대화 상자

class CButyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CButyDlg)

public:
	CButyDlg();   // 표준 생성자입니다.
	CButyDlg(Mat Img, BITMAPINFO* bitmapInfo);
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

	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	CStatic picCtrl_FT;
	Mat myImgAfterChange;
	BITMAPINFO* myBmpInfoAfterChange;
	CImage m_displayImage;
	int eyeRadius; // 추가된 부분

	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRevertFt();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDetect();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

//백업용 코드 저장소 커밋 확인 