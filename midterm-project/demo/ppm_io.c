// ppm_io.c
// 601.220, Fall 2018
// Starter code for midterm project - feel free to edit/add to this file

#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <stdio.h>


/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp); 

  char input[100];
  int columns = 0;
  int rows = 0;

  fgets(input, 100, fp);
  fgets(input, 100, fp);

  if(input[0] == '#') {
    fgets(input, 100, fp);
  }

  sscanf(input, "%d %d", &columns, &rows);
    
  fgets(input, 100, fp);
  
  Image *image = malloc(sizeof(Image));
  image->data = malloc(sizeof(Pixel) * rows * columns);
  image->rows = rows;
  image->cols = columns;

  fread(image->data, sizeof(Pixel), rows*columns, fp);
  fclose(fp);

  return image;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->rows * im->cols, fp);

  if (num_pixels_written != im->rows * im->cols) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

