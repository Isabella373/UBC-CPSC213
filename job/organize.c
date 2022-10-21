#include <stdlib.h>
#include "organize.h"
#include "job.h"

int organize_job(int d) {
    job_t j = job_create(d);
    job_start(j);
    return job_get_data(j);          // Line 8
}

void organize_finish_all() {
    job_start(NULL);
}
