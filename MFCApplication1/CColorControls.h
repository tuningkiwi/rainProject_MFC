#pragma once
#include "afxdialogex.h"
#include "afxcolordialog.h"
#include "windows.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// CColorControls 대화 상자

class CColorControls : public CDialogEx
{
	DECLARE_DYNAMIC(CColorControls)

public:
	CColorControls();   // 표준 생성자입니다.
	CColorControls(Mat Img, BITMAPINFO* bitmapInfo);
	virtual ~CColorControls();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_CONYROLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV 지원입니다.		
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(); //다이얼로그가 생성될 때 호출되는 초기화

	Mat myImg; // 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.	
	CStatic m_imageControl;

	Mat myImgAfterChange;
	BITMAPINFO* myBmpInfoAfterChange;

	void DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo);
	void CreateBitmapInfo(BITMAPINFO** newInfo, int w, int h, int bpp);

	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
		
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBack();

	//afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	//컬러버튼
	CMFCColorButton m_mfcColorBtn;//컬러 버튼
	CStatic m_staticColor;//색상 표시블록
	CBrush m_brushColor;//색상 채우는 브러쉬
	CEdit m_color_edit;//RGB값 출력칸
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//선택한 색으로 변경
	afx_msg void OnBnClickedMfccolorbuttonColor();//RGB값 표시

	//HSV 슬라이더
	CSliderCtrl m_slider_h;//색조 슬라이더
	CSliderCtrl m_slider_s;//채도 슬라이더
	CSliderCtrl m_slider_v;//밝기 슬라이더
	CEdit m_slider_edit_h;//색조값
	CEdit m_slider_edit_s;//채도값
	CEdit m_slider_edit_v;//밝기값
	void CColorControls::UpdateSliderValue(int sliderID, int editID);//슬라이더 조정값 표시
	//afx_msg void OnNMCustomdrawSliderH(NMHDR* pNMHDR, LRESULT* pResult);
	
	//컬러버튼 값으로 변환
	CPoint m_ptDragStart; // 드래그 시작점 저장
	CPoint m_ptDragEnd; // 드래그 끝점 저장
	bool m_bDragging = false; // 드래그 중 여부를 나타내는 플래그
	Vec3b m_selectedColor;//선택한 컬러
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	void CColorControls::ApplyColorToDraggedRegion(CPoint startPoint, CPoint endPoint);
};
