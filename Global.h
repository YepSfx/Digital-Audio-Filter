#include "Drawing.h"
#include "DrawingDlg.h"
#include "WaveFile.h"
#include "WaveCtrl.h"

#define		DEF_APP_TITLE		"Digital Audio Filter"

#define		DEF_STR_LEN			30
#define		DEF_FFTNUM			4
#define		DEF_CH_LEFT			1
#define		DEF_CH_RIGHT		2
#define		DEF_FFT_MAG			0
#define		DEF_FFT_PHASE		1
#define 	DEF_COLOR_LEFT		RGB(0  , 255, 0)
#define		DEF_COLOR_RIGHT		RGB(255, 128, 0)
#define		DEF_COLOR_CUT		RGB(255, 255, 0)
#define		DEF_TIMER_UPDATE	40
#define		DEF_WIN_NUMBER		3
#define		DEF_FILTER_NUMBER	7
#define		DEF_FILTER_LENGTH	65

#define		DEF_WIN_HANN		0
#define		DEF_WIN_HAMM		1
#define		DEF_WIN_BLACKMAN	2

#define		DEF_FILTER_NONE				0
#define		DEF_FILTER_LOWPASS			1
#define		DEF_FILTER_HIGHPASS			2
#define		DEF_FILTER_BANDPASS			3
#define		DEF_FILTER_BANDSTOP			4
#define		DEF_FILTER_MULTIBANDPASS	5
#define		DEF_FILTER_BIQUAD			6
#define		DEF_FILTER_HILLBERT			7
#define		DEF_FILTER_TIMEDIFF			8

typedef char				INT8;
typedef unsigned char		UINT8;
typedef unsigned char		BYTE;
typedef short				INT16;
typedef unsigned short		UINT16;
typedef long				LONGINT;

typedef char				STRING[256];
typedef float				FFTBUFFER;
typedef float				FILTERBUFFER;

static int	FFTSIZE[DEF_FFTNUM + 1]				= {128, 256, 512, 1024, 2048};
static int	BUFFNUM[DEF_FFTNUM + 1]				= {100,  80,  60,   40,   20};

static char	WINTYPE[DEF_WIN_NUMBER][DEF_STR_LEN]		= {"von Hann",  
                                                           "Hamming", 
														   "Blackman"};

static char FILTERTYPE[DEF_FILTER_NUMBER][DEF_STR_LEN]	= {"Bypass",
                                                           "Low Pass", 
														   "High Pass", 
														   "Band Pass", 
														   "Band Stop", 
														   "Two Band Pass",
														   "BiQuad"};									   
                         
typedef struct {
			WAVEFORMATEX	fmt;
			WAVEBUFFER*		pBuffer;	//<--Audio Wave Buffer
			FFTBUFFER*		pFFTBufferL;
			FFTBUFFER*		pFFTBufferR;
			int				BufferSize;
			int				DataLen;	//# of Samples to be read into buffer (
			int				fftSize;	//In Stereo, that should be devied by 2
			long			CurPos;		//origianl data read from stream.
			//  WAVEDATA Structure 
			//  Legend * : Data      (in byte)
			//         + : Blank Data(In Sample)
			//	
			//         ***************+++++++++    <--8 bits Model
			//         |<------BufferSize---->|
			//         |<--DataLen-->| 
			//
			//
			//		   *#*#*#*#*#*#*#*#*#++++++	   <--16 bits Model	
			//		   |<-----Buffer Size---->|
			//		   |<-Data Len------>	  |	
			//		   * L Ch Data	
			//         # R Ch Data 
			}WAVEDATA;
typedef WAVEDATA	FFTDATA;

typedef struct{
			int CurSam;	
			int CurMag;
			int CurPhas;
			}WINDOWFFTDATA;

void			DoEvent();
/////////////////////////////////////////////////////////////////////////////
//Visual Part
int				GetFFTByte     (int size, WAVEFORMATEX fmt);
void			DrawFFTWindow  (CDC *pDC, CRect rect, COLORREF Color);
void			DrawWaveWindow (CDC *pDC, CRect rect, COLORREF Color);
void			DrawWave       (CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data, int Ch);
void			DrawFFT        (CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data, int Ch);
void			DrawPhaseWindow(CDC *pDC, CRect rect, COLORREF Color);
void			DrawPhase      (CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data,  int Ch);
void			DoFFT(FFTBUFFER *pFFTBufferL, FFTBUFFER *pFFTBufferR, WAVEDATA *pData);
void			InitScaling(CRect rect);
void			InitFFTWindow();
void			MakeFFTWindow(int wnd, int Len);
//Audio Part
WINDOWFFTDATA	GetWinFFTData(int CurPos, WAVEDATA *pData,CRect rect, int Ch);
WAVEFORMATEX	GetWaveFormat();
LONGINT			GetWaveLength();
LONGINT			GetFilePos();
LONGINT			SetStop();
LONGINT			GetDataPosition(WAVEFORMATEX fmt, LONGINT nPos);
void			OnWaveDataDone(WAVEBUFFER *pBuffer, int Len, CWnd *pParent);
bool			SetAudioFile(char* FileName);
bool			CloseAudioFile();
void			InitAudioDev(CWnd *pParent);
void			CloseAudioDev();
void			SetFilePos(LONGINT nPos);
void			SetNumBuffer(int NumBuffer);
void			SetPlay(LONGINT idx, int DataLen);
void			SetFFTSize(int idx);
void			OpenPort(WAVEFORMATEX fmt);
void			ClosePort();
bool			isEndOfPlaying();
bool			isPortClose();
bool			isPlaying();
bool			isFileRead();
int				GetNumBuffer();
int				ReadFile(WAVEBUFFER *pBuffer, int Len,bool isFiltering);
int				CheckAudioBuffer();

//Filter Part

void			InitFilter();
bool			GetFilterStatus();
int				GetZeroFilterLength();
int				GetPoleFilterLength();
void			MakeFIRFilterWindow(WAVEFORMATEX fmt, int Length, int win, int filter);
void			MakeBiQuadFilter(WAVEFORMATEX fmt);
void			DoFilter(WAVEBUFFER *pBuffer, int DataLen, WAVEFORMATEX fmt);
void			SetCutOffFreq1(float freq);
void			SetCutOffFreq2(float freq);
void			SetCutOffFreq3(float freq);
void			SetCutOffFreq4(float freq);
void			SetGainA(float gain);
void			SetGainB(float gain);
void			SetBiQuadParam(float Gain, float Reson, float AntiReson, float DampPole, float DampZero);
void			GetBiQuadParam(float *Gain, float *Reson, float *AntiReson, float *DampPole, float *DampZero);
float			GetCutOffFreq1();
float			GetCutOffFreq2();
float			GetCutOffFreq3();
float			GetCutOffFreq4();
float			GetGainA();
float			GetGainB();
float*			GetZeroFilter();
float*			GetPoleFilter();	
int				GetCurrentFilter();

//Delay Part
void InitDelay();