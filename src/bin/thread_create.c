/*******************************************************************************
 * Copyright (C) 2023 by Salvador Z                                            *
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
 * @file thread_create.c
 * @author Salvador Z
 * @date 26 Mar 2023
 * @brief File for show how-to use pthreads
 *
 * @see https://linux.die.net/man/3/pthread_create
 */

#include <pthread.h>
#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <unistd.h>

/* Prints x's to stderr.  The parameter is unused.  Does not return.  */

void *print_xs(void *int_arg) { // To show issues if data sync it's not used
  fprintf(stderr, "'%d'\n", *(int*)int_arg);
  int times = (*(int*)int_arg) + 20;
  while (times--)
    fputc('x', stderr);
  fputc('\n', stderr);
  return NULL;
}

/* The main program. */
int main() {
  int times = 20u;
  pthread_t thread_id;
  /* Create a new thread.  The new thread will run the print_xs function.  */
  pthread_create(&thread_id, NULL, &print_xs, &times);
  /* Print o's continuously to stderr.  */
  do
    fputc('o', stderr);
  while (--times);
  sleep(1); //giving time to run the thread; if process ends all threads die.
  return 0;
}
