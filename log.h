#ifndef __LOG_H___
#define __LOG_H___

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define LOG_FILE_NAME "video_detect.log"
#define IMAGE_PATH    "./images"
#define MAX_LINE_SIZE 10240

#define VERBOSE 1

extern FILE *g_flog ;

static inline char * time2str(time_t t)
{
	if (t == 0)
		time(&t);
	struct tm tm;
	tm = *localtime(&t);

	static char buffer[32];
	strftime(buffer, 32, "%Y%m%d_%H%M%S", &tm);
	return buffer;
}

static inline void __log(const char * file, const char* func, int line, const char* fmt, ... ) 
{
	char buffer[MAX_LINE_SIZE];
	int prefix_len = 0;
	va_list ap;
	if (g_flog == NULL)
		g_flog = fopen(LOG_FILE_NAME, "w");
	
	if (g_flog == NULL){
		perror("open log file error"); 
		exit(-1);
	}
	
	va_start(ap, fmt);
	snprintf(buffer, MAX_LINE_SIZE, "[%s %s:%d %s()]: ", 
			time2str(0), file, line, func);
	prefix_len = strlen(buffer);	
	vsnprintf(buffer+prefix_len, MAX_LINE_SIZE-prefix_len, fmt, ap);
#if VERBOSE
	printf("%s\n", buffer+prefix_len);
#endif
    fprintf(g_flog, "%s\n", buffer);
	fflush(g_flog);
	va_end(ap);
	return ;
}

#define LOG(fmt, ...) do {\
	__log(__FILE__, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__ ); \
}while(0); 

//#undef LOG
//#define LOG(fmt, ...) do {} while(0);
#define FLUSHLOG() do { if (g_flog) { fclose(g_flog); } } while(0);

#define ALERT printf("%s %s() :%d\n", __FILE__, __FUNCTION__, __LINE__);

#endif
