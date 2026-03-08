#include <stdio.h>
#include "moviecodecs.h"

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
static moviecodecs_intf_t const *get_interface()
{
    fprintf(stderr, "Loading library...\n");
    HMODULE lib = LoadLibraryA(".\\moviecodecs.dll");
    if (lib == NULL)
    {
        fprintf(stderr, "Failed to load moviecodecs.dll\n");
        return NULL;
    }

    fprintf(stderr, "Looking up symbol...\n");
    moviecodecs_intf_func func = (moviecodecs_intf_func)GetProcAddress(lib, "codec_interface");
    if (func == NULL)
    {
        fprintf(stderr, "Failed to find codec_interface\n");
        return NULL;
    }

    fprintf(stderr, "Calling function @ %p...\n", func);
    moviecodecs_intf_t const *result = (*func)();
    fprintf(stderr, "Result = %p\n", result);
    return result;
}

#else

#include <dlfcn.h>
static moviecodecs_intf_t const *get_interface()
{
#ifdef __APPLE__
    char const *libname = "./moviecodecs.dylib";
#else
    char const *libname = "./moviecodecs.so";
#endif

    fprintf(stderr, "Loading library...\n");
    void *lib = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
    if (lib == NULL)
    {
        fprintf(stderr, "Failed to load %s:%s\n", libname, dlerror());
        return NULL;
    }

    fprintf(stderr, "Looking up symbol...\n");
    moviecodecs_intf_func func = (moviecodecs_intf_func)dlsym(lib, "codec_interface");
    if (func == NULL)
    {
        fprintf(stderr, "Failed to find codec_interface\n");
        return NULL;
    }

    fprintf(stderr, "Calling function @ %p...\n", func);
    moviecodecs_intf_t const *result = (*func)();
    fprintf(stderr, "Result = %p\n", result);
    return result;
}

#endif

int main(int argc, char *argv[])
{
    moviecodecs_intf_t const *intf = get_interface();
    if (intf == NULL)
        return 1;

    fprintf(stderr, "Creating IV41 codec\n");
    moviecodecs_context_t *codec = intf->create(MOVIECODECS_FOURCC('I','V','4','1'), 320, 240, 0, NULL, 0);
    if (codec == NULL)
    {
        fprintf(stderr, "Failed to initialize IV41 code\n");
        return 1;
    }

    fprintf(stderr, "Success\n");
    return 0;
}
