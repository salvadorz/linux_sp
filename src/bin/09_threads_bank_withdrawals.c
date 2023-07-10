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
 * @file threads_bank_withdrawals.c
 * @author Salvador Z
 * @date 08 Apr 2023
 * @brief File for creating threads to perform several withdrawals
 *
 * @see https://linux.die.net/man/3/pthread_join
 */

#include "threads_banking.h"

#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <string.h> /*memset*/

#define STARTING_BALANCE 1000U
#define WITHDRAW_REQUEST 100U
#define NUM_THREADS 5U

const char *locking_types[] = {"LOCKING_NONE", "LOCKING_MUTEX",
                               "LOCKING_SCOPED"};

/* The key used to associate a file descriptor by each thread. */
pthread_key_t thread_fd_log_key;

/**
 * A struture used to start threads for the withdrawl example
 */
struct withdraw_threadparams {
  /**
   * A pointer to the account used for this thread
   */
  account_t *account;
  /**
   * The amount to request in each withdrawl on this thread
   */
  unsigned int withdraw_request;
  /**
   * Total money disbursed by the thread
   */
  unsigned int withdrawn_money;
};

/**
 * The entry point for each thread for use with POSIX threading
 * @param arg - pointer to a withrdaw_threadparams structure (cast to void *)
 * @return - the parameter passed as arg
 */
static void *start_withdrawl_thread(void *arg) {
  struct withdraw_threadparams *params = (struct withdraw_threadparams *)arg;
  uint32_t withdraw_per_thread = 0;
  char thread_log_filename[32u];
  FILE *thread_log_file;

  /* Generate the filename for this thread's log file.  */
  sprintf(thread_log_filename, "atm_%ssafe-%ld.log",
          (ACCOUNT_LOCKING_NON == params->account->locktype) ? "non-" : "",
          (long)pthread_self());
  /* Open the log file.  */
  thread_log_file = fopen(thread_log_filename, "w");
  /* Store the file pointer in thread-specific data under thread_fd_log_key.  */
  pthread_setspecific(thread_fd_log_key, thread_log_file);

  write_atm_log("ATM Statement.", &thread_fd_log_key);

  withdraw_per_thread =
      do_withdrawls(params->account, params->withdraw_request, &thread_fd_log_key);
  printf("Total disbursed %4d thread %ld\n", withdraw_per_thread,
         pthread_self());
  params->withdrawn_money = withdraw_per_thread;
  return arg;
}

/**
 * Starts @param num_threads withdrawl threads on @param account, each
 * requesting @param withdraw_request dollars per transaction.  Waits for
 * all threads to complete.
 * @return true if one or more threads couldn't be started successfully.
 */
static bool run_withdrawl_threads(struct account *account,
                                  unsigned int withdraw_request,
                                  unsigned int num_threads) {
  bool success = false;
  /**
   * An array of pointers to dynamically allocated pthread_t types
   */
  pthread_t **thread_array;
  thread_array = (pthread_t **)malloc(sizeof(pthread_t *) * num_threads);
  if (thread_array == NULL) {
    printf("Memory allocation for pthread pointer failed\n");

  } else {
    success = true;
    struct withdraw_threadparams params;
    unsigned int thread;
    memset(&params, 0, sizeof(struct withdraw_threadparams));
    params.account = account;
    params.withdraw_request = withdraw_request;
    pthread_key_create(&thread_fd_log_key, close_atm_log);

    for (thread = 0; thread < num_threads; thread++) {
      thread_array[thread] = (pthread_t *)malloc(sizeof(pthread_t));
      if (thread_array[thread] == NULL) {
        printf("Memory allocation failure for thread array entry failed\n");
        success = false;

      } else {
        int rc = pthread_create(thread_array[thread],
                                NULL, // Use default attributes
                                start_withdrawl_thread, &params);
        if (rc != 0) {
          printf("pthread_create failed with error %d creating thread %u\n", rc,
                 thread);
          success = false;
        }
        printf("-Started thread [%d] with id %ld\n", thread,
               (unsigned long int)*thread_array[thread]);
      }
    }
    uint32_t total_withdrawl_from_account = 0;
    for (thread = 0; thread < num_threads; thread++) {
      if (thread_array[thread] != NULL) {
        int rc = pthread_join(*thread_array[thread], (void *)&params);
        if (rc != 0) {
          printf("Attempt to pthread_join thread %u failed with %d\n", thread,
                 rc);
          success = false;
        }
        total_withdrawl_from_account += params.withdrawn_money;
        printf("Total Disbursed $%u from atm %d\n", params.withdrawn_money,
               thread);
        free(thread_array[thread]);
      }
    }
    printf("Total Disbursed $%u from account\n", total_withdrawl_from_account);
    free(thread_array);
  }
  return success;
}

int main() {
  bool success = false;

  struct account useraccount;

  for (int locking_type = ACCOUNT_LOCKING_NON;
       locking_type < ACCOUNT_LOCKING_MAX; locking_type++) {
    printf("Running with locking type: %s\n", locking_types[locking_type]);
    withdraw_account_init(&useraccount, STARTING_BALANCE,
                          (withdraw_locking_t)locking_type);

    if (run_withdrawl_threads(&useraccount, WITHDRAW_REQUEST, NUM_THREADS)) {
      if (useraccount.withdrawl_total > (unsigned long)STARTING_BALANCE) {
        printf("\nWithdrew a total of $%u from an account which only contained "
               "$%u!\n",
               useraccount.withdrawl_total, STARTING_BALANCE);

      } else {
        printf("\nWithdrew a total of $%u from account which contained $%u\n",
               useraccount.withdrawl_total, STARTING_BALANCE);
        success = true;
      }

    } else {
      printf("Error starting withdrawl threads\n");
    }
  }
  return success ? 0 : -1;
}