// DrawingDlg.h : header file
//

#if !defined(AFX_DRAWINGDLG_H__07BC6586_3769_11D7_B568_00E0290F46BF__INCLUDED_)
#define AFX_DRAWINGDLG_H__07BC6586_3769_11D7_B568_00E0290F46BF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define		WM_TRANS_WAVEBUFFER		WM_USER+127
#define		WM_PLAY_END				WM_USER+126

typedef long		LONGINT;


/////////////////////////////////////////////////////////////////////////////
// CDrawingDlg dialog

class CDrawingDlg : public CDialog
{
// Construction
public:
	void SetCoeff();
	void SetFilter();
	CDrawingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawingDlg)
	enum { IDD = IDD_DRAWING_DIALOG };
	CListBox	m_coeff;
	CEdit	m_bqzdamp;
	CEdit	m_bqpdamp;
	CEdit	m_bqanti;
	CEdit	m_bqreson;
	CEdit	m_bqgain;
	CEdit	m_gaina;
	CEdit	m_gainb;
	CEdit	m_len;
	CEdit	m_freq4;
	CEdit	m_freq3;
	CEdit	m_freqhi;
	CEdit	m_freqlo;
	CComboBox	m_fil;
	CComboBox	m_win;
	CStatic	m_data;
	CEdit	m_numbuff;
	CStatic	m_Read;
	CSpinButtonCtrl	m_LenSpinCtrl;
	CScrollBar	m_TimeWinScroll;
	CStatic	m_Ch;
	CStatic	m_Pos;
	CStatic	m_Sam;
	CStatic	m_Res;
	CButton	m_FileOpen;
	CButton m_RadioR;
	CButton m_RadioL;
	CButton	m_RadioMag;
	CButton m_RadioPha;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDrawingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnClose();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRadioLClick();
	afx_msg void OnRadioRClick();
	afx_msg void OnButton3();
	afx_msg void OnButton2();
	afx_msg void OnButton4();
	afx_msg void OnRadiomag();
	afx_msg void OnRadiopha();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton5();
	afx_msg void OnSelchangeSelwin();
	afx_msg void OnSelchangeSelfil();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ClearDisplay();
	bool m_FileLoad;
	int				m_FileIdx;			//File Position in Byte
	int				m_FFTsize;
	LONGINT			m_FileSize;
	int				m_ChSelect;
	int				m_FFTtype;
	void*			m_pFFTData;	
	void*			m_pMovingFFTL;
	void*			m_pMovingFFTR;
	int				m_buffs;
	UINT			m_TimerID;
	void			UpdateWavWindow();
	void			UpdateFFTWindow();
	void			OnLenSpinCtrl(UINT nPos);
	void			OnTimeWinScroll(LONGINT nPos);
	void			MouseMove(CPoint point); 
	void			PutFFTData(void* pData);
	void			DisplayPlayingFFT();
	bool			m_TimerProceed;
	bool			m_MemAccess;
	bool			m_MakingFilter;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGDLG_H__07BC6586_3769_11D7_B568_00E0290F46BF__INCLUDED_)
