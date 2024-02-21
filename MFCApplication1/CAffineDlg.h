#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;



// CAffineDlg 대화 상자

class CAffineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAffineDlg)

public:
	//CAffineDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CAffineDlg(); // 표준 생서자입니다.
	CAffineDlg(Mat Img, BITMAPINFO* bitmapInfo);
	virtual ~CAffineDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Affine_it };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	void ReadImage(Mat originImg, BITMAPINFO* originBmpInfo);
	CStatic picCtrl_IT;
	CButton rotationR;
	CButton rotationL;
	void MakeBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
