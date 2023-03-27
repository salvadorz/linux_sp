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
 * @file signal_disposition.c
 * @author Salvador Z
 * @date 21 Mar 2023
 * @brief File for show how to perform a signal disposition and service handler.
 *
 * @see https://linux.die.net/man/2/sigaction
 */

#include <signal.h>
#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <string.h> /*memset*/
#include <sys/types.h> /*pid_t*/
#include <sys/wait.h>
#include <unistd.h> /*getpid, sleep*/

sig_atomic_t child_exit_status = 0xFFFF;

void clean_up_child_process(int signal_number) {
  /* Clean up the child process.  */
  wait(&child_exit_status);
  (void)signal_number;
}

int main() {
  int return_val = EXIT_FAILURE;
  /* Handle SIGCHLD by calling clean_up_child_process.  */
  struct sigaction sigchld_action;
  memset(&sigchld_action, 0, sizeof(sigchld_action));
  sigchld_action.sa_handler = &clean_up_child_process;
  sigaction(SIGCHLD, &sigchld_action, NULL);

  /* Now do things, including forking a child process.  */
  pid_t pid = fork();
  if (pid == 0) { // if child
    if (0u == sleep(5)) {
      printf("Child process <%d> finishing...\n", getpid());
      return_val = EXIT_SUCCESS;
    }
  } else { // Main program

    do {
      if (0u != sleep(4)) {
        printf("Interrupted by signal handler\n");
        if ((WIFEXITED(child_exit_status)) &&
            EXIT_SUCCESS == (WEXITSTATUS(child_exit_status))) {
          printf("Child process <%d> exited succesfully\n", pid);
          return_val = EXIT_SUCCESS;
        } else
          printf("Child process <%d> exited abnormally with code %d\n", pid,
                 WEXITSTATUS(child_exit_status));
      } else {
        if ((WIFEXITED(child_exit_status)) &&
            EXIT_SUCCESS == (WEXITSTATUS(child_exit_status))) {
          printf("Child process <%d> exited succesfully\n", pid);
          return_val = EXIT_SUCCESS;
        } else if (WIFEXITED(child_exit_status))
          printf("Child process <%d> exited abnormally with code %d\n", pid,
                 WEXITSTATUS(child_exit_status));
        else {
          printf("Child process <%d> running\n", pid);
        }
      }

    } while (0xFFFF == child_exit_status);
  }

  return return_val;
}

