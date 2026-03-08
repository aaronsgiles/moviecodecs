@call makelib-win.cmd

@if not exist obj\test mkdir obj\test
@set DREAMM=..\dreamm\src
cl test.cpp /Zi /Foobj\test\ %DREAMM%\lib\zlib\adler32.c %DREAMM%\lib\zlib\crc32.c %DREAMM%\lib\zlib\deflate.c %DREAMM%\lib\zlib\inffast.c %DREAMM%\lib\zlib\inftrees.c %DREAMM%\lib\zlib\inflate.c %DREAMM%\lib\zlib\trees.c %DREAMM%\lib\zlib\zutil.c %DREAMM%\lib\fmt\src\format.cc /EHsc /std:c++17 -Iinclude -I%DREAMM%\lib -I%DREAMM%\lib\fmt\include -I%DREAMM%\lib\zlib
