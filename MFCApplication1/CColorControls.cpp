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
	DDX_Control(pDX, IDC_COLOR_EDIT, m_color_edit);

	DDX_Control(pDX, IDC_SLIDER_H, m_slider_h);
	DDX_Control(pDX, IDC_SLIDER_EDIT_H, m_slider_edit_h);	
	DDX_Control(pDX, IDC_SLIDER_S, m_slider_s);
	DDX_Control(pDX, IDC_SLIDER_EDIT_S, m_slider_edit_s);
	DDX_Control(pDX, IDC_SLIDER_V, m_slider_v);
	DDX_Control(pDX, IDC_SLIDER_EDIT_V, m_slider_edit_v);
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
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_H, &CColorControls::OnNMCustomdrawSliderH)// EDIT 출력 실행력 낮음
	
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
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

	GetDlgItem(IDC_SLIDER_H)->MoveWindow(1000, 180, 200, 30); //슬라이더H
	GetDlgItem(IDC_SLIDER_EDIT_H)->MoveWindow(1140, 220, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_H)->MoveWindow(1000, 220, 120, 30);

	GetDlgItem(IDC_SLIDER_S)->MoveWindow(1000, 280, 200, 30); //슬라이더S
	GetDlgItem(IDC_SLIDER_EDIT_S)->MoveWindow(1140, 320, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_S)->MoveWindow(1000, 320, 120, 30);

	GetDlgItem(IDC_SLIDER_V)->MoveWindow(1000, 380, 200, 30); //슬라이더V
	GetDlgItem(IDC_SLIDER_EDIT_V)->MoveWindow(1140, 420, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_V)->MoveWindow(1000, 420, 120, 30);

	//DrawImage(); dialog 호출시 oninitDiaog()뒤에 실행되는 메세지들에 의하여, 사진이 출력되지 않음 
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 

	m_mfcColorBtn.SetColor(RGB(255, 255, 255));//색 편집창 기본값
	m_brushColor.CreateSolidBrush(m_mfcColorBtn.GetColor());

	m_slider_h.SetRange(0, 100);	
	m_slider_h.SetPos(50);
	//m_slider_h.SetTicFreq(10);

	m_slider_s.SetRange(0, 100);
	m_slider_s.SetPos(50);
	//m_slider_s.SetTicFreq(10);

	m_slider_v.SetRange(0, 100);
	m_slider_v.SetPos(50);
	//m_slider_v.SetTicFreq(10);
		
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

//슬라이더 조정값 출력
void CColorControls::UpdateSliderValue(int sliderID, int editID)
{
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(sliderID);
	CEdit* edit = (CEdit*)GetDlgItem(editID);

	int pos = slider->GetPos();
	CString str;

	str.Format(L"%d", pos);
	edit->SetWindowTextW(str);
}

//해당 슬라이더 성분값으로 이미지 변환
void CColorControls::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
		
	if (*pScrollBar == m_slider_h) {
        int hueShift = m_slider_h.GetPos() - 50; // 색조의 변화량 계산

        if (hueShift != 0) {
            // 이미지를 HSV 색 공간으로 변환
            Mat hsvImage;
            cvtColor(myImg, hsvImage, COLOR_BGR2HSV);

            // HSV 이미지의 H 채널에 색조를 적용
            for (int i = 0; i < hsvImage.rows; ++i) {
                for (int j = 0; j < hsvImage.cols; ++j) {
                    hsvImage.at<Vec3b>(i, j)[0] += hueShift; // H 채널에 색조 변환
                    // 색조가 0~180 범위 내에 있도록 보정
                    if (hsvImage.at<Vec3b>(i, j)[0] < 0)
                        hsvImage.at<Vec3b>(i, j)[0] += 180;
                    else if (hsvImage.at<Vec3b>(i, j)[0] >= 180)
                        hsvImage.at<Vec3b>(i, j)[0] -= 180;
                }
            }

            // HSV 이미지를 다시 BGR 색 공간으로 변환
            cvtColor(hsvImage, myImgAfterChange, COLOR_HSV2BGR);

            // 비트맵 정보 생성 및 이미지 출력
            CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
            DrawImage(myImgAfterChange, myBmpInfoAfterChange);
        }
	}
	else if (*pScrollBar == m_slider_s) {
		int hueShift = m_slider_s.GetPos() - 50; // 색조의 변화량 계산

		if (hueShift != 0) {
			// 이미지를 HSV 색 공간으로 변환
			Mat hsvImage;
			cvtColor(myImg, hsvImage, COLOR_BGR2HSV);

			// HSV 이미지의 S 채널에 채도를 적용
			for (int i = 0; i < hsvImage.rows; ++i) {
				for (int j = 0; j < hsvImage.cols; ++j) {
					hsvImage.at<Vec3b>(i, j)[1] += hueShift; // S 채널에 채도 변환
					// 채도가 0~255 범위 내에 있도록 보정
					if (hsvImage.at<Vec3b>(i, j)[1] < 0)
						hsvImage.at<Vec3b>(i, j)[1] = 0;
					else if (hsvImage.at<Vec3b>(i, j)[1] > 255)
						hsvImage.at<Vec3b>(i, j)[1] = 255;
				}
			}

			// HSV 이미지를 다시 BGR 색 공간으로 변환
			cvtColor(hsvImage, myImgAfterChange, COLOR_HSV2BGR);

			// 비트맵 정보 생성 및 이미지 출력
			CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
			DrawImage(myImgAfterChange, myBmpInfoAfterChange);
		}
	}
	else if (*pScrollBar == m_slider_v) {
		int brightnessChange = m_slider_v.GetPos() - 50; // 밝기의 변화량 계산

		if (brightnessChange != 0) {
			// 이미지 밝기 조절
			Mat adjustedImage;
			myImg.convertTo(adjustedImage, -1, 1, brightnessChange);

			// 이미지 출력
			DrawImage(adjustedImage, myBmpInfoAfterChange);
		}
	}
	if (*pScrollBar == m_slider_h || *pScrollBar == m_slider_s || *pScrollBar == m_slider_v) {
		// 각 슬라이더에 대한 이벤트 처리
		UpdateSliderValue(IDC_SLIDER_H, IDC_SLIDER_EDIT_H);
		UpdateSliderValue(IDC_SLIDER_S, IDC_SLIDER_EDIT_S);
		UpdateSliderValue(IDC_SLIDER_V, IDC_SLIDER_EDIT_V);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorControls::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent) {
	case 1:
		DrawImage(myImg, myBitmapInfo);//처음 로딩되는 이미지 

	}
	KillTimer(1);//처음 창을 켰을때, 사진을 띄우기 위한 용도라 바로 kill 
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
	myImgAfterChange = myImg.clone();
	myBmpInfoAfterChange = myBitmapInfo;
	MessageBox(L"원본으로 돌아갑니다", L"알림", MB_OK);
}

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

//색 편집창 지정색으로 블록컬러 변경
HBRUSH CColorControls::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR)
		hbr = m_brushColor;
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//색 편집창 지정색 RGB값 출력
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

	// 선택한 색상을 변수에 저장합니다.
	m_selectedColor = Vec3b(blue, green, red);

	// RGB 값을 텍스트로 변환합니다.
	CString strRGB;
	strRGB.Format(_T("%d, %d, %d"), red, green, blue);
	SetDlgItemText(IDC_COLOR_EDIT, strRGB);
}

void CColorControls::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 마우스 왼쪽 버튼을 누른 상태를 기록합니다.
	m_bDragging = true;

	// 드래그 영역의 시작점을 저장합니다.
	m_ptDragStart = point;

	// 부모 클래스의 마우스 왼쪽 버튼을 누른 이벤트 처리 함수를 호출합니다.
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CColorControls::OnMouseMove(UINT nFlags, CPoint point)
{
	// 만약 드래그 중이라면
	if (m_bDragging)
	{
		// 마우스 이동 중인 지점을 저장합니다.
		m_ptDragEnd = point;

		// 드래그 중인 영역을 화면에 그립니다.
		CDC* pDC = GetDC();
		CPen pen(PS_SOLID, 10, RGB(255, 0, 0)); // 빨간색 펜 생성
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->SetROP2(R2_NOTXORPEN); // XOR 모드로 설정하여 반전된 효과를 만듭니다.
		pDC->Rectangle(m_ptDragStart.x, m_ptDragStart.y, m_ptDragEnd.x, m_ptDragEnd.y); // 사각형을 그립니다.
		pDC->SelectObject(pOldPen); // 이전 펜으로 복원합니다.
		ReleaseDC(pDC);
	}

	// 부모 클래스의 마우스 이동 이벤트 처리 함수를 호출합니다.
	CDialogEx::OnMouseMove(nFlags, point);
}

void CColorControls::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 마우스 왼쪽 버튼을 놓은 상태를 기록합니다.
	m_bDragging = false;

	// 드래그 중인 영역을 화면에서 지웁니다.
	CDC* pDC = GetDC();
	pDC->SelectStockObject(WHITE_PEN); // 흰색 펜을 선택하여 지우기 작업을 수행합니다.
	pDC->SetROP2(R2_COPYPEN); // XOR 모드를 해제합니다.
	pDC->Rectangle(m_ptDragStart.x, m_ptDragStart.y, m_ptDragEnd.x, m_ptDragEnd.y); // 이전에 그렸던 사각형을 다시 그려서 지웁니다.
	ReleaseDC(pDC);

	// 선택한 색상을 드래그한 영역에 적용합니다.
	ApplyColorToDraggedRegion(m_ptDragStart, m_ptDragEnd);

	// 부모 클래스의 마우스 왼쪽 버튼을 놓은 이벤트 처리 함수를 호출합니다.
	CDialogEx::OnLButtonUp(nFlags, point);
}

// 드래그한 영역에 선택한 색상을 적용하는 함수
void CColorControls::ApplyColorToDraggedRegion(CPoint startPoint, CPoint endPoint)
{
	// 이미지의 너비와 높이를 가져옵니다.
	int imageWidth = myImg.cols;
	int imageHeight = myImg.rows;

	// 드래그한 영역의 시작점과 끝점을 정렬합니다.
	int startX = min(startPoint.x, endPoint.x);
	int endX = max(startPoint.x, endPoint.x);
	int startY = min(startPoint.y, endPoint.y);
	int endY = max(startPoint.y, endPoint.y);

	// 선택한 색상을 이미지의 드래그한 영역에 적용합니다.
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			// 드래그한 영역의 각 픽셀에 선택한 색상을 적용합니다.
			myImg.at<Vec3b>(y, x) = m_selectedColor;
		}
	}

	// 변경된 이미지를 화면에 출력합니다.
	CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
	DrawImage(myImgAfterChange, myBmpInfoAfterChange);
}

//void CColorControls::OnNMCustomdrawSliderH(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//		
//	UpdateSliderValue(IDC_SLIDER_H, IDC_SLIDER_EDIT_H);
//	UpdateSliderValue(IDC_SLIDER_S, IDC_SLIDER_EDIT_S);
//	UpdateSliderValue(IDC_SLIDER_V, IDC_SLIDER_EDIT_V);
//
//	*pResult = 0;
//}