/*
 * HIKVISION DS-40xxHC/HF BOARD SYSTEM SDK
 * Copyright (c) 2002-2008 Hangzhou Hikvision digtial technology co,. ltd
 */
#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#define MAX_PORT    64

typedef unsigned int    UINT;
typedef unsigned long   ULONG;
typedef unsigned short  USHORT;
typedef unsigned char   UCHAR;
typedef int             HANDLE;
typedef unsigned long   DWORD;
typedef unsigned long*  PDWORD;

typedef struct _rect
{
    int left;
    int top;
    int right;
    int bottom;
} rect;

typedef struct _vwin
{
    int b, c;
    int x, y;
    int w, h;
    int r[4];
} video_win;

typedef struct
{
    int Brightness;
    int Contrast;
    int Saturation;
    int Hue;
} video_info;

typedef struct
{
    int IQuantVal;
    int PQuantVal;
    int BQuantVal;
    int KeyFrameIntervals;
    int BFrames;
    int PFrames;
    int FrameRate;
} Frame_info;

typedef struct
{
    unsigned short  Translucent;
    int             Brightness;
    int             PosX;
    int             PosY;
} Osd_info;

typedef struct
{
    unsigned short  Translucent;
    int             PosX;
    int             PosY;
} Logo_info;

typedef struct
{
    unsigned long   FilePos;    /* the locate for I frame in file */
    unsigned long   Reserve;
    unsigned long   FrameNum;   /* the video frame num, include I frame and BBP frame,exclude the audio frame */
} INDEX_NODE, *PINDEX_NODE;

typedef enum
{
    StandardNone    = 0x80000000,
    StandardNTSC    = 0x00000001,
    StandardPAL     = 0x00000002,
    StandardSECAM   = 0x00000004,
} VideoStandard_t;

typedef enum
{
    PktError            = 0,
    PktIFrames          = 0x0001,
    PktPFrames          = 0x0002,
    PktBBPFrames        = 0x0004,
    PktAudioFrames      = 0x0008,
    PktMotionDetection  = 0x00010,
    PktDspStatus        = 0x00020,
    PktOrigImage        = 0x00040,
    PktSysHeader        = 0x00080,
    PktBPFrames         = 0x00100,
    PktSFrames          = 0x00200,
    PktSubIFrames       = 0x00400,
    PktSubPFrames       = 0x00800,
    PktSubBBPFrames     = 0x01000,
    PktSubSysHeader     = 0x02000
} FrameType_t;

typedef struct tagVersion
{
    ULONG   DspVersion, DspBuildNum;
    ULONG   DriverVersion, DriverBuildNum;
    ULONG   SDKVersion, SDKBuildNum;
} VERSION_INFO, *PVERSION_INFO;

typedef enum
{
    ENC_CIF_FORMAT      = 0,
    ENC_QCIF_FORMAT     = 1,
    ENC_2CIF_FORMAT     = 2,
    ENC_4CIF_FORMAT     = 3,
    ENC_QQCIF_FORMAT    = 4,
    ENC_CIFQCIF_FORMAT  = 5,
    ENC_CIFQQCIF_FORMAT = 6,
    ENC_DCIF_FORMAT     = 7
} PictureFormat_t;

typedef struct tagMotionData
{
    PictureFormat_t PicFormat;
    ULONG           HorizeBlocks;
    ULONG           VerticalBlocks;
    ULONG           BlockSize;
} MOTION_DATA_HEADER, *PMOTION_DATA_HEADER;

typedef struct tagRect
{
    short   RectTop;
    short   RectBottom;
    short   RectLeft;
    short   RectRight;
} RECT;

#define _OSD_BASE   0x9000
#define _OSD_YEAR4  _OSD_BASE + 0
#define _OSD_YEAR2  _OSD_BASE + 1
#define _OSD_MONTH3 _OSD_BASE + 2
#define _OSD_MONTH2 _OSD_BASE + 3
#define _OSD_DAY    _OSD_BASE + 4
#define _OSD_WEEK3  _OSD_BASE + 5
#define _OSD_CWEEK1 _OSD_BASE + 6
#define _OSD_HOUR24 _OSD_BASE + 7
#define _OSD_HOUR12 _OSD_BASE + 8
#define _OSD_MINUTE _OSD_BASE + 9
#define _OSD_SECOND _OSD_BASE + 10
#endif
