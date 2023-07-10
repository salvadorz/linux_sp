/*******************************************************************************
 * Copyright (C) 2022 by Salvador Z                                            *
 *                                                                             *
 * This file is part of ELSU                                                   *
 *                                                                             *
 *   Permission is hereby granted, free of charge, to any person obtaining a   *
 *   copy of this software and associated documentation files (the Software)   *
 *   to deal in the Software without restriction including without limitation  *
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,  *
 *   and/or sell copies ot the Software, and to permit persons to whom the     *
 *   Software is furnished to do so, subject to the following conditions:      *
 *                                                                             *
 *   The above copyright notice and this permission notice shall be included   *
 *   in all copies or substantial portions of the Software.                    *
 *                                                                             *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   *
 *   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARANTIES OF MERCHANTABILITY *
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL   *
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR      *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,     *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE        *
 *   OR OTHER DEALINGS IN THE SOFTWARE.                                        *
 ******************************************************************************/

/**
 * @file buffered_streams.c
 * @author Salvador Z
 * @date 14 Mar 2023
 * @brief File for show the diff buffered vs non-buffered streams
 */

#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <unistd.h> /*sleep*/

int main ()
{
  unsigned int i = 0;
  printf("Buffered stream \n");

  while (10u > i++) {
    printf(".");
    sleep(1);
  }

  printf("\n Non-buffered stream \n");

  while (20u > i) {
    fprintf(stderr,".");
    ++i;
    sleep(1);
  }
  printf("\n END. \n");

  return 0;
}
