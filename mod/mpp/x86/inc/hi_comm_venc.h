/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2017. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2016/11/15
 */
#ifndef __HI_COMM_VENC_H__
#define __HI_COMM_VENC_H__

//#include "hi_comm_rc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

// hi_comm_rc.h 


typedef HI_U32 HI_FR32;

/* rc mode */
typedef enum hiVENC_RC_MODE_E {
    VENC_RC_MODE_H264CBR = 1,
    VENC_RC_MODE_H264VBR,
    VENC_RC_MODE_H264AVBR, /* Not support for Hi3556AV100/Hi3556V200/Hi3559V200 */
    VENC_RC_MODE_H264QVBR,
    VENC_RC_MODE_H264CVBR,
    VENC_RC_MODE_H264FIXQP,
    VENC_RC_MODE_H264QPMAP, /* Not support for Hi3556AV100/Hi3556V200/Hi3559V200 */

    VENC_RC_MODE_MJPEGCBR,
    VENC_RC_MODE_MJPEGVBR,
    VENC_RC_MODE_MJPEGFIXQP,

    VENC_RC_MODE_H265CBR,
    VENC_RC_MODE_H265VBR,
    VENC_RC_MODE_H265AVBR, /* Not support for Hi3556AV100/Hi3556V200/Hi3559V200 */
    VENC_RC_MODE_H265QVBR,
    VENC_RC_MODE_H265CVBR,
    VENC_RC_MODE_H265FIXQP,
    VENC_RC_MODE_H265QPMAP, /* Not support for Hi3556AV100/Hi3556V200/Hi3559V200 */

    VENC_RC_MODE_BUTT,
} VENC_RC_MODE_E;

/* qpmap mode */
typedef enum hiVENC_RC_QPMAP_MODE_E {
    VENC_RC_QPMAP_MODE_MEANQP = 0,
    VENC_RC_QPMAP_MODE_MINQP,
    VENC_RC_QPMAP_MODE_MAXQP,

    VENC_RC_QPMAP_MODE_BUTT,
} VENC_RC_QPMAP_MODE_E;

/* the attribute of h264e fixqp */
typedef struct hiVENC_H264_FIXQP_S {
    HI_U32      u32Gop;                    /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32SrcFrameRate;           /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;         /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32IQp;                    /* RW; Range:[0, 51]; qp of the i frame */
    HI_U32      u32PQp;                    /* RW; Range:[0, 51]; qp of the p frame */
    HI_U32      u32BQp;                    /* RW; Range:[0, 51]; qp of the b frame */
} VENC_H264_FIXQP_S;

/* the attribute of h264e cbr */
typedef struct hiVENC_H264_CBR_S {
    HI_U32      u32Gop;                    /* RW; Range:[1, 65536]; the interval of I Frame. */
    HI_U32      u32StatTime;               /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;           /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;         /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32BitRate;                /* RW; Range:[2, 614400]; average bitrate */
} VENC_H264_CBR_S;

/* the attribute of h264e vbr */
typedef struct hiVENC_H264_VBR_S {
    HI_U32      u32Gop;                     /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32StatTime;                /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;            /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;          /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32MaxBitRate;              /* RW; Range:[2, 614400];the max bitrate */
} VENC_H264_VBR_S;

/* the attribute of h264e cvbr */
typedef struct hiVENC_H264_CVBR_S {
    HI_U32      u32Gop;                     /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32StatTime;                /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;            /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;          /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32MaxBitRate;              /* RW; Range:[2, 614400];the max bitrate */

    HI_U32      u32ShortTermStatTime;       /* RW; Range:[1, 120]; the long-term rate statistic time, the unit is second (s)*/
    HI_U32      u32LongTermStatTime;        /* RW; Range:[1, 1440]; the long-term rate statistic time, the unit is u32LongTermStatTimeUnit*/
    HI_U32      u32LongTermMaxBitrate ;     /* RW; Range:[2, 614400];the long-term target max bitrate, can not be larger than u32MaxBitRate,the unit is kbps */
    HI_U32      u32LongTermMinBitrate ;     /* RW; Range:[0, 614400];the long-term target min bitrate,  can not be larger than u32LongTermMaxBitrate,the unit is kbps */
} VENC_H264_CVBR_S;

/* the attribute of h264e avbr */
typedef struct hiVENC_H264_AVBR_S {
    HI_U32      u32Gop;                  /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32StatTime;             /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;         /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;       /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32MaxBitRate;           /* RW; Range:[2, 614400];the max bitrate */
} VENC_H264_AVBR_S;

/* the attribute of h264e qpmap */
typedef struct hiVENC_H264_QPMAP_S {
    HI_U32      u32Gop;                   /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32StatTime;              /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;        /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
} VENC_H264_QPMAP_S;

typedef struct hiVENC_H264_QVBR_S {
    HI_U32      u32Gop;                   /* RW; Range:[1, 65536];the interval of ISLICE. */
    HI_U32      u32StatTime;              /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;        /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32TargetBitRate;         /* RW; Range:[2, 614400]; the target bitrate */
}VENC_H264_QVBR_S;

/* the attribute of h265e qpmap */
typedef struct hiVENC_H265_QPMAP_S {
    HI_U32      u32Gop;                   /* RW; Range:[1, 65536]; the interval of ISLICE. */
    HI_U32      u32StatTime;              /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;        /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    VENC_RC_QPMAP_MODE_E enQpMapMode;     /* RW;  the QpMap Mode.*/
} VENC_H265_QPMAP_S;

typedef struct hiVENC_H264_CBR_S   VENC_H265_CBR_S;
typedef struct hiVENC_H264_VBR_S   VENC_H265_VBR_S;
typedef struct hiVENC_H264_AVBR_S  VENC_H265_AVBR_S;
typedef struct hiVENC_H264_FIXQP_S VENC_H265_FIXQP_S;
typedef struct hiVENC_H264_QVBR_S  VENC_H265_QVBR_S;
typedef struct hiVENC_H264_CVBR_S  VENC_H265_CVBR_S;

/* the attribute of mjpege fixqp */
typedef struct hiVENC_MJPEG_FIXQP_S {
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate;         /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32Qfactor;               /* RW; Range:[1,99];image quality. */
} VENC_MJPEG_FIXQP_S;

/* the attribute of mjpege cbr */
typedef struct hiVENC_MJPEG_CBR_S {
    HI_U32      u32StatTime;              /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate ;        /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32BitRate;               /* RW; Range:[2, 614400]; average bitrate */
} VENC_MJPEG_CBR_S;

/* the attribute of mjpege vbr */
typedef struct hiVENC_MJPEG_VBR_S {
    HI_U32      u32StatTime;              /* RW; Range:[1, 60]; the rate statistic time, the unit is senconds(s) */
    HI_U32      u32SrcFrameRate;          /* RW; Range:[1, 240]; the input frame rate of the venc chnnel */
    HI_FR32     fr32DstFrameRate;         /* RW; Range:[0.015625, 240]; the target frame rate of the venc chnnel,can not be larger than u32SrcFrameRate */
    HI_U32      u32MaxBitRate;            /* RW; Range:[2, 614400];the max bitrate */
}VENC_MJPEG_VBR_S;

/* the attribute of rc */
typedef struct hiVENC_RC_ATTR_S {
    VENC_RC_MODE_E enRcMode;              /* RW; the type of rc*/
    union {
        VENC_H264_CBR_S    stH264Cbr;
        VENC_H264_VBR_S    stH264Vbr;
        VENC_H264_AVBR_S   stH264AVbr;
        VENC_H264_QVBR_S   stH264QVbr;
        VENC_H264_CVBR_S   stH264CVbr;
        VENC_H264_FIXQP_S  stH264FixQp;
        VENC_H264_QPMAP_S  stH264QpMap;

        VENC_MJPEG_CBR_S   stMjpegCbr;
        VENC_MJPEG_VBR_S   stMjpegVbr;
        VENC_MJPEG_FIXQP_S stMjpegFixQp;

        VENC_H265_CBR_S    stH265Cbr;
        VENC_H265_VBR_S    stH265Vbr;
        VENC_H265_AVBR_S   stH265AVbr;
        VENC_H265_QVBR_S   stH265QVbr;
        VENC_H265_CVBR_S   stH265CVbr;
        VENC_H265_FIXQP_S  stH265FixQp;
        VENC_H265_QPMAP_S  stH265QpMap;
    };
} VENC_RC_ATTR_S;
// end hi_comm_rc.h



// /* the nalu type of H264E */
typedef enum hiH264E_NALU_TYPE_E {
    H264E_NALU_BSLICE = 0,                         /*B SLICE types*/
    H264E_NALU_PSLICE = 1,                         /*P SLICE types*/
    H264E_NALU_ISLICE = 2,                         /*I SLICE types*/
    H264E_NALU_IDRSLICE = 5,                       /*IDR SLICE types*/
    H264E_NALU_SEI    = 6,                         /*SEI types*/
    H264E_NALU_SPS    = 7,                         /*SPS types*/
    H264E_NALU_PPS    = 8,                         /*PPS types*/
    H264E_NALU_BUTT
} H264E_NALU_TYPE_E;

// /* the nalu type of H265E */
typedef enum hiH265E_NALU_TYPE_E {
    H265E_NALU_BSLICE = 0,                          /*B SLICE types*/
    H265E_NALU_PSLICE = 1,                          /*P SLICE types*/
    H265E_NALU_ISLICE = 2,                          /*I SLICE types*/
    H265E_NALU_IDRSLICE = 19,                       /*IDR SLICE types*/
    H265E_NALU_VPS    = 32,                         /*VPS types*/
    H265E_NALU_SPS    = 33,                         /*SPS types*/
    H265E_NALU_PPS    = 34,                         /*PPS types*/
    H265E_NALU_SEI    = 39,                         /*SEI types*/

    H265E_NALU_BUTT
} H265E_NALU_TYPE_E;

typedef enum hiJPEGE_PACK_TYPE_E {
    JPEGE_PACK_ECS = 5,                            /*ECS types*/
    JPEGE_PACK_APP = 6,                            /*APP types*/
    JPEGE_PACK_VDO = 7,                            /*VDO types*/
    JPEGE_PACK_PIC = 8,                            /*PIC types*/
    JPEGE_PACK_DCF = 9,                            /*DCF types*/
    JPEGE_PACK_DCF_PIC = 10,                       /*DCF PIC types*/
    JPEGE_PACK_BUTT
} JPEGE_PACK_TYPE_E;

typedef enum hiPRORES_PACK_TYPE_E {
    PRORES_PACK_PIC = 1,                            /*PIC types*/
    PRORES_PACK_BUTT
} PRORES_PACK_TYPE_E;

/* the data type of VENC */
typedef union hiVENC_DATA_TYPE_U {
    H264E_NALU_TYPE_E    enH264EType;               /* R; H264E NALU types*/
    JPEGE_PACK_TYPE_E    enJPEGEType;               /* R; JPEGE pack types*/
    H265E_NALU_TYPE_E    enH265EType;               /* R; H264E NALU types*/
    PRORES_PACK_TYPE_E   enPRORESType;
} VENC_DATA_TYPE_U;

// /* the pack info of VENC */
typedef struct hiVENC_PACK_INFO_S {
    VENC_DATA_TYPE_U  u32PackType;                    /* R; the pack type*/
    HI_U32 u32PackOffset;
    HI_U32 u32PackLength;
} VENC_PACK_INFO_S;

// /* Defines a stream packet */
typedef struct hiVENC_PACK_S {
    HI_U64               u64PhyAddr;                 /* R; the physics address of stream */
    HI_U8                ATTRIBUTE* pu8Addr;         /* R; the virtual address of stream */
    HI_U32               ATTRIBUTE u32Len;           /* R; the length of stream */

    HI_U64               u64PTS;                     /* R; PTS */
    HI_BOOL              bFrameEnd;                  /* R; frame end */

    VENC_DATA_TYPE_U     DataType;                   /* R; the type of stream */
    HI_U32               u32Offset;                  /* R; the offset between the Valid data and the start address */
    HI_U32               u32DataNum;                 /* R; the  stream packets num */
    VENC_PACK_INFO_S     stPackInfo[8];              /* R; the stream packet Information */
} VENC_PACK_S;

/* Defines the features of an H.264 stream */
typedef struct hiVENC_STREAM_INFO_H264_S {
    HI_U32                 u32PicBytesNum;              /* R; the coded picture stream byte number */
    HI_U32                 u32Inter16x16MbNum;          /* R; the inter16x16 macroblock num */
    HI_U32                 u32Inter8x8MbNum;            /* R; the inter8x8 macroblock num */
    HI_U32                 u32Intra16MbNum;             /* R; the intra16x16 macroblock num */
    HI_U32                 u32Intra8MbNum;              /* R; the intra8x8 macroblock num */
    HI_U32                 u32Intra4MbNum;              /* R; the inter4x4 macroblock num */

    //H264E_REF_TYPE_E       enRefType;                   /* R; Type of encoded frames in advanced frame skipping reference mode*/
    HI_U32                 u32UpdateAttrCnt;            /* R; Number of times that channel attributes or parameters (including RC parameters) are set*/
    HI_U32                 u32StartQp;                  /* R; the start Qp of encoded frames*/
    HI_U32                 u32MeanQp;                   /* R; the mean Qp of encoded frames*/
    HI_BOOL                bPSkip;
} VENC_STREAM_INFO_H264_S;

/* Defines the features of an H.265 stream */
typedef struct hiVENC_STREAM_INFO_H265_S {
    HI_U32                 u32PicBytesNum;              /* R; the coded picture stream byte number */
    HI_U32                 u32Inter64x64CuNum;          /* R; the inter64x64 cu num  */
    HI_U32                 u32Inter32x32CuNum;          /* R; the inter32x32 cu num  */
    HI_U32                 u32Inter16x16CuNum;          /* R; the inter16x16 cu num  */
    HI_U32                 u32Inter8x8CuNum;            /* R; the inter8x8   cu num  */
    HI_U32                 u32Intra32x32CuNum;          /* R; the Intra32x32 cu num  */
    HI_U32                 u32Intra16x16CuNum;          /* R; the Intra16x16 cu num  */
    HI_U32                 u32Intra8x8CuNum;            /* R; the Intra8x8   cu num  */
    HI_U32                 u32Intra4x4CuNum;            /* R; the Intra4x4   cu num  */

    //H265E_REF_TYPE_E       enRefType;                   /* R; Type of encoded frames in advanced frame skipping reference mode*/
    HI_U32                 u32UpdateAttrCnt;            /* R; Number of times that channel attributes or parameters (including RC parameters) are set*/
    HI_U32                 u32StartQp;                  /* R; the start Qp of encoded frames*/
    HI_U32                 u32MeanQp;                   /* R; the mean Qp of encoded frames*/
    HI_BOOL                bPSkip;
} VENC_STREAM_INFO_H265_S;

/* Defines the features of an jpege stream */
typedef struct hiVENC_STREAM_INFO_PRORES_S {
    HI_U32 u32PicBytesNum;
    HI_U32 u32UpdateAttrCnt;
} VENC_STREAM_INFO_PRORES_S;

/* Defines the features of an jpege stream */
typedef struct hiVENC_STREAM_INFO_JPEG_S {
    HI_U32 u32PicBytesNum;                      /* R; the coded picture stream byte number */
    HI_U32 u32UpdateAttrCnt;                    /* R; Number of times that channel attributes or parameters (including RC parameters) are set*/
    HI_U32 u32Qfactor;                          /* R; image quality */
} VENC_STREAM_INFO_JPEG_S;

/* Defines the features of an stream */
typedef struct hiVENC_STREAM_S {
    VENC_PACK_S ATTRIBUTE* pstPack;            /* R; stream pack attribute*/
    HI_U32      ATTRIBUTE u32PackCount;        /* R; the pack number of one frame stream*/
    HI_U32      u32Seq;                        /* R; the list number of stream*/

    union {
        VENC_STREAM_INFO_H264_S   stH264Info;                        /* R; the stream info of h264*/
        VENC_STREAM_INFO_JPEG_S   stJpegInfo;                        /* R; the stream info of jpeg*/
        VENC_STREAM_INFO_H265_S   stH265Info;                        /* R; the stream info of h265*/
        VENC_STREAM_INFO_PRORES_S stProresInfo;                      /* R; the stream info of prores*/
    };
} VENC_STREAM_S;

/* the gop mode */
typedef enum hiVENC_GOP_MODE_E {
    VENC_GOPMODE_NORMALP    = 0,     /* NORMALP */
    VENC_GOPMODE_DUALP      = 1,     /* DUALP;  Not support for Hi3556AV100*/
    VENC_GOPMODE_SMARTP     = 2,     /* SMARTP; Not support for Hi3556AV100 */
    VENC_GOPMODE_ADVSMARTP  = 3,     /* ADVSMARTP ; Only used for Hi3559AV100*/
    VENC_GOPMODE_BIPREDB    = 4,     /* BIPREDB ;Only used for Hi3559AV100/Hi3519AV100*/
    VENC_GOPMODE_LOWDELAYB  = 5,     /* LOWDELAYB; Not support */

    VENC_GOPMODE_BUTT,
} VENC_GOP_MODE_E;

/* the information of the stream */
typedef struct hiVENC_STREAM_BUF_INFO_S {
    HI_U64   u64PhyAddr[MAX_TILE_NUM];             /* R; Start physical address for a stream buffer */
    HI_VOID ATTRIBUTE* pUserAddr[MAX_TILE_NUM];    /* R; Start virtual address for a stream buffer */
    HI_U64  ATTRIBUTE u64BufSize[MAX_TILE_NUM];    /* R; Stream buffer size */
} VENC_STREAM_BUF_INFO_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_VENC_H__ */
