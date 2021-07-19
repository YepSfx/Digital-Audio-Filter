#include "stdafx.h"
#include "Global.h"
#include <math.h>
#include <mmsystem.h>

#define		ZERO8BIT		-128

CWaveFile	*pWaveFile;
CWaveOut	*pWaveOut;

bool		playing = false;
int			NumBuff;
int			FFT;
int			BufferLen;
LONGINT		FileIdx = 0;

void ProcWaveData(WAVEBUFFER *pBuffer, int DataLen, WAVEFORMATEX fmt)
{
	DoFilter(pBuffer, DataLen, fmt);
}

void SetFFTSize(int idx)
{
	FFT = idx;
}

void DoEvent()
{
	MSG		msg;
	
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

LONGINT	GetDataPosition(WAVEFORMATEX fmt, LONGINT nPos)
{
	LONGINT	ans;
	int		mod;

	switch(fmt.nChannels)
	{
		case 1:
			switch(fmt.wBitsPerSample)
			{
				case 8: 
					ans = nPos;
					break;
				case 16:  
					mod = nPos%2;
					
					if (mod != 0)
						ans = nPos - mod;
					else
						ans = nPos;
					break;
			}
			break;
		case 2:
			switch(fmt.wBitsPerSample)
			{
				case 8: 
					mod = nPos%2;
					
					if (mod != 0)
						ans = nPos - mod;
					else
						ans = nPos;
					break;
				case 16: 					
					mod = nPos%4;
					
					if (mod != 0)
						ans = nPos - mod;
					else
						ans = nPos;
					break;
			}
			break;
	}

	if (ans < 0)
			ans = 0;

	return ans;
}

int GetFFTByte(int idx, WAVEFORMATEX fmt)
{
	int fft, ans;

	fft = FFTSIZE[idx];
	
	switch(fmt.nChannels){
		case 1:
			if (fmt.wBitsPerSample == 16)
				ans = fft * 2;
			else 
				ans = fft;
			break;
		case 2:
			if (fmt.wBitsPerSample == 16)
				ans = fft * 4;
			else 
				ans = fft * 2;
			break;
	}

	return ans;
}

bool isPlaying()
{
	return playing;
}

bool isFileRead()
{
	if (pWaveFile->GetAccessStatus() == FILE_READ)
		return true;
	else 
		return false;
}

bool isEndOfPlaying()
{
	if ((pWaveFile->GetCurrentPosition() >= pWaveFile->GetTotalWaveLength())
		&&
		(pWaveOut->GetBufferCount() == 0))
	{
		playing = false;
		return true;
	}
	return false;
}

void SetFilePos(LONGINT nPos)
{
	LONGINT	k;
	pWaveFile->SetPosition(nPos);

	k = pWaveFile->GetCurrentPosition();
}

LONGINT GetFilePos()
{
	return pWaveFile->GetCurrentPosition();
}

bool SetAudioFile(char* FileName)
{
	pWaveFile->SetFile(NULL,FILE_CLOSE);

	return pWaveFile->SetFile(FileName, FILE_READ);
}

void OpenPort(WAVEFORMATEX fmt)
{
	pWaveOut->SetWaveFormat(fmt);
	pWaveOut->SetDeviceStatus(DEV_OPEN);
}

void ClosePort()
{
	pWaveOut->SetDeviceStatus(DEV_CLOSE);
}

bool CloseAudioFile()
{
	return pWaveFile->SetFile(NULL,FILE_CLOSE);
}

WAVEFORMATEX GetWaveFormat()
{
	return pWaveFile->GetWaveFormat();
}

void SetNumBuffer(int NumBuffer)
{
	NumBuff = NumBuffer;
}

int GetNumBuffer()
{
	return NumBuff;
}

void InitAudioDev(CWnd *pParent)
{
	pWaveFile = new CWaveFile(pParent);

	pWaveOut  = new CWaveOut; 
	pWaveOut->Create(pParent);

	pWaveOut->OnWaveDataDone = OnWaveDataDone;
}

void CloseAudioDev()
{
	delete pWaveFile;
	DoEvent();
	delete pWaveOut;
}

LONGINT	GetWaveLength()
{
	return pWaveFile->GetTotalWaveLength();
}

int ReadFile(WAVEBUFFER *pBuffer, int Len, bool isFiltering)
{
	int				ans, i, rtn;
	LONGINT			CurPos;
	WAVEFORMATEX	fmt;

	CurPos	= pWaveFile->GetCurrentPosition(); 
	fmt		= pWaveFile->GetWaveFormat();
	
	switch(isFiltering){
		case true:
			ans = pWaveFile->Read(pBuffer, Len);											

			if (ans < Len)
			{
				switch(fmt.wBitsPerSample){
					case 8:
						for (i = ans ; i < Len ; i++)
								pBuffer[i] = ZERO8BIT; 
						break;
					case 16:
						for (i = ans ; i < Len ; i++)
								pBuffer[i] = 0; 
						break;
				}
			}

			//remove Dummy Data
			if ((CurPos + ans) > GetWaveLength())
					ans = GetWaveLength() - CurPos;

			if (fmt.wBitsPerSample == 16)
				rtn = ans / 2;
			else 
				rtn = ans;
	
			ProcWaveData(pBuffer, rtn, fmt);

			break;
		case false:
			ans = pWaveFile->Read(pBuffer, Len);			

			if (ans < Len)
			{
				switch(fmt.wBitsPerSample){
					case 8:
						for (i = ans ; i < Len ; i++)
								pBuffer[i] = ZERO8BIT; 
						break;
					case 16:
						for (i = ans ; i < Len ; i++)
								pBuffer[i] = 0; 
						break;
				}
			}
			//remove Dummy Data			
			if ((CurPos + ans) > GetWaveLength())
					ans = GetWaveLength() - CurPos;

			if (fmt.wBitsPerSample == 16)
				rtn = ans / 2;
			else 
				rtn = ans;
			
			break;
	}
	
	//remove Dummy at end of file

	return rtn;
}

void MakeCurrentDisplayData(WAVEBUFFER *pBuffer, int Len, CWnd *pParent)
{
	WAVEBUFFER		*pTrans;
	WAVEDATA		*pWaveData;
	WAVEFORMATEX	fmt;
	FFTBUFFER		*pFFTBufferL, *pFFTBufferR;
	int				i;

	fmt			=  pWaveFile->GetWaveFormat();
	pTrans		= (WAVEBUFFER*)malloc(Len);
	pWaveData	= (  WAVEDATA*)malloc(sizeof(WAVEDATA));
	pFFTBufferL	= ( FFTBUFFER*)calloc(FFTSIZE[FFT] * 2, sizeof(FFTBUFFER));
	pFFTBufferR	= ( FFTBUFFER*)calloc(FFTSIZE[FFT] * 2, sizeof(FFTBUFFER));	
	
	for(i=0 ; i < Len ; i++)
	{
		pTrans[i] = pBuffer[i];
	}
	
	FileIdx += Len;

	//Normalize to 1Ch data.
	if (fmt.wBitsPerSample == 16)
		Len = Len / 2;

	pWaveData->pBuffer		= pTrans;
	pWaveData->CurPos		= FileIdx;
	pWaveData->DataLen		= Len;
	pWaveData->fmt			= fmt;
	pWaveData->BufferSize	= BufferLen;
	pWaveData->fftSize		= FFTSIZE[FFT];
	pWaveData->pFFTBufferL	= pFFTBufferL;
	pWaveData->pFFTBufferR	= pFFTBufferR;

	pParent->PostMessage(WM_TRANS_WAVEBUFFER, 0 , (LPARAM)pWaveData);
}

void OnWaveDataDone(WAVEBUFFER *pBuffer, int Len, CWnd *pParent)
{
	MMRESULT		ExRsult;
	WAVEBUFFER		*pThisBuffer;
	WAVEFORMATEX	fmt;
	int				ans;

	if (playing == false)		//When Stop is callled.
	{
		MakeCurrentDisplayData(pBuffer, Len, pParent);
		return;
	}

	if (pWaveFile->GetCurrentPosition() >= pWaveFile->GetTotalWaveLength())
	{
		playing = false;
		MakeCurrentDisplayData(pBuffer, Len, pParent);
		pParent->PostMessage(WM_PLAY_END, 0 , (LPARAM)pWaveFile->GetTotalWaveLength());		
		return;
	}
	
	fmt = pWaveFile->GetWaveFormat();
	pThisBuffer = (WAVEBUFFER*)malloc(Len);

	if (pWaveOut->GetDeviceStatus() == DEV_OPEN)
	{
		if (pWaveFile->GetCurrentPosition() < pWaveFile->GetTotalWaveLength())
		{
			ans = ReadFile(pThisBuffer,  Len, GetFilterStatus());
			
			if (fmt.wBitsPerSample == 16)
								ans = ans * 2;
			
			pWaveOut->SetHDR(pThisBuffer, ans, &ExRsult);
		}
	}
	free(pThisBuffer);
	
	MakeCurrentDisplayData(pBuffer, Len, pParent);
}

LONGINT SetStop()
{
	playing = false;
	
	return FileIdx;
}

void SetPlay(LONGINT idx, int DataLen)
{
	int				i, ans, len;
	WAVEBUFFER		*pBuffer;
	MMRESULT		ExRsult;
	WAVEFORMATEX	fmt;

	if (playing == true)
			return;

	BufferLen = DataLen;
	
	fmt = pWaveFile->GetWaveFormat();
	
	FileIdx = idx;// - DataLen * NumBuff;

	pWaveFile->SetPosition(idx);
	pBuffer = (WAVEBUFFER*)malloc(DataLen);
	for( i = 0; i < NumBuff ; i++)
	{
		ans		= ReadFile(pBuffer, DataLen, GetFilterStatus());		
		if (fmt.wBitsPerSample == 16)
			len = ans * 2;
		else 
			len = ans;
		
		pWaveOut->SetHDR(pBuffer, len, &ExRsult);
//		FileIdx -= ans;
		
		if (pWaveFile->GetCurrentPosition() >= pWaveFile->GetTotalWaveLength())
					break;
	}
	free(pBuffer);
	playing = true;
}

int CheckAudioBuffer()
{
	return pWaveOut->GetBufferCount();
}

bool isPortClose()
{
	if (pWaveOut->GetDeviceStatus() == DEV_CLOSE)
		return true;
	else
		return false;
}


