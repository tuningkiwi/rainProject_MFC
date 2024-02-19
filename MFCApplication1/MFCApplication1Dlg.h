
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CImage m_image;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	Mat m_matImage; // 이미지 정보를 담고 있는 객체.
	BITMAPINFO* m_pBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	void CreateBitmapInfo(int w, int h, int bpp);
	void DrawImage();
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
