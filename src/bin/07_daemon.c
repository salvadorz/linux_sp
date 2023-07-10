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
 * @file daemon.c
 * @author Salvador Z
 * @date 21 Mar 2023
 * @brief File for Daemon Create
 *
 * @see https://linux.die.net/man/3/setsid
 */

#include <fcntl.h>     /*open*/
#include <stdio.h>     /*streams> fopen, fputs*/
#include <stdlib.h>    /*exit*/
#include <sys/types.h> /*pid_t*/
#include <unistd.h>    /*fork, exit,close, setsid*/

#define NR_OPEN (1024)

int _daemon_create(void) {
  // PID: Process ID
  pid_t pid;

  pid = fork(); // Fork off the parent process
  if (pid < 0)
    exit(EXIT_FAILURE);
  if (pid > 0) // Only the parent will hit
    exit(EXIT_SUCCESS); //Terminates the parent

  // Create a SID for child
  if (-1 == setsid())
    exit(EXIT_FAILURE);

  if (-1 == (chdir("/")))
    exit(EXIT_FAILURE);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  /* Closing FD*/
  for (int i = STDERR_FILENO+1; i < NR_OPEN; ++i)
    close(i);

  open("/dev/null", O_RDWR);
  dup(0);
  dup(0);
  while (1) {
    // Do something...
    sleep(10);
  }
  exit(EXIT_SUCCESS);
}

int main() { return _daemon_create(); }