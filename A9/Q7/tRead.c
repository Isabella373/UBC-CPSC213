#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;

void interrupt_service_routine () {
  // TODO
  uthread_t read_thread;
  queue_dequeue(pending_read_queue, (void **)&read_thread, NULL, NULL);
  uthread_unblock(read_thread);
}

void* read_block (void* blocknov) {
  // TODO schedule read and the update (correctly)
  int blockno = * (int *) blocknov;
  // unsigned int *result = malloc(sizeof(int));
  int result;
  disk_schedule_read(&result, blockno);
  queue_enqueue(pending_read_queue, uthread_self(), NULL, NULL);
  uthread_block();
  sum += result;

  return NULL;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // Sum Blocks
  // TODO
  // queue_t queue = queue_create();

  // for (int i = 0; i < num_blocks; i++) {
  //   int *temp = malloc(sizeof(int));
  //   *temp = i;
  //   uthread_t read_thread = uthread_create(&read_block, temp);
  //   queue_enqueue(pending_read_queue, (void *)read_thread, NULL, NULL);
  //   queue_enqueue(queue, (void *)read_thread, NULL, NULL);
  // }

  // for (int i = 0; i < num_blocks; i++) {
  //   uthread_t read_thread;
  //   queue_dequeue(queue, (void **)&read_thread, NULL, NULL);
  //   unsigned int *result;
  //   uthread_join(read_thread, (void **)&result);
  //   sum += *result;
  //   free(result);
  // }

  uthread_t *threads = malloc(sizeof(uthread_t) * num_blocks);
  int *results = malloc(sizeof(int) * num_blocks);
  for (int i = 0; i < num_blocks; i++)
  {
    results[i] = i;
    threads[i] = uthread_create(read_block, &results[i]);
  }

  for (int i = 0; i < num_blocks; i++)
  {
    uthread_join(threads[i], NULL);
  }
  free(results);
  free(threads);

  printf("%d \n", sum);
}
