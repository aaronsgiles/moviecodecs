@setlocal ENABLEDELAYEDEXPANSION

@rem determine platform
@set PLATFORM=windows

@rem debug build
@set DEBUG=0
@if "%1"=="debug" set DEBUG=1
@if "%1"=="debug" shift

@rem determine target architecture; from command line if provided
@set ARCH=
@if not "%1"=="" @set ARCH=%1
@if "%ARCH%"=="" @set ARCH=%VSCMD_ARG_TGT_ARCH%
@if "%ARCH%"=="" @echo Couldn't determine architecture&goto :eof

@rem object directory
@set OBJDIR=obj\%PLATFORM%\%ARCH%

@rem tools
@set CC=
@set LD=
@for /f "usebackq delims=" %%i in (`where cl.exe`) do @if "!CC!"=="" @set CC="%%~i"
@for /f "usebackq delims=" %%i in (`where link.exe`) do @if "!LD!"=="" @set LD="%%~i"
@if "%ARCH%"=="arm64" set CC=%CC:\x64=\arm64%
@if "%ARCH%"=="arm64" set LD=%LD:\x64=\arm64%

@rem target name
@set TARGET=moviecodecs.dll

@rem compiler options
@set COPTS=/nologo /c /FIlibbuild\fi.h /std:c11 /experimental:c11atomics /I. /I.\libavcodec /I.\libbuild /Zi /we4668
@if "%DEBUG%"=="0" set COPTS=%COPTS% /Oxs
@if "%DEBUG%"=="1" set COPTS=%COPTS% /Od

@rem link options
@set LDOPTS=/nologo /dll /debug /opt:icf /opt:ref
@if "%ARCH%"=="arm64" @set LIB=%LIB:x64=arm64%

@rem libavcodec files
@set CODECFILES=
@set CODECFILES=%CODECFILES% libavcodec\aasc.c
@set CODECFILES=%CODECFILES% libavcodec\allcodecs.c
@set CODECFILES=%CODECFILES% libavcodec\avcodec.c
@set CODECFILES=%CODECFILES% libavcodec\bitstream_filters.c
@set CODECFILES=%CODECFILES% libavcodec\blockdsp.c
@set CODECFILES=%CODECFILES% libavcodec\bsf.c
@set CODECFILES=%CODECFILES% libavcodec\bsf\null.c
@set CODECFILES=%CODECFILES% libavcodec\codec_desc.c
@set CODECFILES=%CODECFILES% libavcodec\codec_par.c
@set CODECFILES=%CODECFILES% libavcodec\decode.c
@set CODECFILES=%CODECFILES% libavcodec\encode.c
@set CODECFILES=%CODECFILES% libavcodec\error_resilience.c
@set CODECFILES=%CODECFILES% libavcodec\get_buffer.c
@set CODECFILES=%CODECFILES% libavcodec\h264chroma.c
@set CODECFILES=%CODECFILES% libavcodec\hpeldsp.c
@set CODECFILES=%CODECFILES% libavcodec\idctdsp.c
@set CODECFILES=%CODECFILES% libavcodec\ivi.c
@set CODECFILES=%CODECFILES% libavcodec\ivi_dsp.c
@set CODECFILES=%CODECFILES% libavcodec\indeo2.c
@set CODECFILES=%CODECFILES% libavcodec\indeo3.c
@set CODECFILES=%CODECFILES% libavcodec\indeo4.c
@set CODECFILES=%CODECFILES% libavcodec\indeo5.c
@set CODECFILES=%CODECFILES% libavcodec\jrevdct.c
@set CODECFILES=%CODECFILES% libavcodec\lcevcdec.c
@set CODECFILES=%CODECFILES% libavcodec\mathtables.c
@set CODECFILES=%CODECFILES% libavcodec\me_cmp.c
@set CODECFILES=%CODECFILES% libavcodec\mpeg_er.c
@set CODECFILES=%CODECFILES% libavcodec\mpeg12.c
@set CODECFILES=%CODECFILES% libavcodec\mpeg12data.c
@set CODECFILES=%CODECFILES% libavcodec\mpeg12dec.c
@set CODECFILES=%CODECFILES% libavcodec\mpeg12framerate.c
@set CODECFILES=%CODECFILES% libavcodec\mpegpicture.c
@set CODECFILES=%CODECFILES% libavcodec\mpegutils.c
@set CODECFILES=%CODECFILES% libavcodec\mpegvideo.c
@set CODECFILES=%CODECFILES% libavcodec\mpegvideo_dec.c
@set CODECFILES=%CODECFILES% libavcodec\mpegvideodata.c
@set CODECFILES=%CODECFILES% libavcodec\mpegvideo_motion.c
@set CODECFILES=%CODECFILES% libavcodec\msrledec.c
@set CODECFILES=%CODECFILES% libavcodec\msvideo1.c
@set CODECFILES=%CODECFILES% libavcodec\options.c
@set CODECFILES=%CODECFILES% libavcodec\packet.c
@set CODECFILES=%CODECFILES% libavcodec\simple_idct.c
@set CODECFILES=%CODECFILES% libavcodec\threadprogress.c
@set CODECFILES=%CODECFILES% libavcodec\utils.c
@set CODECFILES=%CODECFILES% libavcodec\videodsp.c
@set CODECFILES=%CODECFILES% libavcodec\vlc.c

@rem libavcodec objs
@set CODECOBJS=%CODECFILES:.c=.obj%
@if "%ARCH%"=="x64" @set CODECOBJS=%CODECOBJS:libav=obj\windows\x64\libav%
@if "%ARCH%"=="arm64" @set CODECOBJS=%CODECOBJS:libav=obj\windows\arm64\libav%
@set CODECOBJS=%CODECOBJS:bsf\=%

@rem libavutil files
@set UTILFILES=
@set UTILFILES=%UTILFILES% libavutil\avstring.c
@set UTILFILES=%UTILFILES% libavutil\avsscanf.c
@set UTILFILES=%UTILFILES% libavutil\bprint.c
@set UTILFILES=%UTILFILES% libavutil\buffer.c
@set UTILFILES=%UTILFILES% libavutil\channel_layout.c
@set UTILFILES=%UTILFILES% libavutil\container_fifo.c
@set UTILFILES=%UTILFILES% libavutil\dict.c
@set UTILFILES=%UTILFILES% libavutil\error.c
@set UTILFILES=%UTILFILES% libavutil\eval.c
@set UTILFILES=%UTILFILES% libavutil\fifo.c
@set UTILFILES=%UTILFILES% libavutil\frame.c
@set UTILFILES=%UTILFILES% libavutil\hwcontext.c
@set UTILFILES=%UTILFILES% libavutil\imgutils.c
@set UTILFILES=%UTILFILES% libavutil\log.c
@set UTILFILES=%UTILFILES% libavutil\log2_tab.c
@set UTILFILES=%UTILFILES% libavutil\mastering_display_metadata.c
@set UTILFILES=%UTILFILES% libavutil\mathematics.c
@set UTILFILES=%UTILFILES% libavutil\mem.c
@set UTILFILES=%UTILFILES% libavutil\opt.c
@set UTILFILES=%UTILFILES% libavutil\parseutils.c
@set UTILFILES=%UTILFILES% libavutil\pixdesc.c
@set UTILFILES=%UTILFILES% libavutil\random_seed.c
@set UTILFILES=%UTILFILES% libavutil\rational.c
@set UTILFILES=%UTILFILES% libavutil\refstruct.c
@set UTILFILES=%UTILFILES% libavutil\reverse.c
@set UTILFILES=%UTILFILES% libavutil\samplefmt.c
@set UTILFILES=%UTILFILES% libavutil\sha.c
@set UTILFILES=%UTILFILES% libavutil\side_data.c
@set UTILFILES=%UTILFILES% libavutil\stereo3d.c
@set UTILFILES=%UTILFILES% libavutil\time.c
@set UTILFILES=%UTILFILES% libavutil\timecode.c
@set UTILFILES=%UTILFILES% libavutil\timecode_internal.c
@set UTILFILES=%UTILFILES% libavutil\utils.c
@set UTILFILES=%UTILFILES% libavutil\video_enc_params.c

@rem libavutil objs
@set UTILOBJS=%UTILFILES:.c=.obj%
@if "%ARCH%"=="x64" @set UTILOBJS=%UTILOBJS:libav=obj\windows\x64\libav%
@if "%ARCH%"=="arm64" @set UTILOBJS=%UTILOBJS:libav=obj\windows\arm64\libav%

@rem build test code
@if not exist %OBJDIR% mkdir %OBJDIR%
@%CC% /nologo /Zi mctest.c /Fo%OBJDIR%\ /Femctest.exe || goto :eof

@rem build wrapper
@if not exist %OBJDIR% mkdir %OBJDIR%
@%CC% %COPTS% /Fo%OBJDIR%\ moviecodecs.c || goto :eof

@rem build avcodec files
@if not exist %OBJDIR%\libavcodec mkdir %OBJDIR%\libavcodec
@%CC% %COPTS% /Fo%OBJDIR%\libavcodec\ %CODECFILES% || goto :eof

@rem build avutil files
@if not exist %OBJDIR%\libavutil mkdir %OBJDIR%\libavutil
@%CC% %COPTS% /Fo%OBJDIR%\libavutil\ %UTILFILES% || goto :eof

@rem build library
@echo Linking...
@%LD% %LDOPTS% /out:%TARGET% %OBJDIR%\moviecodecs.obj %CODECOBJS% %UTILOBJS%
