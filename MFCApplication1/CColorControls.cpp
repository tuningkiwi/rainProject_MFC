// CColorControls.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CColorControls.h"


// CColorControls 대화 상자

IMPLEMENT_DYNAMIC(CColorControls, CDialogEx)

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
	
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_COLOR, &CColorControls::OnBnClickedMfccolorbuttonColor)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_H, &CColorControls::OnNMCustomdrawSliderH)// EDIT 출력 실행력 낮음
	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_DRAWING, &CColorControls::OnBnClickedDrawing)
	ON_BN_CLICKED(IDC_FIND_COLOR, &CColorControls::OnBnClickedFindColor)
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
	GetDlgItem(IDC_DRAWING)->MoveWindow(1000, 720 - 280, 90, 45);//그리기
	GetDlgItem(IDC_FIND_COLOR)->MoveWindow(1110, 720 - 280, 90, 45);//추출하기
	
	GetDlgItem(IDC_MFCCOLORBUTTON_COLOR)->MoveWindow(1000, 100, 200, 30); //색 편집창
	GetDlgItem(IDC_STATIC_COLOR)->MoveWindow(1000, 50, 70, 40);
	GetDlgItem(IDC_COLOR_EDIT)->MoveWindow(1080, 50, 120, 40); //RGB값

	GetDlgItem(IDC_SLIDER_H)->MoveWindow(1000, 160, 200, 30); //슬라이더H
	GetDlgItem(IDC_SLIDER_EDIT_H)->MoveWindow(1140, 200, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_H)->MoveWindow(1000, 200, 120, 30);

	GetDlgItem(IDC_SLIDER_S)->MoveWindow(1000, 260, 200, 30); //슬라이더S
	GetDlgItem(IDC_SLIDER_EDIT_S)->MoveWindow(1140, 300, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_S)->MoveWindow(1000, 300, 120, 30);

	GetDlgItem(IDC_SLIDER_V)->MoveWindow(1000, 360, 200, 30); //슬라이더V
	GetDlgItem(IDC_SLIDER_EDIT_V)->MoveWindow(1140, 400, 50, 30);
	GetDlgItem(IDC_SLIDER_NAME_V)->MoveWindow(1000, 400, 120, 30);

	
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 

	m_mfcColorBtn.SetColor(RGB(255, 255, 255));//색 편집창 기본값
	m_brushColor.CreateSolidBrush(m_mfcColorBtn.GetColor());

	m_slider_h.SetRange(0, 100);//범위	
	m_slider_h.SetPos(50);//위치
	m_slider_h.SetTicFreq(10);//간격

	m_slider_s.SetRange(0, 100);
	m_slider_s.SetPos(50);
	m_slider_s.SetTicFreq(10);

	m_slider_v.SetRange(0, 100);
	m_slider_v.SetPos(50);
	m_slider_v.SetTicFreq(10);
		
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
		GetDlgItem(IDC_IMAGE_CONTROL)->MoveWindow(0, y, requestImg.cols, resize_h);
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2);
		int y = cvRound((wy - requestImg.rows) / 2);
		GetDlgItem(IDC_IMAGE_CONTROL)->MoveWindow(x, y, requestImg.cols, requestImg.rows);
	}
		
	//GetClientRect(left, top, right, bottom ) 클라이언트 영역의 좌표
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//불러올 이미지 사진에 따라 조정된 Picture Ctrl 크기 
	GetDlgItem(IDC_IMAGE_CONTROL)->GetClientRect(&rect);

	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	//대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	//함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
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
	//Thread Safety: 다중 스레드 환경에서 동시에 여러 작업이 수행될 때 원본 이미지를 사용하면 
	//다른 작업이 이미지를 변경할 수 있습니다. 따라서 복사본을 사용하여 스레드 간의 충돌을 방지합니다.
	//객체 지향 설계: 이미지 처리 함수가 해당 이미지의 소유권을 가지지 않고 
	//다른 함수와 독립적으로 작동할 수 있도록 합니다.
	static Mat previousTransformedImage = myImg.clone();//이전에 적용된 변환 이미지를 저장할 변수

	if (*pScrollBar == m_slider_h) {
        int hueShift = m_slider_h.GetPos() - 50; //슬라이더 초기 위치(50)
		myImgAfterChange = previousTransformedImage.clone(); //이전에 적용된 변환 이미지를 복사

        if (hueShift != 0) {
            //이미지를 HSV 색 공간으로 변환			
			Mat hsvImage;
            cvtColor(myImgAfterChange, hsvImage, COLOR_BGR2HSV);

            //HSV 이미지의 H 채널에 색조를 적용
            for (int i = 0; i < hsvImage.rows; ++i) {
                for (int j = 0; j < hsvImage.cols; ++j) {
                    hsvImage.at<Vec3b>(i, j)[0] += hueShift; // H 채널에 색조 변환
                    //색조가 0~180 범위 내에 있도록 보정
                    if (hsvImage.at<Vec3b>(i, j)[0] < 0)
                        hsvImage.at<Vec3b>(i, j)[0] += 180;
                    else if (hsvImage.at<Vec3b>(i, j)[0] >= 180)
                        hsvImage.at<Vec3b>(i, j)[0] -= 180;
                }
            }

            //HSV 이미지를 다시 BGR 색 공간으로 변환
            cvtColor(hsvImage, myImgAfterChange, COLOR_HSV2BGR);

            //비트맵 정보 생성 및 이미지 출력
            CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
            DrawImage(myImgAfterChange, myBmpInfoAfterChange);			
        }
	}
	else if (*pScrollBar == m_slider_s) {
		int saturationShift = m_slider_s.GetPos() - 50;
		myImgAfterChange = previousTransformedImage.clone(); //이전에 적용된 변환 이미지를 복사
		if (saturationShift != 0) {
			
			Mat hsvImage1;
			cvtColor(myImgAfterChange, hsvImage1, COLOR_BGR2HSV);

			//HSV 이미지의 S 채널에 채도를 적용
			for (int i = 0; i < hsvImage1.rows; ++i) {
				for (int j = 0; j < hsvImage1.cols; ++j) {
					hsvImage1.at<Vec3b>(i, j)[1] += saturationShift; // S 채널에 채도 변환
					//채도가 0~255 범위 내에 있도록 보정
					if (hsvImage1.at<Vec3b>(i, j)[1] < 0)
						hsvImage1.at<Vec3b>(i, j)[1] = 0;
					else if (hsvImage1.at<Vec3b>(i, j)[1] > 255)
						hsvImage1.at<Vec3b>(i, j)[1] = 255;
				}
			}
			
			cvtColor(hsvImage1, myImgAfterChange, COLOR_HSV2BGR);						
			CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
			DrawImage(myImgAfterChange, myBmpInfoAfterChange);			
		}
	}
	else if (*pScrollBar == m_slider_v) {
		int brightnessChange = m_slider_v.GetPos() - 50;
		myImgAfterChange = previousTransformedImage.clone(); //이전에 적용된 변환 이미지를 복사
		if (brightnessChange != 0) {
			//이미지 밝기 조절
			Mat adjustedImage;
			myImgAfterChange.convertTo(adjustedImage, -1, 1, brightnessChange);		
			CreateBitmapInfo(&myBmpInfoAfterChange, adjustedImage.cols, adjustedImage.rows, adjustedImage.channels() * 8);
			DrawImage(adjustedImage, myBmpInfoAfterChange);			
		}
	}
	
	if (*pScrollBar == m_slider_h || *pScrollBar == m_slider_s || *pScrollBar == m_slider_v) {
		//각 슬라이더에 대한 이벤트 처리
		UpdateSliderValue(IDC_SLIDER_H, IDC_SLIDER_EDIT_H);
		UpdateSliderValue(IDC_SLIDER_S, IDC_SLIDER_EDIT_S);
		UpdateSliderValue(IDC_SLIDER_V, IDC_SLIDER_EDIT_V);

		previousTransformedImage = myImgAfterChange.clone();
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
	else //24(3채널) or 32bit(4채널)
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

	//슬라이더 위치를 이전 위치로 되돌립니다.
	m_slider_h.SetPos(50);  m_slider_s.SetPos(50);  m_slider_v.SetPos(50);
	m_slider_edit_h.SetWindowTextW(L"50"); m_slider_edit_s.SetWindowTextW(L"50"); m_slider_edit_v.SetWindowTextW(L"50");

	MessageBox(L"원본으로 돌아갑니다", L"알림", MB_OK);
}

HCURSOR CColorControls::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnQueryDragIcon();
}

//색 편집창 지정색으로 블록컬러 변경
HBRUSH CColorControls::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR)
		hbr = m_brushColor;

	//기본값이 적당하지 않으면 다른 브러시를 반환합니다.
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

	//Color Button Control의 현재 색상을 가져옵니다.
	COLORREF color = m_mfcColorBtn.GetColor();

	//RGB 값을 얻어옵니다.
	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	//선택한 색상을 변수에 저장합니다.
	m_selectedColor = Vec3b(blue, green, red);

	//RGB 값을 텍스트로 변환합니다.
	CString strRGB;
	strRGB.Format(_T("%d, %d, %d"), red, green, blue);
	SetDlgItemText(IDC_COLOR_EDIT, strRGB);
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

void CColorControls::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (drawingMode && IsPointInsideImageArea(point))
	{
		start_pos = point;
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CColorControls::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if (drawingMode && IsPointInsideImageArea(point))
	{
		CClientDC dc(this);
		
		//마우스 클릭 지점이 이미지 영역 내에 있는지 확인합니다.
		if (IsPointInsideImageArea(start_pos) && IsPointInsideImageArea(point))
		{
			//이미지 영역 내에서만 그리기 작업을 수행합니다.
			CPen mypen(PS_SOLID, 5, RGB(m_selectedColor[2], m_selectedColor[1], m_selectedColor[0]));
			dc.SelectObject(&mypen);
			dc.SelectObject(GetStockObject(NULL_BRUSH));

			if (nFlags & MK_CONTROL)
			{
				//왼쪽 상단 모서리의 x 좌표, y좌표, 오른쪽 하단 모서리의 x 좌표, y좌표
				dc.Ellipse(start_pos.x, start_pos.y, point.x, point.y);
			}
			else if (nFlags & MK_SHIFT)
			{
				//선 그리기
				dc.MoveTo(start_pos);
				dc.LineTo(point);
			}
			else
			{
				dc.Rectangle(start_pos.x, start_pos.y, point.x, point.y);
			}
		}		
		drawingMode = false; // 그리기 모드를 종료합니다.
	}
			
	CDialogEx::OnLButtonUp(nFlags, point);
}

//그리기 버튼
void CColorControls::OnBnClickedDrawing()
{
	drawingMode = true;
}

//이미지 영역 내에서만 그리기를 허용하는지 확인하는 함수
bool CColorControls::IsPointInsideImageArea(CPoint point)
{
	CRect imageRect;
	GetImageRect(imageRect);
	return imageRect.PtInRect(point);
}

//이미지 영역을 얻어오는 함수
void CColorControls::GetImageRect(CRect& rect)
{
	// 이미지를 표시하는 컨트롤의 위치와 크기를 얻어옵니다.
	CWnd* pImageCtrl = GetDlgItem(IDC_IMAGE_CONTROL); 
	if (pImageCtrl)
	{
		pImageCtrl->GetClientRect(rect); // 클라이언트 영역을 얻어옵니다.
		pImageCtrl->ClientToScreen(rect); // 화면 좌표로 변환합니다.
		ScreenToClient(rect); // 클라이언트 영역으로 다시 변환합니다.
	}
}

//추출 색상을 선택해 해당 색상으로 변환
void CColorControls::FindingColor(const Mat& myImg, const Vec3b& targetColor)
{
	Mat grayImage;
	cvtColor(myImg, grayImage, COLOR_BGR2GRAY);
		
	// 목표 색상 범위 내의 픽셀을 마스크로 지정
	Mat mask;
	Vec3b lowerBound = targetColor - Vec3b(10, 10, 10); // 선택한 색상의 하한 값 설정
	Vec3b upperBound = targetColor + Vec3b(10, 10, 10); // 선택한 색상의 상한 값 설정
	inRange(myImg, lowerBound, upperBound, mask);

	// 입력 이미지와 마스크를 사용하여 목표 색상이 있는 영역을 추출
	//Mat extractedColor;
	myImg.copyTo(myImgAfterChange, mask);
	
	//부모창에 전달하려면 선언되어있는 myImgAfterChange멤버 변수 사용
	/*CreateBitmapInfo(&myBmpInfoAfterChange, extractedColor.cols, extractedColor.rows, extractedColor.channels() * 8);
	DrawImage(extractedColor, myBmpInfoAfterChange);*/

	CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
	DrawImage(myImgAfterChange, myBmpInfoAfterChange);
}

//추출 버튼
void CColorControls::OnBnClickedFindColor()
{
	COLORREF selectedColor = m_mfcColorBtn.GetColor(); 

	int red = GetRValue(selectedColor);
	int green = GetGValue(selectedColor);
	int blue = GetBValue(selectedColor);

	// OpenCV에서 사용할 색상 값으로 변환 (BGR 순서로 변환)
	Vec3b targetColor(blue, green, red);

	FindingColor(myImg, targetColor);
}
