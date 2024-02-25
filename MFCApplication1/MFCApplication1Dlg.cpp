
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "CFilterDlg.h"
#include "CButyDlg.h"
#include "CColorControls.h"
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"
#include "CAffineDlg.h"
#include <iostream>
#include <ctime> 
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CButyDlg.h"


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, FilterBtn);
	DDX_Control(pDX, IDC_COLOR_BTN, colorBtn);
	DDX_Control(pDX, IDC_AFFINE_BTN, cropAffinBtn);
	DDX_Control(pDX, IDC_BUTY_BTN, ButyBtn);
	DDX_Control(pDX, IDC_BRIGHTNESSCTRL_BTN, brightnessBtn);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	//	ON_WM_GETMINMAXINFO()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_FILTER_BTN, &CMFCApplication1Dlg::OnBnClickedFilterBtn)
	ON_BN_CLICKED(IDC_COLOR_BTN, &CMFCApplication1Dlg::OnBnClickedColorBtn)
	ON_BN_CLICKED(IDC_AFFINE_BTN, &CMFCApplication1Dlg::OnBnClickedAffineBtn)
	ON_BN_CLICKED(IDC_BUTY_BTN, &CMFCApplication1Dlg::OnBnClickedButyBtn)
	ON_BN_CLICKED(IDC_BRIGHTNESSCTRL_BTN, &CMFCApplication1Dlg::OnBnClickedBrightnessctrlBtn)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//현재 window 크기 출력
	CRect m_rectCurHist;
	this->GetWindowRect(m_rectCurHist);// right:창의 너비 bottm: 창의 높이 
	int btnlocLeft = int(m_rectCurHist.right * 5 / 6);
	int btnWidth = int(m_rectCurHist.right * 1 / 6);
	GetDlgItem(IDC_PC_VIEW)->MoveWindow(0, 0, btnlocLeft, m_rectCurHist.bottom);

	GetDlgItem(IDC_BUTTON2)->MoveWindow(btnlocLeft, 50, btnWidth, 60);
	GetDlgItem(IDC_FILTER_BTN)->MoveWindow(btnlocLeft, 130, btnWidth, 60);
	GetDlgItem(IDC_COLOR_BTN)->MoveWindow(btnlocLeft, 210, btnWidth, 60);
	GetDlgItem(IDC_AFFINE_BTN)->MoveWindow(btnlocLeft, 290, btnWidth, 60);
	GetDlgItem(IDC_BUTY_BTN)->MoveWindow(btnlocLeft, 370, btnWidth, 60);
	GetDlgItem(IDC_BRIGHTNESSCTRL_BTN)->MoveWindow(btnlocLeft, 450, btnWidth, 60);
	GetDlgItem(IDOK)->MoveWindow(btnlocLeft, m_rectCurHist.bottom -280, btnWidth, 60);
	GetDlgItem(IDCANCEL)->MoveWindow(btnlocLeft, m_rectCurHist.bottom -200, btnWidth, 60);

	CDialogEx::SetBackgroundColor(0x004D3428, 1);//3B261B 2A1B0D
	CFont font;
	font.CreatePointFont(180, _T("함초롬돋움 확장 보통"));//함초롬돋움 확장 보통
	GetDlgItem(IDC_BUTTON2)->SetFont(&font);
	GetDlgItem(IDC_FILTER_BTN)->SetFont(&font);
	GetDlgItem(IDC_COLOR_BTN)->SetFont(&font);
	GetDlgItem(IDC_AFFINE_BTN)->SetFont(&font);
	GetDlgItem(IDC_BUTY_BTN)->SetFont(&font);
	GetDlgItem(IDC_BRIGHTNESSCTRL_BTN)->SetFont(&font);
	GetDlgItem(IDOK)->SetFont(&font);
	GetDlgItem(IDCANCEL)->SetFont(&font);
	font.Detach();//font 종료 꼭 해주기 메모리 할당 해제 


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect wnd;
		this->GetWindowRect(wnd);// right:창의 너비 bottm: 창의 높이 
		int xloc = int(wnd.right * 5 / 6);
		int backImgWid = int(wnd.right * 1 / 6);
		dc.FillSolidRect(xloc, 0, backImgWid, wnd.bottom, RGB(40, 56, 84));//35, 47, 69
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
//void CMFCApplication1Dlg::OnBnClickedButton1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int nWidth = 1080; // 가로폭
//	int nHeight = 720; // 세로폭	
//	int nBpp = 8; // 픽셀당 비트수, 8: GRAY 24: COLOR 
//
//	// 이미지 생성
//	m_image.Create(nWidth, nHeight, nBpp);
//
//	// 이미지 그리기
//	CClientDC dc(GetDlgItem(IDC_PC_VIEW)); // 클라이언트 형태의 변수를 dc로 만들고 초기화를 this로 하겠다. -> this는 현재 가지고 있는 다이얼로그를 의미함
//	m_image.Draw(dc, 0, 0); // 그리기 (어디에 그릴지, 위치를 x, y 축을 설정해 준다.)
//
//}


//BITMAP 정보 구조체 데이터 생성 
void CMFCApplication1Dlg::CreateBitmapInfo(int w, int h, int bpp) {
	if (m_pBitmapInfo != NULL) //기존 비트맵 정보 초기화 
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL; //기존 BITMAP 정보 삭제 
	}

	if (bpp == 8) //1채널 
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	m_pBitmapInfo->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	                                                                                                                          
	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}

//다이얼로그창에 사진 띄우기 
void CMFCApplication1Dlg::DrawImage() {
	CRect m_rectCurWnd;
	this->GetWindowRect(m_rectCurWnd);
	int wx = int(m_rectCurWnd.right*5/6);
	int wy = m_rectCurWnd.bottom;
	
	//불러올 사진 cols 가져오기.
	
	CClientDC dc(GetDlgItem(IDC_PC_VIEW));//IDC_PC_VIEW
	CRect rect;// 이미지를 넣을 사각형 
	if (m_matImage.cols > wx) {
		//cols: 1080 = rows : wid;
		int resize_h = cvRound((wx * m_matImage.rows) / m_matImage.cols);
		int y = cvRound((wy - resize_h) / 2);
		//if (y < 0) { //구현예정. 
		//	//float ratio =  m_matImage.rows / m_matImage.cols;
		//	y = 0;
		//	
		//}
		GetDlgItem(IDC_PC_VIEW)->MoveWindow(0, y, m_matImage.cols, resize_h);
	}
	else {
		int x = cvRound((wx - m_matImage.cols) / 2);
		int y = cvRound((wy - m_matImage.rows) / 2);
		GetDlgItem(IDC_PC_VIEW)->MoveWindow(x, y, m_matImage.cols, m_matImage.rows);
	}

	
	//GetClientRect() 
	//창 클라이언트 영역의 좌표를 검색합니다. 
	// 클라이언트 좌표는 클라이언트 영역의 왼쪽 위와 
	// 오른쪽 아래 모서리를 지정합니다. 
	// 클라이언트 좌표는 창 클라이언트 영역의 왼쪽 위 모서리를 
	// 기준으로 하기 때문에 왼쪽 위 모서리의 좌표는 (0,0)입니다.
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//GetDlgItem(IDC_PC_VIEW)->GetClientRect(&rect);
	
	GetDlgItem(IDC_PC_VIEW)->GetClientRect(&rect);

	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	// 제거된 모든 픽셀 줄을 삭제합니다.
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	
	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	// 대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	// 함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_matImage.cols, m_matImage.rows, m_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//파일 열기 다이얼로그를 생성합니다. 
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)//성공적으로 읽어왔을 때
	{
		CString path = fileDlg.GetPathName();//파일 경로 

		CT2CA pszString(path);
		std::string strPath(pszString);

		m_matImage = imread(strPath, IMREAD_UNCHANGED);//png파일의 경우 alpha채널도 가져옴

		CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);
		DrawImage();
	}
}

void CMFCApplication1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize = CPoint(1920, 1000);
	lpMMI->ptMaxTrackSize = CPoint(1920, 1000);
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


//동희: 필터링 
void CMFCApplication1Dlg::OnBnClickedFilterBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialog filterDlg(IDD_DIALOG1);
	CFilterDlg filterDlg(m_matImage, m_pBitmapInfo);

	// Create and show the dialog box
	INT_PTR nRet = -1;
	nRet = filterDlg.DoModal();

	// Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		// Do something
		m_matImage = filterDlg.myImgAfterChange;
		m_pBitmapInfo = filterDlg.myBmpInfoAfterChange;
		DrawImage(); 
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};
}

//유진: 색상 추출 및 관련 기능 
void CMFCApplication1Dlg::OnBnClickedColorBtn()
{
	CColorControls dlg(m_matImage, m_pBitmapInfo);
	dlg.DoModal();
}

//인태: 자르기 및 어파인 변형 
void CMFCApplication1Dlg::OnBnClickedAffineBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 인태 주석 테스트
	CAffineDlg affinedlg(m_matImage, m_pBitmapInfo);
	
	// Create and show the dialog box
	INT_PTR nRet = -1;
	nRet = affinedlg.DoModal();

	// Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		// Do something
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};
}

//경태: 객체 검출 및 변형 
void CMFCApplication1Dlg::OnBnClickedButyBtn()
{
	//CDialog filterDlg(IDD_DIALOG1);
	CButyDlg butyDlg(m_matImage, m_pBitmapInfo);

	// Create and show the dialog box
	INT_PTR nRet = -1;
	nRet = butyDlg.DoModal();

	// Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		// Do something
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};
}

//동혁: 밝기 및 명암비 조절 
void CMFCApplication1Dlg::OnBnClickedBrightnessctrlBtn()
{
	CBRIGHTNESSCTRL brightdlg(m_matImage, m_pBitmapInfo);
	brightdlg.DoModal();
	
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//경태 깃 분기 만들기 

void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::time_t result = std::time(nullptr);
	std::string name = std::ctime(&result);//마지막에 \n이 삽입됨
	std::string newName = "";
	for (int i = 0; i < name.size()-1; i++) {
		std::cout << i << ":" << name[i] << std::endl;
		if (name[i] != 0x20 && name[i] != ':') {//공백문자 
			newName += name[i];
		}
	}
	std::string filepath = "./rainResult/" + newName + ".bmp";
	imwrite(filepath, m_matImage);
	CDialogEx::OnOK();
}
//주석 추가 

void CMFCApplication1Dlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDCtl) {
		case IDC_BUTTON2: case IDC_FILTER_BTN:	case IDC_COLOR_BTN:
		case IDC_AFFINE_BTN: case IDC_BUTY_BTN:	case IDC_BRIGHTNESSCTRL_BTN:
		case IDOK: case IDCANCEL:
		{
			if (lpDrawItemStruct->itemAction & 0x07) {
				CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
				if (lpDrawItemStruct->itemState & ODS_SELECTED) {//버튼 클릭시 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105), RGB(60, 75, 105));//버튼 외곽선
					p_dc->SetTextColor(RGB(140, 147, 161));
				}
				else {//기본 상태  //&lpDrawItemStruct->rcItem 버튼의 크기
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71), RGB(42, 52, 71));//버튼 외곽선
					p_dc->SetTextColor(RGB(171, 182, 199));
				}

				p_dc->SetBkMode(TRANSPARENT);
			}
		}
		default: break;
	}
	CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	switch (nIDCtl) {
		case IDC_BUTTON2: {
			p_dc->DrawText(L"사진불러오기", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDC_FILTER_BTN: {
			p_dc->DrawText(L"필터링", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDC_COLOR_BTN: {
			p_dc->DrawText(L"컬러링", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDC_AFFINE_BTN: {
			p_dc->DrawText(L"어파인", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDC_BUTY_BTN: {
			p_dc->DrawText(L"뷰티", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDC_BRIGHTNESSCTRL_BTN: {
			p_dc->DrawText(L"밝기", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDOK: {
			p_dc->DrawText(L"저장", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		case IDCANCEL: {
			p_dc->DrawText(L"취소", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		default: break;
	}

//	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
