// CVideoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CVideoDlg.h"


// CVideoDlg 대화 상자

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEO, pParent)
{

}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CVideoDlg::OnInitDialog()
{
    if (!CDialogEx::OnInitDialog())
        return FALSE;

    // 여기에 OnInitDialog 메소드의 초기화 코드를 추가하세요.

    return TRUE;  // TRUE를 반환하면 포커스를 설정하지 않고 대화 상자를 엽니다.
}

void CVideoDlg::OnDestroy()
{
	// 여기에 OnDestroy 메소드의 구현을 추가하세요.
}