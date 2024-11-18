#include "steganography.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long get_file_size(FILE *fptr)
{
  long length;
  long cur_pos;
  cur_pos = ftell(fptr);
  rewind(fptr);
  length = ftell(fptr);
  cur_pos -= length;
  fseek(fptr, 0, SEEK_END);
  length = ftell(fptr) - length;
  fseek(fptr, cur_pos, SEEK_SET);
  return length;
}

static void put_file_to_string(FILE *fptr, char *str)
{
  long cur_pos;
  long len;
  len = get_file_size(fptr);
  cur_pos = ftell(fptr);
  rewind(fptr);
  cur_pos -= ftell(fptr);
  fread(str, 1, len, fptr);
  fseek(fptr, cur_pos, SEEK_SET);
}

static void remove_multiple_spaces_from_string(char *str, int len)
{
  char *ptr;
  char *tmp;
  int shift = 0;
  ptr = strchr(str, ' ');
  while (ptr != NULL) {
    tmp = ptr + 1;
    while (*tmp == ' ') {
      tmp++;
      shift++;
    }
    tmp--;
    if (tmp - ptr > 0) {
      memmove(ptr, tmp, len - (tmp - str));
    }
    ptr = strchr(ptr + 1, ' ');
  }
  shift = len - 1 - shift;
  while (shift != len - 1) {
    str[shift] = '\0';
    shift++;
  }
}

static int count_spaces_in_string(const char *str)
{
  const char *ptr = str;
  int res = 0;
  while (*ptr) {
    if (*ptr == ' ') {
      res++;
    }
    ptr++;
  }
  return res;
}

void steg_encode(FILE *text_file, FILE *message_file, FILE *output_file)
{
  char *text_str;
  char *message_str;
  char *ptr;
  int text_len;
  int message_len;
  int i;
  int j;
  int b;

  text_len = get_file_size(text_file);
  message_len = get_file_size(message_file);
  text_str = malloc(text_len + 1);
  message_str = malloc(message_len + 1);
  memset(text_str, '\0', text_len + 1);
  memset(message_str, '\0', message_len + 1);

  put_file_to_string(text_file, text_str);
  remove_multiple_spaces_from_string(text_str, text_len + 1);
  put_file_to_string(message_file, message_str);

  if (count_spaces_in_string(text_str) < 8 * (message_len + 1)) {
    printf("Not enough spaces to encode data\n");
    exit(4);
  }

  ptr = strchr(text_str, ' ');
  for (i = 0; i < message_len + 1; i++) {
    for (j = 7; j >= 0; j--) {
      b = (message_str[i] & (1 << j)) >> j;
      if (b) {
        memmove(ptr + 1, ptr, text_len - (ptr - text_str));
        ptr = strchr(ptr + 2, ' ');
      } else {
        ptr = strchr(ptr + 1, ' ');
      }
    }
  }

  fwrite(text_str, 1, text_len, output_file);
  free(message_str);
  free(text_str);
}

void steg_decode(FILE *encoded_file, FILE *output_file)
{
  char *ptr;
  char *encoded_str;
  int encoded_len;
  int m;
  char ch;

  encoded_len = get_file_size(encoded_file);
  encoded_str = malloc(encoded_len + 1);
  put_file_to_string(encoded_file, encoded_str);

  m = 7;
  ch = 0;
  ptr = strchr(encoded_str, ' ');
  while (ptr != NULL) {
    if (*(ptr + 1) == ' ') {
      ch += (1 << m);
      ptr += 2;
    } else {
      ptr += 1;
    }
    m--;
    if (m == -1) {
      if (ch == 0) {
        break;
      }
      fputc(ch, output_file);
      m = 7;
      ch = 0;
    }
    ptr = strchr(ptr, ' ');
  }
  free(encoded_str);
}
