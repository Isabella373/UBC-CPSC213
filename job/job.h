#pragma once

typedef struct job* job_t;

job_t job_create(int d);
void job_start(job_t j);
int job_get_data(job_t j);