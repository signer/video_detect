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
    MULTICAST       = 0,    /* �ಥ��RTP */
    REPLYMULTICAST,         /* �ಥ��RTP���ӵ���Ӧ */
    CLOSEMULTICAST,         /* �ر� �ಥ��RTP���� */
    PTOPUDP,                /* UDP+RTP */
    REPLYPTOPUDP,           /* UDP��RTP���ӵ���Ӧ */
    CLOSEPTOPUDP,           /* �ر�UDP��RTP���� */
    PTOPTCP,                /* TCP */
    REPLYPTOPTCP,           /* TCP���ӵ���Ӧ */
    CLOSEPTOPTCP,           /* �ر�TCP���� */
    BECHECK,                /* ѯ�ʷ��������� */
    GETCHANNUM,             /* ��ȡ������ͨ������ */
    COMMANDTOSERVER,        /* ������������ַ��� */
    RESETONE,               /* ��λ������ͨ�� */
    REFUSED,                /* �����������֤ */
    ONLYAUDIO       = 19,   /* new audio video independenceʧ�ܣ��ܾ����� */
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
