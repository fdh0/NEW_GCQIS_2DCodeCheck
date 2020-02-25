# Microsoft Developer Studio Project File - Name="AlgLab2DCodeCheck" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AlgLab2DCodeCheck - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AlgLab2DCodeCheck.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AlgLab2DCodeCheck.mak" CFG="AlgLab2DCodeCheck - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AlgLab2DCodeCheck - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AlgLab2DCodeCheck - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AlgLab2DCodeCheck - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "AlgLab2DCodeCheck - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AlgLab2DCodeCheck - Win32 Release"
# Name "AlgLab2DCodeCheck - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Check.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeModelDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\NoteTypeInfoDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrLearnDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg3.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg4.cpp
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg5.cpp
# End Source File
# Begin Source File

SOURCE=.\ParaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgLab2DCodeCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgLab2DCodeCheck.def
# End Source File
# Begin Source File

SOURCE=.\AlgLab2DCodeCheck.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlgVirtualBaseClass.h
# End Source File
# Begin Source File

SOURCE=.\Check.h
# End Source File
# Begin Source File

SOURCE=.\CodeModelDlg1.h
# End Source File
# Begin Source File

SOURCE=.\inc\dhtemplate.h
# End Source File
# Begin Source File

SOURCE=.\NoteTypeInfoDlg1.h
# End Source File
# Begin Source File

SOURCE=.\OcrLearnDlg1.h
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg1.h
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg2.h
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg3.h
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg4.h
# End Source File
# Begin Source File

SOURCE=.\OcrParaSetupDlg5.h
# End Source File
# Begin Source File

SOURCE=.\ParaDlg.h
# End Source File
# Begin Source File

SOURCE=.\PropDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\AlgLab2DCodeCheck.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AlgLab2DCodeCheck.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section AlgLab2DCodeCheck : {C3E23EED-5399-467A-A7DB-73BF8926074F}
# 	1:16:IDD_DIALOG_SCALE:1029
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CCodeScaleDlg:CCodeScaleDlg
# 	2:19:CLASS: CCodeQualDlg:CCodeQualDlg
# 	2:23:CLASS: COcrParaSetupDlg:COcrParaSetupDlg
# 	2:16:IDD_DIALOG_SCALE:IDD_DIALOG_SCALE
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CCodeThreshDlg:CCodeThreshDlg
# 	2:17:OcrParaSetupDlg.h:OcrParaSetupDlg4.h
# 	2:20:CLASS: CCodeExcurDlg:CCodeExcurDlg
# 	2:19:OcrParaSetupDlg.cpp:OcrParaSetupDlg4.cpp
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:21:CLASS: CUpdateCodeDlg:CUpdateCodeDlg
# End Section
# Section AlgLab2DCodeCheck : {67D5C106-1836-4C8E-BD63-B9315904D781}
# 	1:18:IDD_DIALOG_NEWTYPE:1012
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_DIALOG_NEWTYPE:IDD_DIALOG_NEWTYPE
# 	2:19:NoteTypeInfoDlg.cpp:NoteTypeInfoDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:NoteTypeInfoDlg.h:NoteTypeInfoDlg1.h
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:23:CLASS: CNoteTypeInfoDlg:CNoteTypeInfoDlg
# End Section
# Section AlgLab2DCodeCheck : {8353AFE7-7CC7-422B-92E3-6DE6CD372EC1}
# 	1:22:IDD_DIALOG_OCRLEARNDLG:1011
# 	2:16:Resource Include:resource.h
# 	2:13:OcrLearnDlg.h:OcrLearnDlg1.h
# 	2:15:OcrLearnDlg.cpp:OcrLearnDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:22:IDD_DIALOG_OCRLEARNDLG:IDD_DIALOG_OCRLEARNDLG
# 	2:19:CLASS: COcrLearnDlg:COcrLearnDlg
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# End Section
# Section AlgLab2DCodeCheck : {86075F80-E077-45A4-8F16-3D155E16462C}
# 	1:18:IDD_DIALOG_FACTDOT:1010
# 	2:16:Resource Include:resource.h
# 	2:16:TYPEDEF: LINEVAL:LINEVAL
# 	2:16:CodeModelDlg.cpp:CodeModelDlg1.cpp
# 	2:14:CodeModelDlg.h:CodeModelDlg1.h
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CCodeModelDlg:CCodeModelDlg
# 	2:26:CLASS: s_CodeMoreInkRegion:s_CodeMoreInkRegion
# 	2:18:IDD_DIALOG_FACTDOT:IDD_DIALOG_FACTDOT
# 	2:28:TYPEDEF: s_CodeMoreInkRegion:s_CodeMoreInkRegion
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:14:CLASS: LINEVAL:LINEVAL
# End Section
# Section AlgLab2DCodeCheck : {52D0E3F5-5BCC-420F-BAAA-7365CDB33AE1}
# 	1:20:IDD_DIALOG_PARASETUP:1014
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CCodeScaleDlg:CCodeScaleDlg
# 	2:19:CLASS: CCodeQualDlg:CCodeQualDlg
# 	2:23:CLASS: COcrParaSetupDlg:COcrParaSetupDlg
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CCodeThreshDlg:CCodeThreshDlg
# 	2:17:OcrParaSetupDlg.h:OcrParaSetupDlg1.h
# 	2:20:CLASS: CCodeExcurDlg:CCodeExcurDlg
# 	2:20:IDD_DIALOG_PARASETUP:IDD_DIALOG_PARASETUP
# 	2:19:OcrParaSetupDlg.cpp:OcrParaSetupDlg1.cpp
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:21:CLASS: CUpdateCodeDlg:CUpdateCodeDlg
# End Section
# Section AlgLab2DCodeCheck : {C5B20E54-BE1E-4C87-80C3-A20500C7C3AA}
# 	1:17:IDD_DIALOG_THRESH:1030
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CCodeScaleDlg:CCodeScaleDlg
# 	2:19:CLASS: CCodeQualDlg:CCodeQualDlg
# 	2:23:CLASS: COcrParaSetupDlg:COcrParaSetupDlg
# 	2:17:IDD_DIALOG_THRESH:IDD_DIALOG_THRESH
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CCodeThreshDlg:CCodeThreshDlg
# 	2:17:OcrParaSetupDlg.h:OcrParaSetupDlg5.h
# 	2:20:CLASS: CCodeExcurDlg:CCodeExcurDlg
# 	2:19:OcrParaSetupDlg.cpp:OcrParaSetupDlg5.cpp
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:21:CLASS: CUpdateCodeDlg:CUpdateCodeDlg
# End Section
# Section AlgLab2DCodeCheck : {6B405EAD-6279-4FE6-9113-C62320164FE9}
# 	1:15:IDD_DIALOG_PRAR:1009
# 	2:16:Resource Include:resource.h
# 	2:15:CLASS: CParaDlg:CParaDlg
# 	2:10:ENUM: enum:enum
# 	2:9:ParaDlg.h:ParaDlg.h
# 	2:11:ParaDlg.cpp:ParaDlg.cpp
# 	2:15:IDD_DIALOG_PRAR:IDD_DIALOG_PRAR
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# End Section
# Section AlgLab2DCodeCheck : {E455ED92-331E-4AEE-8929-1E59FF7DB1B4}
# 	1:22:IDD_DIALOG_QUALITYPARA:1028
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_DIALOG_QUALITYPARA:IDD_DIALOG_QUALITYPARA
# 	2:20:CLASS: CCodeScaleDlg:CCodeScaleDlg
# 	2:19:CLASS: CCodeQualDlg:CCodeQualDlg
# 	2:23:CLASS: COcrParaSetupDlg:COcrParaSetupDlg
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CCodeThreshDlg:CCodeThreshDlg
# 	2:17:OcrParaSetupDlg.h:OcrParaSetupDlg3.h
# 	2:20:CLASS: CCodeExcurDlg:CCodeExcurDlg
# 	2:19:OcrParaSetupDlg.cpp:OcrParaSetupDlg3.cpp
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:21:CLASS: CUpdateCodeDlg:CUpdateCodeDlg
# End Section
# Section AlgLab2DCodeCheck : {A3413702-996A-456A-8C44-7BA362AB5BE3}
# 	1:20:IDD_DIALOG_EXCURSION:1027
# 	2:16:Resource Include:resource.h
# 	2:20:IDD_DIALOG_EXCURSION:IDD_DIALOG_EXCURSION
# 	2:20:CLASS: CCodeScaleDlg:CCodeScaleDlg
# 	2:19:CLASS: CCodeQualDlg:CCodeQualDlg
# 	2:23:CLASS: COcrParaSetupDlg:COcrParaSetupDlg
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CCodeThreshDlg:CCodeThreshDlg
# 	2:17:OcrParaSetupDlg.h:OcrParaSetupDlg2.h
# 	2:20:CLASS: CCodeExcurDlg:CCodeExcurDlg
# 	2:19:OcrParaSetupDlg.cpp:OcrParaSetupDlg2.cpp
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# 	2:21:CLASS: CUpdateCodeDlg:CUpdateCodeDlg
# End Section
# Section AlgLab2DCodeCheck : {15FC8684-3129-4B01-BA15-D4C3866A0357}
# 	1:15:IDD_DIALOG_PROP:1007
# 	2:16:Resource Include:resource.h
# 	2:15:CLASS: CPropDlg:CPropDlg
# 	2:10:ENUM: enum:enum
# 	2:11:PropDlg.cpp:PropDlg.cpp
# 	2:9:PropDlg.h:PropDlg.h
# 	2:15:IDD_DIALOG_PROP:IDD_DIALOG_PROP
# 	2:19:Application Include:AlgLab2DCodeCheck.h
# End Section
