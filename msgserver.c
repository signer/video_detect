/*msgserver.c*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>

#define   MSG_FILE "msgserver.c"
#define   BUFFER 255
#define   PERM S_IRUSR|S_IWUSR
/* 服务端创建的消息队列最后没有删除,我们要使用ipcrm命令来删除的 */
/* ipcrm -q <msqid> */

struct msgtype {
	long mtype;
	char buffer[BUFFER+1];
};


struct video_idx 
{
	long type;
	long time;
	long freezing_cnt;
	long black_cnt;
	long mosaic_cnt;
};

int msgid;

void sig(int){
	char cmd[1024];
	sprintf(cmd, "ipcrm -q %d", msgid);
	system(cmd);
	printf("quit\n");	
	exit(0);
}
int main()
{
	struct msgtype msg;
	key_t key;
	
	
	if((key=ftok(MSG_FILE,'a'))==-1)
	{
		fprintf(stderr,"Creat Key Error:%s\n", strerror(errno));
		exit(1);
	}

	if((msgid=msgget(key, PERM|IPC_CREAT|IPC_EXCL))==-1)
	{
		fprintf(stderr, "Creat Message Error:%s\n", strerror(errno));
		exit(1);
	}
	printf("msgid = %d\n", msgid);
	signal(SIGINT, sig);
	struct video_idx idx;
	while(1)
	{
		int len = msgrcv(msgid, &idx, 10000/*sizeof(struct video_idx)*/, 1, 0);
		if (len == -1){
			perror("msgrcv error");
			continue;
		}
		printf("len=%d\n", len);
		fprintf(stderr, "Server Receive:type=%ld, mosaic=%ld, black=%ld, freezing=%ld\n", idx.type, idx.mosaic_cnt , idx.black_cnt, idx.freezing_cnt);
		//msg.mtype = 1;
		//msgsnd(msgid, &msg, sizeof(struct msgtype), 0);
	}
	
	exit(0);
} 
