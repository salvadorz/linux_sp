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
 * @file mem_buffer.c
 * @author Salvador Z
 * @date 19 Mar 2023
 * @brief File for handling a memory buffer from specified files
 *
 *
 * @see https://linux.die.net/man/2/open
 * @see https://linux.die.net/man/2/read
 */

#include "memory_buffer.h"

#include <errno.h>
#include <fcntl.h> /*open*/
#include <stdio.h> /*streams*/
#include <string.h> /*strerror*/
#include <sys/stat.h> /*file char S_IRWX? */
#include <sys/types.h>
#include <unistd.h> /*read, write*/

char* read_from_file(const char *filename, const size_t length, const bool block) {
  char *buf = NULL;

  /*Buffer allocated*/
  buf = (char *)malloc(length);
  if (NULL != buf) {
    /*Open the file*/
    int fd =
        open(filename,
             O_RDONLY |
                 /**
                  * When opening in blocking mode, use O_CREAT|O_RDWR to support
                  * creating the file if it doesn't exist and to allow opening
                  * pipes on Linux when no readers are available (see
                  * http://man7.org/linux/man-pages/man7/fifo.7.html) blocking
                  * mode.
                  */
                 (block ? (O_CREAT | O_RDWR) : O_NONBLOCK),
             S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1) {
      printf("Error %d (%s) opening %s\n", errno, strerror(errno), filename);
      free(buf);
      buf = NULL;
    } else { /*Read the file*/
      const size_t readlen = read(fd, buf, length);

      if (readlen != length) {
        int return_err = errno;
        free(buf);
        buf = NULL;
        if (!block && return_err == EAGAIN) {
          printf("EAGAIN returned from read(), no data available to read\n");
        } else {
          printf("Error returned from read(), errno is %d (%s)\n", return_err,
                 strerror(return_err));
        }
      }
      close(fd);
    } //END reading
  }
  return buf;
}