// ppm_io.c
// Jason Kurlander jkurlan3
// Teng-Ju Yang tyang28

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

  //skips past the first line (which should be P6)
  fgets(input, 100, fp);
  //reads the second line which may or may not be a comment
  fgets(input, 100, fp);

  //if it is a comment then read in the next line
  if(input[0] == '#') {
    fgets(input, 100, fp);
  }

  //convert the line into columns and rows
  sscanf(input, "%d %d", &columns, &rows);

  //skip to the next line
  fgets(input, 100, fp);

  Image *image = malloc(sizeof(Image));
  image->data = malloc(sizeof(Pixel) * rows * columns);
  image->rows = rows;
  image->cols = columns;

  //read the ppm into an image
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
    fclose(fp);
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }
  
  fclose(fp);
  return num_pixels_written;
}


