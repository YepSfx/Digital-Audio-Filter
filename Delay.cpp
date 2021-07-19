#include "stdafx.h"
#include "Global.h"
#include <math.h>
#include <mmsystem.h>

#define			DEF_BUFF_MARGIN		1
#define			DEF_8BIT_PLUS		127
#define			DEF_8BIT_MINUS		128


WAVEBUFFER	*pRingBufferL	= NULL;
WAVEBUFFER	*pRingBufferR	= NULL;

int			BufferLenL		= 0;
int			BufferLenR		= 0;
int			OffSetL			= 0;
int			OffSetR			= 0;
int			MaxLen			= 0;

bool		DelayInWrite	= false;

void DoDelayProc()
{
	MSG		msg;
	
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

INT8 Set8Bit(INT8 data)
{
	INT8 ans;

	if (data&0x80)
		ans = -(DEF_8BIT_MINUS + data);
	else 
		ans = DEF_8BIT_PLUS - data;

	return ans;
}

void InitDelay()
{
	while(DelayInWrite)
				DoDelayProc();

	free(pRingBufferL);
	free(pRingBufferR);

	pRingBufferL	= NULL;
	pRingBufferR	= NULL;
	
	BufferLenL	= 0;
	BufferLenR	= 0;

	OffSetL		= 0;
	OffSetR		= 0;

	MaxLen		= 0;
}

void MakeRingBuffer(WAVEFORMATEX fmt)
{
	while(DelayInWrite)
				DoDelayProc();

	if (pRingBufferL != NULL)
	{
		free(pRingBufferL);
		pRingBufferL = NULL;
	}

	if (pRingBufferR != NULL)
	{
		free(pRingBufferR);
		pRingBufferR = NULL;
	}

	switch(fmt.wBitsPerSample)
	{
		case 8:
			MaxLen	= fmt.nSamplesPerSec * 2;
			switch(fmt.nChannels)
			{
				case 1:
					pRingBufferL = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT8));
					break;
				case 2:
					pRingBufferL = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT8));
					pRingBufferR = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT8));					
					break;
			}
			break;
		case 16:
			MaxLen	= fmt.nSamplesPerSec * 2;			
			switch(fmt.nChannels)
			{
				case 1:
					pRingBufferL = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT16));					
					break;
				case 2:
					pRingBufferL = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT16));					
					pRingBufferR = (INT8*)calloc(MaxLen + DEF_BUFF_MARGIN, sizeof(INT16));										
					break;
			}
			break;
	}
	BufferLenL	= 0;
	BufferLenR	= 0;
	OffSetL		= 0;
	OffSetR		= 0;
}

void SetBufferData(WAVEBUFFER* pBuffer, int Len, WAVEBUFFER *pRing, int *offset, WAVEFORMATEX fmt)
{
	int i, idx;
	INT16 *pI16Buff, *pTmp16Ring;
	idx = *offset;
	
	DelayInWrite = true;
	switch(fmt.wBitsPerSample)
	{
		case 8:
			for(i = 0 ; i < Len ; i++)
			{
				pRing[idx] = pBuffer[i];
				idx++;
			}
			break;
		case 16:
			pI16Buff	= (INT16*)pBuffer;
			pTmp16Ring	= (INT16*)pRing;
			for(i = 0 ; i < Len ; i++)
			{
				pTmp16Ring[idx] = pI16Buff[i];
				idx++;					
			}
			break;
	}	
	*offset = idx;
	DelayInWrite = false;
}

void GetBufferData(WAVEBUFFER *pBuffer, int Len, WAVEBUFFER *pRing, int delay, WAVEFORMATEX fmt)
{
	int		i, idx;
	INT16	*pI16Buff, *pTmp16Buff;

	while(DelayInWrite)
				DoDelayProc();

	switch(fmt.nChannels)
	{
		case 1:
			break;
		case 2:
			pTmp16Buff	= (INT16*)pRing;
			pI16Buff	= (INT16*)pBuffer;
			for(i = 0 ; i < Len ; i++)
			{
				
			}
			break;
	}

}