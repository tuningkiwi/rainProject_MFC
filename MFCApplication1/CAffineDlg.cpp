// CAffineDlg.cpp: 구현 파일
// 인태 Affine 
// 인태 푸시 테스트 

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CAffineDlg.h"


// CAffineDlg 대화 상자

IMPLEMENT_DYNAMIC(CAffineDlg, CDialogEx)

CAffineDlg::CAffineDlg()
	: CDialogEx(IDD_Affine_it)
{

}

CAffineDlg::CAffineDlg(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDD_Affine_it)
{
	myImg = Img;
	myBitmapInfo = bitmapInfo;
}

CAffineDlg::~CAffineDlg()
{
}

void CAffineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PC_IT, picCtrl_IT);
	DDX_Control(pDX, IDC_BUTTON_RR, rotationR);
	DDX_Control(pDX, IDC_BUTTON_LR, rotationL);
	DDX_Control(pDX, IDC_SIZE_SLIDER, SizeSlide);
	DDX_Control(pDX, IDC_BUTTON_FLIP, Flip);
}


BEGIN_MESSAGE_MAP(CAffineDlg, CDialogEx)

	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CAffineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAffineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REVERSE_IT, &CAffineDlg::OnBnClickedReverseIt)
	ON_BN_CLICKED(IDC_BUTTON_RR, &CAffineDlg::OnBnClickedButtonRr)
	ON_BN_CLICKED(IDC_BUTTON_LR, &CAffineDlg::OnBnClickedButtonLr)
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_FLIP, &CAffineDlg::OnBnClickedButtonFlip)
END_MESSAGE_MAP()


// CAffineDlg 메시지 처리기

BOOL CAffineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	MoveWindow(350, 140, 1280, 720);

	CRect wnd;
	this->GetClientRect(&wnd); // 기본 사각형의 x,y 좌표설정이되고 =(0,0) 시작되는함수'GetClientRect' 함수에 
	int btnLocX = int(wnd.right * 5 / 6);
	int btnLocY = 40;

	rotationR.MoveWindow(btnLocX, 120, 200, 45);
	rotationL.MoveWindow(btnLocX, 180, 200, 45);
	
	GetDlgItem(IDC_SIZE_TEXT)->MoveWindow(btnLocX, 280, 200, 20);
	SizeSlide.MoveWindow(btnLocX, 320, 200, 20);
	SizeSlide.SetRange(0, 2);
	SizeSlide.SetTicFreq(1);
	SizeSlide.SetTic(1);
	SizeSlide.SetPos(1);
	Flip.MoveWindow(btnLocX, 720 - 340, 200, 45);

	GetDlgItem(IDCANCEL)->MoveWindow(btnLocX, 720 - 160, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(btnLocX, 720 - 220, 200, 45);
	GetDlgItem(IDC_REVERSE_IT)->MoveWindow(btnLocX, 720 - 280, 200, 45);
	
	SetTimer(1, 80, NULL);//100ms  사진 불러오기 위한 타이머 

	CFont font;
	font.CreatePointFont(140, _T("함초롬돋움 확장 보통"));
	rotationR.SetFont(&font);
	rotationL.SetFont(&font);
	Flip.SetFont(&font);
	GetDlgItem(IDC_SIZE_TEXT)->SetFont(&font);
	GetDlgItem(IDOK)->SetFont(&font);
	GetDlgItem(IDCANCEL)->SetFont(&font);
	GetDlgItem(IDC_REVERSE_IT)->SetFont(&font);
	font.Detach();//font 종료 꼭 해주기 메모리 할당 해제

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다. 
}

void CAffineDlg::ReadImage(Mat originImg, BITMAPINFO* originBmpInfo)
{
	KillTimer(1);
	
	//필터창 크기
	CRect wnd;
	this->GetClientRect(&wnd); // 기본 사각형의 x,y 좌표설정이되고 =(0,0) 시작되는함수'GetClientRect' 함수에 

	int wx = int(wnd.right * 5 / 6);
	int wy = wnd.bottom;

	//불러올 사진 cols 가져오기.
	CClientDC dc(GetDlgItem(IDC_PC_IT));
	CRect rect;// 이미지를 넣을 사각형 
	if (originImg.cols > wx) 
	{
		//cols: 1080 = rows : wid;
		int resize_h = cvRound((wx * originImg.rows) / originImg.cols);
		int resize_w = wx; //width를 최대크기로 설정 
		if (wy - resize_h < 0) { //width를 맞추니, height가 넘친다 
			resize_w = wy * wx / resize_h;
		}
		int x = cvRound((wx - resize_w) / 2);
		int y = cvRound((wy - resize_h) / 2);
		GetDlgItem(IDC_PC_IT)->MoveWindow(x, y, resize_w, resize_h);
		picLTRB.left = x; picLTRB.top = y;
		picLTRB.right = resize_w; picLTRB.bottom = resize_h;

	}
	else 
	{
		int x = cvRound((wx - originImg.cols) / 2);
		int y = cvRound((wy - originImg.rows) / 2);
		GetDlgItem(IDC_PC_IT)->MoveWindow(x, y, originImg.cols, originImg.rows);
		picLTRB.left = x; picLTRB.top = y;
		picLTRB.right = x + originImg.cols; picLTRB.bottom = y + originImg.rows;
	}

	//GetClientRect(left, top, right, bottom ) 클라이언트 영역의 좌표
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//불러올 이미지 사진에 따라 조정된 Picture Ctrl 크기 
	GetDlgItem(IDC_PC_IT)->GetClientRect(&rect);

	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	// 대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	// 함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, originImg.cols, originImg.rows, originImg.data, originBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}

void CAffineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent) {
	case 1:
		ReadImage(myImg, myBitmapInfo);//처음 로딩되는 이미지 
		break;

	}
	//KillTimer(1);//처음 필터창을 켰을때, 사진을 띄우기 위한 용도라 바로 kill 
	CDialogEx::OnTimer(nIDEvent);
}

void CAffineDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	//KillTimer(1);
	//저장 버튼 눌렀을 때, bmpinfo 변경되야 함 
	//취소 버튼 눌렀을 떄, bmpinfo 유지되야함. 

	// TODO: Add your message handler code here
}

//BITMAP 정보 구조체 데이터 생성 
void CAffineDlg::MakeBitmapInfo(BITMAPINFO** btmInfo, int w, int h, int bpp) {
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

void CAffineDlg::OnBnClickedOk()  // 적용 버튼 누르면 부모Dlg에서 변경된 이미지가 출력
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CAffineDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CAffineDlg::OnBnClickedReverseIt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ReadImage(myImg, myBitmapInfo);
	MessageBox(L"원본 이미지로 돌아갑니다", L"알림", MB_OK);
	currentRotatedImg = myImg.clone();
	resultImg = myImg.clone();
	ChangeImg = myImg.clone();
	SizeSlide.SetPos(1);
}

//이미지 시계 방향 회전
void CAffineDlg::OnBnClickedButtonRr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!resultImg.empty())
	{
		ChangeImg = resultImg.clone();
		currentRotatedImg = resultImg.clone();
	}
	else
	{
		ChangeImg = myImg.clone();
	}

	if (!currentRotatedImg.empty())
	{
		ChangeImg = currentRotatedImg;
	}
	
	// 이미지를 회전
	cv::rotate(ChangeImg, currentRotatedImg, ROTATE_90_CLOCKWISE);
	MakeBitmapInfo(&resultmyBitmapInfo, currentRotatedImg.cols, currentRotatedImg.rows, currentRotatedImg.channels() * 8);

	// 화면에 회전된 이미지 표시
	ReadImage(currentRotatedImg, resultmyBitmapInfo);
	
	resultImg = currentRotatedImg.clone();
}

//이미지 반시계 방향 회전
void CAffineDlg::OnBnClickedButtonLr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (!resultImg.empty())
	{
		ChangeImg = resultImg.clone();
		currentRotatedImg = resultImg.clone();
	}
	else 
	{
		ChangeImg = myImg.clone();
	}

	if (!currentRotatedImg.empty())
	{
		ChangeImg = currentRotatedImg;
	}

	// 이미지를 회전
	cv::rotate(ChangeImg, currentRotatedImg, ROTATE_90_COUNTERCLOCKWISE);
	MakeBitmapInfo(&resultmyBitmapInfo, currentRotatedImg.cols, currentRotatedImg.rows, currentRotatedImg.channels() * 8);

	// 화면에 회전된 이미지 표시
	ReadImage(currentRotatedImg, resultmyBitmapInfo);
	
	resultImg = currentRotatedImg.clone();
}


//이미지 크기 조절
void CAffineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!resultImg.empty())
	{
		ChangeImg = resultImg.clone();
	}
	else
	{
		ChangeImg = myImg.clone();
	}

	//ChangeImg = myImg.clone();

	if (*pScrollBar == SizeSlide) 
	{
		// 슬라이더에서 변경된 위치 얻기
		int pos = SizeSlide.GetPos();
		
		// 각 슬라이더 위치에 대한 크기 조정값 정의
		const double scaleFactor[] = { 0.4, 1, 1.4 }; // 순서대로 0, 1, 2에 해당

		// 유효한 슬라이더 위치 범위 확인
		if (pos == 0)
		{
			// 이미지 크기 조정
			resize(ChangeImg, SizeImg, Size(static_cast<int>(myImg.cols * scaleFactor[pos]),
				static_cast<int>(myImg.rows * scaleFactor[pos])),
				INTER_LINEAR);


			// 비트맵 정보 생성 및 이미지 출력
			MakeBitmapInfo(&resultmyBitmapInfo, SizeImg.cols, SizeImg.rows, SizeImg.channels() * 8);
			ReadImage(SizeImg, resultmyBitmapInfo);
			resultImg = SizeImg.clone();
		}
		else if (pos == 1)
		{
			// 이미지 크기 조정
			resize(ChangeImg, SizeImg, Size(static_cast<int>(myImg.cols * scaleFactor[pos]),
				static_cast<int>(myImg.rows * scaleFactor[pos])),
				INTER_LINEAR);


			// 비트맵 정보 생성 및 이미지 출력
			MakeBitmapInfo(&resultmyBitmapInfo, SizeImg.cols, SizeImg.rows, SizeImg.channels() * 8);
			ReadImage(SizeImg, resultmyBitmapInfo);
			resultImg = SizeImg.clone();
		}
		else if (pos == 2)
		{
			// 이미지 크기 조정
			resize(ChangeImg, SizeImg, Size(static_cast<int>(myImg.cols * scaleFactor[pos]),
				static_cast<int>(myImg.rows * scaleFactor[pos])),
				INTER_LINEAR);

			// 비트맵 정보 생성 및 이미지 출력
			MakeBitmapInfo(&resultmyBitmapInfo, SizeImg.cols, SizeImg.rows, SizeImg.channels() * 8);
			ReadImage(SizeImg, resultmyBitmapInfo);
			resultImg = SizeImg.clone();
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 주석 테스트 

//Static Text 글씨크기
BOOL CAffineDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(181, 214, 146));

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

//버튼 색상 및 버튼명 
void CAffineDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	switch (nIDCtl)
	{
		case IDC_BUTTON_RR: case IDC_BUTTON_LR:	case IDC_BUTTON_FLIP:
		{
			if (lpDrawItemStruct->itemAction & 0x07) 
			{
				CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
				if (lpDrawItemStruct->itemState & ODS_SELECTED)
				{//버튼 클릭시 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 0, 0));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 0), RGB(255, 255, 0));//버튼 외곽선
					p_dc->SetTextColor(RGB(0, 0, 0));
				}
				else
				{//버튼이 눌리지 않은 상태일 때 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 192, 203)); // 버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(135, 206, 235), RGB(135, 206, 235));//버튼 외곽선 (검은색)
					p_dc->SetTextColor(RGB(0, 0, 0)); // 텍스트 색상 (검은색)
				}
				p_dc->SetBkMode(TRANSPARENT);
			}
			break;
		}
		case IDCANCEL: case IDOK: case IDC_REVERSE_IT:
		{
			if (lpDrawItemStruct->itemAction & 0x07)
			{
				CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
				if (lpDrawItemStruct->itemState & ODS_SELECTED)
				{//버튼 클릭시 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 0, 0));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 0), RGB(255, 255, 0));//버튼 외곽선
					p_dc->SetTextColor(RGB(0, 0, 0));
				}
				else
				{//버튼이 눌리지 않은 상태일 때 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 253, 208)); // 버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(245, 245, 220), RGB(245, 245, 220));//버튼 외곽선 
					p_dc->SetTextColor(RGB(0, 0, 0)); // 텍스트 색상 (검은색)
				}
				p_dc->SetBkMode(TRANSPARENT);
			}
			break;
		}
		default: break;
	}
	CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	switch (nIDCtl)
	{
	case IDC_BUTTON_RR: {
		p_dc->DrawText(L"시계방향 회전", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDC_BUTTON_LR: {
		p_dc->DrawText(L"반시계방향 회전", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDC_REVERSE_IT: {
		p_dc->DrawText(L"되돌리기", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDOK: {
		p_dc->DrawText(L"적용", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDCANCEL: {
		p_dc->DrawText(L"취소", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDC_BUTTON_FLIP: {
		p_dc->DrawText(L"좌우 반전", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	default:
		break;
	}
	//CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//좌우반전
void CAffineDlg::OnBnClickedButtonFlip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!resultImg.empty())
	{
		ChangeImg = resultImg.clone();
	}
	else
	{
		ChangeImg = myImg.clone();
	}

	// 이미지 좌우 반전
	cv::flip(ChangeImg, FlipImg, 1); // 1은 좌우 반전을 의미합니다.

	// 이미지 크기가 변경되었으므로 비트맵 정보 업데이트
	MakeBitmapInfo(&resultmyBitmapInfo, FlipImg.cols, FlipImg.rows, FlipImg.channels() * 8);

	// 변경된 이미지 출력
	ReadImage(FlipImg, resultmyBitmapInfo);
	resultImg = FlipImg.clone();
}
