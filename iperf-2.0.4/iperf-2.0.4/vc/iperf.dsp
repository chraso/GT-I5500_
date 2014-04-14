# Microsoft Developer Studio Project File - Name="iperf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=iperf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "iperf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "iperf.mak" CFG="iperf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "iperf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "iperf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "iperf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "iperf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "HAVE_CONFIG_H" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "iperf - Win32 Release"
# Name "iperf - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\compat\delay.cpp
# End Source File
# Begin Source File

SOURCE=..\compat\error.c
# End Source File
# Begin Source File

SOURCE=..\src\Extractor.c
# End Source File
# Begin Source File

SOURCE=..\compat\gettimeofday.c
# End Source File
# Begin Source File

SOURCE=..\src\gnu_getopt.c
# End Source File
# Begin Source File

SOURCE=..\src\gnu_getopt_long.c
# End Source File
# Begin Source File

SOURCE=..\compat\headers_slim.h
# End Source File
# Begin Source File

SOURCE=..\compat\inet_ntop.c
# End Source File
# Begin Source File

SOURCE=..\compat\inet_pton.c
# End Source File
# Begin Source File

SOURCE=..\src\Launch.cpp
# End Source File
# Begin Source File

SOURCE=..\src\List.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Listener.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Locale.c
# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\src\PerfSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ReportCSV.c
# End Source File
# Begin Source File

SOURCE=..\src\ReportDefault.c
# End Source File
# Begin Source File

SOURCE=..\src\Reporter.c
# End Source File
# Begin Source File

SOURCE=..\src\Server.cpp
# End Source File
# Begin Source File

SOURCE=..\src\service.c
# End Source File
# Begin Source File

SOURCE=..\src\Settings.cpp
# End Source File
# Begin Source File

SOURCE=..\compat\signal.c
# End Source File
# Begin Source File

SOURCE=..\compat\snprintf.c
# End Source File
# Begin Source File

SOURCE=..\src\SocketAddr.c
# End Source File
# Begin Source File

SOURCE=..\src\sockets.c
# End Source File
# Begin Source File

SOURCE=..\src\stdio.c
# End Source File
# Begin Source File

SOURCE=..\compat\string.c
# End Source File
# Begin Source File

SOURCE=..\src\tcp_window_size.c
# End Source File
# Begin Source File

SOURCE=..\compat\Thread.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\Condition.h
# End Source File
# Begin Source File

SOURCE=..\include\config.h
# End Source File
# Begin Source File

SOURCE=..\include\Extractor.h
# End Source File
# Begin Source File

SOURCE=..\include\gettimeofday.h
# End Source File
# Begin Source File

SOURCE=..\include\gnu_getopt.h
# End Source File
# Begin Source File

SOURCE=..\include\headers.h
# End Source File
# Begin Source File

SOURCE=..\include\inet_aton.h
# End Source File
# Begin Source File

SOURCE="..\include\iperf-int.h"
# End Source File
# Begin Source File

SOURCE=..\include\List.h
# End Source File
# Begin Source File

SOURCE=..\include\Locale.h
# End Source File
# Begin Source File

SOURCE=..\include\Mutex.h
# End Source File
# Begin Source File

SOURCE=..\include\report_CSV.h
# End Source File
# Begin Source File

SOURCE=..\include\report_default.h
# End Source File
# Begin Source File

SOURCE=..\include\Reporter.h
# End Source File
# Begin Source File

SOURCE=..\include\service.h
# End Source File
# Begin Source File

SOURCE=..\include\snprintf.h
# End Source File
# Begin Source File

SOURCE=..\include\SocketAddr.h
# End Source File
# Begin Source File

SOURCE=..\include\Thread.h
# End Source File
# Begin Source File

SOURCE=..\include\util.h
# End Source File
# Begin Source File

SOURCE=..\include\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
