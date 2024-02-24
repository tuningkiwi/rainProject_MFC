#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;


//인태 브런치 테스트 
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
	Mat currentRotatedImg; // 회전된 이미지를 저장하기 위한 변수 
	Mat SizeImg;
	Mat ChangeImg; // Affine 이미지 정보를 담기위한 원본에 원본 이미지 
	Mat FlipImg;
	Mat resultImg; // 부모창에 띄울 최종 변환 이미지 
	BITMAPINFO* myBitmapInfo; // 원본 이미지 Bitmap 정보를 담고 있는 구조체.
	BITMAPINFO* resultmyBitmapInfo;  // 변경된 Bitmap 정보를 담고있는 구조체. 
	void ReadImage(Mat originImg, BITMAPINFO* originBmpInfo);
	CStatic picCtrl_IT;
	CButton rotationR;
	CButton rotationL;
	CButton Flip;
	CSliderCtrl SizeSlide;
	CRect picLTRB;//윈도우 상에서 현재 사진의 위치 
	void MakeBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedReverseIt();
	afx_msg void OnBnClickedButtonRr();
	afx_msg void OnBnClickedButtonLr();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void ResizeImage(int newSize);
	CBrush m_backBrush;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedButtonFlip();
};
