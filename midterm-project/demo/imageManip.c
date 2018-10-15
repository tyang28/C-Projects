// imageManip.c
//601.220, Fall 2018

#include <stdio.h>

Image *swap(Image *im) {
  for(int i = 0; i < im->rows * im->cols; i++) {
    int temp_red = im->data[i].r
    im->data[i].r = im->data[i].g;
    im->data[i].g = im->data[i].b;
    im->data[i].b = temp_red;
  }
  return im;
}

Image *grayscale(Image *im) {

  for(int i = 0; i < im->rows * im->cols; i++) {
    int intensity = (0.3 * im->data->r) + (0.59 * im->data->g) + (0.11 * im-\
>data->b);
    im->data[i].r = intensity;
    im->data[i].g = intensity;
    im->data[i].b = intensity;
  }
  return im;
}

Image *zoom_in(Image *im) {
  Image *zoom = malloc(sizeof(Image));
  zoom->rows = im->rows * 2;
  zoom->cols = im->cols * 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols * zoom->rows);

  for(int i = 0; i < im->rows * im->cols; i++) {
    int row = i / zoom->cols;
    int col = i % zoom->cols;
    int copy_row = (row + 1)/2;
    int copy_col = (col + 1)/2;
    int copy_index = (copy_row * im->cols) + copy_col;
    zoom->data[i].r = im->data[copy_index].r;
    zoom->data[i].b = im->data[copy_index].b;
    zoom->data[i].g = im->data[copy_index].g;
  }
  return im;
}

Image *zoom_out(Image *im) {
  Image *zoom = malloc(sizeof(Image) * 2);
  zoom->rows = im->rows / 2;
  zoom->cols = im->rows / 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols *zoom->rows);

  for(int i = 0; i < zoom->rows * zoom->cols; i++) {
    int r = i / zoom->cols;
    Pixel topright = *(im->data[r * im->cols * 2]);
    Pixel topleft = *(im->data[r * im->cols * 2 - 1]);
    Pixel bottomright = *(im->data[(r+1) * im->cols * 2]);
    Pixel bottomleft = *(im->data[(r+1) * im->cols * 2 - 1]);

    char avgr = (topright.r + topleft.r + bottomright.r + bottomleft.r)/4;
    char avgg = (topright.g + topleft.g + bottomright.g + bottomleft.g)/4;
    char avgb = (topright.b + topleft.b + bottomright.b + bottomleft.b)/4;
    
    zoom->data[i] = {avgr, avgg, avgb};
  }

  return zoom;
}
