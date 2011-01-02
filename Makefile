LIBS:=-lcv -lcxcore -lml -lhighgui -lcvaux -lpthread 
CFLAGS:=-O2 -Wall -I./inc/opencv -I./inc/sys 
CC:=g++

WITHOUT_SDK:=1

ifeq ($(WITHOUT_SDK),1)
CFLAGS += -DWITHOUT_SDK
else
CFLAGS += -lds40xxsdk
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
	cp ./libdetect.so /usr/lib
clean:
	rm -r *.o *.so ./demo ./add_error ./msgserver
