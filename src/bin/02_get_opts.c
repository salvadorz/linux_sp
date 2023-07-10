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
 * @file get_opts.c
 * @author Salvador Z
 * @date 14 Mar 2023
 * @brief File for show how to parse command-line options
 *
 * Must provide two data structures.The first is a character string containing
 * the valid short options, each a single letter. An option that
 * requires an argument is followed by a colon.
 * To specify the available long options, you construct an array of struct
 * option elements.
 *
 * @see https://linux.die.net/man/3/getopt_long
 */

#include <getopt.h> /*getopt_long*/
#include <stdio.h> /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/

/* The name of the program.  */
const char *program_name;

void print_usage(FILE *stream, int exit_code);

/* Prints usage information for the program to STREAM (e.g. stdout or stderr),
   and exit the program with EXIT_CODE.  Does not return. */

void print_usage(FILE *stream, int exit_code) {
  fprintf(stream, "Usage:  %s options [ inputFile ... ]\n", program_name);
  fprintf(stream, "  -h  --help             Display this usage information.\n"
                  "  -o  --output filename  Write output to file.\n"
                  "  -v  --verbose          Print verbose messages.\n");
  exit(exit_code);
}

/* Main program entry point.  ARGC conains number of argument list
   elements; ARGV is an array of pointers to them.  */

int main(int argc, char *argv[]) {
  int next_option;

  /* A string listing valid short options letters.  */
  const char *const short_options = "ho:v";

  /* An array describing valid long options.  */
  const struct option long_options[] = {
      {"help", 0, NULL, 'h'},
      {"output", 1, NULL, 'o'},
      {"verbose", 0, NULL, 'v'},
      {NULL, 0, NULL, 0} /* Required at end of array.  */
  };

  /* The name of the file to receive program output, or NULL for
     standard output.  */
  const char *output_filename = NULL;
  /* Whether to display verbose messages.  */
  int verbose = 0;

  /* Remember the name of the program, to incorporate in messages.
     The name is stored in argv[0].  */
  program_name = argv[0];

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option) {
    case 'h': /* -h or --help */
      /* User has requested usage information.  Print it to standard
         output, and exit with exit code zero (normal termination).  */
      print_usage(stdout, (int)0);
      break;

    case 'o': /* -o or --output */
      /* This option takes an argument, the name of the output file.  */
      output_filename = optarg;
      printf("Writing to file: %s\n", output_filename);
      break;

    case 'v': /* -v or --verbose */
      verbose = 1;
      break;

    case '?': /* The user specified an invalid option.  */
      /* Print usage information to standard error, and exit with exit
         code one (indicating abonormal termination).  */
      print_usage(stderr, 1);

    case -1: /* Done with options.  */
      break;

    default: /* Something else: unexpected.  */
      abort();
    }
  } while (next_option != -1);

  /* Done with options.  OPTIND points to first non-option argument.
     For demonstration purposes, print them if the verbose option was
     specified.  */
  if (verbose) {
    int i;
    for (i = optind; i < argc; ++i)
      printf("Argument: %s\n", argv[i]);
  }

  /* Main program code.  */

  return 0;
}
