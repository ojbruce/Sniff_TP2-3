CC = gcc
CFLAGS = -O3
LDFLAGS = 

.PHONY:  all clean

LIB = eth_lib.o
SNIFF = sniff
WRITE_ETH_FRAME = write_eth_frame

all : $(LIB) $(SNIFF) $(WRITE_ETH_FRAME)

lib : $(LIB)

$(LIB) : eth_lib.c eth_lib.h inet_str.h
	$(CC) -c $(CFLAGS) eth_lib.c

$(SNIFF) : $(LIB) sniff.c
	$(CC) -c $(CFLAGS) sniff.c
	$(CC) $(LDFLAGS) sniff.o $(LIB) -o $@

$(WRITE_ETH_FRAME) : $(LIB) write_eth_frame.c
	$(CC) -c $(CFLAGS) write_eth_frame.c 
	$(CC) $(LDFLAGS) write_eth_frame.o $(LIB) -o $@

clean:
	rm -rf *.o
	rm -rf $(WRITE_ETH_FRAME) $(SNIFF) 
