#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;

// CButyDlg 대화 상자

class CButyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CButyDlg)

public:
	CButyDlg();
	CButyDlg(Mat Img, BITMAPINFO* bitmapInfo);   // 표준 생성자입니다.
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
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	CButton m_butyButton;
};
