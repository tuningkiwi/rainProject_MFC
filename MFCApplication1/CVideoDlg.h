#pragma once
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
using namespace cv;

// CVideoDlg 대화 상자

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CVideoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void OpenVideo(const CString& filePath);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	VideoCapture* m_videoCapture;
	bool m_bVideoPlaying;
};
