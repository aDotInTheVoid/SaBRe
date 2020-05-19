/*  Copyright © 2019 Software Reliability Group, Imperial College London
 *
 *  This file is part of SaBRe.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <assert.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "loader/custom_tls.h"

#define MAX_THREADS 100

static int first_tid = 0;
static thread_local_vars_s *all_ctls[MAX_THREADS] = {NULL};

// TODO(andronat): optimize to reduce number of gettids.

// This should be called once per execution.
void register_first_tid() {
  assert(first_tid == 0);
  first_tid = syscall(SYS_gettid);
}

thread_local_vars_s *get_ctls() {
  pid_t tid = syscall(SYS_gettid) - first_tid;
  assert(tid >= 0 && tid < MAX_THREADS);
  return all_ctls[tid];
}

// This should be called once per new thread.
void setup_default_ctls() {
  thread_local_vars_s *tlv =
      (thread_local_vars_s *)calloc(1, sizeof(thread_local_vars_s));
  assert(tlv != NULL);
  pid_t tid = syscall(SYS_gettid) - first_tid;
  assert(tid >= 0 && tid < MAX_THREADS);
  all_ctls[tid] = tlv;
}
