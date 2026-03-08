
// build neutrally
#define ARCH_X86 0
#define ARCH_X86_32 0
#define ARCH_X86_64 0
#define ARCH_ARM 0
#define ARCH_AARCH64 0
#define ARCH_LOONGARCH 0
#define ARCH_LOONGARCH64 0
#define ARCH_RISCV 0
#define ARCH_MIPS 0
#define ARCH_PPC 0
#define HAVE_MIPSFPU 0
#define HAVE_BIGENDIAN 0
#define HAVE_MMX_INLINE 0

// yes, we are 64-bit only; it's fast
#define HAVE_FAST_64BIT 1

// common math functions; assume we have them all
#define HAVE_COPYSIGN 1
#define HAVE_ATANF 1
#define HAVE_ATAN2F 1
#define HAVE_POWF 1
#define HAVE_CBRT 1
#define HAVE_CBRTF 1
#define HAVE_COPYSIGN 1
#define HAVE_COSF 1
#define HAVE_ERF 1
#define HAVE_EXPF 1
#define HAVE_EXP2 1
#define HAVE_EXP2F 1
#define HAVE_ISINF 1
#define HAVE_HYPOT 1
#define HAVE_LDEXPF 1
#define HAVE_LRINT 1
#define HAVE_LRINTF 1
#define HAVE_LLRINT 1
#define HAVE_LLRINTF 1
#define HAVE_LOG2 1
#define HAVE_LOG2F 1
#define HAVE_LOG10F 1
#define HAVE_SINF 1
#define HAVE_RINT 1
#define HAVE_ROUND 1
#define HAVE_ROUNDF 1
#define HAVE_TRUNC 1
#define HAVE_TRUNCF 1
#define HAVE_ISNAN 1
#define HAVE_ISFINITE 1
#define HAVE_SIMD_ALIGN_64 1
#define HAVE_SIMD_ALIGN_32 1
#define HAVE_SIMD_ALIGN_16 1

// not sure if this is important, but general header file only defines GNU versions
#define HAVE_FAST_CLZ 0

// removes definitions
#define CONFIG_SMALL 1

// don't explicitly build as a shared library; we specify our own exports
#define CONFIG_SHARED 0

// seems prudent so we don't crash
#define CONFIG_SAFE_BITSTREAM_READER 1

// no GPU or hardware support
#define CONFIG_CUDA 0
#define CONFIG_D3D11VA 0
#define CONFIG_D3D12VA 0
#define CONFIG_LIBDRM 0
#define CONFIG_DXVA2 0
#define CONFIG_OPENCL 0
#define CONFIG_QSV 0
#define CONFIG_VAAPI 0
#define CONFIG_VDPAU 0
#define CONFIG_VIDEOTOOLBOX 0
#define CONFIG_MEDIACODEC 0
#define CONFIG_VULKAN 0
#define CONFIG_AMF 0
#define CONFIG_OHCODEC 0

// don't want internal threading
#define HAVE_PRCTL 0
#define HAVE_THREADS 0
#define HAVE_PTHREADS 0
#define HAVE_PTHREAD_SETNAME_NP 0
#define HAVE_PTHREAD_SET_NAME_NP 0
#define HAVE_W32THREADS 0
#define HAVE_OS2THREADS 0
#define CONFIG_FRAME_THREAD_ENCODER 0

// don't care about enabling deprecated warnings
#define HAVE_PRAGMA_DEPRECATED 0

// file-related operations
#define HAVE_LIBC_MSVCRT 0

// doesn't matter because we don't deal with files
#define HAVE_DOS_PATHS 0

// enable the null BSF
#define CONFIG_NULL_BSF 1

// misc other stuff
#define CONFIG_MEMORY_POISONING 0
#define CONFIG_GRAY 0
#define CONFIG_VALGRIND_BACKTRACE 0
#define CONFIG_LIBLCEVC_DEC 0
#define CONFIG_LCMS2 0
#define HAVE_STRERROR_R 0
#define HAVE_UNISTD_H 0
#define HAVE_IO_H 0
#define HAVE_SETCONSOLETEXTATTRIBUTE 0
#define HAVE_ISATTY 0
#define HAVE_GETTIMEOFDAY 0
#define HAVE_WINDOWS_H 0
#define HAVE_GETTIMEOFDAY 0
#define HAVE_GETSYSTEMTIMEASFILETIME 0
#define HAVE_CLOCK_GETTIME 0
#define HAVE_CLOCK_GETTIME 0
#define HAVE_NANOSLEEP 0
#define HAVE_USLEEP 0
#define HAVE_SLEEP 0
#define HAVE_BCRYPT 0
#define HAVE_ARC4RANDOM_BUF 0
#define CONFIG_GCRYPT 0
#define CONFIG_OPENSSL 0
#define CONFIG_LINUX_PERF 0
#define CONFIG_MACOS_KPERF 0
#define HAVE_MACH_ABSOLUTE_TIME 0
#define HAVE_GETHRTIME 0
#define CONFIG_ICONV 0
#define CONFIG_MPEGVIDEOENCDSP 0
#define CONFIG_ME_CMP 0
#define CONFIG_OSSFUZZ 0
#define HAVE_MALLOC_H 0
#define HAVE_POSIX_MEMALIGN 0
#define HAVE_ALIGNED_MALLOC 0
#define HAVE_MEMALIGN 0
#define HAVE_VALGRIND_VALGRIND_H 0
#define HAVE_GMTIME_R 0
#define HAVE_LOCALTIME_R 0
#define CONFIG_SNOW_ENCODER 0

#include "libavutil/internal.h"
#include "libavutil/intmath.h"
