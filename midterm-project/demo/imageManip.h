// imageManip.h
//601.220, Fall 2018

#ifndef IMAGEMANIPH
#define IMAGEMANIPH

#include <stdio.h>
#include "ppm_io.h"

Image *swap(Image *im);

Image *grayscale(Image *im);

Image *zoom_in(Image *im);

Image *zoom_out(Image *im);

#endif
