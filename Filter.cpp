#include "stdafx.h"
#include "Global.h"
#include <mmsystem.h>
#include <math.h>

#define			DEF_IIR_GAIN		0.1
#define			DEF_FIR_GAIN		1.0
#define			DEF_16BIT_MAX		32767
#define			DEF_16BIT_MIN		-32768
#define			DEF_8BIT_MAX		127
#define			DEF_8BIT_MIN		-127
#define			DEF_8BIT_PLUS		127
#define			DEF_8BIT_MINUS		128
#define			DEF_BUFF_MARGIN		2
#define			DEF_BIQUAD_LEN		3

typedef			float				ZEROBUFFER;
typedef			float				POLEBUFFER;

FILTERBUFFER	*pZeroFilter		= NULL;
FILTERBUFFER	*pPoleFilter		= NULL;
ZEROBUFFER		*pPastZeroBufferL	= NULL;
ZEROBUFFER		*pPastZeroBufferR	= NULL;
POLEBUFFER		*pPastPoleBufferL	= NULL;
POLEBUFFER		*pPastPoleBufferR	= NULL;

int				ZeroFilterLen		= DEF_FILTER_LENGTH;
int				PoleFilterLen		= DEF_FILTER_LENGTH;
int				CurFilter			= DEF_FILTER_NONE;
bool			FilterInUse			= false;

float			IIRGain				= DEF_IIR_GAIN;
float			FIRGain				= DEF_FIR_GAIN;

float			CutOffFreq1			= 20.0;
float			CutOffFreq2			= 2000.0;
float			CutOffFreq3			= 3000.0;
float			CutOffFreq4			= 4000.0;
float			GainA				= 1.0;
float			GainB				= 2.0;

float			BiQuadZeroDamp		= 1.0;
float			BiQuadPoleDamp		= 0.99;
float			BiQuadResonFreq		= 3000.0;
float			BiQuadAntiResonFreq	= 5000.0;

void DoProc()
{
	MSG		msg;
	
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

INT8 Set8Value(INT8 data)
{
	INT8 ans;

	if (data&0x80)
		ans = -(DEF_8BIT_MINUS + data);
	else 
		ans = DEF_8BIT_PLUS - data;

	return ans;
}

INT8 Make8Bit(FILTERBUFFER data)
{
	INT8 ans;

	if ((INT8)data&0x80)
		ans = -(DEF_8BIT_MINUS + (INT8)data);
	else 
		ans = DEF_8BIT_PLUS - (INT8)data;

	return ans;
}

int GetCurrentFilter()
{
	return CurFilter;
}

void SetCutOffFreq1(float freq)
{
	CutOffFreq1	= freq;
}

void SetCutOffFreq2(float freq)
{
	CutOffFreq2	= freq;
}

void SetCutOffFreq3(float freq)
{
	CutOffFreq3	= freq;
}

void SetCutOffFreq4(float freq)
{
	CutOffFreq4	= freq;
}

float GetCutOffFreq1()
{
	return CutOffFreq1;
}

float GetCutOffFreq2()
{
	return CutOffFreq2;
}

float GetCutOffFreq3()
{
	return CutOffFreq3;
}

float GetCutOffFreq4()
{
	return CutOffFreq4;
}

void SetGainA(float gain)
{
	GainA = gain;
}

void SetGainB(float gain)
{
	GainB = gain;
}

float GetGainA()
{
	return GainA;
}

float GetGainB()
{
	return GainB;
}

INT8 FIR8Bit(INT8 data, ZEROBUFFER *pPastData)
{
	int				i, ZeroOrder;
	ZEROBUFFER*		pZero;
	FILTERBUFFER	LastOutput;

	pZero		= pPastData;
	ZeroOrder	= ZeroFilterLen - 1;

	pZero[0]	= Set8Value(data) * FIRGain;	
	LastOutput  = pZero[0] * pZeroFilter[0];

	for(i = ZeroOrder ; i > 0 ; i--)
	{
		LastOutput += pZeroFilter[i] * pZero[i];
		pZero[i]	= pZero[i -1];
	}

	if (LastOutput > DEF_8BIT_MAX) 
			LastOutput	= DEF_8BIT_MAX;

	if (LastOutput < DEF_8BIT_MIN)
			LastOutput = DEF_8BIT_MIN;

	return Make8Bit(LastOutput);
}

FILTERBUFFER FIR16Bit(INT16 data, ZEROBUFFER *pPastData)
{
	int				i, ZeroOrder;
	ZEROBUFFER*		pZero;
	FILTERBUFFER	LastOutput;

	pZero		= pPastData;
	ZeroOrder	= ZeroFilterLen - 1;
	
	pZero[0]	= data * FIRGain;
	LastOutput	= pZero[0] * pZeroFilter[0];

	for(i = ZeroOrder ; i > 0 ; i--)
	{
		LastOutput += pZeroFilter[i] * pZero[i];
		pZero[i]	= pZero[i -1];
	}

	if (LastOutput > DEF_16BIT_MAX)
			LastOutput = DEF_16BIT_MAX;
	if (LastOutput < DEF_16BIT_MIN)
			LastOutput = DEF_16BIT_MIN;

	return LastOutput;
}

FILTERBUFFER IIR16Bit(INT16 data, POLEBUFFER *pPoleData, ZEROBUFFER *pZeroData)
{
    int i, ZeroOrder, PoleOrder;
    FILTERBUFFER	LastOutput;
	ZEROBUFFER		*pZero;
	POLEBUFFER		*pPole;
    
	pPole		= pPoleData;
	pZero		= pZeroData;
	ZeroOrder	= ZeroFilterLen - 1;
	PoleOrder	= PoleFilterLen - 1;
	
	pPole[0] = 0.0;
	
	pZero[0] = IIRGain * data;

	for (i = ZeroOrder ; i > 0 ; i--) 
	{
		pPole[0] += pZeroFilter[i] * pZero[i];
		pZero[i]  = pZero[i-1];
	}
  
	pPole[0] += pZeroFilter[0] * pZero[0];

	for (i = PoleOrder ; i > 0 ; i--) 
	{
		pPole[0] += -pPoleFilter[i] * pPole[i];
		pPole[i]  = pPole[i-1];
	}

	LastOutput = pPole[0];

	if (LastOutput > DEF_16BIT_MAX)
			LastOutput = DEF_16BIT_MAX;
	if (LastOutput < DEF_16BIT_MIN)
			LastOutput = DEF_16BIT_MIN;
	
	return LastOutput;
}

INT8 IIR8Bit(INT8 data, POLEBUFFER *pPoleData, ZEROBUFFER *pZeroData)
{
	int				i, ZeroOrder, PoleOrder;
	FILTERBUFFER	LastOutput;
	ZEROBUFFER		*pZero;
	POLEBUFFER		*pPole;

	pPole		= pPoleData;
	pZero		= pZeroData;
	ZeroOrder	= ZeroFilterLen - 1;
	PoleOrder	= PoleFilterLen - 1;

	pPole[0] = 0.0;
	
	pZero[0] = IIRGain * Set8Value(data);

	for (i = ZeroOrder ; i > 0 ; i--) 
	{
		pPole[0] += pZeroFilter[i] * pZero[i];
		pZero[i] = pZero[i-1];
	}
  
	pPole[0] += pZeroFilter[0] * pZero[0];

	for (i = PoleOrder ; i > 0 ; i--) 
	{
		pPole[0] += -pPoleFilter[i] * pPole[i];
		pPole[i] = pPole[i-1];
	}

	LastOutput = pPole[0];

	if (LastOutput > DEF_8BIT_MAX) 
			LastOutput	= DEF_8BIT_MAX;

	if (LastOutput < DEF_8BIT_MIN)
			LastOutput = DEF_8BIT_MIN;

    return Make8Bit(LastOutput);
}

void InitFilter()
{
	while(FilterInUse)
				DoProc();

	free(pZeroFilter);
	free(pPoleFilter);
	free(pPastZeroBufferL);
	free(pPastZeroBufferR);
	free(pPastPoleBufferL);
	free(pPastPoleBufferR);

	pZeroFilter		= NULL;
	pPoleFilter		= NULL;
	pPastZeroBufferL= NULL;
	pPastZeroBufferR= NULL;
	pPastPoleBufferL= NULL;
	pPastPoleBufferR= NULL;
	
	ZeroFilterLen	= 0;
	PoleFilterLen	= 0;
	
	FilterInUse		= false;
}

bool GetFilterStatus()
{
	if (pZeroFilter != NULL)
		return true;
	else 
		return false;
}

int GetZeroFilterLength()
{
	return ZeroFilterLen;
}

int GetPoleFilterLength()
{
	return ZeroFilterLen;
}

void SetBiQuadParam(float Gain, float Reson, float AntiReson, float DampPole, float DampZero)
{
	BiQuadZeroDamp		= DampZero;
	BiQuadPoleDamp		= DampPole;
	BiQuadResonFreq		= Reson;
	BiQuadAntiResonFreq = AntiReson;
	IIRGain				= Gain;	
}

void GetBiQuadParam(float *Gain, float *Reson, float *AntiReson, float *DampPole, float *DampZero)
{
	*DampZero		= BiQuadZeroDamp;
	*DampPole		= BiQuadPoleDamp;
	*Reson			= BiQuadResonFreq;
	*AntiReson		= BiQuadAntiResonFreq;
	*Gain			= IIRGain;	
}

void MakeBiQuadFilter(WAVEFORMATEX fmt)
{
	double OmegaP, OmegaZ, pi;
	float  rp, rz;
	FILTERBUFFER	*pTmpZeroFilter, *pTmpPoleFilter;

	while(FilterInUse)
				DoProc();

	if (pZeroFilter != NULL)
	{
		free(pZeroFilter);
		pZeroFilter = NULL;
	}

	if (pPoleFilter != NULL)
	{
		free(pPoleFilter);
		pPoleFilter = NULL;
	}

	pi					= 4.0 * atan(1.0);
	OmegaP				= (BiQuadResonFreq		/(fmt.nSamplesPerSec)) * 2 * pi;	//   Cutoff Freq
	OmegaZ				= (BiQuadAntiResonFreq	/(fmt.nSamplesPerSec)) * 2 * pi;	//---------------- = Normalize OmegaC
																					// (Sample Rate/2)

	if (BiQuadPoleDamp >= 1.0)
			BiQuadPoleDamp = (float)0.99;

	rp					= BiQuadPoleDamp;
	rz					= BiQuadZeroDamp;

	pTmpZeroFilter	= (FILTERBUFFER*)calloc(DEF_BIQUAD_LEN,  sizeof(FILTERBUFFER));
	pTmpPoleFilter	= (FILTERBUFFER*)calloc(DEF_BIQUAD_LEN,  sizeof(FILTERBUFFER));

	pTmpZeroFilter[0] = 1;
	pTmpZeroFilter[1] = -(float)(2.0 * rz * cos(OmegaZ));  // 0.0;
	pTmpZeroFilter[2] =  (rz * rz);    					//-1.0;

	pTmpPoleFilter[0] = 1;
	pTmpPoleFilter[1] = -(float)(2.0 * rp * cos(OmegaP));  //-1.65;
	pTmpPoleFilter[2] =  (rp * rp);						//0.99;

	pZeroFilter			= pTmpZeroFilter;
	pPoleFilter			= pTmpPoleFilter;
	ZeroFilterLen		= DEF_BIQUAD_LEN;
	PoleFilterLen		= DEF_BIQUAD_LEN;
	CurFilter			= DEF_FILTER_BIQUAD;
}

void MakeFIRFilterWindow(WAVEFORMATEX fmt, int Length, int win, int filter)
{
	int i, M;
	double pi, phase = 0, delta; 
	double OmegaC1, OmegaC2, OmegaC3, OmegaC4;
	FILTERBUFFER *pTmpZeroFilter;
    
	while(FilterInUse)
				DoProc();
	
	if (filter == DEF_FILTER_NONE)
	{				
		InitFilter();
		return;
	}

	if (pZeroFilter != NULL)
	{
		free(pZeroFilter);
		pZeroFilter = NULL;
	}

	if (Length == 0)
				return;

	if ((Length % 2) == 0)
				Length += 1;

	pi					= 4. * atan(1.0);
	delta				= 2  * pi / (double)(Length - 1);
	OmegaC1				= (CutOffFreq1/(fmt.nSamplesPerSec)) * 2 * pi;  //   Cutoff Freq
	OmegaC2				= (CutOffFreq2/(fmt.nSamplesPerSec)) * 2 * pi;	//---------------- = Normalize OmegaC
	OmegaC3				= (CutOffFreq3/(fmt.nSamplesPerSec)) * 2 * pi;	// (Sample Rate/2)
	OmegaC4				= (CutOffFreq4/(fmt.nSamplesPerSec)) * 2 * pi;	
	M					= (Length - 1) / 2;																		

	pTmpZeroFilter		= (FILTERBUFFER*)calloc(Length,  sizeof(FILTERBUFFER));
	for (i = 0 ; i < Length ; i++)	
	{
		switch(win)
		{
			case DEF_WIN_HANN:
				pTmpZeroFilter[i] = (float)(0.5  - 0.5*cos(phase));
				break;
			case DEF_WIN_HAMM:
				pTmpZeroFilter[i] = (float)(0.54 - 0.46*cos(phase));
				break;
			case DEF_WIN_BLACKMAN:
				pTmpZeroFilter[i] = (float)(0.42 - 0.5*cos(phase) + 0.08*cos(2*phase));
				break;
		}
		
		if (i == M)
			pTmpZeroFilter[i] = 1.0;

		switch(filter)
		{
			case DEF_FILTER_LOWPASS:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)(sin( (i - M) * OmegaC1 ) / ( (i - M) * pi )) * GainA;
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= (float)(OmegaC1 / pi) * GainA;
				}
				break;
			case DEF_FILTER_HIGHPASS:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)(-sin( (i - M) * OmegaC2 ) / ( (i - M) * pi )) * GainA;
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= (float)(1.0 - OmegaC2 / pi) * GainA;
				}
				break;
			case DEF_FILTER_BANDPASS:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)((sin( (i - M) * OmegaC2 ) - sin( (i - M) * OmegaC1 )) / ( (i - M) * pi )) * GainA;
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= (float)((OmegaC2 - OmegaC1) / pi) * GainA;
				}
				break;
			case DEF_FILTER_BANDSTOP:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)((sin( (i - M) * OmegaC1 ) - sin( (i - M) * OmegaC2 )) / ( (i - M) * pi ));
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= (float)(1.0 - (OmegaC2 - OmegaC1) / pi);
				}
				GainA = 1.0;
				break;
			case DEF_FILTER_MULTIBANDPASS:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)(
					((sin( (i - M) * OmegaC2 ) - sin( (i - M) * OmegaC1 )) / ( (i - M) * pi )) * GainA + 
					((sin( (i - M) * OmegaC4 ) - sin( (i - M) * OmegaC3 )) / ( (i - M) * pi )) * GainB
					);
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= (float)( 
					((OmegaC2 - OmegaC1) / pi) * GainA +
					((OmegaC4 - OmegaC3) / pi) * GainB
					);
				}
				break;
			case DEF_FILTER_HILLBERT:
				if (i <= M)
				{
					if ((i - M) % 2 == 0)
						pTmpZeroFilter[i] *= 0;
					else
						pTmpZeroFilter[i] *= (float)(2. / ((i - M)*pi));
				}
				break;
			case DEF_FILTER_TIMEDIFF:
				if (i < M)
				{
					pTmpZeroFilter[i] *= (float)( cos((i - M) * pi) / (i - M) );
				}
			
				if (i == M)
				{
					pTmpZeroFilter[i] *= 0;
				}
				break;
		}

		if (i > M)
		{
			pTmpZeroFilter[i] = pTmpZeroFilter[Length - 1 - i];
		}

		phase += delta;
	}
	pZeroFilter			= pTmpZeroFilter;
	ZeroFilterLen		= Length;
	PoleFilterLen		= 1;
	CurFilter			= filter;
}

void DoFilter(WAVEBUFFER *pBuffer, int DataLen, WAVEFORMATEX fmt)
{
	INT16		*pI16Buff;
	int			i, Len;

	if (CurFilter != DEF_FILTER_BIQUAD)
	{
		if (pZeroFilter == NULL)
							return;
		if (ZeroFilterLen == 0)
							return;
	}else{
		if (pZeroFilter == NULL)
							return;
		if (ZeroFilterLen == 0)
							return;	
		if (pPoleFilter == NULL)
							return;
		if (PoleFilterLen == 0)
							return;	
	}

	FilterInUse = true;

	switch(fmt.wBitsPerSample){
		case 8:
			switch(fmt.nChannels){
				case 1:
					Len = DataLen;
					if (pPastZeroBufferL == NULL)
					{
						pPastZeroBufferL = (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}

					if (pPastPoleBufferL == NULL)
					{
						pPastPoleBufferL = (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}
				
					for(i = 0 ; i < Len ; i++)
					{
						if (CurFilter != DEF_FILTER_BIQUAD)
						{
							pBuffer[i] = FIR8Bit(pBuffer[i], pPastZeroBufferL);
						}else{
							pBuffer[i] = IIR8Bit(pBuffer[i], pPastPoleBufferL, pPastZeroBufferL);
						}
					}

					break;
				case 2:
					Len = DataLen;

					if (pPastZeroBufferL== NULL)
					{
						pPastZeroBufferL= (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}
				
					if (pPastPoleBufferL== NULL)
					{
						pPastPoleBufferL= (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}

					if (pPastZeroBufferR== NULL)
					{
						pPastZeroBufferR= (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}

					if (pPastPoleBufferR == NULL)
					{
						pPastPoleBufferR = (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}

	
					for(i = 0 ; i < Len ; i += 2)
					{
						if (CurFilter != DEF_FILTER_BIQUAD)
						{
							pBuffer[i]   = FIR8Bit(pBuffer[i]  , pPastZeroBufferL);
							pBuffer[i+1] = FIR8Bit(pBuffer[i+1], pPastZeroBufferR);						
						}else{
							pBuffer[i]   = IIR8Bit(pBuffer[i]  , pPastPoleBufferL, pPastZeroBufferL);
							pBuffer[i+1] = IIR8Bit(pBuffer[i+1], pPastPoleBufferR, pPastZeroBufferR);						
						}
					}
					break;
			}
			break;
		case 16:
			pI16Buff = (INT16*)pBuffer;
			
			switch(fmt.nChannels){
				case 1:
					Len = DataLen ;
					if (pPastZeroBufferL == NULL)
					{
						pPastZeroBufferL = (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}

					if (pPastPoleBufferL == NULL)
					{
						pPastPoleBufferL = (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}

					
					for(i = 0 ; i < Len ; i++)
					{
						if (CurFilter != DEF_FILTER_BIQUAD)
						{
							pI16Buff[i] = (INT16)FIR16Bit(pI16Buff[i], pPastZeroBufferL);
						}else{
							pI16Buff[i] = (INT16)IIR16Bit(pI16Buff[i], pPastPoleBufferL, pPastZeroBufferL);
						}
					}

					break;
				case 2:
					Len = DataLen ;

					if (pPastZeroBufferL== NULL)
					{
						pPastZeroBufferL= (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}
				
					if (pPastPoleBufferL == NULL)
					{
						pPastPoleBufferL = (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}

					if (pPastZeroBufferR== NULL)
					{
						pPastZeroBufferR= (ZEROBUFFER*)calloc(ZeroFilterLen + DEF_BUFF_MARGIN, sizeof(ZEROBUFFER));				
					}

					if (pPastPoleBufferR == NULL)
					{
						pPastPoleBufferR = (POLEBUFFER*)calloc(PoleFilterLen + DEF_BUFF_MARGIN, sizeof(POLEBUFFER));				
					}

					for(i = 0 ; i < Len ; i+=2)
					{
						if (CurFilter != DEF_FILTER_BIQUAD)
						{
							pI16Buff[i]   = (INT16)FIR16Bit(pI16Buff[i]  , pPastZeroBufferL);
							pI16Buff[i+1] = (INT16)FIR16Bit(pI16Buff[i+1], pPastZeroBufferR);						
						}else{

							pI16Buff[i]   = (INT16)IIR16Bit(pI16Buff[i]  , pPastPoleBufferL, pPastZeroBufferL);
							pI16Buff[i+1] = (INT16)IIR16Bit(pI16Buff[i+1], pPastPoleBufferR, pPastZeroBufferR);						
						}
					}

					break;
			}
			break;
	}

	FilterInUse = false;
}

FILTERBUFFER* GetZeroFilter()
{
	return pZeroFilter;
}

FILTERBUFFER* GetPoleFilter()
{
	return pPoleFilter;
}