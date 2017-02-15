@echo off
set libgai=..\gai
set compilerflags=/nologo -Od -MTd -EHsc -Z7 -I%libgai%
set linkerflags=-incremental:no -opt:ref opengl32.lib

REM del *.pdb > NUL 2> NUL

if "%1"=="dll" (
	echo building .dll and .lib file from libgai and link the executable with those
	cl %compilerflags% -DGAI_EXPORT ../gai/gai_core_win32.cc /LD /link /IMPLIB:libgai.lib /OUT:libgai.dll
	cl %compilerflags% opengl_win32.cc /link %linkerflags% libgai.lib /OUT:opengl_win32_dyn.exe	
) else (
	echo standard build with static linking
	cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
)