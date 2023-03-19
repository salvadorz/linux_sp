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
 * @file mem_buffer_tmp.c
 * @author Salvador Z
 * @date 15 Mar 2023
 * @brief File for writing a memory buffer on a temporary file
 *
 * `mkstemp` function creates a unique temporary filename
 * only the current user can access it.
 *
 * @see https://linux.die.net/man/3/mkstemp
 */

#include "memory_buffer.h"

#include <assert.h>
#include <stdio.h> /*streams> fopen, fputs*/
#include <unistd.h>


tmp_file_handle write_tmp_mem_buff(char *buffer, size_t length) {
  /* The XXXXXX will be replaced with characters that make the filename unique.  */
  char tmp_fileName[] = "/tmp/tmp_file.XXXXXX";

  int fd = mkstemp(tmp_fileName);

  if (-1 != fd) { /* Unlink the file immediately, it will be removed
     when the file descriptor is closed.  */
    unlink(tmp_fileName);

    /* Write the number of bytes to the file first.  */
    write(fd, &length, sizeof(length));
    /* Now write the data itself.  */
    write(fd, buffer, length);
  } else {
    assert (fd > 0);
  }

  /* Use the file descriptor as the handle for the temporary file. */
  return fd;
}


char* read_tmp_mem_buff(tmp_file_handle tmp_file, size_t *length) {
  char *buffer = NULL;

  if (-1 != tmp_file) {
    /* converting the file_handle back to a file descriptor */
    int fd = (int)tmp_file;
    /* Going to the beginning of the file.  */
    lseek(fd, 0, SEEK_SET);
    /* Read the size of the buffered data in the tmp file. */
    ssize_t rd_bytes = read(fd, length, sizeof(*length));
    if (sizeof(*length) == rd_bytes)
      /* Allocate a buffer and fetch the data from tmp */
      buffer = (char *)malloc(*length);
    
    read(fd, buffer, *length);
    /* will cause the temporary file to go away.  */
    close(fd);
  } else {
    assert(fd > 0); //assert if fails the condition
  }
  
  return buffer;
}