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
 * @file banking.c
 * @author Salvador Z
 * @date 08 Apr 2023
 * @brief File for show usage of withdrawals on a safeway by using sync and
 * pthrads.
 *
 * @see https://linux.die.net/man/3/pthread_mutex_lock
 */
#include "threads_banking.h"

#include <errno.h>
#include <stdio.h>  /*streams> fopen, fputs*/
#include <stdlib.h> /*NULL (stddef)*/
#include <string.h> /*strerror*/

/**
 * Simulate disbursing @param amount on our fictional ATM
 */
void disburse_money(unsigned int amount) {
  printf("Disbursing $%u from thread %lu\n", amount,
         (unsigned long int)pthread_self());
}

/**
 * Thread safe implementation of withdraw() using mutexes if locktype ask for it
 */
static bool withdraw(account_t *account, unsigned int amount) {
  bool success = false;

  if ((ACCOUNT_LOCKING_MUTEX == account->locktype) &&
      (0 != pthread_mutex_lock(&account->mutex))) {
    printf("pthread_mutex_lock failed with %s\n", strerror(errno));
  } else {
    const int balance = account->current_balance;
    if (balance >= (long)amount) {
      success = true;
      printf("Withdrawl approved\n");
      account->current_balance = balance - amount;
      account->withdrawl_total += amount;
    }
    if (((ACCOUNT_LOCKING_MUTEX == account->locktype)) &&
        (0 != pthread_mutex_unlock(&account->mutex))) {
      printf("pthread_mutex_unlock failed with %s\n", strerror(errno));
      success = false; // not sure if we should give out cash in this case,
                       // error on the safe side...
    }
    if (success) {
      disburse_money(amount);
    }
  }
  return success;
}

/**
 * Initialize @param account with starting balance @param starting_balance
 * Use @param locktype when performing account transactions
 * @return true when initialized successfully, false if failed to initialize
 */
bool withdraw_account_init(account_t *account,
                           unsigned int starting_balance,
                           withdraw_locking_t locktype) {
  int rc = 0;
  bool success = true;
  memset(account, 0, sizeof(account_t));
  account->current_balance = starting_balance;
  account->locktype = locktype;
  rc = pthread_mutex_init(&account->mutex, NULL);
  if (rc != 0) {
    printf("Failed to initialize account mutex, error was %d", rc);
    success = false;
  }
  return success;
}

/**
 * Do withdrawls on the account specified by @param account
 * in @param withdraw_request increments until the account does not have
 * enough remaining to complete the withdrawl Assumes the @param account
 * structure has been previously initialized with "withdraw_account_init"
 */
void do_withdrawls(struct account *account, unsigned int withdraw_request) {
  bool withdraw_success = false;
  do {
    withdraw_success = withdraw(account, withdraw_request);
  } while (withdraw_success);
}