#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;

// CColorControls 대화 상자

class CColorControls : public CDialogEx
{
	DECLARE_DYNAMIC(CColorControls)

public:
	//CColorControls(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CColorControls();   // 표준 생성자입니다.
	CColorControls(Mat Img, BITMAPINFO* bitmapInfo);
	virtual ~CColorControls();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_CONYROLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV 지원입니다.
	//CWnd m_imageControl; // 이미지를 표시할 컨트롤 변수 선언
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();//다이얼로그가 생성될 때 호출되는 초기화
	Mat myImg;// 이미지 정보를 담고 있는 객체.
	BITMAPINFO* myBitmapInfo; // Bitmap 정보를 담고 있는 구조체.
	
};
