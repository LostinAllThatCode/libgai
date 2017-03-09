@echo off
set libgai=..\gai
set compilerflags=/nologo -O2 -MTd -EHsc -Z7 -I%libgai%
set linkerflags=-incremental:no -opt:ref opengl32.lib

del *.pdb > NUL 2> NUL

if "%1"=="dll" (
	echo building .dll and .lib file from libgai and link the executable with those
	cl %compilerflags% -DGAI_OPENGL_DEBUG -DGAI_EXPORT ../gai/gai_core_win32.cc /LD /link /IMPLIB:libgai.lib /OUT:libgai.dll
	cl %compilerflags% opengl_win32.cc /link %linkerflags% libgai.lib /OUT:opengl_win32_dyn.exe	
	goto :end
) 

if "%1"=="static" (
	echo standard build with static linking
	cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
	goto :end
)

if "%1"=="partial" (
	cl %compilerflags% -DGAI_SOURCE game.cc /LD /link /OUT:game.dll -PDB:game%random%.pdb
	cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
	goto :end
)

:end
