#include "stdafx.h"
#include "Global.h"
#include "fft.h"
#include <mmsystem.h>
#include <math.h>

typedef struct{
			double	raw;
			double	log;
		}MAGDATA;

#define		DEF_ORIGINAL_WIDTH	512
#define		PI					3.14159
#define		DEG(x)				(int)(x * 180 / PI)

#define		DEF_8BIT_PLUS		127
#define		DEF_8BIT_MINUS		128
#define		DEF_8BIT			127
#define		DEF_16BIT			256
#define		DEF_16BIT_SCALE		65536
#define		DEF_BASE_16			32767
#define		DEF_BASE_8			127
#define		DEF_FFT_DBSCALE		96
#define		DEF_FFT_PHASESCALE	360 //2*pi
#define		DEF_LOG_ZERO		-260


float		*pFFTWindow			= NULL;
bool		FFTInUse			= false;
// how much to scale a design that assumes 96-DPI pixels
float		scale;

#define		SCALE(arg)			((int) ((arg) * scale))
#define		UNSCALE(arg)		((int) ((arg) / scale))

void InitScaling(CRect rect) 
{
   scale = (float)(rect.right - rect.left) / DEF_ORIGINAL_WIDTH;
}
///////////////////////////////////////////////////////////

void DoFFTProc()
{
	MSG		msg;
	
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

INT8 Bit8Value(INT8 data)
{
	INT8 size;

	if (data&0x80)
		size = -(DEF_8BIT_MINUS + data);
	else 
		size = DEF_8BIT_PLUS - data;

	return size;
}

int Bit16Value(INT16 num)
{
	int gab;
	
	gab = DEF_16BIT_SCALE - num;

	gab = gab / DEF_16BIT;
	gab = gab - DEF_16BIT;

	return gab;
}

int CheckYRange(CRect rect, int y)
{
	if (y <= rect.top)
				y = rect.top + 1;
	else if (y >= rect.bottom)
				y = rect.bottom - 1;

	return y;
}

MAGDATA GetSamMag(int Sam, FFTBUFFER *pProcCh, int Res)
{
	MAGDATA	rtn;
	double	compX, compY;
	double	ans;

	compX = (double)pProcCh[Sam*2];
	compY = (double)pProcCh[Sam*2 + 1];

	ans = sqrt((compX * compX) + (compY * compY));
	rtn.raw = ans;
	
	switch(Res)
	{
		case 8:
			if (ans == 0)
			{
				rtn.log = DEF_LOG_ZERO;
			}
			else
			{
				rtn.log = 20*log10(ans/DEF_BASE_8);			
			}
			break;
		case 16:
			if (ans == 0)
			{
				rtn.log = DEF_LOG_ZERO;
			}
			else
			{
				rtn.log   = 20*log10(ans/DEF_BASE_16);						
			}
			break;
	}
	
	return rtn;
}

double GetSamPhase(int Sam, FFTBUFFER *pProcCh)
{
	double		ans;
	double		compX, compY;

	compX = (double)pProcCh[Sam*2];
	compY = (double)pProcCh[Sam*2 + 1];

	ans   = atan2( compY, compX); 

	return ans;
}

void DrawMonoWave(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data)
{
	int		i, xpos, middle, gab, x,y, xtick, inc;
	float	yper;
	CPen	LinePen, *pOldPen;
	INT16	*pI16Buf;

	LinePen.CreatePen(PS_SOLID, 0, Color);
	pOldPen		= (CPen*)pDC->SelectObject(&LinePen);

	yper		= (float)(rect.bottom - rect.top)/256;
	middle		= rect.bottom - ((rect.bottom - rect.top)/2);
	
	xpos		= 0; 
	
	switch(data->fmt.wBitsPerSample)
	{
		case 8:
			xtick = (rect.right - rect.left) / (data->fftSize );
			
			if (xtick ==0)
			{
				inc		= data->fftSize / (rect.right - rect.left);

				if (inc%2 !=0)
						inc += 1;

				xtick	= 1 ;

				for(i = 0 ; i < data->DataLen ; i += inc)			
				{
					gab = Bit8Value(data->pBuffer[i]);
					x	= SCALE(xpos + rect.left);
					y	= middle + (int)(gab*yper);
					
					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
				}

			}else{
				for(i = 0 ; i < data->DataLen ; i++)			
				{
					gab = Bit8Value(data->pBuffer[i]);
					x = SCALE(xpos + rect.left);
					y = middle + (int)(gab*yper);
					
					y = CheckYRange(rect,y);
					
					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
			
				}
			}
			break;
		case 16:
			xtick = (rect.right - rect.left) / (data->fftSize );
			pI16Buf = (INT16*)(data->pBuffer);
			
			if (xtick ==0)
			{
				inc		= (data->fftSize) / (rect.right - rect.left);

				if (inc%2 !=0)
						inc += 1;

				xtick	= 1 ;
				for(i = 0 ; i < data->DataLen ; i += inc)			
				{
					gab = Bit16Value(pI16Buf[i]);
					
					x	= SCALE(xpos + rect.left);
					y	= middle + (int)(gab*yper);
					
					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
				}			
			}else{
				for(i = 0 ; i < data->DataLen ; i++)			
				{
					gab = Bit16Value(pI16Buf[i]);
				
					x = SCALE(xpos + rect.left);
					y = middle + (int)(gab*yper);
					
					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
			
				}
			}
			break;
	}
	
	pDC->SelectObject(pOldPen);
	LinePen.DeleteObject();
}

void DrawStereoWave(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data, int Ch)
{
	int		i, xpos, middle, gab, x,y, xtick, inc;
	float	yper;
	CPen	LinePen, *pOldPen;
	INT16	*pI16Buf;
	
	LinePen.CreatePen(PS_SOLID, 0, Color);
	pOldPen		= (CPen*)pDC->SelectObject(&LinePen);

	yper		= (float)(rect.bottom - rect.top)/256;
	middle		= rect.bottom - ((rect.bottom - rect.top)/2);
	
	xpos		= 0; 
	
	switch(data->fmt.wBitsPerSample)
	{
		case 8:
			xtick = (rect.right - rect.left) / (data->fftSize);
			
			if (xtick ==0)
			{
				inc		= (data->fftSize) / (rect.right - rect.left);

				if (inc%2 !=0)
						inc += 1;

				xtick	= 1 ;

				inc		*= 2;

				for(i = 0 ; i < data->DataLen ; i += inc)			
				{
					switch(Ch){
						case DEF_CH_LEFT:
							gab = Bit8Value(data->pBuffer[i]);
							break;
						case DEF_CH_RIGHT:
							gab = Bit8Value(data->pBuffer[i+1]);
							break;
					}

					x	= SCALE(xpos + rect.left);
					y	= middle + (int)(gab*yper);

					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
				}
			}else{
				for(i = 0 ; i < data->DataLen ; i+= 2)			
				{
					switch(Ch){
						case DEF_CH_LEFT:
							gab = Bit8Value(data->pBuffer[i]);
							break;
						case DEF_CH_RIGHT:
							gab = Bit8Value(data->pBuffer[i+1]);
							break;
					}

					x = SCALE(xpos   + rect.left);
					y = middle + (int)(gab*yper);

					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
			
				}
			}
			break;
		case 16:
			xtick = (rect.right - rect.left) / (data->fftSize);
			pI16Buf = (INT16*)(data->pBuffer);
			
			if (xtick ==0)
			{
				inc		= (data->fftSize) / (rect.right - rect.left);
				
				if (inc%2 !=0)
						inc += 1;

				xtick	= 1 ;

				inc		*= 2;

				for(i = 0 ; i < data->DataLen ; i += inc)			
				{
					switch(Ch){
						case DEF_CH_LEFT:
							gab = Bit16Value(pI16Buf[i]);							
							break;
						case DEF_CH_RIGHT:
							gab = Bit16Value(pI16Buf[i+1]);
							break;
					}
					
					x	= SCALE(xpos   + rect.left);
					y	= middle + (int)(gab*yper);

					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
				}
			
			}else{
				for(i = 0 ; i < data->DataLen ; i += 2)			
				{
					switch(Ch){
						case DEF_CH_LEFT:
							gab = Bit16Value(pI16Buf[i]);							
							break;
						case DEF_CH_RIGHT:
							gab = Bit16Value(pI16Buf[i+1]);
							break;
					}
				
					x = SCALE(xpos   + rect.left);
					y = middle + (int)(gab*yper);

					y = CheckYRange(rect,y);

					if (xpos == 0)
						pDC->MoveTo(x, y);
					else
						pDC->LineTo(x, y);

					xpos += xtick;
			
				}
			}
			
			break;
	}
	
	pDC->SelectObject(pOldPen);
	LinePen.DeleteObject();
}

void DrawWave(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data,int Ch)
{
	switch(data->fmt.nChannels)
	{
		case 1: DrawMonoWave(pDC, rect, Color, data);
			break;
		case 2: DrawStereoWave(pDC, rect, Color, data, Ch);
			break;
	}
}

void DrawWaveWindow(CDC *pDC, CRect rect, COLORREF Color)
{
	CPen		Pen,*pOldPen;
	CBrush		hBrush, *pOldBrush;

	hBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush = (CBrush*)pDC->SelectObject(&hBrush);

	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	hBrush.DeleteObject();

	Pen.CreatePen(PS_SOLID, 0, Color);
	pOldPen=(CPen*)pDC->SelectObject(&Pen);

	pDC->MoveTo(rect.left,  rect.bottom - (rect.bottom - rect.top)/2);
	pDC->LineTo(rect.right, rect.bottom - (rect.bottom - rect.top)/2);

	pDC->SelectObject(pOldPen);

	Pen.DeleteObject();
}

void DrawFFTWindow(CDC *pDC, CRect rect, COLORREF Color)
{
	CPen		Pen,*pOldPen;
	CBrush		hBrush, *pOldBrush;

	hBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush = (CBrush*)pDC->SelectObject(&hBrush);

	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	hBrush.DeleteObject();

	Pen.CreatePen(PS_SOLID, 0, Color);
	pOldPen=(CPen*)pDC->SelectObject(&Pen);

	pDC->SelectObject(pOldPen);

	Pen.DeleteObject();

}

void Draw8BitFFT(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *pBuffer, int Ch)
{
	int			i, xpos,  x, y, middle, xtick, inc, gab;
	float		yper;
	CPen		LinePen, *pOldPen;
	FFTBUFFER	*pProcCh;
	MAGDATA		data;	

	switch(Ch){
		case DEF_CH_LEFT:
			pProcCh = pBuffer->pFFTBufferL;
			break;
		case DEF_CH_RIGHT:
			pProcCh = pBuffer->pFFTBufferR;
			break;
	}

	LinePen.CreatePen(PS_SOLID, 0, Color);
	pOldPen		= (CPen*)pDC->SelectObject(&LinePen);
	
	xpos		= 0; 

	yper		= (float)(rect.bottom - rect.top)/DEF_FFT_DBSCALE;
	xtick		= (rect.right - rect.left) / (pBuffer->fftSize - 1);
	middle		=  rect.top;

	if (xtick ==0)
	{
		inc		= pBuffer->fftSize / (rect.right - rect.left);

		if (inc%2 !=0)
				inc += 1;

		xtick	= 1 ;

		for(i = 0 ; i < pBuffer->fftSize ; i += inc)			
		{
			x	= SCALE(xpos + rect.left);
			data = GetSamMag(i, pProcCh, 8);

			if (data.raw != 0)
			{
				gab = (int)data.log;
				y	= middle - (int)(gab * yper);
			}
			else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

				xpos += xtick;
		}
	}else{
		for(i = 0 ; i < pBuffer->fftSize ; i++)			
		{
			x	= SCALE(xpos + rect.left);
			data = GetSamMag(i, pProcCh, 8);
			
			if (data.raw != 0)
			{
				gab = (int)data.log;	
				y	= middle - (int)(gab * yper);
			}
			else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

			xpos += xtick;
		}
	}			

	pDC->SelectObject(pOldPen);
	LinePen.DeleteObject();
}

void Draw16BitFFT(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *pBuffer, int Ch)
{
	int			i, xpos,  x, y, middle, xtick, inc, gab;
	float		yper;
	CPen		LinePen, *pOldPen;
	FFTBUFFER	*pProcCh;
	MAGDATA		data;

	switch(Ch){
		case DEF_CH_LEFT:
			pProcCh = pBuffer->pFFTBufferL;
			break;
		case DEF_CH_RIGHT:
			pProcCh = pBuffer->pFFTBufferR;
			break;
	}

	LinePen.CreatePen(PS_SOLID, 0, Color);
	pOldPen		= (CPen*)pDC->SelectObject(&LinePen);
	
	xpos		= 0; 

	yper		= (float)(rect.bottom - rect.top)/DEF_FFT_DBSCALE;
	xtick		= (rect.right - rect.left) / (pBuffer->fftSize - 1);
	middle		=  rect.top;

	if (xtick ==0)
	{
		inc		= pBuffer->fftSize / (rect.right - rect.left);

		if (inc%2 !=0)
				inc += 1;

		xtick	= 1 ;

		for(i = 0 ; i < pBuffer->fftSize ; i += inc)			
		{
			x	= SCALE(xpos + rect.left);
			data = GetSamMag(i, pProcCh, 16);		
			
			if (data.raw != 0)
			{
				gab = (int)data.log;		
				y	= middle - (int)(gab * yper);
			}else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

				xpos += xtick;
		}
	}else{
		for(i = 0 ; i < pBuffer->fftSize ; i++)			
		{
			x		= SCALE(xpos + rect.left);
			data	= GetSamMag(i, pProcCh, 16);		

			if (data.raw != 0)
			{
				gab = (int)data.log;
				y	=  middle - (int)(gab * yper);
			}else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

			xpos += xtick;
		}
	}			

	pDC->SelectObject(pOldPen);
	LinePen.DeleteObject();
}

void DrawFFT(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data,int Ch)
{
	switch(data->fmt.wBitsPerSample){
		case 8:	Draw8BitFFT(pDC, rect, Color, data, Ch); 
			break;
		case 16:Draw16BitFFT(pDC, rect, Color, data, Ch); 
			break;
	}
}

void ExtendMonoFFTBuffer(FFTBUFFER *Target, WAVEDATA *data)
{
	int i;
	INT16 *pI16Buf;

	switch(data->fmt.wBitsPerSample){
		case 8:
			for (i = 0 ; i < data->fftSize ; i++)
			{
				if (i < data->DataLen)
				{
					if (pFFTWindow != NULL)
						Target[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i]) * pFFTWindow[i];					
					else
						Target[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i]);					
				}
				else 
					Target[i] = 0;	//Zero Padding
			}
			break;
		case 16:
			pI16Buf = (INT16*)(data->pBuffer);
			for (i = 0 ; i < data->fftSize ; i++)
			{
				if (i < data->DataLen)
				{					
					if (pFFTWindow != NULL)
						Target[i] = (FFTBUFFER)pI16Buf[i] * pFFTWindow[i];
					else
						Target[i] = (FFTBUFFER)pI16Buf[i];
				}
				else 
					Target[i] = 0;	//Zero Padding
			}
			break;
	}
}

void ExtendStereoFFTBuffer(FFTBUFFER *TargetL, FFTBUFFER *TargetR, WAVEDATA *data)
{
	int i;
	INT16 *pI16Buf;

	switch(data->fmt.wBitsPerSample){
		case 8:
			for (i = 0 ; i < data->fftSize ; i += 1)
			{
				if (i < data->DataLen)
				{
					if (pFFTWindow != NULL)
					{
						TargetL[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i*2])   * pFFTWindow[i];
						TargetR[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i*2+1]) * pFFTWindow[i];
					}else{
						TargetL[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i*2]);
						TargetR[i] = (FFTBUFFER)Bit8Value(data->pBuffer[i*2+1]);
					}
				}else {
					TargetL[i] = 0;	//Zero Padding
					TargetR[i] = 0;	//Zero Padding
				}
			}
			break;
		case 16:
			pI16Buf = (INT16*)(data->pBuffer);
			for (i = 0 ; i < data->fftSize ; i += 1)
			{
				if (i < data->DataLen){
					if (pFFTWindow != NULL)
					{
						TargetL[i] = (FFTBUFFER)pI16Buf[i*2]   * pFFTWindow[i];
						TargetR[i] = (FFTBUFFER)pI16Buf[i*2+1] * pFFTWindow[i];
					}else{
						TargetL[i] = (FFTBUFFER)pI16Buf[i*2];
						TargetR[i] = (FFTBUFFER)pI16Buf[i*2+1];
					}
				}
				else {
					TargetL[i] = 0;	//Zero Padding
					TargetR[i] = 0;	//Zero Padding
				}
			}
			break;
	}
}

void InitFFTWindow()
{
	while(FFTInUse)
			DoFFTProc();
	
	free(pFFTWindow);
	pFFTWindow	= NULL;	
}

void MakeFFTWindow(int wnd, int Len)
{
	int i;
	double pi, phase = 0, delta;
	float  *pTmpWindow;	
	
	while(FFTInUse)
			DoFFTProc();
	
	if (pFFTWindow != NULL)
				InitFFTWindow();

	pTmpWindow		= (float*)calloc(Len, sizeof(float));
    pi				= 4. * atan(1.0);
	delta			= 2 * pi / (double)(Len - 1);

	switch(wnd)
	{
		case DEF_WIN_HANN:
			for (i = 0 ; i < Len ; i++)	
			{
				pTmpWindow[i] = (float)(0.5  - 0.5*cos(phase));
				phase += delta;
			}
			break;
		case DEF_WIN_HAMM:
			for (i = 0 ; i < Len ; i++)				
			{
				pTmpWindow[i] = (float)(0.54 - 0.46*cos(phase));
				phase += delta;
			}
			break;
		case DEF_WIN_BLACKMAN:
			for (i = 0 ; i < Len ; i++)							
			{
				pTmpWindow[i] = (float)(0.42 - 0.5*cos(phase) + 0.08*cos(2*phase));
				phase += delta;
			}
			break;
	}
	pFFTWindow = pTmpWindow;			
}

void DoFFT(FFTBUFFER *pFFTBufferL, FFTBUFFER *pFFTBufferR, WAVEDATA* pData)
{
	FFTInUse	= true;
	switch(pData->fmt.nChannels){
		case 1:
			ExtendMonoFFTBuffer(pFFTBufferL,pData);
			rfft(pFFTBufferL, pData->fftSize , FORWARD);
			break;
		case 2:
			ExtendStereoFFTBuffer(pFFTBufferL, pFFTBufferR, pData);
			rfft(pFFTBufferL, pData->fftSize , FORWARD);
			rfft(pFFTBufferR, pData->fftSize , FORWARD);
			break;
	}
	FFTInUse	= false;
}

void DrawPhaseLine(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *pBuffer, int Ch)
{
	int			i, xpos,  x, y, middle, xtick, inc, gab;
	float		yper;
	CPen		LinePen, *pOldPen;
	double		ans;
	FFTBUFFER	*pProcCh;

	switch(Ch){
		case DEF_CH_LEFT:
			pProcCh = pBuffer->pFFTBufferL;
			break;
		case DEF_CH_RIGHT:
			pProcCh = pBuffer->pFFTBufferR;
			break;
	}

	LinePen.CreatePen(PS_SOLID, 0, Color);
	pOldPen		= (CPen*)pDC->SelectObject(&LinePen);
	
	xpos		= 0; 

	yper		= (float)(rect.bottom - rect.top)/DEF_FFT_PHASESCALE;
	xtick		= (rect.right - rect.left) / (pBuffer->fftSize - 1);
	middle		=  rect.bottom - ((rect.bottom - rect.top)/2);

	if (xtick ==0)
	{
		inc		= pBuffer->fftSize / (rect.right - rect.left);

		if (inc%2 !=0)
				inc += 1;

		xtick	= 1 ;

		for(i = 0 ; i < pBuffer->fftSize ; i += inc)			
		{
			ans = GetSamPhase(i, pProcCh); 
			x	= SCALE(xpos + rect.left);

			if (ans != 0)
			{
				gab = DEG(ans);
				y	= middle - (int)(gab*yper);
			}else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

				xpos += xtick;
		}
	}else{
		for(i = 0 ; i < pBuffer->fftSize ; i++)			
		{
			ans = GetSamPhase(i, pProcCh); 
			x	= SCALE(xpos + rect.left);

			if (ans != 0)
			{
				gab = DEG(ans);
				y	= middle - (int)(gab*yper);
			}else
				y = rect.bottom;

			y = CheckYRange(rect,y);

			if (xpos == 0)
				pDC->MoveTo(x, y);
			else
				pDC->LineTo(x, y);

			xpos += xtick;
		}
	}			

	pDC->SelectObject(pOldPen);
	LinePen.DeleteObject();
}

void DrawPhaseWindow(CDC *pDC, CRect rect, COLORREF Color)
{
	CPen		Pen,*pOldPen;
	CBrush		hBrush, *pOldBrush;

	hBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush = (CBrush*)pDC->SelectObject(&hBrush);

	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	hBrush.DeleteObject();

	Pen.CreatePen(PS_SOLID, 0, Color);
	pOldPen=(CPen*)pDC->SelectObject(&Pen);

	pDC->MoveTo(rect.left,  rect.bottom - (rect.bottom - rect.top)/2);
	pDC->LineTo(rect.right, rect.bottom - (rect.bottom - rect.top)/2);

	pDC->SelectObject(pOldPen);

	Pen.DeleteObject();
}

void DrawPhase(CDC *pDC, CRect rect, COLORREF Color, WAVEDATA *data,int Ch)
{
	switch(data->fmt.wBitsPerSample){
		case 8:	DrawPhaseLine(pDC, rect, Color, data, Ch); 
			break;
		case 16:DrawPhaseLine(pDC, rect, Color, data, Ch); 
			break;
	}
}

WINDOWFFTDATA	GetWinFFTData(int CurPos, WAVEDATA *pData,CRect rect, int Ch)
{
	WINDOWFFTDATA	ans;
	int				SamTick,inc,idx;
	FFTBUFFER		*pProcCh;

	switch(Ch)
	{
		case DEF_CH_LEFT:
			pProcCh = pData->pFFTBufferL;
			break;
		case DEF_CH_RIGHT:
			pProcCh = pData->pFFTBufferR;
			break;
	}

	SamTick = (pData->fmt.nSamplesPerSec/2) / ((rect.right - rect.left) - 1);
	ans.CurSam = CurPos * SamTick;

	SamTick = (rect.right - rect.left) / (pData->fftSize);
	if (SamTick == 0)
	{
		inc	= pData->fftSize / (rect.right - rect.left);	
		
		if (inc%2 != 0)
					inc +=1;

		idx = UNSCALE(CurPos) * inc;
	}else{
		inc = (rect.right - rect.left) / (pData->fftSize);	
		idx = UNSCALE(CurPos) /inc;
	}

	ans.CurSam = (int)(((float)idx / pData->fftSize) * (pData->fmt.nSamplesPerSec/2));

	if (idx == pData->fftSize)
		idx -= 1;

	switch(pData->fmt.wBitsPerSample)
	{
		case 8:
			ans.CurMag  = (int)(GetSamMag(idx,   pProcCh,8).log);
			break;
		case 16:
			ans.CurMag  = (int)(GetSamMag(idx,   pProcCh,16).log);
			break;
	}
	
	ans.CurPhas = DEG(GetSamPhase(idx,   pProcCh));
	
	return ans;
}
