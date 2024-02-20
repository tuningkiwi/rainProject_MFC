#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;



// CFilterDlg 대화 상자

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	//CFilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CFilterDlg();   // 표준 생성자입니다.
	CFilterDlg(Mat Img, BITMAPINFO* bitmapInfo);
	virtual ~CFilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
//	virtual INT_PTR DoModal();
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedOk();
	CStatic picCtrl_FT;
	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);
//	afx_msg void OnBnClickedImgloadFt();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	// 진흙효과를 내는 엠보싱 필터 구현버튼 입니다
	CButton embossFT;
	afx_msg void OnBnClickedEmbossFt();
	Mat myImgAfterChange; 
	BOOL colorToGray();
	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	BITMAPINFO* myBmpInfoAfterChange;
};
