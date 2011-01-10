LIBS:=-lcv -lcxcore -lhighgui -lcvaux -lpthread 
CFLAGS:= -g -Wall -I./inc/sys -I/usr/include/opencv
CC:=g++

WITHOUT_SDK:=1

ifeq ($(WITHOUT_SDK),1)
CFLAGS += -DWITHOUT_SDK
else
CFLAGS += -lds40xxsdk -O2
endif

all : libdetect.so demo add_error msgserver 
demo: main.c
	gcc $^ -L./ -ldetect -o $@
libdetect.so : detect.cpp image.cpp log.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -fPIC -shared -o $@ 
msgserver: msgserver.c
	$(CC) $^ -o $@
add_error: add_error.cpp
	$(CC) $^ -o $@
install:
	sudo cp ./libdetect.so /usr/lib
clean:
	rm -r *.o *.so ./demo ./add_error ./msgserver
run : libdetect.so install 
	./demo 1 ../video/1.avi
