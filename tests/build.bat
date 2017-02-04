echo off

set libgai=..\gai
set compilerflags=-Od -MTd -EHsc -Z7 -FC -I%libgai%
set linkerflags=-incremental:no -opt:ref

REM del *.pdb > NUL 2> NUL

if "%1"=="dll" (
	echo "Building dynamically linked executable."
	REM build .dll and .lib file from libgai and link the executable with those
	cl %compilerflags% -DGAI_EXPORT ../gai/gai_core_win32.cc /LD /link /IMPLIB:libgai.lib /OUT:libgai.dll
	
	if "%2"=="clang" (
		set ogl="C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\OpenGL32.Lib"
		clang -O0 -Wno-deprecated-declarations -I%libgai% opengl_win32.cc libgai.lib %ogl% -o opengl_win32_dyn_clang.exe
	) else (
		cl %compilerflags% opengl_win32.cc /link %linkerflags% libgai.lib opengl32.lib /OUT:opengl_win32_dyn.exe
	)

) else (
	echo "Building statically linked executable."
	if "%1"=="clang" (
		clang -DGAI_SOURCE -O0 -Wno-deprecated-declarations -I%libgai% opengl_win32.cc -o opengl_win32.exe
	) else (
		cl %compilerflags% -DGAI_SOURCE opengl_win32.cc /link %linkerflags%
	)
)