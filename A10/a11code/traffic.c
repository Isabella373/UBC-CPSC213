#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY  3
#define NUM_ITERATIONS 100
#define NUM_CARS       20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             NUM_CARS
#define WAIT_TIME_BETWEEN_CROSSES NUM_CARS


/**
 * You might find these declarations useful.
 */
enum Direction {EAST = 0, WEST = 1};
const static enum Direction oppositeEnd [] = {WEST, EAST};

struct Street {
  // TODO
  uthread_mutex_t mx;
  uthread_cond_t canCross[2];
  int count;
  enum Direction direction;
  int waiting[2];
} Street;

void initializeStreet(void) {
  // TODO
  Street.mx = uthread_mutex_create();
  Street.canCross[0] = uthread_cond_create(Street.mx);
  Street.canCross[1] = uthread_cond_create(Street.mx);
  Street.count = 0;
  Street.direction = EAST;
  Street.waiting[EAST] = 0;
  Street.waiting[WEST] = 0;

}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogramLock;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void enterStreet (enum Direction g) {
  // TODO
  uthread_mutex_lock(Street.mx);

  if(Street.count == 0) {
    if(Street.waiting[g] != 0){
      Street.direction = g;
    }
  }

  int t = entryTicker;

  while(Street.count >= MAX_OCCUPANCY || g != Street.direction) {
    uthread_cond_wait(Street.canCross[g]);
  }

  int wait_time = entryTicker - t;

  recordWaitingTime(wait_time);

  Street.count++;
  entryTicker++;
  occupancyHistogram[g][Street.count]++;

  Street.waiting[g] --;

  uthread_mutex_unlock(Street.mx);
}

void leaveStreet(void) {
  // TODO
  uthread_mutex_lock(Street.mx);
  if(Street.count > 0){
    Street.count --;
  }

  if ((Street.count == 0) && (Street.waiting[oppositeEnd[Street.direction]] != 0)) {
    Street.direction = oppositeEnd[Street.direction];
    VERBOSE_PRINT("A");

    for (int i = 0; i < MAX_OCCUPANCY; i++) {
      if ((Street.direction == EAST) && (Street.waiting[EAST] != 0)) {
        uthread_cond_signal(Street.canCross[0]);
      } else if ((Street.direction == WEST) && (Street.waiting[WEST] != 0)) {
        uthread_cond_signal(Street.canCross[1]);
      }
    }

  } else if (Street.count < MAX_OCCUPANCY) {
    if (Street.direction == EAST && Street.waiting[EAST] != 0) {
      uthread_cond_signal(Street.canCross[0]);
    } else if ((Street.direction == WEST) && (Street.waiting[WEST] != 0))
    {
      uthread_cond_signal(Street.canCross[1]);
    }

  }
  uthread_mutex_unlock(Street.mx);
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogramLock);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogramLock);
}

//
// TODO
// You will probably need to create some additional procedures etc.
//
void* car(){
  int d = random() % 2;
  uthread_mutex_lock(Street.mx);

  Street.waiting[d]++;
  uthread_mutex_unlock(Street.mx);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    enterStreet(d);
    for (int j = 0; j < CROSSING_TIME; j++) {
      uthread_yield();
    }

    leaveStreet();

    for (int l = 0; l < WAIT_TIME_BETWEEN_CROSSES; l++) {
      uthread_yield();
    }
  }
}


int main (int argc, char** argv) {

  uthread_init(8);

  waitingHistogramLock = uthread_mutex_create();
  entryTicker = 0;

  initializeStreet();
  uthread_t pt [NUM_CARS];

  // TODO
  for(int i = 0;i< NUM_CARS; i++) {
    pt[i] = uthread_create(car, NULL);
      }
   for(int i = 0;i< NUM_CARS; i++) {
     uthread_join(pt[i], NULL);
   }
   
  
  printf ("Times with 1 car  going east: %d\n", occupancyHistogram [EAST] [1]);
  printf ("Times with 2 cars going east: %d\n", occupancyHistogram [EAST] [2]);
  printf ("Times with 3 cars going east: %d\n", occupancyHistogram [EAST] [3]);
  printf ("Times with 1 car  going west: %d\n", occupancyHistogram [WEST] [1]);
  printf ("Times with 2 cars going west: %d\n", occupancyHistogram [WEST] [2]);
  printf ("Times with 3 cars going west: %d\n", occupancyHistogram [WEST] [3]);
  
  printf ("Waiting Histogram\n");
  for (int i=0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
