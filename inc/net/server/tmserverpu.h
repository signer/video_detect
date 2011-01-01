#ifndef _TMSERVERPU_H_
#define _TMSERVERPU_H_

#ifndef MAX_CHANNEL_SUPPORT
#define MAX_CHANNEL_SUPPORT 128                 /* modified at 20060405 */
#endif

typedef struct
{
    char    m_datatype[MAX_CHANNEL_SUPPORT];    /* not use now */
    char    m_channum;
    int     m_waittime; /* 1-300,minutes, not use now */
    int     m_bufnum;   /* not use now */
    /*
     * add at 2003.1.27 ;
     * int m_channelHandle[MAX_CHANNEL_SUPPORT];
     */
} SERVER_VIDEOINFO, *PSERVER_VIDEOINFO;

/* Function */
#ifdef __cplusplus
#define FUNCTION_TYPE   extern "C"
#else
#define FUNCTION_TYPE
#endif
FUNCTION_TYPE int MP4_ServerGetState();
FUNCTION_TYPE int MP4_ServerSetNetPort(int iServerPort, int iClientPort);
FUNCTION_TYPE int MP4_ServerSetMessage(void (*MessageCallBack) (char*buf, int iLen));
FUNCTION_TYPE int MP4_ServerStart(PSERVER_VIDEOINFO videoinfo);
FUNCTION_TYPE int MP4_ServerStop();
FUNCTION_TYPE int MP4_ServerCheckIP(int (*CheckIPCallBack) (int iChannel, char*sIP));
FUNCTION_TYPE int MP4_ServerCheckPassword(int (*CheckPasswordCallBack) (char*usename, int namelen, char*password, int passlen));

FUNCTION_TYPE int MP4_ServerWriteData(int nPort, char*  pPacketBuffer, int nPacketSize,
                                      int frameType, int breakable);
FUNCTION_TYPE int MP4_ServerStringToClient(char* m_lAddrIP, char* m_sCommand, int miLen);
FUNCTION_TYPE int MP4_ServerSetBufNum(int nChannel, int dBufNum);
FUNCTION_TYPE int MP4_ServerResetChannel(int nChannel);
FUNCTION_TYPE int MP4_ServerSetStart(void (*StartCapCallBack) (int port));
FUNCTION_TYPE int MP4_ServerSetStop(void (*StopCapCallBack) (int port));
FUNCTION_TYPE int MP4_ServerSetTTL(unsigned char cTTLVal);
FUNCTION_TYPE int MP4_ServerCommandToClient(int iChannel, unsigned char cCommand);

/* add at 2003.11.24 to set the multicast add and port */
FUNCTION_TYPE int MP4_ServerSetMulticast(int iChannel, char* m_sIpAddr, int iPort);

/*
 * add at 2004.11.8 to support two stream type data in the same channel in server
 * side. subchan: only be 0 or 1
 */
FUNCTION_TYPE int MP4_ServerWriteDataEx(int nPort, char*  pPacketBuffer, int nPacketSize,
                                        int frameType, int breakable, int subchan);
FUNCTION_TYPE int MP4_ServerGetLastErrorNum();

/* add at 2005.10.7 to set the ip and passwd check */
FUNCTION_TYPE int MP4_ServerCheckIPandPassword(int (*CheckIPandPass) (int iChannel, char*username,
                                               int namelen, char*password, int passlen, char*sIP));

/* add at 2005.10.7 to register disconnect event */
FUNCTION_TYPE int MP4_ServerDisconnectEvent(void (*DisconnectEvent) (int nport, char*sIP));

/* add at 2005.10.27 to disconnect the client */
FUNCTION_TYPE int MP4_ServerDisconnectClient(int iChannel, char* ClientAddr);

#endif
