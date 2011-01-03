#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cv.h>

bool is_black(const IplImage *image);

bool is_freezing(const IplImage *last, const IplImage *cur);

bool is_mosaic(const IplImage *image);

void save_image(const IplImage *image, const char* prefix);

#endif
