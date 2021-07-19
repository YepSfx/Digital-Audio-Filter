// Drawing.h : main header file for the DRAWING application
//

#if !defined(AFX_DRAWING_H__07BC6584_3769_11D7_B568_00E0290F46BF__INCLUDED_)
#define AFX_DRAWING_H__07BC6584_3769_11D7_B568_00E0290F46BF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawingApp:
// See Drawing.cpp for the implementation of this class
//

class CDrawingApp : public CWinApp
{
public:
	CDrawingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDrawingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWING_H__07BC6584_3769_11D7_B568_00E0290F46BF__INCLUDED_)
