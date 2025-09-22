#pragma once
#include <cstdint>
#include "im2d.h"
#include "RgaUtils.h"

int RGA_handle_convert(void *image, void *resized_image, size_t src_width, size_t src_height, int src_format = RK_FORMAT_RGB_888, int dst_format = RK_FORMAT_RGB_565);

int convert_color_from_file_v1(void *src_buf, void *dst_buf, int src_width, int src_height, 
                               int src_format = RK_FORMAT_RGB_888, int dst_format = RK_FORMAT_RGB_565);

int convert_color_from_file_v2(const char *path, void **dst_buf, int *dst_size, int src_width, int src_height,
                               int src_format = RK_FORMAT_RGBA_8888, int dst_format = RK_FORMAT_RGB_565);

