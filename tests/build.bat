@echo off

set libgai=..\gai
set compilerflags=-Od -MTd -EHsc -Z7 -FC -I%LibGAI%
set linkerflags=-incremental:no -opt:ref

etags *.*
etags -a %LibGAI%\*.*

del *.pdb > NUL 2> NUL

cl %CompilerFlags% opengl_win32.cc /link %LinkerFlags%
clang -O0 -Wno-deprecated-declarations -I%LibGAI% opengl_win32.cc -o opengl_win32_clang.exe
