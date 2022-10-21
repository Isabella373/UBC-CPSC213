CFLAGS += -std=gnu11 -g
EXES   =  job_main
OBJS   =  job_main.o job.o organize.o refcount.o

all: $(EXES)

job_main: $(OBJS)

clean:
	rm -f $(EXES) $(OBJS)
tidy:
	rm -f $(OBJS)