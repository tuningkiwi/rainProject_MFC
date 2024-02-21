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
	
	CButton embossFT;// 진흙효과를 내는 엠보싱 필터 구현버튼
	afx_msg void OnBnClickedEmbossFt();
	Mat myImgAfterChange; 
	BOOL colorToGray();
	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	BITMAPINFO* myBmpInfoAfterChange;
	afx_msg void OnBnClickedRevertFt();
	afx_msg void OnBnClickedCancel();
	CStatic fogLB_FT;//안개필터 라벨 변수
	CSliderCtrl fogslider_FT;//안개필터 강도 슬라이더
//	afx_msg void OnNMCustomdrawSliderFogFt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// sharpening slider
	CSliderCtrl sharpSliderFT;
	// sharpening label
	CStatic sharpLB_FT;
	// 양방향필터 버튼 변수
	CButton bilateralBtn_FT;
	afx_msg void OnBnClickedBilateralFt();
	// 노이즈필터
	CSliderCtrl noiseFT;
	// 노이즈 라벨
	CStatic noiseLB_FT;
};

//백업용 코드 저장소 커밋 확인 