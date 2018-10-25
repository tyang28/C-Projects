// imageManip.c
//Jason Kurlander jkurlan3
//Teng-Ju Yang tyang28

#include <stdio.h>
#include "ppm_io.h"
#include "imageManip.h"
#include <stdlib.h>
#include <math.h>


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

//function that does the contrast for a specific color in a specific pixel
unsigned char ccolor(unsigned char color, double cont) {
  double new = (double)color;
  //convert to scale between -.5 and .5
  new /= 255;
  new -= .5;
  //scale to contrast
  new *= cont;
  //convert back to unsigned char range
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
  if(!zoom) {
    return NULL;
  }
  zoom->rows = im->rows * 2;
  zoom->cols = im->cols * 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols * zoom->rows);
  if(!zoom->data) {
    return NULL;
  }
  
  //for each pixel in the new zoom image
  for(int i = 0; i < zoom->rows * zoom->cols; i++) {
    int row = i / zoom->cols;
    int col = i % zoom->cols;
    
    int copy_row = row / 2;
    int copy_col = col / 2;

    int copy_index = (copy_row * im->cols) + copy_col;

    zoom->data[i].r = im->data[copy_index].r;
    zoom->data[i].b = im->data[copy_index].b;
    zoom->data[i].g = im->data[copy_index].g;
  }
  
  return zoom;
}

Image *zoom_out(Image *im) {

  Image *zoom = malloc(sizeof(Image));
  if(!zoom) {
    return NULL;
  }
  zoom->rows = im->rows / 2;
  zoom->cols = im->cols / 2;
  zoom->data = malloc(sizeof(Pixel) * zoom->cols *zoom->rows);
  if(!zoom->data) {
    return NULL;
  }

  //for each pixel in the new zoom image
  for(int i = 0; i < zoom->rows * zoom->cols; i++) {
    int r = i / zoom->cols;
    int c = i % zoom->cols;

    //each of the four pixels in the original image that will be averaged
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

  //for each pixel within the 1-d image array between the coordinates
  //**this does not necessarily mean it's inside the rectangle** (but it should make it more efficient)
  for(int i = y1*im->cols + x1; i <= y2*im->cols + x2; i++) {
    int r = i / im->cols;
    int c = i % im->cols;

    //if it's within the rectangle
    if(r>=y1 && r<=y2 && c>=x1 && c<=x2) {
      //set color to black
      im->data[i].r = 0;
      im->data[i].g = 0;
      im->data[i].b = 0;
    }
  }

  return im;
}



Image *blur(Image *im, double sigma) {

  //create new Image to put the blurred image
  Image *new = malloc(sizeof(Image));
  if(!new) {
    return NULL;
  }
  new->cols = im->cols;
  new->rows = im->rows;
  new->data = malloc(new->cols * new->rows * sizeof(Pixel));
  if(!new->data) {
    free(new);
    return NULL;
  }

  //size of the gaussian distribution array (which must be odd)
  int span = sigma * 10;
  if(span % 2 == 0) {
    span++;
  }

  //gaussian distribution array
  double gauss[span][span];

  //fill the array
  for(int i = 0; i < span; i++) {
    for(int j = 0; j<span; j++) {
      //offset from the center
      int dx = abs( i - span/2 );
      int dy = abs( j - span/2 );

      //equation for gaussian distribution
      double g = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
      
      gauss[i][j] = g;
    }
  }
  
  //for each pixel in the image
  for(int index = 0; index < im->cols*im->rows; index++) {
    
    double norm_sum = 0;
    double rsum = 0;
    double gsum = 0;
    double bsum = 0;
    
    //location of pixel in terms of row/col
    int row = index / im->cols;
    int col = index % im->cols;

    //loop through surrounding pixels
    for(int crow = row - span/2; crow <= row + span/2; crow++) {
      for(int ccol = col - span/2; ccol <= col + span/2; ccol++) {
	//if the pixel we're looking at is actually on the image
	if(crow >= 0 && crow < im->rows && ccol >= 0 && ccol < im->cols) {
	  norm_sum += gauss[crow - row + span/2][ccol - col + span/2];
	  rsum += im->data[crow * im->cols + ccol].r * gauss[crow - row + span/2][ccol - col + span/2];
	  gsum += im->data[crow * im->cols + ccol].g * gauss[crow - row + span/2][ccol - col + span/2];
	  bsum += im->data[crow * im->cols + ccol].b * gauss[crow - row + span/2][ccol - col + span/2];
	}
      }
    }
    //set the blurred pixel
    new->data[index].r = rsum / norm_sum;
    new->data[index].g = gsum / norm_sum;
    new->data[index].b = bsum / norm_sum;
    
  }
  
  return new;
}

//function to square a double (for the gaussian matrix)
double sq(double s) {
  return s * s;
}

//function that frees an image and its pixels
void freeim(Image *im) {
  free(im->data);
  free(im);
}
