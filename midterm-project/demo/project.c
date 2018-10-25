//project.c
//Jason Kurlander jkurlan3
//Teng-Ju Yang tyang28

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

  //swap
  if(strcmp(argv[3], "swap") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = swap(im);
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    
    //grayscale
  } else if(strcmp(argv[3], "grayscale") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = grayscale(im);
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    //zoom_in
  } else if(strcmp(argv[3], "zoom_in") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = zoom_in(im);
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    freeim(im);

    //zoom_out
  } else if(strcmp(argv[3], "zoom_out") == 0) {
    if(argc != 4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = zoom_out(im);
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    freeim(im);

    //contrast
  } else if(strcmp(argv[3], "contrast") == 0) {
    double contr;
    if(argc != 5 || sscanf(argv[4], "%lf", &contr) != 1 || contr < 0) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }

    transform = contrast(im, contr);
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    
    //occlude
  } else if(strcmp(argv[3], "occlude") == 0) {
    
    char *leftover1;
    char *leftover2;
    char *leftover3;
    char *leftover4;
    
    if(argc != 8) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    
    int x1 = strtol(argv[4], &leftover1, 10);
    int y1 = strtol(argv[5], &leftover2, 10);
    int x2 = strtol(argv[6], &leftover3, 10);
    int y2 = strtol(argv[7], &leftover4, 10);

    printf("%d %d %d %d", x1, y1, x2, y2);
    
    if(*leftover1 || *leftover2 || *leftover3 || *leftover4) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }


    if(x1 < 0 || x1 >= im->cols || x2 < 0 || x2 >= im->cols ||
       y1 < 0 || y1 >= im->rows || y2 < 0 || y2 >= im->rows ||
       x1 >= x2 || y1 >= y2) {
      freeim(im);
      fprintf(stderr, "Arguments for occlude operation were out of range for the given input immage\n");
      return 6;
    }
    
    transform = occlude(im, x1, y1, x2, y2);

    //blur
  } else if(strcmp(argv[3], "blur") == 0) {
    double sigma;
    if(argc != 5 || sscanf(argv[4], "%lf", &sigma) != 1 || sigma <= 0) {
      freeim(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation\n");
      return 5;
    }
    transform = blur(im, sigma);
        
    if(!transform) {
      freeim(im);
      fprintf(stderr, "Function returned null\n");
      return 8;
    }
    freeim(im);
    
    //other
  } else {
    freeim(im);
    fprintf(stderr, "No  operation name was specified, or operation name specified was invalid\n");
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
