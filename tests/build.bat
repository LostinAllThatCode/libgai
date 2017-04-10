@echo off
REM TODO: Check if -EHa- causes the crash handling to not work properly anymore!

set libgai=..\gai
set compilerflags=-nologo -fp:fast -Gm- -GR- -O2 -MT -EHa- -Z7 -WX -W4 -wd4100 -wd4201 -I%libgai%
set linkerflags=-incremental:no -opt:ref user32.lib gdi32.lib opengl32.lib winmm.lib

del *.pdb > NUL 2> NUL

if "%1"=="dll" (
	echo building .dll and .lib file from libgai and link the executable with those
	cl %compilerflags% -DGAI_OPENGL_DEBUG -DGAI_EXPORT ../gai/gai_core_win32.cc /LD /link /IMPLIB:libgai.lib /OUT:libgai.dll
	cl %compilerflags% opengl_win32.cc /link %linkerflags% libgai.lib /OUT:opengl_win32_dyn.exe	
	goto :end
) 

if "%1"=="static" (
	echo standard build with static linking
	REM cl %compilerflags% opengl_win32.cc /link %linkerflags%
	cl %compilerflags% renderer.cc /LD /link /OUT:renderer.dll -PDB:renderer%random%.pdb -incremental:no -opt:ref
	cl %compilerflags% renderer_test.cc /link %linkerflags%
	goto :end
)

if "%1"=="partial" (
	cl %compilerflags% -DGAI_SOURCE game.cc /LD /link /OUT:game.dll -PDB:game%random%.pdb
	cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
	goto :end
)

:end
