// imageManip.h
//601.220, Fall 2018

#ifndef IMAGEMANIPH
#define IMAGEMANIPH

#include <stdio.h>
#include "ppm_io.h"

Image *swap(Image *im);

Image *grayscale(Image *im);

Image *contrast(Image *im, double cont);

unsigned char ccolor(unsigned char color, double cont);

Image *zoom_in(Image *im);

Image *zoom_out(Image *im);

Image *occlude(Image *im, int x1, int y1, int x2, int y2);

#endif
