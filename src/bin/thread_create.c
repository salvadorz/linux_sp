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
 * @see https://linux.die.net/man/3/pthread_join
 */

#include <pthread.h>
#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <unistd.h>

typedef struct thread_args {
  char c;
  // N times to print
  int times;
} thread_args_t;

/* Prints a char to stderr.  The parameter is received by pthread_create.  Does not return.  */

void *print_char(void *thread_arg) {

  thread_args_t data = (*(thread_args_t *)thread_arg);
  while (data.times--)
    fputc(data.c, stderr);
  fputc('\n', stderr);
  return NULL;
}

/* The main program. */
int main() {

  pthread_t     thread_1_id, thread_2_id;
  thread_args_t thread_1_arg, thread_2_arg;

  thread_1_arg.c = 'x';
  thread_1_arg.times = 15u;
  /* Create a new thread.  The new thread will run the print_char function.*/
  pthread_create(&thread_1_id, NULL, &print_char, &thread_1_arg);

  thread_2_arg.c = 'o';
  thread_2_arg.times = 20u;
  /* Create a new thread.  The new thread will run the print_char function.*/
  pthread_create(&thread_2_id, NULL, &print_char, &thread_2_arg);

  /* waiting for the threads*/
  pthread_join(thread_1_id, NULL);
  pthread_join(thread_2_id, NULL);
  return 0;
}
