.SUFFIXES : .o .c .cpp
#CC=gcc
#CXX=g++
#INC=./include/
CFLAGS=-W -Wall
LDFLAGS=-s
LDLIBS=-pthread
#LDLIBS=-pthread -lm -lz -lcrypt -lrt
#OBJS=main.o sock.o thread.o commpkt.o jigpkt.o date.o str.o file.o config.o tty.o ap520.o ap522.o ap471.o
OBJS=main.o sock.o thread.o commpkt.o jigpkt.o str.o file.o config.o tty.o ap471.o
OBJS_AP471=./AP471/apcommon.o ./AP471/cnfg471.o ./AP471/rpnt471.o ./AP471/rsts471.o ./AP471/wpnt471.o ./AP471/wprt471.o ./AP471/rprt471.o
SRCS=$(OBJS:.o=.c)
TARGET=jigd

all: $(TARGET)

clean:
	rm *.o
	rm $(TARGET)

.cpp.o:
	$(CXX) -c $< -o $@

$(TARGET): $(OBJS) $(OBJS_AP471) jigapp.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^
	ctags -R
	sudo cp $(TARGET) /media/sf_Downloads/

