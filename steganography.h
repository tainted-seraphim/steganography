#ifndef STEGANOGRAPHY_H_HEADER
#define STEGANOGRAPHY_H_HEADER

#include <stdio.h>

void steg_encode(FILE *text_file, FILE *message_file, FILE *output_file);

void steg_decode(FILE *encoded_file, FILE *output_file);

#endif
