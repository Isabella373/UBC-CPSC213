#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf(S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void)0) // do nothing
#endif

#define MAX_OCCUPANCY 3
#define NUM_ITERATIONS 100
#define NUM_CARS 20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME NUM_CARS
#define WAIT_TIME_BETWEEN_CROSSES NUM_CARS

/**
 * You might find these declarations useful.
 */
enum Direction
{
    EAST = 0,
    WEST = 1
};
const static enum Direction oppositeEnd[] = {WEST, EAST};

struct Street
{
    // TODO
    uthread_sem_t mx;
    uthread_sem_t canCross[2];
    int count;
    enum Direction direction;
    int count_with_direction[2];
    int waiting[2];
} Street;

void initializeStreet(void)
{
    // TODO
    Street.mx = uthread_sem_create(1);
    Street.direction = 0;
    Street.canCross[EAST] = uthread_sem_create(0);
    Street.canCross[WEST] = uthread_sem_create(0);
    Street.count = 0;
    Street.count_with_direction[EAST] = 0;
    Street.count_with_direction[WEST] = 0;
    Street.waiting[0] = 0;
    Street.waiting[1] = 0;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int entryTicker; // incremented with each entry
int waitingHistogram[WAITING_HISTOGRAM_SIZE];
int waitingHistogramOverflow;
uthread_sem_t waitingHistogramLock;
int occupancyHistogram[2][MAX_OCCUPANCY + 1];

void enterStreet(enum Direction g)
{
    // TODO
    uthread_sem_wait(Street.mx);
    int wait_time;
    int t;

    if (Street.direction == g)
    {
        for (int i = 0; i < MAX_OCCUPANCY - Street.count; i++)
        {
            uthread_sem_signal(Street.canCross[g]);
        }
    }

    if (Street.count == 0 && Street.direction != g)
    {
        if (Street.waiting[g] != 0)
        {
            Street.direction = g;
            for (int i = 0; i < MAX_OCCUPANCY; i++)
                uthread_sem_signal(Street.canCross[Street.direction]);
        }
    }

    if (Street.direction == g && Street.count_with_direction[g] < MAX_OCCUPANCY)
    {
        Street.count++;
        Street.count_with_direction[g]++;
        recordWaitingTime(0);
        entryTicker++;
        occupancyHistogram[g][Street.count]++;
        uthread_sem_signal(Street.mx);
        return;
    }
    else
    {
        t = entryTicker;
    }

    uthread_sem_signal(Street.mx);
    uthread_sem_wait(Street.canCross[g]);

    uthread_sem_wait(Street.mx);
    wait_time = entryTicker - t;

    recordWaitingTime(wait_time);
    Street.count++;
    Street.count_with_direction[g]++;
    entryTicker++;
    occupancyHistogram[g][Street.count]++;
    uthread_sem_signal(Street.mx);
}

void leaveStreet(enum Direction g)
{
    uthread_sem_wait(Street.mx);

    Street.count--;
    Street.count_with_direction[g]--;

    if ((Street.count == 0) && (Street.waiting[oppositeEnd[Street.direction]] != 0))
    {
        Street.direction = oppositeEnd[Street.direction];

        for (int i = 0; i < MAX_OCCUPANCY; i++)
        {
            if ((Street.direction == EAST) && (Street.waiting[EAST] != 0))
            {
                uthread_sem_signal(Street.canCross[EAST]);
            }
            else if ((Street.direction == WEST) && (Street.waiting[WEST] != 0))
            {
                uthread_sem_signal(Street.canCross[WEST]);
            }
        }
    }
    else if (Street.count < MAX_OCCUPANCY)
    {
        if (Street.direction == EAST && Street.waiting[EAST] != 0)
        {
            uthread_sem_signal(Street.canCross[EAST]);
        }
        else if (Street.direction == WEST && Street.waiting[WEST] != 0)
        {
            uthread_sem_signal(Street.canCross[WEST]);
        }
    }

    uthread_sem_signal(Street.mx);
}

void recordWaitingTime(int waitingTime)
{
    uthread_sem_wait(waitingHistogramLock);
    if (waitingTime < WAITING_HISTOGRAM_SIZE)
        waitingHistogram[waitingTime]++;
    else
        waitingHistogramOverflow++;
    uthread_sem_signal(waitingHistogramLock);
}

void *car()
{
    int d = random() % 2;

    uthread_sem_wait(Street.mx);

    Street.waiting[d]++;
    uthread_sem_signal(Street.mx);

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        enterStreet(d);

        for (int j = 0; j < CROSSING_TIME; j++)
        {
            uthread_yield();
        }

        leaveStreet(d);

        for (int j = 0; j < WAIT_TIME_BETWEEN_CROSSES; j++)
        {
            uthread_yield();
        }
    }

    uthread_sem_wait(Street.mx);
    Street.waiting[d]--;
    uthread_sem_signal(Street.mx);
}

int main(int argc, char **argv)
{

    uthread_init(8);

    waitingHistogramLock = uthread_sem_create(1);
    entryTicker = 0;

    initializeStreet();
    uthread_t pt[NUM_CARS];

    for (int i = 0; i < NUM_CARS; i++)
        pt[i] = uthread_create(car, NULL);

    for (int i = 0; i < NUM_CARS; i++)
        uthread_join(pt[i], NULL);

    printf("Times with 1 car  going east: %d\n", occupancyHistogram[EAST][1]);
    printf("Times with 2 cars going east: %d\n", occupancyHistogram[EAST][2]);
    printf("Times with 3 cars going east: %d\n", occupancyHistogram[EAST][3]);
    printf("Times with 1 car  going west: %d\n", occupancyHistogram[WEST][1]);
    printf("Times with 2 cars going west: %d\n", occupancyHistogram[WEST][2]);
    printf("Times with 3 cars going west: %d\n", occupancyHistogram[WEST][3]);

    printf("Waiting Histogram\n");
    for (int i = 0; i < WAITING_HISTOGRAM_SIZE; i++)
        if (waitingHistogram[i])
            printf("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
                   i, i == 1 ? " " : "s", waitingHistogram[i]);
    if (waitingHistogramOverflow)
        printf("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
               WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}