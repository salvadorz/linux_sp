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
 * @file process_execvp.c
 * @author Salvador Z
 * @date 20 Mar 2023
 * @brief File for spawn a process from a fork
 *
 *
 * @see https://linux.die.net/man/2/execvp
 * @see https://linux.die.net/man/2/wait
 */

#include "processes.h"

#include <errno.h>
#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <sys/types.h> /*pid_t*/
#include <sys/wait.h> /*waitpid*/
#include <unistd.h> /*execvp*/

int process_spawn(char *program, char **arg_list) {
  pid_t child_pid;
  /* Duplicate this process. */
  child_pid = fork();
  if (child_pid != 0){
    /* This is the parent process. */
    int stat = 0; //the parent will sleep until child changes it's state
    child_pid = waitpid(child_pid, &stat, 0);
    if (-1 == child_pid){
      perror("waitpid");
      exit(EXIT_FAILURE);
    }
  }
  else {
    /* Now execute Program */
    if ( -1 == execvp(program, arg_list)){
    /* The execvp function returns only if an error occurs. */
      fprintf(stderr, "Error in execvp from %d\n", getppid());
      perror ("execvp");
      abort();
    }
  }
  return EXIT_SUCCESS;
}