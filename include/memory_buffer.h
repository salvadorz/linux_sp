/*
 * @memory_buffer.h
 *
 * @version: 1.0
 * @Author:  Salvador Z
 * @brief:   Writes into a memory buffer
 */

#ifndef MEMORY_BUFFER_H_
#define MEMORY_BUFFER_H_

#include <stdbool.h>
#include <stdlib.h> /*NULL (stddef)*/

/* A handle for a temporary file the underlying type is a file descriptor.  */
typedef int tmp_file_handle;

/**
 * @brief Writes a unique temporary file generating a temporary
 *
 * @param buffer into a temporary file
 * @param length amount of bytes and
 * @return tmp_file_handle handle to the temporary file. Which is already
 * unlinked
 */
tmp_file_handle write_tmp_mem_buf(char *buffer, size_t length);

/**
 * @brief Reads the contents of a temporary file TMP_FILE. The
 *  temporary file is removed after close.
 *
 * @param tmp_file handle for the unique created file
 * @param length to retrieve amount of bytes stored on the mem buffer
 * @return char* is a allocated buffer. Must be free.
 */
char *read_tmp_mem_buff(tmp_file_handle tmp_file, size_t *length);

/**
 * @brief Returns a buffer of length allocated to read from filename.
 *
 * @param filename path to read
 * @param length amount of bytes to be read
 * @return char* is a allocated buffer. Must be free.
 */
char *read_from_file(const char *filename, const size_t length, const bool block);

#endif // MEMORY_BUFFER_H_