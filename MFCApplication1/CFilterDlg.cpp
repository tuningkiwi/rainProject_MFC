// CFilterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CFilterDlg.h"

//비디오 필터모드 
#define BASICVM		0 //기본:필터 적용안함
#define EMBOSSVM	1 //진흙
#define BILATERALVM	2 //양방향
#define PARTBLURVM	3 //부분블러
#define FOGVM		4 //안개
#define SHARPVM		5 //샤프닝
#define NOISEVM		6 //노이즈 

// CFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg()
	: CDialogEx(IDD_DIALOG1)
{

}

// #생성자
CFilterDlg::CFilterDlg(Mat Img, BITMAPINFO* bitmapInfo, int fileMode)
	: CDialogEx(IDD_DIALOG1)
{
	// myImg = Img; // 이미지 매트릭스 정보 가져오기 
	// myBitmapInfo = bitmapInfo;
	myfileMode = fileMode;
	bmpHistory.push_back(Img);
	bmpInfoHistory.push_back(bitmapInfo);
}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PC_FT, picCtrl_FT);
	DDX_Control(pDX, IDC_EMBOSS_FT, embossFT);
	DDX_Control(pDX, IDC_FOGLB_FT, fogLB_FT);//IDC_SLIDER_GAUSSIAN_FT
	DDX_Control(pDX, IDC_SLIDER_FOG_FT, fogslider_FT);
	DDX_Control(pDX, IDC_SHARP_SLIDER_FT, sharpSliderFT);
	DDX_Control(pDX, IDC_SHARP_FT, sharpLB_FT);
	DDX_Control(pDX, IDC_BILATERAL_FT, bilateralBtn_FT);
	DDX_Control(pDX, IDC_SLIDER_NOISE_FT, noiseFT);
	DDX_Control(pDX, IDC_NOISELB_FT, noiseLB_FT);
	DDX_Control(pDX, IDC_PARTBLUR_FT, partBlutBtn);
	DDX_Control(pDX, IDC_STATIC_POINTLOC, pointLocFT);
	DDX_Control(pDX, IDC_SLIDER_PARTBLUR, partBlurSlider);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
//	ON_WM_GETMINMAXINFO()
ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
//ON_BN_CLICKED(IDC_IMGLOAD_FT, &CFilterDlg::OnBnClickedImgloadFt)
ON_WM_TIMER()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_EMBOSS_FT, &CFilterDlg::OnBnClickedEmbossFt)
ON_BN_CLICKED(IDC_REVERT_FT, &CFilterDlg::OnBnClickedRevertFt)
ON_BN_CLICKED(IDCANCEL, &CFilterDlg::OnBnClickedCancel)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FOG_FT, &CFilterDlg::OnNMCustomdrawSliderFogFt)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BILATERAL_FT, &CFilterDlg::OnBnClickedBilateralFt)
ON_WM_MOUSEMOVE()
//ON_WM_PAINT()
ON_BN_CLICKED(IDC_PARTBLUR_FT, &CFilterDlg::OnBnClickedPartblurFt)
//ON_WM_PAINT()
ON_WM_DRAWITEM()
ON_WM_CTLCOLOR()
ON_WM_LBUTTONDOWN()
//ON_STN_CLICKED(IDC_STATIC_POINTLOC, &CFilterDlg::OnStnClickedStaticPointloc)
END_MESSAGE_MAP()



// CFilterDlg 메시지 처리기

//시작함수
BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(350, 140, 1280, 720);
	//MoveWindow(0,0, 1920, 1000);

	CRect wnd;
	this->GetClientRect(&wnd); // 기본 사각형의 x,y 좌표설정이되고 =(0,0) 시작되는함수'GetClientRect' 함수에 
	int btnLocX = int(wnd.right * 5 / 6);
	int btnLocY = 40;
	int bottom_btnLocY = wnd.bottom - 40;

	embossFT.MoveWindow(btnLocX, btnLocY, 200, 45); btnLocY += 45;//1000, 40, 200, 45
	bilateralBtn_FT.MoveWindow(btnLocX, btnLocY, 200, 45); btnLocY += 45;
	partBlutBtn.MoveWindow(btnLocX, btnLocY, 200, 45); btnLocY += 45;
	pointLocFT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	partBlurSlider.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	partBlurSlider.SetRange(1, 5);
	partBlurSlider.SetTicFreq(1);
	partBlurSlider.SetPos(1);
	fogLB_FT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;//1000, 105, 200, 20
	fogslider_FT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	fogslider_FT.SetRange(0, 5);
	fogslider_FT.SetTicFreq(1);
	fogslider_FT.SetTic(5);
	fogslider_FT.SetPos(0);
	sharpLB_FT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	sharpSliderFT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	sharpSliderFT.SetRange(0, 5);
	sharpSliderFT.SetTicFreq(1);
	sharpSliderFT.SetPos(0);
	noiseLB_FT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	noiseFT.MoveWindow(btnLocX, btnLocY, 200, 20); btnLocY += 20;
	noiseFT.SetRange(0, 100);
	noiseFT.SetTicFreq(10);
	noiseFT.SetPos(0);

	blurRangeHalfWid = 1;

	GetDlgItem(IDCANCEL)->MoveWindow(btnLocX, bottom_btnLocY, 200, 45); bottom_btnLocY -= 55;
	GetDlgItem(IDOK)->MoveWindow(btnLocX, bottom_btnLocY, 200, 45); bottom_btnLocY -= 55;
	GetDlgItem(IDC_REVERT_FT)->MoveWindow(btnLocX, bottom_btnLocY, 200, 45);
	partBlurModeOn = false;
	//DrawImage(); dialog 호출시 oninitDiaog()뒤에 실행되는 메세지들에 의하여, 사진이 출력되지 않음 
	if (myfileMode == 0) {
		SetTimer(0, 80, NULL);//100ms  사진 불러오기 위한 타이머 
	}
	else {
		SetTimer(1, 80, NULL);//100ms  동영상 불러오기 위한 타이머 
	}

	CDialogEx::SetBackgroundColor(0x004D3428, 1);

	CFont font;
	font.CreatePointFont(120, _T("함초롬돋움 확장 보통"));//함초롬돋움 확장 보통
	embossFT.SetFont(&font);
	fogLB_FT.SetFont(&font);
	sharpLB_FT.SetFont(&font);
	bilateralBtn_FT.SetFont(&font);
	noiseLB_FT.SetFont(&font);
	partBlutBtn.SetFont(&font);
	pointLocFT.SetFont(&font);
	GetDlgItem(IDOK)->SetFont(&font);
	GetDlgItem(IDCANCEL)->SetFont(&font);
	GetDlgItem(IDC_REVERT_FT)->SetFont(&font);
	font.Detach();//font 종료 꼭 해주기 메모리 할당 해제 

	cntScroll = 0;
	//비디오모드 설정 
	//0:기본 1:진흙 2:양방향 3:부분블러 4:안개필터 5:샤프닝 6:노이즈
	videoMode[0] = BASICVM; //0번 
	videoMode[1] = 0;//트랙바의 경우 필터의 강도 
	// videoMode.push_back("basic",0,0);
	// videoMode.push_back("emboss",0,0);
	// videoMode.push_back("bilateral",0,0);
	// videoMode.push_back("partBlur",0,0);
	// videoMode.push_back("fog",0,0);
	// videoMode.push_back("sharpen",0,0);
	// videoMode.push_back("noise",0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//INT_PTR CFilterDlg::DoModal()
//{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

//	return CDialogEx::DoModal();
//}


//void CFilterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	lpMMI->ptMinTrackSize = CPoint(1080,720);
//	lpMMI->ptMaxTrackSize = CPoint(1080,720);
//	CFilterDlg::OnGetMinMaxInfo(lpMMI);
	//CDialogEx::OnGetMinMaxInfo(lpMMI);
//}


//#저장버튼 저장함수
//적용 버튼을 누르면, 창을 종료하고, 부모창에도 변경된 이미지가 보여짐 
void CFilterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//메세지 박스 적용되었습니다 알림 넣기 

	KillTimer(2);
	CDialogEx::OnOK();
}


//#드로우이미지 다이얼로그창blurLoc에 사진 띄우기 
void CFilterDlg::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) {
	KillTimer(0);//0사진타이머 

	//현재 불러올 이미지에 대한 picture contorl 크기 조정  
	CRect rect = pictureControlSizeSet();

	//GetClientRect(left, top, right, bottom ) 클라이언트 영역의 좌표
	//함수가 성공하면 반환 값이 0이 아닙니다.
	//불러올 이미지 사진에 따라 조정된 Picture Ctrl 크기 
	GetDlgItem(IDC_PC_FT)->GetClientRect(&rect);
	CClientDC dc(GetDlgItem(IDC_PC_FT)); 
	//픽셀을 삭제합니다. 이 모드는 해당 정보를 보존하지 않고 
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

	//StretchDIBits 함수는 DIB, JPEG 또는 PNG 이미지의 픽셀 사각형에 
	// 대한 색 데이터를 지정된 대상 사각형에 복사합니다.
	//dc.GetSafeHdc(): 출력 디바이스 컨텍스트를 가져옵니다
	// 함수가 성공하면 반환 값은 복사된 검사 줄의 수입니다. 이 값은 미러된 콘텐츠에 대해 음수일 수 있습니다.
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, requestImg.cols, requestImg.rows, requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}

//void CFilterDlg::OnBnClickedImgloadFt()
//{
	// TODO: Add your control notification handler code here
//	DrawImage();
//}

//타이머함수
void CFilterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==0) {//0번타이머

		//처음 다이얼로그 창을 띄울 때 onInitDialog()에서 drawimage가 이후 
		// 자동 수행되는 메시지 함수에의해서 출력이 안되서 
		//onInitDialog()에 타이머로 걸어놓음  
		DrawImage(bmpHistory.back(), bmpInfoHistory.back());//처음 로딩되는 이미지 
		KillTimer(0);
	}else if (nIDEvent==1) {//1번타이머 video capture 세팅 

		CRect wnd;
		this->GetClientRect(&wnd); // 기본 사각형의 x,y 좌표설정이되고 =(0,0) 시작되는함수'GetClientRect' 함수에 
		int wid = int(wnd.right * 5 / 6);
		int hei = int(wid*720/1280);
		int y = cvRound((wnd.bottom - hei) / 2);
		picCtrl_FT.MoveWindow(0, y, wid, hei);//픽쳐컨트롤 크기 조정


		// 웹캠 열기
		capture = new VideoCapture(0, CAP_DSHOW);
		if (!capture->isOpened())
		{
			MessageBox(_T("웹캠을 열 수 없습니다."));
			return;
		}

		// 웹캠 크기 설정
		capture->set(CAP_PROP_FRAME_WIDTH, wid);
		capture->set(CAP_PROP_FRAME_HEIGHT, hei);

		SetTimer(2, 30, NULL);//본격 동영상 출력을 위한 프로세스로 넘어간다 
		KillTimer(1);//카메라세팅 타이머 
		
	}else if (nIDEvent==2) {//2번타이머 영상 출력 
		Mat videoFrame;
		capture->read(videoFrame);
		//이미지 처리 함수
		bmpHistory.push_back(videoFrame);
		BITMAPINFO* bmpinfo = CreateBitmapInfo(bmpHistory.back().cols, bmpHistory.back().rows, bmpHistory.back().channels() * 8);
		bmpInfoHistory.push_back(bmpinfo);
		// BITMAPINFO* dstinfo = bmpInfoHistory.back();
		//bmpInfoHistory.push_back(dstinfo);
	 	
		int filtermode = videoMode[0];
		int level = videoMode[1];
		if(filtermode ==FOGVM ){
			fogFilter(level);
		}
		videoFrame = bmpHistory.back();
		//videoFrame = bmpHistory.back();
		//화면에 보여주기 위한 처리입니다.
		int bpp = 8 * videoFrame.elemSize();
		assert((bpp == 8 || bpp == 24 || bpp == 32));

		//Mat mat_temp = videoFrame;
		// bmpHistory.push_back(videoFrame); //borderline 처리한 이미지 행렬
		RECT r;
		picCtrl_FT.GetClientRect(&r);

		cv::Size winSize(r.right, r.bottom);
		cimage_mfc.Create(winSize.width, winSize.height, 24);
		BITMAPINFO* bitInfo = bmpInfoHistory.back();

		if (videoFrame.cols == winSize.width && videoFrame.rows == winSize.height)
		{
			// source and destination have same size
			// transfer memory block
			// NOTE: the padding border will be shown here. Anyway it will be max 3px width

			SetDIBitsToDevice(cimage_mfc.GetDC(),
				//destination rectangle
				0, 0, winSize.width, winSize.height,
				0, 0, 0, videoFrame.rows,
				videoFrame.data, bitInfo, DIB_RGB_COLORS);
		}
		else
		{
			// destination rectangle
			int destx = 0, desty = 0;
			int destw = winSize.width;
			int desth = winSize.height;

			// rectangle defined on source bitmap
			// using imgWidth instead of mat_temp.cols will ignore the padding border
			int imgx = 0, imgy = 0;
			int imgWidth = videoFrame.cols; //- border;
			int imgHeight = videoFrame.rows;

			StretchDIBits(cimage_mfc.GetDC(),
				destx, desty, destw, desth,
				imgx, imgy, imgWidth, imgHeight,
				videoFrame.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		HDC dc = ::GetDC(picCtrl_FT.m_hWnd);
		cimage_mfc.BitBlt(dc, 0, 0);

		::ReleaseDC(picCtrl_FT.m_hWnd, dc);

		cimage_mfc.ReleaseDC();
		cimage_mfc.Destroy();
		if (bmpHistory.size() == 6) {
			bmpHistory.clear();
			bmpInfoHistory.clear();
		}
		
	}
	
	// Handle other timer events or call the base class
	CDialogEx::OnTimer(nIDEvent);	

}

//윈도우 창이 DESTORY 갑자기 종료될때, 저장해야 하는 정보들 기능 수행
void CFilterDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}

//진흙필터함수 :1채널 필터링 : grayscale이미지로 변환 필요 
void CFilterDlg::OnBnClickedEmbossFt()//1채널 필터링 
{
	// TODO: Add your control notification handler code here
	if (bmpHistory.back().channels() >=3) {

		Mat src = colorToGray();//color 사진의 경우 gray로 변경.
		//그레이로 잘 출력되는 지 확인 
		/*BITMAPINFO* srcinfo = CreateBitmapInfo(src.cols,src.rows,src.channels()*8);
		DrawImage(src, srcinfo);*/ 
		float data[] = { -1,-1,0,-1,0,1,0,1,1 };
		Mat emboss(3, 3, CV_32FC1, data);

		Mat dst;
		filter2D(src, dst, -1, emboss, Point(-1, -1), 128);

		bmpHistory.push_back(dst);
		BITMAPINFO* bmpinfo = CreateBitmapInfo(bmpHistory.back().cols, bmpHistory.back().rows, bmpHistory.back().channels() * 8);
		bmpInfoHistory.push_back(bmpinfo);
		if(myfileMode==0){//사진 모드 
			DrawImage(bmpHistory.back(), bmpInfoHistory.back());
		}
	
	}else{
		MessageBox(L"이 사진은 채널 1개인 이미지 입니다\n이 기능은 채널 3개 이미지만 처리합니다", L"알림", IDOK);
	}
}

//컬러(채널3)를 그레이(채널1)로 변경 
Mat CFilterDlg::colorToGray()
{
	// TODO: Add your implementation code here.
	//현재 채널 정보를 확인해서 gray가 아닐 경우
	
	Mat dst; 
	cvtColor(bmpHistory.back(), dst, COLOR_BGR2GRAY);
	return dst; 
	
}


//BITMAP 정보 구조체 데이터 생성 
BITMAPINFO* CFilterDlg::CreateBitmapInfo(int w, int h, int bpp) {
	BITMAPINFO* bmpinfo = NULL;

	if (bpp == 8) //1채널 
		bmpinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		bmpinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	(bmpinfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	(bmpinfo)->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	(bmpinfo)->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	(bmpinfo)->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	(bmpinfo)->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	(bmpinfo)->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	(bmpinfo)->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	(bmpinfo)->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	(bmpinfo)->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.
	(bmpinfo)->bmiHeader.biWidth = w;
	(bmpinfo)->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			(bmpinfo)->bmiColors[i].rgbBlue = (BYTE)i;
			(bmpinfo)->bmiColors[i].rgbGreen = (BYTE)i;
			(bmpinfo)->bmiColors[i].rgbRed = (BYTE)i;
			(bmpinfo)->bmiColors[i].rgbReserved = 0;
		}
	}
	return bmpinfo;
}

//#되돌리기함수 기능 (원본 이미지로 돌아감) 
void CFilterDlg::OnBnClickedRevertFt()
{
	// TODO: Add your control notification handler code here
	if (videoMode[0] > 0) {//비디오 모드에서 필터가 적용중일 때 초기화
		videoMode[0] = 0;
		videoMode[1] = 0;
		partBlurSlider.SetPos(1);
		fogslider_FT.SetPos(0);
		sharpSliderFT.SetPos(0);
		noiseFT.SetPos(0);
		MessageBox(L"필터링 제거했습니다\n 원본 영상입니다", L"알림", MB_OK);

	}
	else {//사진 필터링 적용 중 
		if (bmpHistory.size() > 1) {
			bmpHistory.pop_back(); bmpInfoHistory.pop_back();

			DrawImage(bmpHistory.back(), bmpInfoHistory.back());
			partBlurSlider.SetPos(1);
			fogslider_FT.SetPos(0);
			sharpSliderFT.SetPos(0);
			noiseFT.SetPos(0);
			MessageBox(L"이전으로 돌아갑니다", L"알림", MB_OK);
		}
		else {
			MessageBox(L"더이상 되돌아갈 이미지가 없고\n원본이미지만 남았습니다", L"알림", MB_OK);
		}
	
	}
	
}

//필터 창을 종료하고, 부모창 사진에는 변경이 안되어 있어야함. 
void CFilterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	KillTimer(2);//비디오출력
	CDialogEx::OnCancel();
}


//void CFilterDlg::OnNMCustomdrawSliderFogFt(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
//	int sigma = fogslider_FT.GetPos();
//
//	GaussianBlur(myImg, myImgAfterChange, Size(), (double)sigma);
//	CreateBitmapInfo(&myBmpInfoAfterChange, myImgAfterChange.cols, myImgAfterChange.rows, myImgAfterChange.channels() * 8);
//	DrawImage(myImgAfterChange, myBmpInfoAfterChange);
//	*pResult = 0;
//}


//스크롤 트랙바 조절에 따른 처리 기능 
//각 필터의 강도 조절 
void CFilterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (*pScrollBar == fogslider_FT) {//안개필터 
		int sigma = fogslider_FT.GetPos();
		cntScroll++;
		if(sigma ==0 || cntScroll==2){
			cntScroll = 0;
			return;
		}
		sharpSliderFT.SetPos(0);
		if(myfileMode==0){//사진모드
			int ret = fogFilter(sigma); 
			if(ret != 1){
				//안개필터 적용 안됨. 
				MessageBox(L"안개필터적용 에러", L"알림", IDOK);	
			}
		}else{//비디오모드
			videoMode[0] = { FOGVM };
			videoMode[1] = sigma;
		}
		
	}
	else if (*pScrollBar == sharpSliderFT) {//샤프닝필터
		int sigma = sharpSliderFT.GetPos();
		cntScroll++;
		if(sigma ==0 || cntScroll==2){
			cntScroll = 0;
			return;
		}
		fogslider_FT.SetPos(0);
		int ret = sharpFilter(sigma); 
		if(ret != 1){
			//샤프닝필터 적용 안됨. 
			MessageBox(L"안개필터적용 에러", L"알림", IDOK);	
		}
	}
	else if (*pScrollBar == noiseFT) {//노이즈필터
		int stddev = noiseFT.GetPos();
		cntScroll++;
		if(stddev ==0 || cntScroll==2){
			cntScroll = 0;
			return;
		}
		int ret = noiseFilter(stddev); 
		if(ret < 1){
			//샤프닝필터 적용 안됨. 
			MessageBox(L"안개필터적용 에러", L"알림", IDOK);	
		}
	}
	else if (*pScrollBar == partBlurSlider) {//부분블러함수에서 블러범위지정할때 사용
		blurRangeHalfWid = partBlurSlider.GetPos();
	}

	//CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//양방향필터 
void CFilterDlg::OnBnClickedBilateralFt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (bmpHistory.back().channels() >=3) {

		Mat src = colorToGray();
		Mat dst;
		bilateralFilter(src, dst, -1, 10, 5); // -1 sigmaSpace로부터 자동생성됨. 10: 색공간에서의 가우시안 표준 편차 5: 좌표 공간에서의 가우시안 표준편차 
		
		bmpHistory.push_back(dst);
		BITMAPINFO* bmpinfo = CreateBitmapInfo(bmpHistory.back().cols, bmpHistory.back().rows, bmpHistory.back().channels() * 8);
		bmpInfoHistory.push_back(bmpinfo);
		if(myfileMode==0){//사진 모드 
			DrawImage(bmpHistory.back(), bmpInfoHistory.back());
		}
	}else{
		MessageBox(L"이 사진은 채널 1개인 이미지 입니다\n이 기능은 채널3개 이미지만 처리합니다", L"알림", IDOK);
	}
}


void CFilterDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//if (partBlurModeOn) {
	//	
	//	// TODO: Add your message handler code here
	//	circlePos.x += point.x - prev_mousePos.x;
	//	circlePos.y += point.y - prev_mousePos.y;
	//	prev_mousePos = point;
	//	Invalidate();
	//}

	CDialogEx::OnMouseMove(nFlags, point);
}


//void CFilterDlg::OnPaint()
//{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//}
	//else
	//{
	//	CPaintDC dc(this);
	//	CRect wnd;
	//	this->GetWindowRect(wnd);// right:창의 너비 bottm: 창의 높이 
	//	int xloc = int(wnd.right * 5 / 6);
	//	int backImgWid = int(wnd.right * 1 / 6);
	//	dc.FillSolidRect(xloc, 0, backImgWid, wnd.bottom, RGB(40, 56, 84));//35, 47, 69
	//	//CDialogEx::OnPaint();
	//}
//}

//#부분블러버튼
void CFilterDlg::OnBnClickedPartblurFt()
{
	//// TODO: Add your control notification handler code here
	if (bmpHistory.back().channels() >=3) {
		partBlurModeOn = !partBlurModeOn;
	}else{
		MessageBox(L"이 사진은 채널 1개인 이미지 입니다\n이 기능은 채널3개 이미지만 처리합니다", L"사용불가 알림", IDOK);
	}
}



//버튼컬러 구현 
void CFilterDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDCtl) {
		case IDC_PARTBLUR_FT:
		{
			if (lpDrawItemStruct->itemAction & 0x07) {
				CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
				if (lpDrawItemStruct->itemState && ODS_SELECTED && partBlurModeOn == false) {//버튼 클릭시 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71), RGB(42, 52, 71));//버튼 외곽선
					p_dc->SetTextColor(RGB(171, 182, 199));
				}
				else {//기본 상태  //&lpDrawItemStruct->rcItem 버튼의 크기
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105), RGB(60, 75, 105));//버튼 외곽선
					p_dc->SetTextColor(RGB(140, 147, 161));
				}

				p_dc->SetBkMode(TRANSPARENT);
			}break;
		}
		case IDC_EMBOSS_FT: case IDC_BILATERAL_FT:
		case IDC_REVERT_FT:
		case IDOK: case IDCANCEL: {
			if (lpDrawItemStruct->itemAction & 0x07) {
				CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
				if (lpDrawItemStruct->itemState && ODS_SELECTED) {//버튼 클릭시 
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71), RGB(42, 52, 71));//버튼 외곽선
					p_dc->SetTextColor(RGB(171, 182, 199));
				}
				else {//기본 상태  //&lpDrawItemStruct->rcItem 버튼의 크기
					p_dc->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(42, 52, 71));//버튼의 색상
					p_dc->Draw3dRect(&lpDrawItemStruct->rcItem, RGB(60, 75, 105), RGB(60, 75, 105));//버튼 외곽선
					p_dc->SetTextColor(RGB(140, 147, 161)); 
					
				}

				p_dc->SetBkMode(TRANSPARENT);
			}break;
		}
		default: break;
	}
	CDC* p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	switch (nIDCtl) {
	case IDC_EMBOSS_FT: {
		p_dc->DrawText(L"진흙필터", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDC_BILATERAL_FT: {
		p_dc->DrawText(L"양방향필터", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		break;
	}
	case IDC_PARTBLUR_FT: {
		if (partBlurModeOn ==true) {
			p_dc->DrawText(L"부분블러ON", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
		else {
			p_dc->DrawText(L"부분블러OFF", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		}
	}
	case IDC_REVERT_FT: {
		p_dc->DrawText(L"되돌리기", -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
	//CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//#텍스트컬러
HBRUSH CFilterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	int staticLb = pWnd->GetDlgCtrlID(); 
	switch (staticLb) {//라벨의 텍스트 컬러를 변경합니다 
		case IDC_FOGLB_FT: case IDC_SHARP_FT:
		case IDC_NOISELB_FT: case IDC_REVERT_FT: 
		case IDC_STATIC_POINTLOC:
		pDC->SetTextColor(RGB(171, 182, 199));
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//마우스 왼쪽 버튼 클릭 
void CFilterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//1. IDC_STATIC_POINTLOC  위치 조정.
	//	1. LBUTTON DOWN >> CPOINT  값을 가져옴
	//	2. IDC_STATIC_POINTLOC >> 현재 위치 출력
	if (partBlurModeOn ==true) {//부분 블러 모드 버튼을 클릭했을 때 
		blurLoc = point;//누른 지점을 저장한다
		CString loc;
		loc.Format(_T("point x: %u y: %u"), point.x, point.y);
		SetDlgItemText(IDC_STATIC_POINTLOC, (LPCTSTR)loc);
		int ret = partBlurProc(point);
		if(ret == 1){//마우스 클릭 위치가 이미지 위치 정보 안에 있을 때. 
			if(myfileMode==0){//사진 모드 
				DrawImage(bmpHistory.back(), bmpInfoHistory.back());
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


//부분블러함수 처리 : 컬러영상에 대한 / 흑백영상은 추후 구현 
//평균값 마스크: 3x3 1/9 필터 
int CFilterDlg::partBlurProc(CPoint point) {
	//1. 마우스 포인터 위치값으로, 이미지상의 실제 위치를 계산한다 
	// picLTRB에는 마지막 draw 했을 때의 rect 정보가 담김
	
	CPoint locInImg;//본인 창에서의 이미지 위치 x= left, y =top 
	locInImg.x = point.x - picLTRB.left;// 
	locInImg.y = point.y - picLTRB.top;

	//mouse 위치 정보가 이미지를 벗어났을 때. 함수 종료. 
	if(locInImg.x <0 || locInImg.y <0){
		return -1;
	}

	//진흙필터 >되돌리기> 부분블러 ON > 에러 
	//range 범위가 벗어낫을 때 에러 
	CRect blurArea;//이미지 공간 안에서의 블러 영역 
	int range = blurRangeHalfWid*50;
	blurArea.left = locInImg.x - range; blurArea.top = locInImg.y - range;
	blurArea.right = locInImg.x + range; blurArea.bottom = locInImg.y + range;
	if (blurArea.left < 0) { blurArea.left = 0; }
	if(blurArea.top <0) { blurArea.top = 0; }
	if (blurArea.right > picLTRB.right- picLTRB.left) { 
		blurArea.right = picLTRB.right - picLTRB.left-1; }
	if (blurArea.bottom > picLTRB.bottom - picLTRB.top) {
		blurArea.bottom = picLTRB.bottom - picLTRB.top-1;	}

	Mat src= bmpHistory.back().clone(); 
	Mat dstimg= bmpHistory.back().clone();

	for (int y = blurArea.top; y <= blurArea.bottom; y++) {
		for (int x = blurArea.left; x <= blurArea.right; x++) {
			if(x-1<1||y-1<1||x+1>blurArea.right||y+1>blurArea.bottom){
				continue;
			}
			Vec3b& p1 = src.at<Vec3b>(y - 1, x - 1);
			Vec3b& p2 = src.at<Vec3b>(y - 1, x);
			Vec3b& p3 = src.at<Vec3b>(y - 1, x + 1);
			Vec3b& p4 = src.at<Vec3b>(y, x - 1);
			Vec3b& p5 = src.at<Vec3b>(y, x);
			Vec3b& p6 = src.at<Vec3b>(y, x + 1);
			Vec3b& p7 = src.at<Vec3b>(y + 1, x - 1);
			Vec3b& p8 = src.at<Vec3b>(y + 1, x);
			Vec3b& p9 = src.at<Vec3b>(y+1, x+1);

			Vec3b& dst = dstimg.at<Vec3b>(y, x);

			//b
			dst[0] = int(cvRound((p1[0] + p2[0] + p3[0] + p4[0] +
				p5[0] + p6[0] + p7[0] + p8[0] + p9[0])/9));
			int b = dst[0];
			//g
			dst[1] = int(cvRound((p1[1] + p2[1] + p3[1] + p4[1] +
				p5[1] + p6[1] + p7[1] + p8[1] + p9[1])/9));
			int g = dst[1];
			//r
			dst[2] = int(cvRound((p1[2] + p2[2] + p3[2] + p4[2] +
				p5[2] + p6[2] + p7[2] + p8[2] + p9[2])/ 9));
			int r = dst[2];
		}
	}


	bmpHistory.push_back(dstimg);
	BITMAPINFO* dstimginfo = bmpInfoHistory.back();
	bmpInfoHistory.push_back(dstimginfo);

	
	return 1;
}


//#fog #안개필터함수 
int CFilterDlg::fogFilter(int sigma){
	sharpSliderFT.SetPos(0);

	Mat dst; 
	GaussianBlur(bmpHistory.back(), dst, Size(), (double)sigma);
	bmpHistory.push_back(dst);
	BITMAPINFO* dstinfo = bmpInfoHistory.back();
	bmpInfoHistory.push_back(dstinfo);

	if(myfileMode==0){//사진 모드 
		DrawImage(bmpHistory.back(), bmpInfoHistory.back());
	}
	return 1;
}

//#샤프 #샤프닝필터함수
int CFilterDlg::sharpFilter(int sigma){
	fogslider_FT.SetPos(0);

	Mat tmp;
	GaussianBlur(bmpHistory.back(), tmp, Size(), (double)sigma);
	float alpha = 1.f;
	Mat dst = (1 + alpha) * bmpHistory.back() - alpha * tmp;
	bmpHistory.push_back(dst);
	BITMAPINFO* dstinfo = bmpInfoHistory.back();
	bmpInfoHistory.push_back(dstinfo);
	if(myfileMode==0){//사진 모드 
		DrawImage(bmpHistory.back(), bmpInfoHistory.back());
	}
	return 1; 
}

int CFilterDlg::noiseFilter(int stddev){
//1채널 이미지, 변환 이미지 유. 
	if(bmpHistory.back().channels() <3){//1채널 이미지 
		//colorToGray();
		Mat noise(bmpHistory.back().size(), CV_32SC1);
		randn(noise, 0, stddev);
		Mat tmp = bmpHistory.back().clone();
		Mat dst;		
		add(tmp, noise, dst, Mat(), CV_8U);
		
		bmpHistory.push_back(dst);
		BITMAPINFO* bmpinfo = bmpInfoHistory.back();
		bmpInfoHistory.push_back(bmpinfo);
		if(myfileMode==0){//사진 모드 
			DrawImage(bmpHistory.back(), bmpInfoHistory.back());
		}
		return 1;
	}else {//3채널이미지 
		Mat noise(bmpHistory.back().size(), CV_32SC3);
		randn(noise, 0, stddev);
		Mat tmp = bmpHistory.back().clone();
		Mat dst;		
		add(tmp, noise, dst, Mat(), CV_8UC3);
		
		bmpHistory.push_back(dst);
		BITMAPINFO* bmpinfo = bmpInfoHistory.back();
		bmpInfoHistory.push_back(bmpinfo);
		if(myfileMode==0){//사진 모드 
			DrawImage(bmpHistory.back(), bmpInfoHistory.back());
		}
		return 3;
	}
}

CRect CFilterDlg::pictureControlSizeSet(){
	//필터창 크기
	CRect wnd;
	this->GetClientRect(&wnd); // 기본 사각형의 x,y 좌표설정이되고 =(0,0) 시작되는함수'GetClientRect' 함수에 
	int wx = int(wnd.right * 5 / 6);
	int wy = wnd.bottom;

	//불러올 사진 cols 가져오기.

	CRect rect;// 이미지를 넣을 사각형 
	if (bmpHistory.back().cols > wx) {
		//cols: 1080 = rows : wid;
		int resize_h = cvRound((wx * bmpHistory.back().rows) / bmpHistory.back().cols);
		int resize_w = wx; //width를 최대크기로 설정 
		if (wy - resize_h < 0) { //width를 맞추니, height가 넘친다 
			resize_w = wy * wx / resize_h;
		}
		int x = cvRound((wx - resize_w) / 2);
		int y = cvRound((wy - resize_h) / 2);
		GetDlgItem(IDC_PC_FT)->MoveWindow(x, y, resize_w, resize_h);
		picLTRB.left = x; picLTRB.top = y;
		picLTRB.right = resize_w; picLTRB.bottom = resize_h;
	}
	else {
		int x = cvRound((wx - bmpHistory.back().cols) / 2);
		int y = cvRound((wy - bmpHistory.back().rows) / 2);
		GetDlgItem(IDC_PC_FT)->MoveWindow(x, y, bmpHistory.back().cols, bmpHistory.back().rows);
		picLTRB.left = x; picLTRB.top = y;
		picLTRB.right = x + bmpHistory.back().cols; picLTRB.bottom = y+ bmpHistory.back().rows;
	}
	return rect;
}

//void CFilterDlg::OnStnClickedStaticPointloc()
//{
//	// TODO: Add your control notification handler code here
//}


void CFilterDlg::videoPrint() {
	//KillTimer(2);
	// int wid = 1280;
	// int hei = 720;
	// picCtrl_FT.MoveWindow(0, 0, wid, hei);

	// // 웹캠 열기
	// capture = new VideoCapture(0, CAP_DSHOW);
	// if (!capture->isOpened())
	// {
	// 	MessageBox(_T("웹캠을 열 수 없습니다."));
	// 	return;
	// }

	// // 웹캠 크기 설정
	// capture->set(CAP_PROP_FRAME_WIDTH, wid);
	// capture->set(CAP_PROP_FRAME_HEIGHT, hei);

	// // 타이머 설정
	// //SetTimer(1, 30, NULL);

	// capture->read(mat_frame);

	// //이곳에 OpenCV 함수들을 적용합니다.
	// //여기에서는 그레이스케일 이미지로 변환합니다.
	// //cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);

	// //화면에 보여주기 위한 처리입니다.
	// int bpp = 8 * mat_frame.elemSize();
	// assert((bpp == 8 || bpp == 24 || bpp == 32));

	// int padding = 0;
	// //32 bit image is always DWORD aligned because each pixel requires 4 bytes
	// if (bpp < 32)
	// 	padding = 4 - (mat_frame.cols % 4);

	// if (padding == 4)
	// 	padding = 0;

	// int border = 0;
	// //32 bit image is always DWORD aligned because each pixel requires 4 bytes
	// if (bpp < 32)
	// {
	// 	border = 4 - (mat_frame.cols % 4);
	// }

	// Mat mat_temp;
	// if (border > 0 || mat_frame.isContinuous() == false)
	// {
	// 	// Adding needed columns on the right (max 3 px)
	// 	cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	// }
	// else
	// {
	// 	mat_temp = mat_frame;
	// }

	// RECT r;
	// picCtrl_FT.GetClientRect(&r);
	// //r.right = 500;
	// //r.bottom = 500;
	// cv::Size winSize(r.right, r.bottom);

	// cimage_mfc.Create(winSize.width, winSize.height, 24);

	// //CreateBitmapInfo(BITMAPINFO * *bmpinfo, int w, int h, int bpp)

	// BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	// bitInfo->bmiHeader.biBitCount = bpp;
	// bitInfo->bmiHeader.biWidth = mat_temp.cols;
	// bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	// bitInfo->bmiHeader.biPlanes = 1;
	// bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// bitInfo->bmiHeader.biCompression = BI_RGB;
	// bitInfo->bmiHeader.biClrImportant = 0;
	// bitInfo->bmiHeader.biClrUsed = 0;
	// bitInfo->bmiHeader.biSizeImage = 0;
	// bitInfo->bmiHeader.biXPelsPerMeter = 0;
	// bitInfo->bmiHeader.biYPelsPerMeter = 0;

	// //그레이스케일 인경우 팔레트가 필요
	// if (bpp == 8)
	// {
	// 	RGBQUAD* palette = bitInfo->bmiColors;
	// 	for (int i = 0; i < 256; i++)
	// 	{
	// 		palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
	// 		palette[i].rgbReserved = 0;
	// 	}
	// }

	// // Image is bigger or smaller than into destination rectangle
	// // we use stretch in full rect

	// if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	// {
	// 	// source and destination have same size
	// 	// transfer memory block
	// 	// NOTE: the padding border will be shown here. Anyway it will be max 3px width

	// 	SetDIBitsToDevice(cimage_mfc.GetDC(),
	// 		//destination rectangle
	// 		0, 0, winSize.width, winSize.height,
	// 		0, 0, 0, mat_temp.rows,
	// 		mat_temp.data, bitInfo, DIB_RGB_COLORS);
	// }
	// else
	// {
	// 	// destination rectangle
	// 	int destx = 0, desty = 0;
	// 	int destw = winSize.width;
	// 	int desth = winSize.height;

	// 	// rectangle defined on source bitmap
	// 	// using imgWidth instead of mat_temp.cols will ignore the padding border
	// 	int imgx = 0, imgy = 0;
	// 	int imgWidth = mat_temp.cols - border;
	// 	int imgHeight = mat_temp.rows;

	// 	StretchDIBits(cimage_mfc.GetDC(),
	// 		destx, desty, destw, desth,
	// 		imgx, imgy, imgWidth, imgHeight,
	// 		mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	// }

	// HDC dc = ::GetDC(picCtrl_FT.m_hWnd);
	// cimage_mfc.BitBlt(dc, 0, 0);

	// ::ReleaseDC(picCtrl_FT.m_hWnd, dc);

	// cimage_mfc.ReleaseDC();
	// cimage_mfc.Destroy();
}

