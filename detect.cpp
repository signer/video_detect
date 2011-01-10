#include "detect.h"
#include "image.h"
#include "log.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <semaphore.h>

#include <DataType.h>
#include <ds40xxsdk.h>
using namespace std;
#define MSG_INTERVAL 5  // send msg every MSG_INTERVAL seconds
#define IMG_WIDTH 704
#define IMG_HEIGHT 576
#define IMG_BUF_SIZE 704*576*2

#define RELEASE_FRAME(x) if (x) {cvReleaseImage(&x);x=NULL;}else{}


#ifdef WITHOUT_SDK // for test 
#define ChannelOpen(x,y) 0
#define GetOriginalImage(x,y,z) 0
#define ChannelClose(x) 0
#endif

pthread_t g_thread_id;
int g_running = 0;
int g_queue_id = 0;
int g_channel = -1;
CvCapture *g_capture = NULL;


// video idx member functions

video_idx::video_idx(){
	init();
}

void video_idx::init(){
	type = 1;
	time = 0;
	freezing_cnt = 0;
	black_cnt = 0;
	mosaic_cnt = 0;
}


const char *video_idx::str(){
	static char buf[1024];
	sprintf(buf, "idx={type:%ld, time:%ld(%s), freeze:%ld, black:%ld, mosaic:%ld}", 
			type, time, time2str(time), freezing_cnt, black_cnt, mosaic_cnt);
	return buf;
}

			
void stream_callback(int channel, char* databuf, int frametype, int framelength){
	// do nothing
}

// interfaces for capture images
int open_channel(const char * filename = NULL)
{
	if (filename != NULL){
		if (!g_capture){
			g_capture = cvCaptureFromAVI(filename);
			LOG("opening channel from avi %s", filename);
		}
		return g_capture ? 0 : -1;
	}else{
#ifdef WITHOUT_SDK
		fprintf(stderr, "please set WITHOUT_SDK:=1 in Makefile");
		exit(-1);
#else

		if (g_channel == -1){
			if (InitDSPs() == 0){
				LOG("init dsps failed");
				return -1;
			}
			LOG("init dsps ok");
			g_channel = ChannelOpen(0, stream_callback);
			if (g_channel < 0){
				LOG("open channel 0 failed");
			}
		}
		return (g_channel>=0) ? 0 : -1;
#endif
	}
}

IplImage* query_frame()
{
	static IplImage *pIplImage = NULL; 
	if (g_capture){
		IplImage *pFrame = cvQueryFrame(g_capture);	
		if (pFrame == NULL)
			return NULL;
		if (!pIplImage)
			pIplImage = cvCreateImage(cvSize(pFrame->width, pFrame->height),IPL_DEPTH_8U,1); 
		cvCvtColor(pFrame, pIplImage, CV_BGR2GRAY);
	}else{
#ifdef WITHOUT_SDK
		fprintf(stderr, "please set WITHOUT_SDK:=1 in Makefile");
		exit(-1);
#else
		if (!pIplImage)
			pIplImage = cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT),IPL_DEPTH_8U,1); 
		static int n = 0;
		unsigned char img_buf[IMG_BUF_SIZE];
		int size = IMG_BUF_SIZE;
		GetOriginalImage(g_channel, img_buf, &size);

		//char filename[1024];
		//sprintf(filename, "./images/frame%d.bmp", n++);
		//SaveYUVToBmpFile(filename, img_buf, IMG_WIDTH, IMG_HEIGHT); 
		memcpy(pIplImage->imageData, img_buf, IMG_WIDTH*IMG_HEIGHT);
#endif
	}
	return pIplImage;
}

void close_channel()
{
	if (g_capture){
		cvReleaseCapture(&g_capture);
		g_capture = NULL;
	}else{
		if (g_channel >= 0){
			g_channel = -1;
		}
	}
}

// main thread of video detect
void* detect(void *para){
	g_running = 1;

	struct video_idx idx;
	idx.freezing_cnt = 0;
	idx.mosaic_cnt   = 0;
	idx.black_cnt    = 0;
	IplImage *prev_frame = NULL;
	IplImage *cur_frame = NULL;
	int nFrmNum = 0;
	time_t last_time = time(NULL);
	while (g_running){
		int to_save = 0;
		
		IplImage *pFrame = query_frame();
		
		if (pFrame == NULL){
			LOG("query frame failed, reach eof?");
			sleep(5);
			RELEASE_FRAME(prev_frame);
			continue;
		}

		cout << "Frame " << ++nFrmNum << endl;
		cur_frame = cvCloneImage(pFrame);

		cvShowImage("video", cur_frame);
		
#ifdef WITHOUT_SDK
		if (cvWaitKey(1000000) > 0){
			//continue;
		}
#endif
		// freezing
		if (prev_frame && is_freezing(prev_frame, cur_frame)){
			LOG("freezing @ Frame %d", nFrmNum);
			idx.freezing_cnt++;
			to_save = 1;
		}
		
		
		// black
		if (is_black(cur_frame)){
			LOG("black @ Frame %d", nFrmNum);
			idx.black_cnt++;
			to_save = 1;
		}
		

		// mosaic
		if (is_mosaic(cur_frame)){
			LOG("mosaic @ Frame %d", nFrmNum);
			idx.mosaic_cnt++;
			to_save = 1;
		}

		time_t cur_time = time(NULL);

		if (to_save){
			save_image(pFrame, "frame");
		}

		// send msg every 5 seconds
		if (cur_time-last_time >= MSG_INTERVAL){
			last_time = cur_time;
			idx.time = cur_time;
			cout << "msg sending"<< endl;
			msgsnd(g_queue_id, &idx, sizeof(idx)-sizeof(long), 0);
			LOG("send msg, %s", idx.str()); 
			idx.freezing_cnt = 0;
			idx.black_cnt = 0;
			idx.mosaic_cnt = 0;
		}

		
		RELEASE_FRAME(prev_frame);
		
		prev_frame = cur_frame;
		usleep(100);
	}
	return 0;
}

int video_start(int queue_id, int channel)
{
	LOG("queue_id=%d", queue_id);
	g_queue_id = queue_id;
	g_channel = channel;
	if (!g_running)
	{
		pthread_create(&g_thread_id, NULL, detect, NULL);
	}
	return 0;
}

int video_start2(int queue_id, const char* filename)
{
	if (-1 == access(IMAGE_PATH, F_OK)){
		mkdir(IMAGE_PATH, S_IRWXU);
	}
	LOG("queue_id=%d", queue_id);
	g_queue_id = queue_id;
	if (!g_running)
	{
		if (0 != open_channel(filename)){
			return -1;
		}
		LOG("open channel ok");
		pthread_create(&g_thread_id, NULL, detect, NULL);
	}
	return 0;
}

int video_end()
{
	LOG("");
	g_running = 0;
	close_channel();
	return 0;
}
