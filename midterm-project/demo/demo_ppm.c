//demo_ppm.c

#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"

int main() {
  
  FILE *nika = fopen("nika.ppm", "rb");
  Image *im = read_ppm(nika);
  if (!im) {
    fprintf(stderr, "Uh oh. Image read failed!\n");
    return 1;
  }
  
  // write image to disk
  FILE *fp = fopen("demo.ppm", "wb");
  if (!fp) {
    fprintf(stderr, "Uh oh. Output file failed to open!\n");
    free(im->data);
    free(im);
    return 1;
  }
  int num_pixels_written = write_ppm(fp, im);
  fclose(fp);
  printf("Image created with %d pixels.\n", num_pixels_written);
  
  // clean up!
  free(im->data);
  free(im);
  return 0;
  
  
  
}
