CFLAGS += -std=gnu11 -g
EXES    = e4

all:  $(EXES)
clean:
	-rm -f $(EXES)

namelist: e4.c
# don't treat all and clean as file targets
.PHONY: all clean
