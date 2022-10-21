#include <stdlib.h>
#include <stdio.h>
#include "organize.h"

int main () {
  printf("Starting to review job data\n");
  int work_started = 0;
  work_started += organize_job(7);
  work_started += organize_job(3);
  work_started += organize_job(12);
  work_started += organize_job(5);
  work_started += organize_job(15);
  organize_finish_all();
  printf("Total work started: %d\n", work_started);
}