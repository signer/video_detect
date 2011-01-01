LIBS:=-lcv -lcxcore -lml -lhighgui -lpthread  -lds40xxsdk -lm4play -ltmnetc -ltmnets
CFLAGS:=-O2 -I/usr/include/opencv/ -I/usr/include/opencv2
CC:=g++

all : libdetect.so demo add_error msgserver 
demo: main.c
	gcc $(CFLAGS) $^ -L./ -ldetect -o $@
libdetect.so : detect.cpp image.cpp log.c
	$(CC) $(CFLAGS) $(LIBS) $^ -fPIC -shared -o $@ 
msgserver: msgserver.c
	$(CC) $^ -o $@
add_error: add_error.cpp
	$(CC) $^ -o $@
install:
	cp ./libdetect.so /usr/lib
clean:
	rm -r *.o *.so ./demo ./add_error ./msgserver
