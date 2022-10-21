#include <stdlib.h>
#include <stdio.h>
#include "job.h"
#include "refcount.h"

struct job {
  int data;
  int num;
};

int number = 0;
job_t pendingjob = NULL;

job_t job_create(int d) {
  struct job *j = rc_malloc(sizeof(*j));
  j->num = ++number;
  j->data = d;
  return j;
}

int job_get_data(job_t j) {
  return j->data;
}

void job_start(job_t j) {
  if (pendingjob) {
    printf("**Completing job: #%d**\n", pendingjob->num);   // Line 26
    rc_free_ref(pendingjob);
    pendingjob = NULL;                                      // Line 27
  } 
  if (j) {
    if (j->data % 3 == 0) {
      printf("**Starting job: #%d**\n", j->num);
      pendingjob = j;                                       // Line 32
      rc_keep_ref(j);
    } else {                                                // Line 33
      printf("**Skipped job #%d**\n", j->num);
      j-> data = 0;                                         // Line 35
    }                                                       // Line 36
  }
}

