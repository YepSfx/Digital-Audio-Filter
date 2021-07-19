// DrawingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Drawing.h"
#include "DrawingDlg.h"
#include "aboutBox.h"

#include <stdio.h>

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingDlg dialog

CDrawingDlg::CDrawingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawingDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingDlg)
	DDX_Control(pDX, IDC_LIST1, m_coeff);
	DDX_Control(pDX, IDC_ZDAMP,			m_bqzdamp);
	DDX_Control(pDX, IDC_PDAMP,			m_bqpdamp);
	DDX_Control(pDX, IDC_BQANT,			m_bqanti);
	DDX_Control(pDX, IDC_BQRES,			m_bqreson);
	DDX_Control(pDX, IDC_BQGAIN,		m_bqgain);
	DDX_Control(pDX, IDC_GAINA,			m_gaina);
	DDX_Control(pDX, IDC_GAINB,			m_gainb);
	DDX_Control(pDX, IDC_FILLEN,		m_len);
	DDX_Control(pDX, IDC_FREQ4,			m_freq4);
	DDX_Control(pDX, IDC_FREQ3,			m_freq3);
	DDX_Control(pDX, IDC_HIFREQ,		m_freqhi);
	DDX_Control(pDX, IDC_LOFREQ,		m_freqlo);
	DDX_Control(pDX, IDC_SELFIL,		m_fil);
	DDX_Control(pDX, IDC_SELWIN,		m_win);
	DDX_Control(pDX, IDC_DATA,			m_data);
	DDX_Control(pDX, IDC_EDIT1,			m_numbuff);
	DDX_Control(pDX, IDC_READSIZE,		m_Read);
	DDX_Control(pDX, IDC_LENGTH,		m_LenSpinCtrl);
	DDX_Control(pDX, IDC_SCROLLTIME,	m_TimeWinScroll);
	DDX_Control(pDX, IDC_STEREO,		m_Ch);
	DDX_Control(pDX, IDC_FILEPOS,		m_Pos);
	DDX_Control(pDX, IDC_SAMPLE,		m_Sam);
	DDX_Control(pDX, IDC_RES,			m_Res);
	DDX_Control(pDX, IDC_BUTTON1,		m_FileOpen);
	DDX_Control(pDX, IDC_RADIOR,		m_RadioR);
	DDX_Control(pDX, IDC_RADIOL,		m_RadioL);
	DDX_Control(pDX, IDC_RADIOMAG,		m_RadioMag);
	DDX_Control(pDX, IDC_RADIOPHA,		m_RadioPha);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrawingDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawingDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1,		OnButton1)
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIOL,		OnRadioLClick)
	ON_BN_CLICKED(IDC_RADIOR,		OnRadioRClick)
	ON_BN_CLICKED(IDC_BUTTON3,		OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2,		OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4,		OnButton4)
	ON_BN_CLICKED(IDC_RADIOMAG,		OnRadiomag)
	ON_BN_CLICKED(IDC_RADIOPHA,		OnRadiopha)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5,		OnButton5)
	ON_CBN_SELCHANGE(IDC_SELWIN,	OnSelchangeSelwin)
	ON_CBN_SELCHANGE(IDC_SELFIL,	OnSelchangeSelfil)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingDlg message handlers

BOOL CDrawingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	WAVEDATA*	pData;
	STRING		tmp;
	int			i;
	CRect		rect;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CWnd *pWnd = GetDlgItem(IDC_FFTWINDOW);
	pWnd->GetWindowRect(&rect);
	
	InitScaling(rect);

	for (i = 0 ; i < DEF_FILTER_NUMBER ; i++)
	{
		m_fil.AddString(FILTERTYPE[i]);
	}
	m_fil.SetCurSel(0);

	for (i = 0 ; i < DEF_WIN_NUMBER ; i++)
	{
		m_win.AddString(WINTYPE[i]);
	}
	m_win.SetCurSel(0);
	
	m_freqlo.SetWindowText("500.00");
	m_freqhi.SetWindowText("2000.00");
	m_freq3.SetWindowText("3000.00");
	m_freq4.SetWindowText("4000.00");
	m_gaina.SetWindowText("1.00");
	m_gainb.SetWindowText("1.00");

	m_bqgain.SetWindowText("0.10");
	m_bqreson.SetWindowText("3000.00");
	m_bqanti.SetWindowText("5000.00");
	m_bqpdamp.SetWindowText("0.99");
	m_bqzdamp.SetWindowText("1.00");

	m_pMovingFFTL	= NULL;
	m_pMovingFFTR	= NULL;
	m_coeff.ResetContent();

	InitAudioDev(this);

	this->SetWindowText(DEF_APP_TITLE);

	m_ChSelect = DEF_CH_LEFT;
	m_FFTtype  = DEF_FFT_MAG;	

	m_RadioL.SetCheck(1);
	m_RadioMag.SetCheck(1);

	m_LenSpinCtrl.SetRange(0,DEF_FFTNUM);

	m_TimeWinScroll.SetScrollRange(0,255);
	m_FileSize = 256;

	pData			  = (WAVEDATA*)malloc(sizeof(WAVEDATA));
	pData->pBuffer	  = NULL;
	pData->pFFTBufferL= NULL;
	pData->pFFTBufferR= NULL;

	m_pFFTData = pData;

	m_FFTsize = 1; 
	m_FileIdx = 0;

	sprintf(tmp,"%d",DEF_FILTER_LENGTH);
	m_len.SetWindowText(tmp);

	m_numbuff.SetWindowText("32");

	m_LenSpinCtrl.SetPos(3);
	OnLenSpinCtrl(3);

	m_TimerProceed	= false;
	m_MemAccess		= false;
	m_FileLoad		= false;
	m_MakingFilter	= false;
	
	InitDelay();
	InitFFTWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrawingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		UpdateWavWindow();	
		UpdateFFTWindow();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDrawingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDrawingDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
	STRING			tmp, tmp2;
	WAVEFORMATEX	fmt;
	
	OnButton3();
	DoEvent();

	CFileDialog OpenFile(TRUE, "*.wav", NULL, OFN_READONLY, "Wave Audio(*.wav)|*.wav|All Files(*.*)|*.*||", this);

	strcpy(tmp ,"\n");
	strcpy(tmp2,"\n");

	if (OpenFile.DoModal() == IDOK)
	{
		ClosePort();	
		CloseAudioFile();
		DoEvent();

		m_FileLoad = true;

		strcpy(tmp, OpenFile.GetPathName());
		if (SetAudioFile(tmp) == true)
			DoEvent();
		else{
			MessageBox("Cannot Open Wave File",NULL,MB_OK);
			CloseAudioFile();
			DoEvent();
			m_TimeWinScroll.SetScrollPos(0);
			ClearDisplay();
			return;
		}

		fmt = GetWaveFormat();

		if (fmt.wFormatTag != WAVE_FORMAT_PCM)
		{
			MessageBox("None Windows PCM Audio Format!",NULL,MB_OK);
			CloseAudioFile();
			DoEvent();
			m_TimeWinScroll.SetScrollPos(0);
			ClearDisplay();
			return;
		}
		
		if (fmt.wBitsPerSample > 16)
		{
			MessageBox("Too High Resolution! (Maximun 16 Bits)",NULL,MB_OK);
			CloseAudioFile();
			DoEvent();
			m_TimeWinScroll.SetScrollPos(0);
			ClearDisplay();
			return;
		}
		
		if (fmt.nSamplesPerSec > 44100)
		{
			MessageBox("Too High Sample Rate! (Maximum 44100 Hz)",NULL,MB_OK);
			CloseAudioFile();
			DoEvent();
			m_TimeWinScroll.SetScrollPos(0);
			ClearDisplay();
			return;
		}

		strcpy(tmp, OpenFile.GetFileName());
		sprintf(tmp2,"%s - %s", DEF_APP_TITLE, tmp);	
		this->SetWindowText(tmp2);
		
		m_FileSize = GetWaveLength();

		sprintf(tmp,"# of CHs: %d\n", (INT16)fmt.nChannels); 
		m_Ch.SetWindowText(tmp);
		m_RadioL.SetCheck(1);
		m_RadioR.SetCheck(0);
		m_ChSelect = DEF_CH_LEFT;

		m_RadioMag.SetCheck(1);
		m_RadioPha.SetCheck(0);
		m_FFTtype = DEF_FFT_MAG;

		sprintf(tmp,"Sample Rate: %d\n", fmt.nSamplesPerSec);
		m_Sam.SetWindowText(tmp);

		sprintf(tmp,"Resolutions: %d Bits\n", (INT16)fmt.wBitsPerSample);
		m_Res.SetWindowText(tmp);
		

		sprintf(tmp,"Sample Position: 0 / %d\n", (m_FileSize - 1));
		m_Pos.SetWindowText(tmp);

		m_TimeWinScroll.SetScrollRange(0, (m_FileSize - 1));

		m_FileLoad = false;

		m_TimeWinScroll.SetScrollPos(0);
		OnTimeWinScroll(0);
		SetCoeff();
		return;
	}	
	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	
	KillTimer(m_TimerID);
	DoEvent();

	free(((WAVEDATA*)m_pFFTData)->pBuffer);
	free(((WAVEDATA*)m_pFFTData)->pFFTBufferL);
	free(((WAVEDATA*)m_pFFTData)->pFFTBufferR);	
	free((WAVEDATA*)m_pFFTData);
	free(m_pMovingFFTL);
	free(m_pMovingFFTR);
	
	InitFilter();
	DoEvent();
	InitDelay();
	DoEvent();
	InitFFTWindow();
	DoEvent();
	CloseAudioDev();
	DoEvent();
}

void CDrawingDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if ((CScrollBar*)&m_LenSpinCtrl == pScrollBar)
	{
		if (isPlaying() == true)
		{	
			m_LenSpinCtrl.SetPos(m_FFTsize);
			return;
		}
		else{
			if (isPortClose() == false)
			{
				m_LenSpinCtrl.SetPos(m_FFTsize);
				return;
			}
		}
		OnLenSpinCtrl(nPos);
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDrawingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	LONGINT CurPos;
	int		MinPos, MaxPos, Multiple;
	SCROLLINFO info;
	WAVEFORMATEX fmt;

	if ((CScrollBar*)&m_TimeWinScroll == pScrollBar)
	{

		CurPos = m_TimeWinScroll.GetScrollPos();
		m_TimeWinScroll.GetScrollRange(&MinPos, &MaxPos);
		fmt = GetWaveFormat();

		switch(nSBCode)
		{
			
			case SB_THUMBPOSITION:
				break;
			case SB_THUMBTRACK:
				ZeroMemory(&info, sizeof(info));
				info.cbSize = sizeof(info);
				info.fMask  = SIF_TRACKPOS;
				m_TimeWinScroll.GetScrollInfo(&info, SIF_TRACKPOS);
				Multiple = info.nTrackPos / GetFFTByte(m_FFTsize, fmt);
				CurPos   = GetFFTByte(m_FFTsize, fmt) * Multiple;
				break;
			case SB_LEFT:
				CurPos = MinPos;
				break;
			case SB_RIGHT:
				CurPos = MaxPos;
				break;
			case SB_LINELEFT:
				switch(fmt.nChannels){
					case 1:
						switch(fmt.wBitsPerSample){
							case 8:
								if (CurPos > MinPos)
										CurPos -= 1;
								if (CurPos <= MinPos)
										CurPos = MinPos;
								break;
							case 16:
								if (CurPos > MinPos)
										CurPos -= 2;
								if (CurPos <= MinPos)
										CurPos = MinPos;
								break;
						}
						break;
					case 2:
						switch(fmt.wBitsPerSample){
							case 8:
								if (CurPos > MinPos)
										CurPos -= 2;
								if (CurPos <= MinPos)
										CurPos = MinPos;
								break;
							case 16:
								if (CurPos > MinPos)
										CurPos -= 4;
								if (CurPos <= MinPos)
										CurPos = MinPos;
								break;
						}
						break;
				}
				break;
			case SB_LINERIGHT:
				switch(fmt.nChannels){
					case 1:
						switch(fmt.wBitsPerSample){
							case 8:
								if (CurPos < MaxPos)
										CurPos += 1;
								if (CurPos >= MaxPos)
										CurPos = MaxPos;
								break;
							case 16:
								if (CurPos < MaxPos)
										CurPos += 2;
								if (CurPos >= MaxPos)
										CurPos = MaxPos;
								break;
						}
						break;
					case 2:
						switch(fmt.wBitsPerSample){
							case 8:
								if (CurPos < MaxPos)
										CurPos += 2;
								if (CurPos >= MaxPos)
										CurPos = MaxPos;
								break;
							case 16:
								if (CurPos < MaxPos)
										CurPos += 4;
								if (CurPos >= MaxPos)
										CurPos = MaxPos;
								break;
						}
						break;
				}
				break;
			case SB_PAGELEFT:   
				if (CurPos > MinPos)
							CurPos -= GetFFTByte(m_FFTsize,fmt);
				if (CurPos <= MinPos)
					        CurPos = MinPos;
				break;
			case SB_PAGERIGHT:
				if (CurPos < MaxPos)
							CurPos += GetFFTByte(m_FFTsize,fmt);
				if (CurPos >= MaxPos)
							CurPos = MaxPos;
				break;
			case SB_ENDSCROLL:
				break;
		}

		m_TimeWinScroll.SetScrollPos(CurPos);
		
		OnTimeWinScroll(CurPos);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDrawingDlg::OnRadioLClick() 
{
	// TODO: Add your control notification handler code here
	m_ChSelect = DEF_CH_LEFT;	
	m_RadioR.SetCheck(0);
	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnRadioRClick() 
{
	// TODO: Add your control notification handler code here
	WAVEFORMATEX fmt;

	fmt = GetWaveFormat();

	if (fmt.nChannels == 1)
	{	
		m_ChSelect = DEF_CH_LEFT;
		m_RadioL.SetCheck(1);		
		m_RadioR.SetCheck(0);		
		return;
	}
	
	m_RadioL.SetCheck(0);			
	m_ChSelect = DEF_CH_RIGHT;		
	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnTimeWinScroll(LONGINT nPos)
{
	STRING			tmp;
	int				Len,nbyte; 
	LONGINT			FilterLen, pos;

	WAVEFORMATEX	fmt;
	WAVEBUFFER		*pBuffer;
	WAVEDATA		*pData;
	FFTBUFFER		*pFFTBufferL, *pFFTBufferR;

	if (isFileRead() != true)
		return;

	if (isPlaying() == true)
		return;
	
	SetFilter();
	DoEvent();

	fmt = GetWaveFormat();
	nPos = GetDataPosition(fmt, nPos);

	switch(fmt.wBitsPerSample){
		case 8:  nbyte = 1;
			break;
		case 16: nbyte = 2;
			break;
	}
	
	if (nPos > (m_FileSize-1))
		sprintf(tmp,"Sample Position:  %d / %d\n", nPos/(fmt.nChannels * nbyte) - 1, m_FileSize/(fmt.nChannels*nbyte) - 1);	
	else 
		sprintf(tmp,"Sample Position:  %d / %d\n", nPos/(fmt.nChannels * nbyte), m_FileSize/(fmt.nChannels * nbyte) - 1 );

	m_Pos.SetWindowText(tmp);
	m_TimeWinScroll.SetScrollPos(nPos);	
	m_FileIdx = nPos;

	pData = (WAVEDATA*)m_pFFTData;

	free(pData->pBuffer);
	free(pData->pFFTBufferL);
	free(pData->pFFTBufferR);
	free(pData);

	m_pFFTData = NULL;

	pBuffer		= (WAVEBUFFER*)calloc(GetFFTByte(m_FFTsize,fmt), sizeof(WAVEBUFFER));
	pData		= (WAVEDATA  *)malloc(sizeof(WAVEDATA));

	pFFTBufferL	= (FFTBUFFER *)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));
	pFFTBufferR	= (FFTBUFFER *)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));	

	FilterLen = GetFFTByte(m_FFTsize,fmt);
	if (GetFilterStatus()==true)
	{	
		pos = nPos;
		if ((pos - FilterLen) > 0)
		{
			SetFilePos(nPos - FilterLen);
			ReadFile(pBuffer,GetFFTByte(m_FFTsize,fmt), GetFilterStatus());
		}else{
			SetFilePos(0);
			ReadFile(pBuffer,GetFFTByte(m_FFTsize,fmt), GetFilterStatus());
		}
	}

	SetFilePos(nPos);
	Len = ReadFile(pBuffer,GetFFTByte(m_FFTsize,fmt), GetFilterStatus());

	pData->fmt			= fmt;
	pData->pBuffer		= pBuffer;
	pData->BufferSize	= GetFFTByte(m_FFTsize,fmt);
	pData->DataLen		= Len; 
	pData->CurPos		= nPos;
	pData->fftSize		= FFTSIZE[m_FFTsize];
	
	DoFFT(pFFTBufferL, pFFTBufferR, pData);
	
	pData->pFFTBufferL   = pFFTBufferL;
	pData->pFFTBufferR	 = pFFTBufferR;
	
	m_pFFTData	= pData;

	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnLenSpinCtrl(UINT nPos)
{
	WAVEFORMATEX	fmt;
	STRING			tmp;
	int				size;
	int				num;
		
	size = FFTSIZE[nPos];

	m_FFTsize = nPos;

	num = BUFFNUM[m_FFTsize];
	m_buffs = num;

	sprintf(tmp,"%d\n",size);
	m_Read.SetWindowText(tmp);	

	sprintf(tmp,"%d",num);
	m_numbuff.SetWindowText(tmp);

	fmt = GetWaveFormat();
	
	OnTimeWinScroll(m_FileIdx);
}

void CDrawingDlg::UpdateWavWindow()
{
	CRect			rect;
	WAVEDATA		*data;
	COLORREF		Color;	

	CDC*  pDC		= GetDC();
	CWnd* pWndTime	= GetDlgItem(IDC_TIMEWINDOW);

	switch(m_ChSelect){
		case DEF_CH_LEFT : Color = DEF_COLOR_LEFT;
			break;
		case DEF_CH_RIGHT: Color = DEF_COLOR_RIGHT;
			break;
	}

	pWndTime->GetWindowRect(&rect);
	ScreenToClient(&rect);
	DrawWaveWindow(pDC, rect, Color);
	if (m_pFFTData == NULL)
	{
		ReleaseDC(pDC);
		return;
	}
	data = (WAVEDATA*)m_pFFTData;
	DrawWave(pDC, rect, Color, data, m_ChSelect);
	
	ReleaseDC(pDC);
}

void CDrawingDlg::UpdateFFTWindow()
{
	CRect			rect;
	WAVEDATA		*data;
	COLORREF		Color;	

	CDC*  pDC		= GetDC();
	CWnd* pWndFFT	= GetDlgItem(IDC_FFTWINDOW);
	
	switch(m_ChSelect){
		case DEF_CH_LEFT : Color = DEF_COLOR_LEFT;
			break;
		case DEF_CH_RIGHT: Color = DEF_COLOR_RIGHT;
			break;
	}

	pWndFFT->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	switch(m_FFTtype){
		case DEF_FFT_MAG:
			DrawFFTWindow(pDC, rect, Color);
			break;
		case DEF_FFT_PHASE:
			DrawPhaseWindow(pDC, rect, Color);
			break;
	}
	
	if (m_pFFTData == NULL)
	{
		ReleaseDC(pDC);
		return;
	}
	if (m_MemAccess == true)
	{
		ReleaseDC(pDC);
		return;
	}

	data = (WAVEDATA*)m_pFFTData;

	switch(m_FFTtype){
		case DEF_FFT_MAG:
			DrawFFT(pDC, rect, Color, data, m_ChSelect);
			break;
		case DEF_FFT_PHASE:
			DrawPhase(pDC, rect, Color, data, m_ChSelect);
			break;
	}
	ReleaseDC(pDC);
}

void CDrawingDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	WAVEFORMATEX	fmt;

	if (isPlaying() == true)
		return;
	
	if (isFileRead() != true)
		return;
	
	fmt = GetWaveFormat();
	OpenPort(fmt);

	SetNumBuffer(m_buffs);
	SetFFTSize(m_FFTsize);

	m_TimerProceed	= false;
	m_MemAccess		= false;
	
	SetFilter();
	
	SetPlay(m_FileIdx, GetFFTByte(m_FFTsize,fmt));

	m_TimerID = SetTimer(1, DEF_TIMER_UPDATE, NULL);
}

void CDrawingDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	STRING tmp;

	if (isPlaying() == true)
		return;

	m_numbuff.GetWindowText(tmp, sizeof(tmp));

	m_buffs = atoi(tmp);

	SetNumBuffer(m_buffs);
}

void CDrawingDlg::OnRadiomag() 
{
	// TODO: Add your control notification handler code here
	m_FFTtype = DEF_FFT_MAG;	
	m_RadioPha.SetCheck(0);
	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnRadiopha() 
{
	// TODO: Add your control notification handler code here
	m_FFTtype = DEF_FFT_PHASE;		
	m_RadioMag.SetCheck(0);			
	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::MouseMove(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	STRING			tmp;
	CPoint			Loc;
	CRect			rect;
	WINDOWFFTDATA	data;
	WAVEDATA		*pData, *pMData;;
	int				x,y;

	Loc = point;

	CWnd *pWnd = GetDlgItem(IDC_FFTWINDOW);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	if ((Loc.y >= rect.top)&&(Loc.y <= rect.bottom))
	{
		if ((Loc.x >= rect.left)&&(Loc.x <= rect.right))
		{
			if (isFileRead() == false)
				return;

			if (m_FileLoad == true)
				return;

			if (m_MakingFilter == true)
				return;

			if (m_MemAccess == true)
				return;

			x = Loc.x - rect.left;
			y = Loc.y - rect.top;
			
			if (isPlaying() != true)
			{
				pData = (WAVEDATA*)m_pFFTData;
				if ((pData->pBuffer == NULL)||
					(pData->pFFTBufferL == NULL)||
					(pData->pFFTBufferR == NULL))
							return;

				data  =  GetWinFFTData(x, pData, rect,m_ChSelect);
			}else{
				pMData = (WAVEDATA*)malloc(sizeof(WAVEDATA));
				pMData->fmt 		= ((WAVEDATA*)m_pFFTData)->fmt;
				pMData->BufferSize	= ((WAVEDATA*)m_pFFTData)->BufferSize;
				pMData->DataLen		= ((WAVEDATA*)m_pFFTData)->DataLen;
				pMData->fftSize		= ((WAVEDATA*)m_pFFTData)->fftSize; 				
				pMData->CurPos		= ((WAVEDATA*)m_pFFTData)->CurPos;
				pMData->pFFTBufferL	= (FFTBUFFER*)m_pMovingFFTL;
				pMData->pFFTBufferR	= (FFTBUFFER*)m_pMovingFFTR;			
				
				if ((m_pMovingFFTL == NULL)||(m_pMovingFFTR == NULL))
				{
					free(pMData);
					return;
				}
				
				data  =  GetWinFFTData(x, pMData, rect,m_ChSelect);				
				free(pMData);
			}
			
			switch(m_FFTtype)
			{
				case DEF_FFT_MAG:
					sprintf(tmp,"Point Freq Info   %d Hz   %d dB\n", data.CurSam, data.CurMag);
					break;
				case DEF_FFT_PHASE:
					sprintf(tmp,"Point Freq Info   %d Hz   %d deg\n", data.CurSam, data.CurPhas);
					break;
			}
			
			m_data.SetWindowText(tmp);
		}
	}
}

void CDrawingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	MouseMove(point);
	CDialog::OnMouseMove(nFlags, point);
}

void CDrawingDlg::PutFFTData(void* pData)
{
	if (m_TimerProceed == true)
	{
		free(((WAVEDATA*)pData)->pBuffer);
		free(((WAVEDATA*)pData)->pFFTBufferL);
		free(((WAVEDATA*)pData)->pFFTBufferR);	
		free( (WAVEDATA*)pData);
		return;
	}
	
	free(((WAVEDATA*)m_pFFTData)->pBuffer);
	free(((WAVEDATA*)m_pFFTData)->pFFTBufferL);
	free(((WAVEDATA*)m_pFFTData)->pFFTBufferR);	
	free( (WAVEDATA*)m_pFFTData);
	
	m_pFFTData	= pData;
}

void CDrawingDlg::DisplayPlayingFFT()
{
	STRING			tmp;
	int				nbyte;
	LONGINT			nPos;
	WAVEFORMATEX	fmt;

	fmt			= GetWaveFormat();
	m_FileIdx	= ((WAVEDATA*)m_pFFTData)->CurPos;
	nPos		= m_FileIdx;

	m_MemAccess	= true;

	DoFFT( ((WAVEDATA*)m_pFFTData)->pFFTBufferL, 
		   ((WAVEDATA*)m_pFFTData)->pFFTBufferR,
		   ((WAVEDATA*)m_pFFTData) );

	free(m_pMovingFFTL);
	free(m_pMovingFFTR);
	
	m_pMovingFFTL = NULL;
	m_pMovingFFTR = NULL;

	m_pMovingFFTL = (FFTBUFFER*)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));
	m_pMovingFFTR = (FFTBUFFER*)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));	
	
	memcpy(m_pMovingFFTL, ((WAVEDATA*)m_pFFTData)->pFFTBufferL, 
		   FFTSIZE[m_FFTsize] * 2 * sizeof(FFTBUFFER));  
	memcpy(m_pMovingFFTR, ((WAVEDATA*)m_pFFTData)->pFFTBufferR, 
		   FFTSIZE[m_FFTsize] * 2 * sizeof(FFTBUFFER));  
	
	m_MemAccess	= false;

	switch(fmt.wBitsPerSample){
		case 8:  nbyte = 1;
			break;
		case 16: nbyte = 2;
			break;
	}
	
	m_TimeWinScroll.SetScrollPos(nPos);

	if (nPos > (m_FileSize-1))
		sprintf(tmp,"Sample Position:  %d / %d\n", 
		nPos/(fmt.nChannels * nbyte) - 1, m_FileSize/(fmt.nChannels*nbyte) - 1);	
	else 
		sprintf(tmp,"Sample Position:  %d / %d\n", 
		nPos/(fmt.nChannels * nbyte),     m_FileSize/(fmt.nChannels*nbyte) - 1);
	
	m_Pos.SetWindowText(tmp);

	UpdateWavWindow();
	UpdateFFTWindow();
}

void CDrawingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == m_TimerID)
	{
		m_TimerProceed = true;		
		DisplayPlayingFFT();	
		m_TimerProceed = false;		

		if (isEndOfPlaying() == true)
		{
			SetStop();
			
			while( CheckAudioBuffer() != 0)
				DoEvent();

			ClosePort();
			KillTimer(m_TimerID);		
			
			m_TimerProceed	= false;		
			m_MemAccess		= false;

			m_FileIdx = GetFilePos();
			OnTimeWinScroll(m_FileIdx);
		}
	}	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CDrawingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message){
		case WM_TRANS_WAVEBUFFER:
			m_MemAccess = true;
			PutFFTData((WAVEDATA*)lParam);			
			m_MemAccess = false;
			break;
		case WM_PLAY_END:
			SetStop();
			
			while(CheckAudioBuffer() != 0)
				DoEvent();
			
			KillTimer(m_TimerID);
			DoEvent();			
			
			ClosePort();
			
			m_TimerProceed	= false;
			m_MemAccess		= false;

			m_FileIdx = (LONGINT)lParam;
			OnTimeWinScroll(m_FileIdx);
			
			break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDrawingDlg::OnButton3() 
{
	//	TODO: Add your control notification handler code here
	//	Stop Button
	if (isPlaying() == false)
		return;
	SetStop();
	
	while(CheckAudioBuffer() != 0)
		DoEvent();
	
	ClosePort();
	KillTimer(m_TimerID);	
	
	m_TimerProceed	= false;
	m_MemAccess		= false;
	//Update FFT data for display;
	m_FileIdx = GetFilePos();
	OnTimeWinScroll(m_FileIdx);
}

BOOL CDrawingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int nKey;
    switch(pMsg->message){
		case WM_KEYDOWN:
             nKey = (int)pMsg->wParam;
             switch(nKey){
			        case VK_RETURN: //Enter
                    case VK_ESCAPE: // ESC?
                    return TRUE;
             }
            break;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDrawingDlg::SetFilter()
{
	STRING			tmp;
	WAVEFORMATEX	fmt;
	int				win, fil, len; 
	float			frq1,frq2,frq3,frq4, gaina,gainb;
	CRect			rect;
	
	m_MakingFilter	= true;
	fmt = GetWaveFormat();

	win = m_win.GetCurSel();
	fil	= m_fil.GetCurSel();

	if (fil != DEF_FILTER_BIQUAD)
	{
		m_freqlo.GetWindowText(tmp, sizeof(STRING));
		frq1 = (float)atof(tmp);

		m_freqhi.GetWindowText(tmp, sizeof(STRING));
		frq2 = (float)atof(tmp);

		m_freq3.GetWindowText(tmp, sizeof(STRING));
		frq3 = (float)atof(tmp);

		m_freq4.GetWindowText(tmp, sizeof(STRING));
		frq4 = (float)atof(tmp);

		m_len.GetWindowText(tmp, sizeof(STRING));
		len = atoi(tmp);
	
		m_gaina.GetWindowText(tmp, sizeof(STRING));
		gaina = (float)atof(tmp);

		m_gainb.GetWindowText(tmp, sizeof(STRING));
		gainb = (float)atof(tmp);

		SetCutOffFreq1(frq1);
		SetCutOffFreq2(frq2);
		SetCutOffFreq3(frq3);
		SetCutOffFreq4(frq4);
		SetGainA(gaina);
		SetGainB(gainb);
	}else{
		m_bqgain.GetWindowText(tmp, sizeof(STRING));
		gaina	= (float)atof(tmp);

		m_bqreson.GetWindowText(tmp,sizeof(STRING));
		frq1	= (float)atof(tmp);

		m_bqanti.GetWindowText(tmp,sizeof(STRING));
		frq2	= (float)atof(tmp);
		
		m_bqpdamp.GetWindowText(tmp,sizeof(STRING));
		frq3	= (float)atof(tmp);

		m_bqzdamp.GetWindowText(tmp,sizeof(STRING));
		frq4	= (float)atof(tmp);

		SetBiQuadParam(gaina,frq1,frq2,frq3,frq4);
	}

	InitFilter();
	InitFFTWindow();
	InitDelay();
	MakeFFTWindow(win,FFTSIZE[m_FFTsize]);
	if (fil != DEF_FILTER_BIQUAD)
	{
		MakeFIRFilterWindow(fmt, len, win, fil);
	}else{ 
		MakeBiQuadFilter(fmt);
	}
	DoEvent();

	if (GetFilterStatus() != false)
	{	
		if (fil != DEF_FILTER_BIQUAD)
		{
			sprintf(tmp,"%.2f", frq1 = GetCutOffFreq1());
			m_freqlo.SetWindowText(tmp);
			sprintf(tmp,"%.2f", frq2 = GetCutOffFreq2());
			m_freqhi.SetWindowText(tmp);
			sprintf(tmp,"%.2f", frq3 = GetCutOffFreq3());
			m_freq3.SetWindowText(tmp);
			sprintf(tmp,"%.2f", frq4 = GetCutOffFreq4());
			m_freq4.SetWindowText(tmp);
			
		}else{
			GetBiQuadParam(&gaina, &frq1, &frq2, &frq3, &frq4);

			sprintf(tmp, "%.2f", frq1);
			m_bqreson.SetWindowText(tmp);

			sprintf(tmp, "%.2f", frq2);
			m_bqanti.SetWindowText(tmp);
		}
	}
	m_MakingFilter	= false;
}

void CDrawingDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	SetFilter();	

	if (isPlaying() == false)
					OnTimeWinScroll(m_FileIdx);

		SetCoeff();
}

void CDrawingDlg::OnSelchangeSelwin() 
{
	// TODO: Add your control notification handler code here
	SetFilter();		
	
	if (isPlaying() == false)
		OnTimeWinScroll(m_FileIdx);

		SetCoeff();
}

void CDrawingDlg::OnSelchangeSelfil() 
{
	// TODO: Add your control notification handler code here
	int filter;

	SetFilter();	

	filter = m_fil.GetCurSel();

	switch(filter){
		case DEF_FILTER_LOWPASS:
			m_gaina.SetReadOnly(FALSE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(FALSE);
			m_freqhi.SetReadOnly(TRUE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_HIGHPASS:
			m_gaina.SetReadOnly(FALSE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(TRUE);
			m_freqhi.SetReadOnly(FALSE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_BANDPASS:
			m_gaina.SetReadOnly(FALSE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(FALSE);
			m_freqhi.SetReadOnly(FALSE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_BANDSTOP:
			m_gaina.SetReadOnly(TRUE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(FALSE);
			m_freqhi.SetReadOnly(FALSE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_NONE:
		case DEF_FILTER_MULTIBANDPASS:
			m_gaina.SetReadOnly(FALSE);
			m_gainb.SetReadOnly(FALSE);
			m_freqlo.SetReadOnly(FALSE);
			m_freqhi.SetReadOnly(FALSE);
			m_freq3.SetReadOnly(FALSE);
			m_freq4.SetReadOnly(FALSE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_BIQUAD:
			m_bqgain.SetReadOnly(FALSE);
			m_bqreson.SetReadOnly(FALSE);
			m_bqanti.SetReadOnly(FALSE);
			m_bqpdamp.SetReadOnly(FALSE);
			m_bqzdamp.SetReadOnly(FALSE);

			m_gaina.SetReadOnly(TRUE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(TRUE);
			m_freqhi.SetReadOnly(TRUE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);
			break;
		case DEF_FILTER_HILLBERT:
		case DEF_FILTER_TIMEDIFF:
			m_gaina.SetReadOnly(TRUE);
			m_gainb.SetReadOnly(TRUE);
			m_freqlo.SetReadOnly(TRUE);
			m_freqhi.SetReadOnly(TRUE);
			m_freq3.SetReadOnly(TRUE);
			m_freq4.SetReadOnly(TRUE);

			m_bqgain.SetReadOnly(TRUE);
			m_bqreson.SetReadOnly(TRUE);
			m_bqanti.SetReadOnly(TRUE);
			m_bqpdamp.SetReadOnly(TRUE);
			m_bqzdamp.SetReadOnly(TRUE);
			break;
	}

	if (isPlaying() == false)
		OnTimeWinScroll(m_FileIdx);

		SetCoeff();
}

void CDrawingDlg::ClearDisplay()
{
	CRect			rect;
	COLORREF		Color;	
	STRING			tmp;
	WAVEDATA		*pData;
	WAVEBUFFER		*pBuffer;
	FFTBUFFER		*pFFTBufferL, *pFFTBufferR;
	int				Dummy = 128;

	CDC*  pDC	= GetDC();
	CWnd* pWnd	= GetDlgItem(IDC_TIMEWINDOW);

	switch(m_ChSelect){
		case DEF_CH_LEFT : Color = DEF_COLOR_LEFT;
			break;
		case DEF_CH_RIGHT: Color = DEF_COLOR_RIGHT;
			break;
	}

	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	DrawWaveWindow(pDC, rect, Color);

	pWnd= GetDlgItem(IDC_FFTWINDOW);
	
	switch(m_ChSelect){
		case DEF_CH_LEFT : Color = DEF_COLOR_LEFT;
			break;
		case DEF_CH_RIGHT: Color = DEF_COLOR_RIGHT;
			break;
	}

	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	switch(m_FFTtype){
		case DEF_FFT_MAG:
			DrawFFTWindow(pDC, rect, Color);
			break;
		case DEF_FFT_PHASE:
			DrawPhaseWindow(pDC, rect, Color);
			break;
	}
	ReleaseDC(pDC);

	sprintf(tmp,"%s", DEF_APP_TITLE);	
	this->SetWindowText(tmp);

	sprintf(tmp,"Sample Position:  0 / 255");
	m_Pos.SetWindowText(tmp);
	m_TimeWinScroll.SetScrollRange(0,255);

	pData = (WAVEDATA*)m_pFFTData;

	free(pData->pBuffer);
	free(pData->pFFTBufferL);
	free(pData->pFFTBufferR);
	free(pData);

	m_pFFTData = NULL;

	pBuffer		= (WAVEBUFFER*)calloc(Dummy, sizeof(WAVEBUFFER));
	pData		= (WAVEDATA  *)malloc(sizeof(WAVEDATA));
	pFFTBufferL	= (FFTBUFFER *)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));
	pFFTBufferR	= (FFTBUFFER *)calloc(FFTSIZE[m_FFTsize] * 2, sizeof(FFTBUFFER));	

	pData->pBuffer		 = pBuffer;
	pData->BufferSize	 = Dummy;
	pData->DataLen		 = 0; 
	pData->CurPos		 = 0;
	pData->fftSize		 = FFTSIZE[m_FFTsize];
	pData->pFFTBufferL   = pFFTBufferL;
	pData->pFFTBufferR	 = pFFTBufferR;

	m_pFFTData	= pData;

}

void CDrawingDlg::SetCoeff()
{
	int		i, Len, fil;
	float	*pZero, *pTmp = NULL;
	STRING	tmp;
	
	m_coeff.ResetContent();
	
	if (isFileRead() != true)
					return;
	
	fil = m_fil.GetCurSel();
	if ((fil == DEF_FILTER_BIQUAD)||(fil == DEF_FILTER_NONE))
					return;

	while(pTmp == NULL)
			pTmp = GetZeroFilter();
	
	Len = GetZeroFilterLength();
	pZero = (float*)calloc(Len, sizeof(float));
	memcpy(pZero, pTmp, Len*sizeof(float));
	for(i = 0 ; i < Len ; i++)
	{
		sprintf(tmp,"h[%i] = %.6f", i, pZero[i]);
		m_coeff.AddString(tmp);
		DoEvent();
	}
	free(pZero);
}

void CDrawingDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	CAboutBox aboutbox;

	aboutbox.DoModal();
		
}
