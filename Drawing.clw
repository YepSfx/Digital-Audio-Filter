; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutBox
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Drawing.h"

ClassCount=3
Class1=CDrawingApp
Class2=CDrawingDlg

ResourceCount=4
Resource2=IDD_DRAWING_DIALOG
Resource1=IDR_MAINFRAME
Resource3=IDD_ABOUT
Class3=CAboutBox
Resource4=IDD_DRAWING_DIALOG (English (U.S.))

[CLS:CDrawingApp]
Type=0
HeaderFile=Drawing.h
ImplementationFile=Drawing.cpp
Filter=N

[CLS:CDrawingDlg]
Type=0
HeaderFile=DrawingDlg.h
ImplementationFile=DrawingDlg.cpp
Filter=W
LastObject=CDrawingDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_DRAWING_DIALOG]
Type=1
Class=CDrawingDlg
ControlCount=2
Control1=IDC_SCROLLTIME,scrollbar,1342177280
Control2=IDC_TIMEWINDOW,,1342242816

[DLG:IDD_DRAWING_DIALOG (English (U.S.))]
Type=1
Class=CDrawingDlg
ControlCount=59
Control1=IDC_SCROLLTIME,scrollbar,1342177280
Control2=IDC_TIMEWINDOW,static,1342177543
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_FILEPOS,static,1342308352
Control5=IDC_LENGTH,msctls_updown32,1342177312
Control6=IDC_READSIZE,static,1350701057
Control7=IDC_STATIC,static,1342308352
Control8=IDC_SAMPLE,static,1342308352
Control9=IDC_RES,static,1342308352
Control10=IDC_STEREO,static,1342308352
Control11=IDC_STATIC,button,1342309127
Control12=IDC_RADIOL,button,1342177289
Control13=IDC_RADIOR,button,1342177289
Control14=IDC_FFTWINDOW,static,1342177287
Control15=IDC_BUTTON2,button,1342242816
Control16=IDC_BUTTON3,button,1342242816
Control17=IDC_EDIT1,edit,1350639745
Control18=IDC_BUTTON4,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,button,1342309127
Control23=IDC_RADIOMAG,button,1342177289
Control24=IDC_RADIOPHA,button,1342177289
Control25=IDC_DATA,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_SELWIN,combobox,1344339970
Control28=IDC_STATIC,static,1342308352
Control29=IDC_SELFIL,combobox,1344339970
Control30=IDC_LOFREQ,edit,1350631552
Control31=IDC_HIFREQ,edit,1350631552
Control32=IDC_BUTTON5,button,1342242816
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,static,1342308352
Control37=IDC_FREQ4,edit,1350631552
Control38=IDC_STATIC,static,1342308352
Control39=IDC_FREQ3,edit,1350631552
Control40=IDC_STATIC,static,1342308352
Control41=IDC_FILLEN,edit,1350639744
Control42=IDC_GAINA,edit,1350631552
Control43=IDC_STATIC,static,1342308352
Control44=IDC_GAINB,edit,1350631552
Control45=IDC_STATIC,static,1342308352
Control46=IDC_STATIC,static,1342308352
Control47=IDC_BQGAIN,edit,1350631552
Control48=IDC_STATIC,static,1342308352
Control49=IDC_BQRES,edit,1350631552
Control50=IDC_BQANT,edit,1350631552
Control51=IDC_STATIC,static,1342308352
Control52=IDC_STATIC,static,1342308352
Control53=IDC_PDAMP,edit,1350631552
Control54=IDC_STATIC,static,1342308352
Control55=IDC_ZDAMP,edit,1350631552
Control56=IDC_STATIC,static,1342308352
Control57=IDC_LIST1,listbox,1352728833
Control58=IDC_STATIC,static,1342308352
Control59=IDC_BUTTON6,button,1342242816

[DLG:IDD_ABOUT]
Type=1
Class=CAboutBox
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STATIC,static,1350572558

[CLS:CAboutBox]
Type=0
HeaderFile=AboutBox.h
ImplementationFile=AboutBox.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK

