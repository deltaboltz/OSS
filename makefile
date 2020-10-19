CC      = gcc
CFLAGS  = -Wall -g
TARGET1 = ./src/oss
TARGET2 = ./src/ossrcv
OBJS1   = oss.o
OBJS2   = ossrcv.o

.SUFFIXES: .c .o

all: $(TARGET1) $(TARGET2)

oss: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $@

ossrcv: $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.log *.o *~ $(TARGET1) $(TARGET2)