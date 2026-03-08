//=========================================================
//  moviecodecs.h
//
//  Abstraction interface for libavcodec DLL
//  (c) ASG HC1 LLC
//=========================================================

#pragma once

#include <stdint.h>

#ifndef ASG_MOVIECODECS_H
#define ASG_MOVIECODECS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_DLL
#ifdef _MSC_VER
#define ASG_EXPORT __declspec(dllexport)
#else
#define ASG_EXPORT __attribute__ ((visibility("default")))
#endif
#else
#define ASG_EXPORT
#endif

//
// helper to make a FOURCC code
//
#define MOVIECODECS_FOURCC(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

//
// public codec interface
//
struct moviecodecs_context_t
{
    uint32_t width;
    uint32_t height;
};
typedef struct moviecodecs_context_t moviecodecs_context_t;

//
// frame description
//
struct moviecodecs_output_t
{
    uint32_t *dest;
    uint32_t width;
    uint32_t height;
    uint32_t rowpixels;
};
typedef struct moviecodecs_output_t moviecodecs_output_t;

//
// public DLL interface
//
struct moviecodecs_intf_t
{
    //
    // locate and create a new code instance for the 'fourcc' code
    //
    // 'width' and 'height' are optional, and can be useful if known from the container
    // 'config' and 'configsize' are optional pointers to configuration data from the container
    //
    moviecodecs_context_t *(*create)(uint32_t fourcc, uint32_t width, uint32_t height, uint32_t depth, void *config, uint32_t configsize);

    //
    // decode a frame using 'codec' into an RGB frame buffer
    //
    // 'data' and 'datasize' describe the compressed input data
    // 'argbdest' and 'rowpixels' describe the output data
    //
    int (*decode_rgb)(moviecodecs_context_t *codec, uint8_t *data, uint32_t datasize, moviecodecs_output_t *dest);

    //
    // clean up a codec when finished
    //
    void (*destroy)(moviecodecs_context_t *codec);
};
typedef struct moviecodecs_intf_t moviecodecs_intf_t;

//
// return a pointer to the codec interface; this is the only export in the DLL
//
ASG_EXPORT moviecodecs_intf_t const *codec_interface();
typedef moviecodecs_intf_t const *(*moviecodecs_intf_func)();

#ifdef __cplusplus
}
#endif

#endif