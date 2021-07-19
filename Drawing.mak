# Microsoft Developer Studio Generated NMAKE File, Based on Drawing.dsp
!IF "$(CFG)" == ""
CFG=Drawing - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Drawing - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Drawing - Win32 Release" && "$(CFG)" !=\
 "Drawing - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Drawing.mak" CFG="Drawing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Drawing - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Drawing - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Drawing - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Drawing.exe"

!ELSE 

ALL : "$(OUTDIR)\Drawing.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Drawing.obj"
	-@erase "$(INTDIR)\Drawing.pch"
	-@erase "$(INTDIR)\Drawing.res"
	-@erase "$(INTDIR)\DrawingDlg.obj"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Drawing.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\Drawing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Drawing.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Drawing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Drawing.pdb" /machine:I386 /out:"$(OUTDIR)\Drawing.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Drawing.obj" \
	"$(INTDIR)\Drawing.res" \
	"$(INTDIR)\DrawingDlg.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Drawing - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Drawing.exe"

!ELSE 

ALL : "$(OUTDIR)\Drawing.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Drawing.obj"
	-@erase "$(INTDIR)\Drawing.pch"
	-@erase "$(INTDIR)\Drawing.res"
	-@erase "$(INTDIR)\DrawingDlg.obj"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Drawing.exe"
	-@erase "$(OUTDIR)\Drawing.ilk"
	-@erase "$(OUTDIR)\Drawing.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /Fp"$(INTDIR)\Drawing.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Drawing.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Drawing.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=WaveFile.Lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Drawing.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Drawing.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Drawing.obj" \
	"$(INTDIR)\Drawing.res" \
	"$(INTDIR)\DrawingDlg.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\Drawing.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Drawing - Win32 Release" || "$(CFG)" ==\
 "Drawing - Win32 Debug"
SOURCE=.\Drawing.cpp

!IF  "$(CFG)" == "Drawing - Win32 Release"

DEP_CPP_DRAWI=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	

"$(INTDIR)\Drawing.obj" : $(SOURCE) $(DEP_CPP_DRAWI) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ELSEIF  "$(CFG)" == "Drawing - Win32 Debug"

DEP_CPP_DRAWI=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	

"$(INTDIR)\Drawing.obj" : $(SOURCE) $(DEP_CPP_DRAWI) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ENDIF 

SOURCE=.\Drawing.rc
DEP_RSC_DRAWIN=\
	".\res\Drawing.ico"\
	".\res\Drawing.rc2"\
	

"$(INTDIR)\Drawing.res" : $(SOURCE) $(DEP_RSC_DRAWIN) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DrawingDlg.cpp

!IF  "$(CFG)" == "Drawing - Win32 Release"

DEP_CPP_DRAWING=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	

"$(INTDIR)\DrawingDlg.obj" : $(SOURCE) $(DEP_CPP_DRAWING) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ELSEIF  "$(CFG)" == "Drawing - Win32 Debug"

DEP_CPP_DRAWING=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	".\Global.H"\
	{$(INCLUDE)}"WaveFile.h"\
	

"$(INTDIR)\DrawingDlg.obj" : $(SOURCE) $(DEP_CPP_DRAWING) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ENDIF 

SOURCE=.\Global.CPP

!IF  "$(CFG)" == "Drawing - Win32 Release"

DEP_CPP_GLOBA=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	".\Global.H"\
	".\StdAfx.h"\
	{$(INCLUDE)}"WaveFile.h"\
	

"$(INTDIR)\Global.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ELSEIF  "$(CFG)" == "Drawing - Win32 Debug"

DEP_CPP_GLOBA=\
	".\Drawing.h"\
	".\DrawingDlg.h"\
	".\Global.H"\
	{$(INCLUDE)}"WaveFile.h"\
	

"$(INTDIR)\Global.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"\
 "$(INTDIR)\Drawing.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Drawing - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\Drawing.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Drawing.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Drawing - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\Drawing.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Drawing.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

