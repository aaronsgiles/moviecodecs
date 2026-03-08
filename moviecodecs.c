//=========================================================
//  moviecodecs.c
//
//  Abstraction interface for libavcodec DLL
//  (c) ASG HC1 LLC
//=========================================================

#define BUILDING_DLL
#include "moviecodecs.h"

#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"

//
// full codec structure
//
struct context_internal_t
{
    moviecodecs_context_t public;
    AVCodecContext *context;
    AVFrame *frame;
};
typedef struct context_internal_t context_internal_t;

//
// convert Y/Cr/Cb values to RGB
// TODO: smooth out the Cr/Cb components
//
uint32_t ycrcb_to_rgb(uint8_t y, uint8_t _cr, uint8_t _cb)
{
    int32_t cr = (int32_t)_cr - 128;
    int32_t cb = (int32_t)_cb - 128;
    int32_t r = y + cr * 1402 / 1000;
    if (r < 0) r = 0; if (r > 255) r = 255;
    int32_t g = y - cb * 344 / 1000 - cr * 714 / 1000;
    if (g < 0) g = 0; if (g > 255) g = 255;
    int32_t b = y + cb * 1772 / 1000;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return (r << 16) | (g << 8) | b;
}

//
// convert a YUV4:1:0 frame to RGB
//
void yuv410_to_rgb(AVFrame *frame, moviecodecs_output_t *dest)
{
    int effheight = (dest->height < frame->height) ? dest->height : frame->height;
    int effwidth = (dest->width < frame->width) ? dest->width : frame->width;

    uint32_t *dptr = dest->dest;
    for (int y = 0; y < effheight; y++)
    {
        uint8_t *ydata = frame->data[0] + frame->linesize[0] * y;
        uint8_t *cbdata = frame->data[1] + frame->linesize[1] * (y/4);
        uint8_t *crdata = frame->data[2] + frame->linesize[2] * (y/4);
        for (int x = 0; x < effwidth; x++)
        {
            uint8_t yy = ydata[x];
            uint8_t cb = cbdata[x/4];
            uint8_t cr = crdata[x/4];
            dptr[x] = ycrcb_to_rgb(yy, cr, cb);
        }
        dptr += dest->rowpixels;
    }
}

//
// convert a PAL8 frame to RGB
//
void pal8_to_rgb(AVFrame *frame, moviecodecs_output_t *dest)
{
    int effheight = (dest->height < frame->height) ? dest->height : frame->height;
    int effwidth = (dest->width < frame->width) ? dest->width : frame->width;
    uint32_t const *pal = (uint32_t const *)frame->data[1];

    uint32_t *dptr = dest->dest;
    for (int y = 0; y < effheight; y++)
    {
        uint8_t *pixdata = frame->data[0] + frame->linesize[0] * y;
        for (int x = 0; x < effwidth; x++)
            dptr[x] = pal[pixdata[x]];
        dptr += dest->rowpixels;
    }
}

//
// convert an RGB5-5-5 frame to RGB
//
void rgb555_to_rgb(AVFrame *frame, moviecodecs_output_t *dest)
{
    int effheight = (dest->height < frame->height) ? dest->height : frame->height;
    int effwidth = (dest->width < frame->width) ? dest->width : frame->width;

    uint32_t *dptr = dest->dest;
    for (int y = 0; y < effheight; y++)
    {
        uint16_t *pixdata = (uint16_t *)(frame->data[0] + frame->linesize[0] * y);
        for (int x = 0; x < effwidth; x++)
        {
            uint16_t raw = pixdata[x];
            uint8_t r = (raw & 0x7c00) >> 10;
            uint8_t g = (raw & 0x03e0) >> 5;
            uint8_t b = (raw & 0x001f) >> 0;
            dptr[x] = (r << 19) | (g << 11) | (b << 3);
        }
        dptr += dest->rowpixels;
    }
}

//
// locate and create a new code instance for the 'fourcc' code
//
// 'width' and 'height' are optional, and can be useful if known from the container
// 'config' and 'configsize' are optional pointers to configuration data from the container
//
static moviecodecs_context_t *codec_create(uint32_t fourcc, uint32_t width, uint32_t height, uint32_t depth, void *config, uint32_t configsize)
{
    // see if it's one we known
    enum AVCodecID id = AV_CODEC_ID_NONE;
    switch (fourcc)
    {
        case MOVIECODECS_FOURCC('I','V','3','2'): id = AV_CODEC_ID_INDEO3; break;
        case MOVIECODECS_FOURCC('I','V','4','1'): id = AV_CODEC_ID_INDEO4; break;
        case MOVIECODECS_FOURCC('m','s','v','c'): id = AV_CODEC_ID_MSVIDEO1; break;
    }

    // fail if we can't
    if (id == AV_CODEC_ID_NONE)
        return NULL;

    // find the decoder
    AVCodec const *codec = avcodec_find_decoder(id);
    if (codec == NULL)
        return NULL;

    // allocate a context
    AVCodecContext *context = avcodec_alloc_context3(codec);
    if (context == NULL)
        return NULL;

    // configure the context
    context->width = width;
    context->height = height;
    context->bits_per_coded_sample = depth;
    context->extradata = config;
    context->extradata_size = configsize;

    // open the codec
    int res = avcodec_open2(context, NULL, NULL);
    if (res != 0)
    {
        avcodec_free_context(&context);
        return NULL;
    }

    // create a frame
    AVFrame *frame = av_frame_alloc();
    if (frame == NULL)
    {
        avcodec_free_context(&context);
        return NULL;
    }

    // allocate a final struct to return
    context_internal_t *result = (context_internal_t *)calloc(1, sizeof(context_internal_t));
    result->public.width = context->width;
    result->public.height = context->height;
    result->context = context;
    result->frame = frame;
    return &result->public;
}

//
// decode a frame using 'codec' into an RGB frame buffer
//
// 'data' and 'datasize' describe the compressed input data
// 'argbdest' and 'rowpixels' describe the output data
//
static int codec_decode_rgb(moviecodecs_context_t *codec, uint8_t *data, uint32_t datasize, moviecodecs_output_t *dest)
{
    context_internal_t *codecfull = (context_internal_t *)codec;

    // create a packet
    AVPacket *packet = av_packet_alloc();
    if (packet == NULL)
        return 0;
    av_new_packet(packet, datasize);
    memcpy(packet->buf->data, data, datasize);

    // send the packet to the decoder
    int res = avcodec_send_packet(codecfull->context, packet);
    if (res != 0)
        return 0;

    // get the frame back
    res = avcodec_receive_frame(codecfull->context, codecfull->frame);
    if (res != 0)
        return 0;

    // free the packet
    av_packet_free(&packet);

    // convert based on pixel format
    switch (codecfull->frame->format)
    {
        case AV_PIX_FMT_YUV410P:    ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
            yuv410_to_rgb(codecfull->frame, dest);
            return 1;

        case AV_PIX_FMT_PAL8:       ///< 8 bits with AV_PIX_FMT_RGB32 palette
            pal8_to_rgb(codecfull->frame, dest);
            return 1;

        case AV_PIX_FMT_RGB555:     ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), little-endian, X=unused/undefined
            rgb555_to_rgb(codecfull->frame, dest);
            return 1;
    }

    // no decoder
    return 0;
}

//
// clean up a codec when finished
//
static void codec_destroy(moviecodecs_context_t *codec)
{
    context_internal_t *codecfull = (context_internal_t *)codec;

    // drain the output
    if (codecfull->context != NULL && codecfull->frame != NULL)
    {
        avcodec_send_packet(codecfull->context, NULL);
        while (avcodec_receive_frame(codecfull->context, codecfull->frame) != AVERROR_EOF) ;
    }

    // free the bits
    av_frame_free(&codecfull->frame);
    avcodec_free_context(&codecfull->context);

    // free the codec
    free(codecfull);
}

//
// return the interface pointers
//
moviecodecs_intf_t const *codec_interface()
{
    static moviecodecs_intf_t const intf =
    {
        &codec_create,
        &codec_decode_rgb,
        &codec_destroy
    };
    return &intf;
}
