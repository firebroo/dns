TARGET = sniffer

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

INSTALLDIR = /bin/

CC = cc
CFLAGS = -O2 -I/usr/include/libbson-1.0 -lbson-1.0

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

install:
	mv $(TARGET) $(INSTALLDIR)

clean:
	rm -rf $(TARGET) $(OBJS)

depclean:
	sudo rm -rf $(TARGET) $(OBJS) $(INSTALLDIR)$(TARGET)


%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<
