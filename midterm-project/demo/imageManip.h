// imageManip.h
// Jason Kurlander jkurlan3
// Teng-Ju Yang tyang28

#ifndef IMAGEMANIPH
#define IMAGEMANIPH

#include <stdio.h>
#include "ppm_io.h"

#define PI  (double)3.14159265358979323846

Image *swap(Image *im);

Image *grayscale(Image *im);

Image *contrast(Image *im, double cont);

unsigned char ccolor(unsigned char color, double cont);

Image *zoom_in(Image *im);

Image *zoom_out(Image *im);

Image *occlude(Image *im, int x1, int y1, int x2, int y2);

Image *blur(Image *im, double sigma);

double sq(double s);

void freeim(Image *im);

#endif
