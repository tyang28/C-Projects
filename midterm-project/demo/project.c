//demo_ppm.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "imageManip.h"

int main(int argc, char **argv) {

  if(argc < 3) {
    fprintf(stderr, "Failed to supply input filename or output filename, or both\n");
    return 1;
  }

  FILE *input = fopen(argv[1], "rb");
  if(!input) {
    fprintf(stderr, "Specified input file could not be opened\n");
    return 2;
  }

  Image *im = read_ppm(input);
  if(!im) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }

  if(argc < 4) {
    freeim(im);
    fprintf(stderr, "No  operation name was specified, or operation name specified was invalid\n");
    return 4;
  }

  Image *transform;

  if(strcmp(argv[3], "swap") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = swap(im);

  } else if(strcmp(argv[3], "grayscale") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = grayscale(im);

  } else if(strcmp(argv[3], "zoom_in") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = zoom_in(im);
    freeim(im);
    
  } else if(strcmp(argv[3], "zoom_out") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = zoom_out(im);
    freeim(im);
    
  } else if(strcmp(argv[3], "contrast") == 0) {
    double contr;
    if(sscanf(argv[4], "%lf", &contr) != 1 || argc != 5) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = contrast(im, contr);

  } else if(strcmp(argv[3], "occlude") == 0) {
    int x1;
    int y1;
    int x2;
    int y2;
    if(sscanf(argv[4], "%d", &x1) != 1 ||
       sscanf(argv[5], "%d", &y1) != 1 ||
       sscanf(argv[6], "%d", &x2) != 1 ||
       sscanf(argv[7], "%d", &y2) != 1 ||
       argc != 8) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = occlude(im, x1, y1, x2, y2);
    if(transform == NULL) {
      freeim(im);
      fprintf(stderr, "Arguments for occlude operation were out of range for the given input immage\n");
      return 6;
    }

  } else if(strcmp(argv[3], "blur") == 0) {
    printf("this isn't working yet\n");

  } else {
    free(im);
    fprintf(stderr, "No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }
  
  // write image to disk
  FILE *output = fopen(argv[2], "wb");
  if(!output) {
    freeim(transform);
    fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
    return 7;
  }
  
  int num_pixels_written = write_ppm(output, transform);
  if(!num_pixels_written) {
    freeim(transform);
    fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
    return 7;
  }

  printf("Image created with %d pixels.\n", num_pixels_written);
  
  // clean up!
  freeim(transform);

  //return no errors
  return 0;
}
