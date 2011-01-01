#include "detect.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
	int queue_id = 0;
	const char * filename = NULL;
	if (argc >= 2){
		queue_id = atoi(argv[1]);	
		if (argc >= 3){
			filename = argv[2];
		}
	}else{
		printf("usage: %s queue_id [filename]\n", argv[0]);
		return 0;
	}
	int ret = video_start2(queue_id, filename);
	if (ret != 0){
		fprintf(stderr,"video start error\n");
		exit(1);
	}
	sleep(10000);
	return 0;
}
