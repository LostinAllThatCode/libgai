@echo off

set libgai=..\gai
set compilerflags=-Od -MTd -EHsc -Z7 -I%libgai%
set linkerflags=-incremental:no -opt:ref

REM del *.pdb > NUL 2> NUL

if "%1"=="dll" (
	echo "Building dynamically linked executable."
	REM build .dll and .lib file from libgai and link the executable with those
	cl %compilerflags% -DGAI_EXPORT ../gai/gai_core_win32.cc /LD /link /IMPLIB:libgai.lib /OUT:libgai.dll
	cl %compilerflags% opengl_win32.cc /link %linkerflags% libgai.lib opengl32.lib /OUT:opengl_win32_dyn.exe
) else (
	echo "Building statically linked executable."
	cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
)

if not %errorlevel%==0 (
	echo "%0(0): error: sublime regex sucks: Please build from console since sublime is a bitch!"
)