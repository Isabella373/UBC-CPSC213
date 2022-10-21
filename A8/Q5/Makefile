CFLAGS += -std=gnu11 -g
EXES    = sm

all:  $(EXES)
clean:
	-rm -f $(EXES)

namelist: sm-jt.c
# don't treat all and clean as file targets
.PHONY: all clean
