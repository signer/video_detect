/*
 * yxq add at 9.27 ;
 * use to set the use of the SDL with call the function MP4_SetSdlUse().If set 1
 * then direct play by playsdk.If set 0 then deal with it by yourself
 */
#ifndef _TMCLIENTPU_H_
#define _TMCLIENTPU_H_

#define NO_USE_SDL  0
#define USE_SDL     1
enum { PTOPUDPMODE = 0, PTOPTCPMODE, MULTIMODE, ONLYAUDIOMODE, NOUSEMODE };

typedef struct
{
    char            m_bRemoteChannel;
    char            m_bSendMode;
    char            m_bRight;   /* note: In v38, this parameter have been changed to index subchan0 or subchan1 */
    char*           m_sIPAddress;
    char*           m_sUserName;
    char*           m_sUserPassword;
    int             m_bUserCheck;

    unsigned int    subshow_x;
    unsigned int    subshow_y;
    unsigned int    subshow_uWidth;
    unsigned int    subshow_uHeight;
} CLIENT_VIDEOINFO, *PCLIENT_VIDEOINFO;

typedef struct tagClientShowRect
{
    unsigned int    uWidth;
    unsigned int    uHeight;
} CLIENTSHOWRECT;

/* Fuction */
#ifdef __cplusplus
#define FUNCTION_TYPE   extern "C"
#else
#define FUNCTION_TYPE
#endif
FUNCTION_TYPE int   MP4_ClientSetNetPort(int iServerPort, int iClientPort);
FUNCTION_TYPE int   MP4_ClientStartup(CLIENTSHOWRECT clientrect,
                                      void MessageCallBack (int imark, char command, char*buf, int iLen));
FUNCTION_TYPE int   MP4_ClientCleanup();

FUNCTION_TYPE int   MP4_ClientBeginMix();
FUNCTION_TYPE int   MP4_ClientMixAudioStart(int StockHandle);
FUNCTION_TYPE int   MP4_ClientMixAudioStop(int StockHandle);
FUNCTION_TYPE int   MP4_ClientStopMix();

FUNCTION_TYPE int   MP4_ClientGetServerChanNum(char* m_sIPAddress);

FUNCTION_TYPE int   MP4_ClientStart(PCLIENT_VIDEOINFO pClientinfo,
                                    void ReadDataCallBack (int StockHandle, char*pPacketBuffer, int nPacketSize));
FUNCTION_TYPE int   MP4_ClientStop(int StockHandle);

FUNCTION_TYPE int   MP4_ClientGetState(int StockHandle);
FUNCTION_TYPE int   MP4_ClientCommandToServer(char* m_sIPAddress, char* m_sCommand, int m_iLen);
FUNCTION_TYPE int   MP4_ClientCommandToServer_Handle(int StockHandle, char* m_sCommand, int m_iLen);

FUNCTION_TYPE int   MP4_ClientStartCapture(int StockHandle);
FUNCTION_TYPE int   MP4_ClientStartCaptureFile(int StockHandle, char* FileName);
FUNCTION_TYPE int   MP4_ClientStopCapture(int StockHandle);
FUNCTION_TYPE int   MP4_ClientShut(char* m_lAddrIP, char cChannel);

FUNCTION_TYPE int   MP4_ClientSetBufferSize(int StockHandle, int iBufsize);
FUNCTION_TYPE int   MP4_ClientSetDelayBufferSize(int StockHandle, int iDelaybufsize);

FUNCTION_TYPE int   MP4_ClientAudioStart(int StockHandle);
FUNCTION_TYPE int   MP4_ClientAudioStop();
FUNCTION_TYPE int   MP4_ClientAudioVolume(int StockHandle, unsigned int sVolume);

/* yxq add at 9.27 */
FUNCTION_TYPE void  MP4_SetSdlUse(unsigned int used);
FUNCTION_TYPE int   MP4_ClientGetLastErrorNum();
FUNCTION_TYPE int   MP4_ClientStartCaptureEx(int StockHandle, char* headerbuf, int* headerlen);

/* add at 2003.11.24 to set the multicast add and port */
FUNCTION_TYPE int   MP4_ClientSetMulticast(int StockHandle, char* m_sIpAddr, int iPort);

/* add at 2004.8.16 to set the media relay server */
FUNCTION_TYPE int   MP4_ClientSetMR(char*  strFirstMRIP, char*  strSecondMRIP,
                                    unsigned int wFirstMRPort);

/*
 * add at 2004.10.27: Decoder: 1--enable the client to decoder the stream;
 * 0--disable;
 * default--enable
 */
FUNCTION_TYPE void  MP4_SetDecoder(unsigned int Decoder);

/* add at 2005.6.27 to export the setting for decoder quality:0--low, 1--high */
FUNCTION_TYPE int   MP4_ClientSetDecodeQuality(int StockHandle, int quality);

/* add at 2005.10.7 to set the timeout for connect */
FUNCTION_TYPE int   MP4_ClientSetTimeout(int time);
FUNCTION_TYPE int   MP4_ClientSetDecodeIOnly(int StockHandle, int enable);

/* 2007.01.15 tyu add for heart */
FUNCTION_TYPE int   MP4_ClientSetHeartbeat(int timeout);    /* Just for UDP */

#endif
