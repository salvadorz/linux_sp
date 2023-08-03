#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h> /*getpid*/

#define NUM_THREADS  (64)
#define NUM_CPUS     (8)
#define CPU_AFFINITY (3)

typedef struct {
  int threadIdx;
} threadParams_t;

// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
pthread_t mainthread;
pthread_t startthread;
threadParams_t threadParams[NUM_THREADS];

pthread_attr_t g_fifo_sched_attr;
pthread_attr_t g_orig_sched_attr;
struct sched_param g_fifo_param;

#define SCHED_POLICY   SCHED_FIFO
#define MAX_ITERATIONS (1000000)

void print_scheduler(void) {
  int schedType = sched_getscheduler(getpid());

  switch (schedType) {
  case SCHED_FIFO:
    printf("Pthread policy is SCHED_FIFO\n");
    break;
  case SCHED_OTHER:
    printf("Pthread policy is SCHED_OTHER\n");
    break;
  case SCHED_RR:
    printf("Pthread policy is SCHED_RR\n");
    break;
  default:
    printf("Pthread policy is UNKNOWN\n");
  }
}

void set_scheduler(void) {
  int max_prio, rc, cpuidx;
  cpu_set_t cpuset;

  printf("INITIAL ");
  print_scheduler();

  pthread_attr_init(&g_fifo_sched_attr);
  pthread_attr_setinheritsched(&g_fifo_sched_attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&g_fifo_sched_attr, SCHED_POLICY);
  CPU_ZERO(&cpuset);
  cpuidx = (CPU_AFFINITY);
  CPU_SET(cpuidx, &cpuset);
  pthread_attr_setaffinity_np(&g_fifo_sched_attr, sizeof(cpu_set_t), &cpuset);

  max_prio = sched_get_priority_max(SCHED_POLICY);
  g_fifo_param.sched_priority = max_prio;

  if ((rc = sched_setscheduler(getpid(), SCHED_POLICY, &g_fifo_param)) < 0)
    perror("sched_setscheduler");

  pthread_attr_setschedparam(&g_fifo_sched_attr, &g_fifo_param);

  printf("ADJUSTED ");
  print_scheduler();
}

void *counterThread(void *threadp) {
  int sum = 0, i, iterations;
  threadParams_t *threadParams = (threadParams_t *)threadp;
  // pthread_t mythread;
  double start = 0.0, stop = 0.0;
  struct timeval startTime, stopTime;

  gettimeofday(&startTime, 0);
  start = ((startTime.tv_sec * 1000000.0) + startTime.tv_usec) / 1000000.0;

  for (iterations = 0; iterations < MAX_ITERATIONS; iterations++) {
    sum = 0;
    for (i = 1; i < (threadParams->threadIdx) + 1; i++)
      sum = sum + i;
  }

  gettimeofday(&stopTime, 0);
  stop = ((stopTime.tv_sec * 1000000.0) + stopTime.tv_usec) / 1000000.0;

  printf("\nThread idx=%d, sum[0...%d]=%d, running on CPU=%d, start=%lf, stop=%lf",
         threadParams->threadIdx, threadParams->threadIdx, sum, sched_getcpu(), start, stop);

  return NULL;
}

void *starterThread(void *threadp) {
  int i;

  (void)threadp;
  printf("starter thread running on CPU=%d\n", sched_getcpu());

  for (i = 0; i < NUM_THREADS; i++) {
    threadParams[i].threadIdx = i;

    pthread_create(&threads[i],               // pointer to thread descriptor
                   &g_fifo_sched_attr,          // use FIFO RT max priority attributes
                   counterThread,             // thread function entry point
                   (void *)&(threadParams[i]) // parameters to pass in
    );
  }

  for (i = 0; i < NUM_THREADS; i++)
    pthread_join(threads[i], NULL);

  return NULL;
}

int main() {
  int rc;
  int j;
  cpu_set_t cpuset;

  set_scheduler();

  CPU_ZERO(&cpuset);

  // get affinity set for main thread
  mainthread = pthread_self();

  // Check the affinity mask assigned to the thread
  rc = pthread_getaffinity_np(mainthread, sizeof(cpu_set_t), &cpuset);
  if (rc != 0)
    perror("pthread_getaffinity_np");
  else {
    printf("main thread running on CPU=%d, CPUs =", sched_getcpu());

    // Shows the index of CPUS which are present by checking CPU_ISSET(#, cpuset)
    for (j = 0; j < CPU_SETSIZE; j++)
      if (CPU_ISSET(j, &cpuset)) printf(" %d", j);

    printf("\n");
  }

  pthread_create(&startthread,     // pointer to thread descriptor
                 &g_fifo_sched_attr, // use FIFO RT max priority attributes
                 starterThread,    // thread function entry point
                 (void *)0         // parameters to pass in
  );

  pthread_join(startthread, NULL);

  printf("\nTEST COMPLETE\n");
}