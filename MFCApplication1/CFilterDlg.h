#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include <string>
using namespace cv;

#define	MEANBLUR	1
#define	BLACKBLUR 	2

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
	virtual BOOL OnInitDialog();//다이얼로그 띄우기 직전 아이템 속성 업데이트 
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	afx_msg void OnBnClickedOk();//부모창에 변환된 이미지 전달
	CStatic picCtrl_FT;
	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);//이미지 그리기
	afx_msg void OnTimer(UINT_PTR nIDEvent);//타이머
	afx_msg void OnDestroy();//윈도우가 강제 종료될 시, 데이터를 저장함
	
	CButton embossFT;// 진흙효과를 내는 엠보싱 필터 구현버튼
	afx_msg void OnBnClickedEmbossFt();//진흙효과내는 엠보싱 필터 기능함수 
	Mat myImgAfterChange; 
	BOOL colorToGray();//컬러영상 >> 그레이 영상으로 변환 
	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);
	BITMAPINFO* myBmpInfoAfterChange;
	afx_msg void OnBnClickedRevertFt();//원본 사진으로 되돌리기 기능
	afx_msg void OnBnClickedCancel();// 변환된 사진 저장하지 않고 창 종료 
	CStatic fogLB_FT;//안개필터 라벨 변수
	CSliderCtrl fogslider_FT;//안개필터 강도 슬라이더
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl sharpSliderFT;// sharpening slider
	CStatic sharpLB_FT;// sharpening label
	CButton bilateralBtn_FT;// 양방향필터 버튼 변수
	afx_msg void OnBnClickedBilateralFt();//양방향필터 메소드 
	CSliderCtrl noiseFT;// 노이즈필터 슬라이더
	CStatic noiseLB_FT;// 노이즈 라벨
	CPoint prev_mousePos;//이전 마우스 위치 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	CButton partBlutBtn;//마우스로 클릭시 해당 주변 블러처리
	afx_msg void OnBnClickedPartblurFt();
	BOOL partBlurModeOn;//부분블러 모드 ON 
	CPoint blurLoc;//블러적용할 left, top
	CRect picLTRB;//윈도우 상에서 현재 사진의 위치 
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic pointLocFT;// 부분블러모드시 마우스 클릭한 곳 위치 출력
	void partBlurProc(CPoint point);//부분 블러 처리 메소드 
	// 부분블러 적용 범위 크기 조절
// 부분블러 적용 범위 크기 조절
	CSliderCtrl partBlurSlider;
	int blurRangeHalfWid;
};

//백업용 코드 저장소 커밋 확인 