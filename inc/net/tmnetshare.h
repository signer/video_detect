#ifndef _TMNETSHARE_H_
#define _TMNETSHARE_H_

#define TM_TV_SEC       1
#define TM_TV_USEC      0

#define HEADERLEN       40
#define SENDBUFLEN      1024 * 8
#define SENDAUDIOBUFLEN 512

/*
 * #define MULTILOCALTEST ;
 * COMMAND
 */
enum
{
    MULTICAST       = 0,    /* 多播＋RTP */
    REPLYMULTICAST,         /* 多播＋RTP连接的响应 */
    CLOSEMULTICAST,         /* 关闭 多播＋RTP连接 */
    PTOPUDP,                /* UDP+RTP */
    REPLYPTOPUDP,           /* UDP＋RTP连接的响应 */
    CLOSEPTOPUDP,           /* 关闭UDP＋RTP连接 */
    PTOPTCP,                /* TCP */
    REPLYPTOPTCP,           /* TCP连接的响应 */
    CLOSEPTOPTCP,           /* 关闭TCP连接 */
    BECHECK,                /* 询问服务器类型 */
    GETCHANNUM,             /* 获取服务器通道个数 */
    COMMANDTOSERVER,        /* 向服务器发送字符串 */
    RESETONE,               /* 复位服务器通道 */
    REFUSED,                /* 服务器身份验证 */
    ONLYAUDIO       = 19,   /* new audio video independence失败，拒绝访问 */
    MRELAY,
    REPLAYMRELAY,
    CLOSEMRELAY,

    /* UPDATEINFO */
};

enum { QULIFIED = 0, ERRORCHANNELNO, CHANNELINUSE, LESSNETWIDTH };

typedef struct
{
    unsigned long   commandlen;
    unsigned char   mark;
    unsigned char   right;
    unsigned char   commandID;
    unsigned char   channel;
} C2SCOMMAND;
#endif
