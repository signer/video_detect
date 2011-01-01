#ifndef _DS40XXSDK_H_
#define _DS40XXSDK_H_

/* The api for DS40xxHC DS400HT DS400MD board Last update time: 2005-7-18 */
#ifdef __cplusplus
extern "C"
{
#endif

#define ErrorCodeSuccess                0x0000

#define ErrorCodeDSPUninit              0x0001
#define ErrorCodeDSPNotReady            0x0002
#define ErrorCodeDSPLoadFail            0x0004
#define ErrorCodeDSPSyncObjectFail      0x0005
#define ErrorCodeDSPCountOverflow       0x0009
#define ErrorCodeEncodeChannelOverflow  0x000a
#define ErrorCodeDecodeChannelOverflow  0x000b
#define ErrorCodeBoardOverflow          0x000c
#define ErrorCodeDSPHexBlockLenOverflow 0x000d
#define ErrorCodeDSPProgramCheckoutFail 0x000e
#define ErrorCodeDSPInitRecheckFail     0x000f
#define ErrorCodeDSPBusy                0x0010

#define ErrorCodeNoCardInstalled        0x0102
#define ErrorCodeIoctlFail              0x0103
#define ErrorCodeMemLocateFail          0x0104
#define ErrorCodeDuplicateSN            0x0105
#define ErrorCodeCreateThreadFail       0x0106
#define ErrorCodeDSPCmdInvalid          0x0107
#define ErrorCodeHOSTCmdInvalid         0x0108
#define ErrorCodeDSPRespondCmdFail      0x0109
#define ErrorCodeOrderFail              0x010a
#define ErrorCodeKernelFail             0x010b
#define ErrorCodeStreamBufUnderflow     0x010c

#define ErrorCodeChannelOutofRange      0x0200
#define ErrorCodeInvalidEncodeChannel   0x0201
#define ErrorCodeInvalidArgument        0x0202
#define ErrorCodeNotSupport             0x0203
#define ErrorCodeMmapFail               0x0204

#define ErrorCodeFileInvalid            0x0301
#define ErrorCodeOpenFileFail           0x0302
#define ErrorCodeFileSizeZero           0x0303
#define ErrorCodeBadFileFormat          0x0304
#define ErrorCodeBadFileHeader          0x0305
#define ErrorCodeParaOver               0x0306
#define ErrorCodeCreateFileFail         0x0307
#define ErrorCodeNoSpare                0x0308
#define ErrorCodeInvalidDevice          0x0309
#define ErrorCodeInsufficientMem        0x030a
#define ErrorCodeCardSerialError        0x030b
#define ErrorCodeInvalidDecodeChannel   0x030c
#define ErrorCodeOutOfMemory            0x030d
#define ErrorCodeSemCreateFail          0x030e

/* type define // */
#define STREAM_TYPE_VIDEO   1
#define STREAM_TYPE_AUDIO   2
#define STREAM_TYPE_AVSYNC  3

#define HW_JUMP_FORWARD     309
#define HW_JUMP_BACKWARD    310

/* preview mode */
#define NORMAL_SIZE 0                   /* just not change */
#define D1_SIZE     1                   /* 704 x 576 */
#define DCIF_SIZE   2                   /* 528 x 288 */
#define CIF_SIZE    3                   /* 352 x 288 */
#define QCIF_SIZE   4                   /* 176 x 144 */
#define MINI_SIZE   5                   /* 88 x 72 */

typedef enum
{
    DS400XM             = 0,            /* 400XM */
    DS400XH             = 1,            /* 400XH */
    DS4004HC            = 2,            /* 4004HC */
    DS4008HC            = 3,            /* 4008HC */
    DS4016HC            = 4,            /* 4016HC */
    DS4001HF            = 5,            /* 4001HF */
    DS4004HF            = 6,            /* 4004HF */
    DS4002MD            = 7,            /* 4002MD */
    DS4004MD            = 8,            /* 4004MD */
    DS4016HCS           = 9,            /* 4016HCS */
    DS4002HT            = 10,           /* 4002HT */
    DS4004HT            = 11,           /* 4004HT */
    DS4008HT            = 12,           /* 4008HT */
    DS4004HC_PLUS       = 13,           /* 4004HC+ */
    DS4008HC_PLUS       = 14,           /* 4008HC+ */
    DS4016HC_PLUS       = 15,           /* 4016HC+ */
    DS4008HF            = 16,           /* 4008HF */
    DS4008MD            = 17,           /* 4008MD */
    DS4008HS            = 18,           /* 4008HS */
    DS4016HS            = 19,           /* 4016HS */
    INVALID_BOARD_TYPE  = 0xffffffff,
} BOARD_TYPE_DS;
typedef struct
{
    BOARD_TYPE_DS   type;               /* 板卡类型 */
    char            sn[16];             /* 序列号 */
    UINT            dspCount;           /* 板卡包含的DSP个数 */
    UINT            firstDspIndex;      /* 板卡上第一个DSP的索引 */
    UINT            encodeChannelCount; /* 板卡包含的编码通道个数 */
    UINT            firstEncodeChannelIndex;    /* 板卡上第一个编码通道的索引 */
    UINT            decodeChannelCount;         /* 板卡包含的解码通道个数 */
    UINT            firstDecodeChannelIndex;    /* 板卡上第一个解码通道的索引 */
    UINT            displayChannelCount;        /* 板卡包含的视频输出通道个数 */
    UINT            firstDisplayChannelIndex;   /* 板卡上第一个视频输出通道的索引 */
    UINT            reserved1;
    UINT            reserved2;
    UINT            reserved3;
    UINT            version;                    /* the version for hardware:format:major.minor.build, */
    /* major:bit 16-19,minor:bit 8-15, build: bit 0-7 */
} DS_BOARD_DETAIL;
typedef struct
{
    UINT    encodeChannelCount;                 /* 板卡包含的编码通道个数 */
    UINT    firstEncodeChannelIndex;            /* 板卡上第一个编码通道的索引 */
    UINT    decodeChannelCount;                 /* 板卡包含的解码通道个数 */
    UINT    firstDecodeChannelIndex;            /* 板卡上第一个解码通道的索引 */
    UINT    displayChannelCount;                /* 板卡包含的视频输出通道个数 */
    UINT    firstDisplayChannelIndex;           /* 板卡上第一个视频输出通道的索引 */
    UINT    reserved1;
    UINT    reserved2;
    UINT    reserved3;
    UINT    reserved4;
} DSP_DETAIL;
typedef struct tagChannelCapability
{
    UCHAR   bAudioPreview;
    UCHAR   bAlarmIO;
    UCHAR   bWatchDog;
} CHANNEL_CAPABILITY, *PCHANNEL_CAPABILITY;
typedef enum { brCBR = 0, brVBR = 1, } BitrateControlType_t;

typedef struct
{
    UINT    year;
    UINT    month;
    UINT    day;
    UINT    dayOfWeek;
    UINT    hour;
    UINT    minute;
    UINT    second;
    UINT    milliSecond;
} SYSTEMTIME;

typedef struct tagFramsStatistics
{
    ULONG   VideoFrames;
    ULONG   AudioFrames;
    ULONG   FramesLost;
    ULONG   QueueOverflow;
    ULONG   CurBps;
} FRAMES_STATISTICS, *PFRAMES_STATISTICS;

typedef struct _Preview_Config
{
    ULONG   imageSize;
    ULONG   w;
    ULONG   h;
    char*   dataAddr;
    sem_t*  SyncSem;    /* the semphore for have new image coming */
    sem_t*  ChangeSem;  /* the semphore for the image size changed */
} PREVIEWCONFIG, *PPREVIEWCONFIG;

/*
 -----------------------------------------------------------------------------------------------------------------------
 * Version info
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
    ULONG   DspVersion, DspBuildNum;
    ULONG   DriverVersion, DriverBuildNum;
    ULONG   SDKVersion, SDKBuildNum;
} HW_VERSION, *PHW_VERSION;

typedef struct
{
    long    bToScreen;
    long    bToVideoOut;
    long    nLeft;
    long    nTop;
    long    nWidth;
    long    nHeight;
    long    nReserved;
} DISPLAY_PARA, *PDISPLAY_PARA;
#define MAX_DISPLAY_REGION  16
typedef struct
{
    UINT    left;
    UINT    top;
    UINT    width;
    UINT    height;
    UINT    r;
    UINT    g;
    UINT    b;
    UINT    param;
} REGION_PARAM;

/*
 -----------------------------------------------------------------------------------------------------------------------
 * add at 2006.8.6 ;
 * face detecting: just for checking and evaluating, it maybe update more.
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
    short   x, y, width, height;
} FACE_AREA_DEMO;
typedef struct
{
    FACE_AREA_DEMO  faceArea;
    FACE_AREA_DEMO  leftEyeArea;
    FACE_AREA_DEMO  rightEyeArea;
    FACE_AREA_DEMO  leftPupilArea;
    FACE_AREA_DEMO  rightPupilArea;
    FACE_AREA_DEMO  noseArea;
    FACE_AREA_DEMO  mouthArea;
} FACE_INFO_DEMO;
typedef void (*FACE_DETECTION_DEMO_CALLBACK)
    (
        UINT nChannel,
        UINT nFaceCount,
        FACE_INFO_DEMO * pFaceInfo,
        char *pData,
        UINT nDataSize,
        UINT nImageWidth,
        UINT nImageHeight
    );

typedef void (*STREAM_READ_CALLBACK) (int cahnnelNum, char*pBuf, int frameType, int length);
typedef void*  (*IMAGE_STREAM_CALLBACK) (UINT channelNumber, void* context);
typedef void (*MOTION_DETECTION_CALLBACK) (ULONG channelNumber, int bMotionDetected, void *context);
typedef void (*DECODER_VIDEO_CAPTURE_CALLBACK)
    (
        UINT nChannelNumber,
        void *DataBuf,
        UINT width,
        UINT height,
        UINT nFrameNum,
        UINT nFrameTime,
        SYSTEMTIME * pFrameAbsoluteTime,
        void *context
    );
int     GetLastErrorNum();
int     InitDSPs();
int     DeInitDSPs();

int     ChannelOpen(UINT channelNum, STREAM_READ_CALLBACK streamReadCallback);
int     ChannelClose(UINT hChannelHandle);

int     GetTotalChannels();
int     GetTotalDSPs();
int     GetBoardCount();
int     GetBoardDetail(UINT boardNum, DS_BOARD_DETAIL* BoardDetail);
int     GetDspDetail(UINT dspNum, DSP_DETAIL* DspDetail);
int     GetEncodeChannelCount();
int     GetDecodeChannelCount();
int     GetDisplayChannelCount();
int     GetBoardInfo(int hChannelHandle, UINT* boardType, char* serialNo);
int     GetCapability(int hChannelHandle, CHANNEL_CAPABILITY* capability);

int     StopVideoPreview(int hChannelHandle);
int     StartVideoPreview(int hChannelHandle, PREVIEWCONFIG* pPreviewConf, UINT useSyncSem);
int     StartVideoPreviewEx(int hChannelHandle, PREVIEWCONFIG*  pPreviewConf, UINT useSyncSem,
                            UINT mode);

int     SetVideoPara(int hChannelHandle, int brightness, int contrast, int saturation, int hue);
int     GetVideoPara(int hChannelHandle, VideoStandard_t*  videoStandard, int*  brightness,
                     int*  contrast, int*  saturation, int*  hue);
void    GetSDKVersion(PVERSION_INFO versionInfo);
int     SetStreamType(int hChannelHandle, int type);
int     GetStreamType(int hChannelHandle, int* streamType);
int     SetSubStreamType(int hChannelHandle, int type);
int     GetSubStreamType(int hChannelHandle, int* streamType);

int     StartVideoCapture(int hChannelHandle);
int     StartSubVideoCapture(int hChannelHandle);
int     StopVideoCapture(int hChannelHandle);
int     StopSubVideoCapture(int hChannelHandle);

int     SetIBPMode(int hChannelHandle, int KeyFrameIntervals, int BFrames, int PFrames, int FrameRate);
int     SetDefaultQuant(int hChannelHandle, int IQuantVal, int PQuantVal, int BQuantVal);

int     SetEncoderPictureFormat(int hChannelHandle, PictureFormat_t pictureFormat);
int     SetSubEncoderPictureFormat(int hChannelHandle, PictureFormat_t pictureFormat);

int     SetupBitrateControl(int hChannelHandle, ULONG maxbps);
int     SetBitrateControlMode(int hChannelHandle, BitrateControlType_t brc);

int     SetVideoStandard(int hChannelHandle, VideoStandard_t videoStandard);
int     SetDefaultVideoStandard(VideoStandard_t VideoStandard);
int     SetVideoDetectPrecision(int hChannelHandle, unsigned int value);
int     GetVideoSignal(int hChannelHandle);
int     SetInputVideoPosition(int hChannelHandle, unsigned int x, unsigned int y);

int     SetOsdDisplayMode(int hChannelHandle, int brightness, int translucent, int twinkInterval,
                          unsigned short*  format1, unsigned short*  format2);
int     SetOsd(int hChannelHandle, int enable);
int     SetupDateTime(int hChannelHandle, SYSTEMTIME* now);     /* now is invalid for Ver4.0 */
int     SetOsdDisplayModeEx(int hChannelHandle, int color, int Translucent, int param,
                            int nLineCount, unsigned short **  Format);

int     LoadYUVFromBmpFile(char* FileName, unsigned char* yuv, int BufLen, int* Width, int* Height);
int     SetLogoDisplayMode(int hChannelHandle, unsigned short ColorKeyR, unsigned short ColorKeyG,
                           unsigned short ColorKeyB, unsigned short bTranslucent, int TwinkleInterval);
int     SetLogo(int hChannelHandle, int x, int y, int w, int h, unsigned char* yuv);
int     StopLogo(int hChannelHandle);
int     SetupMask(int hChannelHandle, RECT* rectList, int iAreas);
int     StopMask(int hChannelHandle);

int     AdjustMotionDetectPrecision(int hChannelHandle, int iGrade, int iFastMotionDetectFps,
                                    int iSlowMotionDetectFps);
int     SetupMotionDetection(int hChannelHandle, RECT* RectList, int iAreas);
int     StartMotionDetection(int hChannelHandle);
int     StopMotionDetection(int hChannelHandle);
int     MotionAnalyzer(int hChannelHandle, char* MotionData, int iThreshold, int* iResult);
int     SetupMotionDetectionEx(int hChannelHandle, int iGrade, int iFastMotionDetectFps,
                               int iSlowMotionDetectFps, UINT delay, RECT*  RectList, int iAreas,
                               MOTION_DETECTION_CALLBACK MotionDetectionCallback, int reserved);

int     SetAudioPreview(int hChannelHandle, int bEnable);
int     GetSoundLevel(int hChannelHandle);

int     RegisterImageStreamCallback(IMAGE_STREAM_CALLBACK pFunc, void* context);
int     SetImageStream(int hChannelHandle, int bStart, unsigned int fps, unsigned width,
                       unsigned height, unsigned char*  imageBuf);

int     GetOriginalImage(int hChannelHandle, unsigned char* imageBuf, int* size);
int     SaveYUVToBmpFile(char* FileName, unsigned char* yuv, int Width, int Height);
int     GetJpegImage(int hChannelHandle, unsigned char*  ImageBuf, unsigned long*  Size,
                     unsigned int nQuality);

int     SetupSubChannel(int hChannelHandle, int iSubChannel);
int     GetSubChannelStreamType(void* dataBuf, int frameType);

int     GetFramesStatistics(int hChannelHandle, PFRAMES_STATISTICS framesStatistics);
int     CaptureIFrame(int hChannelHandle);

int     SetDeInterlace(int hChannelHandle, UINT mode, UINT level);
int     ResetDSP(int hChannelHandle);

typedef void (*COVER_DETECTION_CALLBACK) (ULONG channelNumber, int bCoverDetected, void *context);
int SetCoverDetection(int hChannelHandle, int bEnable, int iGrade, int delay,
                      COVER_DETECTION_CALLBACK CoverDetectionCallback);
int SetupMosaic(int hChannelHandle, int iAreas, RECT* rectList, int reserved);
int SetWatchDog(unsigned int boardNumber, int bEnable);         /* just use in ds4016hcs */
typedef void (*ALARM_IN_CALLBACK)
    (
        UINT boardNumber,
        UINT alarmInFlag,
        UINT alarmInStatus,
        void *context
    );
int SetAlarmIn(unsigned int boardNumber, unsigned int alarmInEnableFlag, unsigned int alarmInParam,
               ALARM_IN_CALLBACK pCallbackFunc, int reserved);  /* just use in ds4016hcs */
int SetAlarmOut(unsigned int boardNumber, unsigned int alarmOutFlag, unsigned int alarmOutValue,
                int reserved);  /* just use in ds4016hcs */

/* modified at 2007.7.14 */
int SetFaceDetectionDemo(int hChannelHandle, int bEnable, UINT nFrameInterval,
                         FACE_DETECTION_DEMO_CALLBACK pFunc, int bCompress, UINT nCompressQuality,
                         int bLocateEyePos);

/* The api list for DS400xMD */
int HW_InitDecDevice(long* pDeviceTotal);
int HW_ReleaseDecDevice();
int HW_ChannelOpen(long ChannelNum, int* phChannel);
int HW_ChannelClose(int hChannel);

int HW_OpenStream(int hChannel, char* pFileHeadBuf, int nSize);
int HW_CloseStream(int hChannel);
int HW_InputData(int hChannel, char* pBuf, int nSize);

int HW_OpenFile(int hChannel, char* sFileName);
int HW_CloseFile(int hChannel);

int HW_Play(int hChannel);
int HW_Stop(int hChannel);
int HW_Pause(int hChannel, ULONG bPause);

int HW_PlaySound(int hChannel);
int HW_StopSound(int hChannel);
int HW_SetVolume(int hChannel, ULONG nVolume);

int HW_StartCapFile(int hChannel, char* sFileName);
int HW_StopCapFile(int hChannel);

int HW_GetPictureSize(int hChannel, ULONG* pWidth, ULONG* pHeight);
int HW_GetYV12Image(int hChannel, char* pBuffer, ULONG nSize);
int HW_ConvertToBmpFile(char*  pBuf, ULONG nSize, ULONG nWidth, ULONG nHeight, char*  sFileName,
                        ULONG nReserved);
int HW_GetSpeed(int hChannel, long* pSpeed);
int HW_SetSpeed(int hChannel, long nSpeed);
int HW_SetPlayPos(int hChannel, ULONG nPos);
int HW_GetPlayPos(int hChannel, ULONG* pPos);
int HW_SetJumpInterval(int hChannel, ULONG nSecond);
int HW_Jump(int hChannel, ULONG nDirection);

int HW_GetVersion(PHW_VERSION pVersion);
int HW_GetCurrentFrameRate(int hChannel, ULONG* pFrameRate);
int HW_GetCurrentFrameNum(int hChannel, ULONG* pFrameNum);
int HW_GetFileTotalFrames(int hChannel, ULONG* pTotalFrames);
int HW_GetFileTime(int hChannel, ULONG* pFileTime);
int HW_GetCurrentFrameTime(int hChannel, ULONG* pFrameTime);
int HW_GetPlayedFrames(int hChannel, ULONG* pDecVFrames);
int HW_SetFileEndMsg(int hChannel, sem_t* nMsg);
int HW_SetStreamOpenMode(int hChannel, ULONG nMode);
int HW_GetStreamOpenMode(int hChannel, ULONG* pMode);
int HW_SetAudioPreview(int hChannel, UINT bEnable);

int HW_StartDecVgaDisplay(int hChannel, PREVIEWCONFIG* pPreviewConf, UINT useSyncSem);
int HW_StopDecChanVgaDisplay(int hChannel);

int SetDisplayStandard(UINT nDisplayChan, VideoStandard_t VideoStandard);
int SetDisplayRegion(UINT nDisplayChannel, UINT nRegionCount, REGION_PARAM* pInit, UINT nReserved);
int ClearDisplayRegion(UINT nDisplayChannel, UINT flgRegion);
int SetDisplayRegionPosition(UINT nDisplayChan, UINT region, UINT x, UINT y);
int FillDisplayRegion(UINT nDisplayChan, UINT region, unsigned char* pImage);
int SetDecoderAudioOutput(UINT nDecodeChannel, UINT bEnable, UINT nOutputChannel);
int SetDecoderVideoOutput(UINT nDecodeChannel, UINT nPort, UINT bOpen, UINT nDisplayChannel,
                          UINT nDisplayRegion, UINT nReserved);
int SetDecoderVideoExtOutput(UINT nDecodeChannel, UINT nPort, UINT bOpen, UINT nDisplayChannel,
                             UINT nDisplayRegion, UINT nReserved);

int SetEncoderVideoExtOutput(UINT nEncodeChannel, UINT nPort, UINT bOpen, UINT nDisplayChannel,
                             UINT nDisplayRegion, UINT nReserved);

/* V4.1 add */
typedef void (*FILE_REF_DONE_CALLBACK) (UINT nChannel, UINT nSize);
int HW_SetFileRef(int hChannel, UINT bEnable, FILE_REF_DONE_CALLBACK FileRefDoneCallback);
int HW_GetFileAbsoluteTime(int hChannel, SYSTEMTIME* pStartTime, SYSTEMTIME* pEndTime);
int HW_GetCurrentAbsoluteTime(int hChannel, SYSTEMTIME* pTime);
int HW_LocateByAbsoluteTime(int hChannel, SYSTEMTIME time);
int HW_LocateByFrameNumber(int hChannel, UINT frmNum);

/* V4.1.5 add at 2006.1.19 */
int HW_ExportFileRef(int hChannel, char* pBuffer, UINT nSize);
int HW_ImportFileRef(int hChannel, char* pBuffer, UINT nSize);
int SetDisplayVideoCapture(UINT chan, UINT bStart, UINT fps, UINT width, UINT height,
                           unsigned char*  imageBuffer);
int RegisterDisplayVideoCaptureCallback(IMAGE_STREAM_CALLBACK pFunc, void* context);

/* add at 2006.3.21 */
int SetDisplayVideoBrightness(int hChannel, int Brightness);

/* add at 2006.3.29 */
int HW_ResetStream(int hChannel);

/* add at 2006.4.6 */
int HW_InputDataByFrame(int hChannel, char* pBuf, int nSize);

/* add at 2006.4.25 */
int SetChannelStreamCRC(int hChannelHandle, int bEnable);
int SetSubChannelStreamCRC(int hChannelHandle, int bEnable);

/* add at 2006.6.29 */
int HW_SetDecoderPostProcess(int hChannel, UINT param);

/* add at 2006.10.19 */
int RegisterDecoderVideoCaptureCallback(DECODER_VIDEO_CAPTURE_CALLBACK DecoderVideoCaptureCallback,
                                        void*  context);
int HW_SetDecoderVideoCapture(int hChannel, int bStart, UINT param);

/* add at 2007.07.14 */
int SetSystemParameters(unsigned int* param);

#ifdef __cplusplus
}
#endif
#endif
