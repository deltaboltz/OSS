CC      = gcc
CFLAGS  = -Wall -g
SRCDIR	= src
BINDIR	= bin
TARGET1 = $(BINDIR)/oss
TARGET2 = $(BINDIR)/ossrcv
OBJS1   = $(SRCDIR)/oss.o
OBJS2   = $(SRCDIR)/ossrcv.o

.SUFFIXES: .c .o

.PHONY: all clean

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1) | $(BINDIR)
	$(CC) $^ -o $@

$(TARGET2): $(OBJS2) | $(BINDIR)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BINDIR) ./src/*.o
