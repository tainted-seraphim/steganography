#include "steganography.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *inputfile;
  FILE *messagefile;
  FILE *outputfile;

  if (argc == 2) {
    if (strcmp(argv[1], "--help") == 0) {
      printf("To encode: ./steganography -e text_file.txt "
             "message_file.txt output_file.txt\n");
      printf("To decode: ./steganography -d encoded_file.txt"
             " output_file.txt\n");
      return 0;
    } else {
      printf("Incorrect arguments.\nTry ./steganography "
             "--help\n");
    }
  } else if (argc == 4) {
    if (strcmp(argv[1], "-d") != 0) {
      printf("Incorrect arguments.\nTry ./steganography "
             "--help\n");
      return 1;
    }
    if (strcmp(argv[2], argv[3]) == 0) {
      printf("Input file and output file must be different\n");
      return 2;
    }
    inputfile = fopen(argv[2], "r");
    if (inputfile == NULL) {
      printf("Cannot open file \'%s\'\n", argv[2]);
      return 3;
    }
    outputfile = fopen(argv[3], "w");
    if (outputfile == NULL) {
      printf("Cannot open file \'%s\'\n", argv[3]);
      return 3;
    }
    steg_decode(inputfile, outputfile);
    fclose(outputfile);
    fclose(inputfile);
  } else if (argc == 5) {
    if (strcmp(argv[1], "-e") != 0) {
      printf("Incorrect arguments.\nTry ./steganography "
             "--help\n");
      return 1;
    }
    if (strcmp(argv[2], argv[3]) == 0 || strcmp(argv[3], argv[4]) == 0) {
      printf("Output file must be different from input file "
             "and message file\n");
      return 2;
    }
    inputfile = fopen(argv[2], "r");
    if (inputfile == NULL) {
      printf("Cannot open file \'%s\'\n", argv[2]);
      return 3;
    }
    messagefile = fopen(argv[3], "r");
    if (messagefile == NULL) {
      printf("Cannot open file \'%s\'\n", argv[3]);
      return 3;
    }
    outputfile = fopen(argv[4], "w");
    if (outputfile == NULL) {
      printf("Cannot open file \'%s\'\n", argv[4]);
      return 3;
    }
    steg_encode(inputfile, messagefile, outputfile);
    fclose(outputfile);
    fclose(messagefile);
    fclose(inputfile);
  } else {
    printf("Incorrect arguments.\nTry ./steganography --help\n");
  }
  return 0;
}
