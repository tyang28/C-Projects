// imageManip.c
//601.220, Fall 2018

#include <stdio.h>
#include "ppm_io.h"
#include "imageManip.h"
#include <stdlib.h>

Image *swap(Image *im) {
  for(int i = 0; i < im->rows * im->cols; i++) {
    int temp_red = im->data[i].r;
    im->data[i].r = im->data[i].g;
    im->data[i].g = im->data[i].b;
    im->data[i].b = temp_red;
  }
  return im;
}

Image *grayscale(Image *im) {

  for(int i = 0; i < im->rows * im->cols; i++) {
    int intensity = (0.3 * im->data[i].r) + (0.59 * im->data[i].g) + (0.11 * im->data[i].b);
    im->data[i].r = intensity;
    im->data[i].g = intensity;
    im->data[i].b = intensity;
  }
  return im;
}

Image *contrast(Image *im, double cont) {
  
  for(int i = 0; i < im->rows * im->cols; i++) {
    im->data[i].r = ccolor(im->data[i].r, cont);
    im->data[i].g = ccolor(im->data[i].g, cont);
    im->data[i].b = ccolor(im->data[i].b, cont);
  }

  return im;
}

unsigned char ccolor(unsigned char color, double cont) {
  double new = (double)color;
  new /= 255;
  new -= .5;
  new *= cont;
  new += .5;
  new *=255;
  if(new > 255) {
    new = 255;
  }
  if(new < 0) {
    new = 0;
  }

  return (unsigned char)new;
}
  
  

Image *zoom_in(Image *im) {
  Image *zoom = malloc(sizeof(Image));
  zoom->rows = im->rows * 2;
  zoom->cols = im->cols * 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols * zoom->rows);

  for(int i = 0; i < zoom->rows * zoom->cols; i++) {
    int row = i / zoom->cols;
    int col = i % zoom->cols;
    int copy_row = (row + 1)/2;
    int copy_col = (col + 1)/2;
    int copy_index = (copy_row * im->cols) + copy_col;
    zoom->data[i].r = im->data[copy_index].r;
    zoom->data[i].b = im->data[copy_index].b;
    zoom->data[i].g = im->data[copy_index].g;
  }
  return zoom;
}

Image *zoom_out(Image *im) {
  Image *zoom = malloc(sizeof(Image));
  zoom->rows = im->rows / 2;
  zoom->cols = im->cols / 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols *zoom->rows);

  for(int i = 0; i < zoom->rows * zoom->cols; i++) {
    int r = i / zoom->cols;
    int c = i % zoom->cols;
    Pixel topright = (im->data[r * im->cols * 2 + 2*c + 1]);
    Pixel topleft = (im->data[r * im->cols * 2 + 2*c]);
    Pixel bottomright = (im->data[(2*r+1) * im->cols+ 2*c + 1]);
    Pixel bottomleft = (im->data[(2*r+1) * im->cols + 2*c]);

    char avgr = (topright.r + topleft.r + bottomright.r + bottomleft.r)/4;
    char avgg = (topright.g + topleft.g + bottomright.g + bottomleft.g)/4;
    char avgb = (topright.b + topleft.b + bottomright.b + bottomleft.b)/4;

    zoom->data[i].r = avgr;
    zoom->data[i].g = avgg;
    zoom->data[i].b = avgb;
    
    
  }

  return zoom;
}

Image *occlude(Image *im, int x1, int y1, int x2, int y2) {

  if(x1 < 0 || y1 < 0 || x2 >= im->cols || y2 >= im->rows ||
     x1 >= x2 || y1 >= y2) {
    return NULL;
  }

  for(int i = y1*im->cols + x1; i <= y2*im->cols + x2; i++) {
    int r = i / im->cols;
    int c = i % im->cols;
    if(r>=y1 && r<=y2 && c>=x1 && c<=x2) {
      im->data[i].r = 0;
      im->data[i].g = 0;
      im->data[i].b = 0;
    }
  }

  return im;
}
