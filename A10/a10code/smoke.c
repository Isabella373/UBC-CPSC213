#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 10000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signaling[TOBACCO + 1];
int smoking[TOBACCO + 1];

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->match   = uthread_cond_create(agent->mutex);
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

struct Session {
  struct Agent* agent;
  int match, paper, tobacco;
};

struct Smoker {
  struct Session* sesh;
  int type;
};

struct Session* createSession(struct Agent* agent) {
  struct Session* sesh = malloc (sizeof (struct Session));
  sesh->agent = agent;
  sesh->match = 0;
  sesh->paper = 0;
  sesh->tobacco = 0;
  return sesh;
}

struct Smoker* createSmoker(struct Session* pool, int type) {
  struct Smoker* smoker = malloc (sizeof (struct Smoker));
  smoker->sesh = pool;
  smoker->type = type;
  return smoker;
}

void* smoker (void* av) {
  struct Smoker* s = av;
  struct Session* ss = s->sesh;
  struct Agent* a = ss->agent;
  uthread_mutex_lock(a->mutex);
  while (0==0) {
    if (s->type == MATCH) {
      uthread_cond_wait(a->match);
      if (ss->paper > 0 && ss->tobacco > 0) {
        ss->paper--;
        ss->tobacco--;
        smoking[s->type]++;
        uthread_cond_signal(a->smoke);
      } else {
        ss->match++;
      }

    } else if (s->type == PAPER) {
      uthread_cond_wait(a->paper);
      if (ss->match > 0 && ss->tobacco > 0) {
        ss->match--;
        ss->tobacco--;
        smoking[s->type]++;
        uthread_cond_signal(a->smoke);
      } else {
        ss->paper++;
      }

    } else if (s->type == TOBACCO) {
      uthread_cond_wait(a->tobacco);
      if (ss->paper > 0 && ss->match > 0) {
        ss->match--;
        ss->paper--;
        smoking[s->type]++;
        uthread_cond_signal(a->smoke);
      } else {
        ss->tobacco++;
      }

    }
    if (ss->paper > 0 && ss->tobacco > 0) {
      uthread_cond_signal(a->match);
    } else if (ss->paper > 0 && ss->match > 0) {
      uthread_cond_signal(a->tobacco);
    } else if (ss->tobacco > 0 && ss->match > 0) {
      uthread_cond_signal(a->paper);
    }
  }
  uthread_mutex_unlock(a->mutex);
}


/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signaling[matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  uthread_t smokerm, smokerp, smokert;
  struct Agent*  a = createAgent();
  struct Session* sesh = createSession(a);

  smokerm = uthread_create(smoker, createSmoker(sesh, MATCH));
  smokerp = uthread_create(smoker, createSmoker(sesh, PAPER));
  smokert = uthread_create(smoker, createSmoker(sesh, TOBACCO));

  uthread_join (uthread_create (agent, a), 0);
  assert (signaling[MATCH]   == smoking[MATCH]);
  assert (signaling[PAPER]   == smoking[PAPER]);
  assert (signaling[TOBACCO] == smoking[TOBACCO]);
  assert (smoking[MATCH] + smoking[PAPER] + smoking[TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoking[MATCH], smoking[PAPER], smoking[TOBACCO]);
}
