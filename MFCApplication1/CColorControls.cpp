// CColorControls.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CColorControls.h"


// CColorControls 대화 상자

IMPLEMENT_DYNAMIC(CColorControls, CDialogEx)

//CColorControls::CColorControls(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_COLOR_CONYROLS, pParent)

CColorControls::CColorControls()
	: CDialogEx(IDD_COLOR_CONYROLS) //부모 클래스의 생성자를 호출
{
}

CColorControls::CColorControls(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_COLOR_CONYROLS), myImg(Img), myBitmapInfo(bitmapInfo)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}

CColorControls::~CColorControls()	
{
}

void CColorControls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_CONTROL, m_imageControl);

	DDX_Control(pDX, IDC_MFCCOLORBUTTON_COLOR, m_mfcColorBtn);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);

	DDX_Control(pDX, IDC_SLIDER_H, m_slider_h);
	DDX_Control(pDX, IDC_SLIDER_EDIT_H, m_slider_edit_h);
	DDX_Control(pDX, IDC_COLOR_EDIT, m_color_edit);
}


BEGIN_MESSAGE_MAP(CColorControls, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
		
	ON_BN_CLICKED(IDOK, &CColorControls::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CColorControls::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BACK, &CColorControls::OnBnClickedBack)
	

	//ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_COLOR, &CColorControls::OnBnClickedMfccolorbuttonColor)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_H, &CColorControls::OnNMCustomdrawSliderH)
END_MESSAGE_MAP()


// CColorControls 메시지 처리기

BOOL CColorControls::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(350, 140, 1280, 720);


	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);
	GetDlgItem(IDC_BACK)->MoveWindow(1000, 720 - 220, 200, 45);
	
	GetDlgItem(IDC_MFCCOLORBUTTON_COLOR)->MoveWindow(1000, 100, 200, 30); //색 편집창
	GetDlgItem(IDC_STATIC_COLOR)->MoveWindow(1000, 50, 70, 40);
	GetDlgItem(IDC_COLOR_EDIT)->MoveWindow(1080, 50, 120, 40); //RGB값

	GetDlgItem(IDC_SLIDER_H)->MoveWindow(1000, 180, 140, 30); //슬라이더H
	GetDlgItem(IDC_SLIDER_EDIT_H)->MoveWindow(1150, 180, 50, 30);

	//DrawImage(); dialog 호출시 oninitDiaog()뒤에 실행되는 메세지들에 의하여, 사진이 출력되지 않음 
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 

	m_mfcColorBtn.SetColor(RGB(255, 255, 255));//색 편집창 기본값
	m_brushColor.CreateSolidBrush(m_mfcColorBtn.GetColor());

	m_slider_h.SetRange(0, 100);	
	m_slider_h.SetPos(50);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CColorControls::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) {
	KillTimer(1);

	//필터창 크기
	CRect rect;
	this->GetClientRect(&rect);
	//CRect ftWnd(0,0,1280,720);// four-integers are left, top, right, and bottom
	// picctrl 너비, 높이 조정 
	int wx = int(rect.right * 5 / 6);
	int wy = rect.bottom;

	//불러올 사진 cols 가져오기.
	CClientDC dc(GetDlgItem(IDC_IMAGE_CONTROL));
	//CRect rect;// 이미지를 넣을 사각형 
	if (requestImg.cols > wx) {
		//cols: 1080 = rows : wid;
		int resize_h = cvRound((wx * requestImg.rows) / requestImg.cols);
		int y = cvRound((wy - resize_h) / 2);
		//if (y < 0) { //구현예정. 
		//	//float ratio =  m_matImage.rows / m_matImage.cols;
		//	y = 0;
		//	
		//}
		GetDlgItem(IDC_IMAGE_CONTROL)->MoveWindow(0, y, requestImg.cols, resize_h);
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2);
		int y = cvRound((wy - requestImg.rows) / 2);
		GetDlgItem(IDC_IMAGE_CONTROL)->MoveWindow(x, y, requestImg.cols, requestImg.rows);
	}

	//CPaintDC dc1(GetDlgItem(IDC_LUMINANCE_BTN)); // device context for painting
	//CRect rect1;
	//GetDlgItem(IDC_LUMINANCE_BTN)->GetWindowRect(&rect1);
	//ScreenToClient(&rect1);
	//dc1.FillSolidRect(rect1, m_selectedColor);

	//GetClientRect(left, top, right, bottom ) 클라이언트 영역의 좌표
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//불러올 이미지 사진에 따라 조정된 Picture Ctrl 크기 
	GetDlgItem(IDC_IMAGE_CONTROL)->GetClientRect(&rect);

	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	// 대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	// 함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, requestImg.cols, requestImg.rows, requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}

void CColorControls::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CColorControls::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CColorControls::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case 1:
		DrawImage(myImg, myBitmapInfo);//처음 로딩되는 이미지 

	}
	KillTimer(1);//처음 필터창을 켰을때, 사진을 띄우기 위한 용도라 바로 kill 
	//CDialogEx::OnTimer(nIDEvent);
}

void CColorControls::CreateBitmapInfo(BITMAPINFO** btmInfo, int w, int h, int bpp) {
	if (*btmInfo != NULL) //기존 비트맵 정보 초기화 
	{
		delete* btmInfo;
		*btmInfo = NULL; //기존 BITMAP 정보 삭제 
	}

	if (bpp == 8) //1채널 
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	(*btmInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	(*btmInfo)->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	(*btmInfo)->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	(*btmInfo)->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	(*btmInfo)->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	(*btmInfo)->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	(*btmInfo)->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	(*btmInfo)->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	(*btmInfo)->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			(*btmInfo)->bmiColors[i].rgbBlue = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbGreen = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbRed = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbReserved = 0;
		}
	}

	(*btmInfo)->bmiHeader.biWidth = w;
	(*btmInfo)->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}

void CColorControls::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CColorControls::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CColorControls::OnBnClickedBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DrawImage(myImg, myBitmapInfo);
	MessageBox(L"원본으로 돌아갑니다", L"알림", MB_OK);
}


//void CColorControls::OnBnClickedLuminanceBtn()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN, this);
//	
//	if (dlg.DoModal() == IDOK) {
//
//		COLORREF color = dlg.GetColor();		
//		TRACE(_T("RGB value of the selected color - red = %u, ")
//			_T("green = %u, blue = %u\n"),
//			GetRValue(color), GetGValue(color), GetBValue(color));
//				
//	}
//	 
//	//CClientDC dc(this);
//	//typedef struct UserPaletteData
//	//{
//	//	LOGPALETTE log_palette;
//	//	PALETTEENTRY entry[256 - 1]; //log_palette에서 초과되는 색상 값을 저장
//
//	//}UPD;
//
//	//UPD upd;
//	//upd.log_palette.palVersion = 0x300; //팔레트 버전
//	//upd.log_palette.palNumEntries = (WORD)256;//팔레트 수
//
//	////dc에 연결된 256개의 색상을 가져온다
//	//if (!::GetSystemPaletteEntries(dc, 0, 256, upd.log_palette.palPalEntry)) {
//	//	memset(upd.log_palette.palPalEntry, 0, sizeof(PALETTEENTRY) * 256);
//	//}
//
//	////가져온 색상으로 팔레트를 생성
//	//m_stock_palette.CreatePalette(NULL);
//
//	////m_colorPicker.SetPalette(palette);
//
//	//m_luminance_btn.SetType(CMFCColorPickerCtrl::LUMINANCE);
//	//m_luminance_btn.SetPalette(&m_stock_palette);
//	//m_luminance_btn.SetColor(RGB(0, 255, 0));
//
//	//COLORREF color = m_luminance_btn.GetColor(); 
//}


//void CColorControls::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
//}


HCURSOR CColorControls::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnQueryDragIcon();
}


HBRUSH CColorControls::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR)
		hbr = m_brushColor;
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CColorControls::OnBnClickedMfccolorbuttonColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_brushColor.DeleteObject();
	m_brushColor.CreateSolidBrush(m_mfcColorBtn.GetColor());
	m_staticColor.Invalidate();
	m_staticColor.UpdateWindow();
		
	// Color Button Control의 현재 색상을 가져옵니다.
	COLORREF color = m_mfcColorBtn.GetColor();

	// RGB 값을 얻어옵니다.
	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	// RGB 값을 텍스트로 변환합니다.
	CString strRGB;
	strRGB.Format(_T("%d, %d, %d"), red, green, blue);
	SetDlgItemText(IDC_COLOR_EDIT, strRGB);
}


void CColorControls::OnNMCustomdrawSliderH(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_H);
	CStatic* Edit = (CStatic*)GetDlgItem(IDC_SLIDER_EDIT_H);
	int pos = slider->GetPos();
	CString str;
	str.Format(L"%d", pos);
	Edit->SetWindowTextW(str);
	
	// OpenCV 이미지의 각 채널을 가져옵니다.
	//vector<Mat> bgr_planes;
	//split(myImg, bgr_planes);

	//bgr_planes[0] = bgr_planes[0] + pos; // Blue 채널에 pos 값을 더합니다.

	// 변경된 채널을 다시 합쳐서 새로운 이미지를 생성합니다.
	//merge(bgr_planes, myImgAfterChange);

	// 변경된 이미지를 화면에 표시
	//DrawImage(myImgAfterChange, myBmpInfoAfterChange);

	*pResult = 0;
}
