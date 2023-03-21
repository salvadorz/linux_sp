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
* @file fork_ex.c
* @author Salvador Z
* @date 20 Mar 2023
* @brief File to sho the fork process on linux
*
*
* @see https://linux.die.net/man/2/fork
*/

#include <stdbool.h>
#include <stdio.h>     /*streams*/
#include <sys/types.h> /*pid_t*/
#include <unistd.h>    /*getpid*/

int main() {
  bool parent = true;
  int childexec = 0;
  pid_t pid = fork();
  if (pid == 0) {//if child
    parent = false;
    childexec++;
  }
  printf("Hello from %s PID<%d>\n", parent ? "parent" : "child", getpid());
  if (parent) {
    printf("  My Child pid is <%d>\n", pid);
  } else
    printf("  My Parent pid is <%d>\n", getppid());
  printf("childexec is %d from %s thread\n", childexec,
         parent ? "parent" : "child");
  return 0;
}