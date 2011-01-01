#ifndef __DETECT_H__
#define __DETECT_H__

#ifdef __cplusplus
extern "C" {
#endif

struct video_idx 
{
	long type;
	long time;
	long freezing_cnt;
	long black_cnt;
	long mosaic_cnt;
#ifdef __cplusplus
	video_idx();
	void init();
	const char* str();
#endif
};


int video_start(int queue_id, int channel);

int video_end();

// for test only
int video_start2(int queue_id, const char * filename);

#ifdef __cplusplus
}
#endif

#endif
