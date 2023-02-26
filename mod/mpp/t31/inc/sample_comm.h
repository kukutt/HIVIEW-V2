

#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

//#include <pthread.h>

#define VENC_MAX_CHN_NUM               16
#define MAX_TILE_NUM                   1
#define VI_MAX_PHY_PIPE_NUM            2
#define VI_MAX_VIR_PIPE_NUM            0
#define VI_MAX_PIPE_NUM                (VI_MAX_PHY_PIPE_NUM + VI_MAX_VIR_PIPE_NUM)
#define VPSS_MAX_PHY_CHN_NUM           3

typedef unsigned char           HI_U8;
typedef unsigned short          HI_U16;
typedef unsigned int            HI_U32;
typedef char                    HI_CHAR;
typedef short                   HI_S16;
typedef int                     HI_S32;
typedef double                  HI_DOUBLE;
typedef unsigned long long  HI_U64;
typedef long long           HI_S64;
typedef enum {
    HI_FALSE = 0,
    HI_TRUE  = 1,
} HI_BOOL;
#define HI_VOID                 void
typedef unsigned int            hi_u32;



//#include "hi_common.h"

#define ATTRIBUTE __attribute__((aligned (4)))
typedef HI_S32 AI_CHN;
typedef HI_S32 AO_CHN;
typedef HI_S32 AENC_CHN;
typedef HI_S32 ADEC_CHN;
typedef HI_S32 AUDIO_DEV;
typedef HI_S32 VI_DEV;
typedef HI_S32 VI_PIPE;
typedef HI_S32 VI_CHN;
typedef HI_S32 VI_STITCH_GRP;
typedef HI_S32 VO_DEV;
typedef HI_S32 VO_LAYER;
typedef HI_S32 VO_CHN;
typedef HI_S32 VO_WBC;
typedef HI_S32 GRAPHIC_LAYER;
typedef HI_S32 VENC_CHN;
typedef HI_S32 VDEC_CHN;
typedef HI_S32 IVE_HANDLE;
typedef HI_S32 CLS_HANDLE;
typedef HI_S32 FD_CHN;
typedef HI_S32 MD_CHN;
typedef HI_S32 ISP_DEV;
typedef HI_S32 BLK_DEV;
typedef HI_S32 SENSOR_ID;
typedef HI_S32 MIPI_DEV;
typedef HI_S32 SLAVE_DEV;
typedef HI_S32 SVP_NNIE_HANDLE;
typedef HI_S32 SVP_DSP_HANDLE;
typedef HI_S32 SVP_ALG_CHN;
typedef HI_S32 VPSS_GRP;
typedef HI_S32 VPSS_GRP_PIPE;
typedef HI_S32 VPSS_CHN;
typedef HI_S32 AVS_GRP;
typedef HI_S32 AVS_PIPE;
typedef HI_S32 AVS_CHN;
typedef HI_S32 MCF_GRP;
typedef HI_S32 MCF_PIPE;
typedef HI_S32 MCF_CHN;

typedef enum hiMOD_ID_E
{
    HI_ID_CMPI    = 0,
    HI_ID_VB      = 1,
    HI_ID_SYS     = 2,
    HI_ID_RGN      = 3,
    HI_ID_CHNL    = 4,
    HI_ID_VDEC    = 5,
    HI_ID_AVS     = 6,
    HI_ID_VPSS    = 7,
    HI_ID_VENC    = 8,
    HI_ID_SVP     = 9,
    HI_ID_H264E   = 10,
    HI_ID_JPEGE   = 11,
    HI_ID_MPEG4E  = 12,
    HI_ID_H265E   = 13,
    HI_ID_JPEGD   = 14,
    HI_ID_VO      = 15,
    HI_ID_VI      = 16,
    HI_ID_DIS     = 17,
    HI_ID_VALG    = 18,
    HI_ID_RC      = 19,
    HI_ID_AIO     = 20,
    HI_ID_AI      = 21,
    HI_ID_AO      = 22,
    HI_ID_AENC    = 23,
    HI_ID_ADEC    = 24,
    HI_ID_VPU    = 25,
    HI_ID_PCIV    = 26,
    HI_ID_PCIVFMW = 27,
    HI_ID_ISP      = 28,
    HI_ID_IVE      = 29,
    HI_ID_USER    = 30,
    HI_ID_DCCM    = 31,
    HI_ID_DCCS    = 32,
    HI_ID_PROC    = 33,
    HI_ID_LOG     = 34,
    HI_ID_VFMW    = 35,
    HI_ID_H264D   = 36,
    HI_ID_GDC     = 37,
    HI_ID_PHOTO   = 38,
    HI_ID_FB      = 39,
    HI_ID_HDMI    = 40,
    HI_ID_VOIE    = 41,
    HI_ID_TDE     = 42,
    HI_ID_HDR      = 43,
    HI_ID_PRORES  = 44,
    HI_ID_VGS     = 45,

    HI_ID_FD      = 47,
    HI_ID_ODT      = 48, //Object detection trace
    HI_ID_VQA      = 49, //Video quality  analysis
    HI_ID_LPR      = 50, //Object detection trace
    HI_ID_SVP_NNIE     = 51,
    HI_ID_SVP_DSP      = 52,
    HI_ID_DPU_RECT     = 53,
    HI_ID_DPU_MATCH    = 54,

    HI_ID_MOTIONSENSOR = 55,
    HI_ID_MOTIONFUSION = 56,

    HI_ID_GYRODIS      = 57,
    HI_ID_PM           = 58,
    HI_ID_SVP_ALG      = 59,
	HI_ID_IVP          = 60,
	HI_ID_MCF          = 61,

    HI_ID_BUTT,
} MOD_ID_E;

typedef struct hiMPP_CHN_S
{
    MOD_ID_E    enModId;
    HI_S32      s32DevId;
    HI_S32      s32ChnId;
} MPP_CHN_S;

typedef enum
{
    PT_PCMU          = 0,
    PT_1016          = 1,
    PT_G721          = 2,
    PT_GSM           = 3,
    PT_G723          = 4,
    PT_DVI4_8K       = 5,
    PT_DVI4_16K      = 6,
    PT_LPC           = 7,
    PT_PCMA          = 8,
    PT_G722          = 9,
    PT_S16BE_STEREO  = 10,
    PT_S16BE_MONO    = 11,
    PT_QCELP         = 12,
    PT_CN            = 13,
    PT_MPEGAUDIO     = 14,
    PT_G728          = 15,
    PT_DVI4_3        = 16,
    PT_DVI4_4        = 17,
    PT_G729          = 18,
    PT_G711A         = 19,
    PT_G711U         = 20,
    PT_G726          = 21,
    PT_G729A         = 22,
    PT_LPCM          = 23,
    PT_CelB          = 25,
    PT_JPEG          = 26,
    PT_CUSM          = 27,
    PT_NV            = 28,
    PT_PICW          = 29,
    PT_CPV           = 30,
    PT_H261          = 31,
    PT_MPEGVIDEO     = 32,
    PT_MPEG2TS       = 33,
    PT_H263          = 34,
    PT_SPEG          = 35,
    PT_MPEG2VIDEO    = 36,
    PT_AAC           = 37,
    PT_WMA9STD       = 38,
    PT_HEAAC         = 39,
    PT_PCM_VOICE     = 40,
    PT_PCM_AUDIO     = 41,
    PT_MP3           = 43,
    PT_ADPCMA        = 49,
    PT_AEC           = 50,
    PT_X_LD          = 95,
    PT_H264          = 96,
    PT_D_GSM_HR      = 200,
    PT_D_GSM_EFR     = 201,
    PT_D_L8          = 202,
    PT_D_RED         = 203,
    PT_D_VDVI        = 204,
    PT_D_BT656       = 220,
    PT_D_H263_1998   = 221,
    PT_D_MP1S        = 222,
    PT_D_MP2P        = 223,
    PT_D_BMPEG       = 224,
    PT_MP4VIDEO      = 230,
    PT_MP4AUDIO      = 237,
    PT_VC1           = 238,
    PT_JVC_ASF       = 255,
    PT_D_AVI         = 256,
    PT_DIVX3         = 257,
    PT_AVS             = 258,
    PT_REAL8         = 259,
    PT_REAL9         = 260,
    PT_VP6             = 261,
    PT_VP6F             = 262,
    PT_VP6A             = 263,
    PT_SORENSON          = 264,
    PT_H265          = 265,
    PT_VP8             = 266,
    PT_MVC             = 267,
    PT_PNG           = 268,
    /* add by hisilicon */
    PT_AMR           = 1001,
    PT_MJPEG         = 1002,
    PT_AMRWB         = 1003,
    PT_PRORES        = 1006,
    PT_OPUS          = 1007,
    PT_BUTT
} PAYLOAD_TYPE_E;



typedef struct hiPOINT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
} POINT_S;

typedef struct hiSIZE_S
{
    HI_U32 u32Width;
    HI_U32 u32Height;
} SIZE_S;

typedef struct hiRECT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
    HI_U32 u32Width;
    HI_U32 u32Height;
} RECT_S;

// /* we ONLY define picture format used, all unused will be deleted!*/
typedef enum hiPIXEL_FORMAT_E
{
    PIXEL_FORMAT_RGB_444 = 0,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_888,

    PIXEL_FORMAT_BGR_444,
    PIXEL_FORMAT_BGR_555,
    PIXEL_FORMAT_BGR_565,
    PIXEL_FORMAT_BGR_888,

    PIXEL_FORMAT_ARGB_1555,
    PIXEL_FORMAT_ARGB_4444,
    PIXEL_FORMAT_ARGB_8565,
    PIXEL_FORMAT_ARGB_8888,
    PIXEL_FORMAT_ARGB_2BPP,

    PIXEL_FORMAT_ABGR_1555,
    PIXEL_FORMAT_ABGR_4444,
    PIXEL_FORMAT_ABGR_8565,
    PIXEL_FORMAT_ABGR_8888,

    PIXEL_FORMAT_RGB_BAYER_8BPP,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    PIXEL_FORMAT_RGB_BAYER_14BPP,
    PIXEL_FORMAT_RGB_BAYER_16BPP,


    PIXEL_FORMAT_YVU_PLANAR_422,
    PIXEL_FORMAT_YVU_PLANAR_420,
    PIXEL_FORMAT_YVU_PLANAR_444,

    PIXEL_FORMAT_YVU_SEMIPLANAR_422,
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    PIXEL_FORMAT_YVU_SEMIPLANAR_444,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_YVYU_PACKAGE_422,
    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,
    PIXEL_FORMAT_YYUV_PACKAGE_422,
    PIXEL_FORMAT_YYVU_PACKAGE_422,
    PIXEL_FORMAT_UVYY_PACKAGE_422,
    PIXEL_FORMAT_VUYY_PACKAGE_422,
    PIXEL_FORMAT_VY1UY0_PACKAGE_422,

    PIXEL_FORMAT_YUV_400,
    PIXEL_FORMAT_UV_420,


    /* SVP data format */
    PIXEL_FORMAT_BGR_888_PLANAR,
    PIXEL_FORMAT_HSV_888_PACKAGE,
    PIXEL_FORMAT_HSV_888_PLANAR,
    PIXEL_FORMAT_LAB_888_PACKAGE,
    PIXEL_FORMAT_LAB_888_PLANAR,
    PIXEL_FORMAT_S8C1,
    PIXEL_FORMAT_S8C2_PACKAGE,
    PIXEL_FORMAT_S8C2_PLANAR,
    PIXEL_FORMAT_S8C3_PLANAR,
    PIXEL_FORMAT_S16C1,
    PIXEL_FORMAT_U8C1,
    PIXEL_FORMAT_U16C1,
    PIXEL_FORMAT_S32C1,
    PIXEL_FORMAT_U32C1,
    PIXEL_FORMAT_U64C1,
    PIXEL_FORMAT_S64C1,

    PIXEL_FORMAT_BUTT
} PIXEL_FORMAT_E;

typedef struct hiBITMAP_S
{
    PIXEL_FORMAT_E enPixelFormat;  /* Bitmap's pixel format */
    HI_U32 u32Width;               /* Bitmap's width */
    HI_U32 u32Height;              /* Bitmap's height */
    HI_VOID* ATTRIBUTE pData;      /* Address of Bitmap's data */
} BITMAP_S;


#include "hi_comm_venc.h"
#include "hi_comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Information of stitch group */
typedef struct hiVI_STITCH_GRP_ATTR_S {
    HI_BOOL                     bStitch;
    HI_U32                      u32MaxPTSGap;            /* RW;MAX PTS Gap between frame of pipe,unit:us */
    HI_U32                      u32PipeNum;              /* RW;Range [2, VI_MAX_PIPE_NUM] */
    VI_PIPE                     PipeId[VI_MAX_PIPE_NUM]; /* RW;Array of pipe ID */
} VI_STITCH_GRP_ATTR_S;


typedef struct hiAUDIO_STREAM_S {
    HI_U8 ATTRIBUTE *pStream;         /* the virtual address of stream */
    HI_U64 ATTRIBUTE u64PhyAddr;      /* the physics address of stream */
    HI_U32 u32Len;          /* stream lenth, by bytes */
    HI_U64 u64TimeStamp;    /* frame time stamp */
    HI_U32 u32Seq;          /* frame seq, if stream is not a valid frame,u32Seq is 0 */
} AUDIO_STREAM_S;

#define ALIGN_UP(x, a)           ( ( ((x) + ((a) - 1) ) / a ) * a )


#define SAMPLE_VO_DEV_DHD0 0                  /* VO's device HD0 */
#define SAMPLE_VO_DEV_UHD  SAMPLE_VO_DEV_DHD0 /* VO's ultra HD device:HD0 */
#define SAMPLE_VO_LAYER_VHD0 0

#define SAMPLE_AUDIO_EXTERN_AI_DEV 0
#define SAMPLE_AUDIO_EXTERN_AO_DEV 0
#define SAMPLE_AUDIO_INNER_AI_DEV 0
#define SAMPLE_AUDIO_INNER_AO_DEV 0
#define SAMPLE_AUDIO_INNER_HDMI_AO_DEV 1

#define SAMPLE_AUDIO_PTNUMPERFRM   480

#define WDR_MAX_PIPE_NUM        2



/*******************************************************
    enum define
*******************************************************/

typedef enum hiPIC_SIZE_E
{
    PIC_CIF,
    PIC_360P,      /* 640 * 360 */
    PIC_512P,      /* 640 * 512 */    
    PIC_VGA,
    PIC_640x360,
    PIC_D1_PAL,    /* 720 * 576 */
    PIC_D1_NTSC,   /* 720 * 480 */
    PIC_720P,      /* 1280 * 720  */
    PIC_960P,      /* 1280 * 960  */
    PIC_1080P,     /* 1920 * 1080 */
    PIC_2304x1296,
    PIC_2592x1520,
    PIC_2592x1536,
    PIC_2592x1944,
    PIC_2688x1520,
    PIC_2688x1536,
    PIC_2688x1944,
    PIC_2716x1524,
    PIC_3840x2160,
    PIC_4096x2160,
    PIC_3000x3000,
    PIC_4000x3000,
    PIC_7680x4320,
    PIC_3840x8640,
    PIC_BUTT
} PIC_SIZE_E;

typedef enum hiSAMPLE_SNS_TYPE_E
{
    SONY_IMX327_MIPI_2M_30FPS_12BIT,
    SONY_IMX327_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT,
    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX307_MIPI_2M_30FPS_12BIT,
    SONY_IMX307_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT,
    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX335_MIPI_5M_30FPS_12BIT,
    SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1,
    SONY_IMX335_MIPI_4M_30FPS_12BIT,
    SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1,
    SMART_SC4236_MIPI_3M_30FPS_10BIT,
    SMART_SC4236_MIPI_3M_20FPS_10BIT,
    SMART_SC2231_MIPI_2M_30FPS_10BIT,
    SMART_SC2335_MIPI_2M_30FPS_10BIT,
    SMART_SC2235_DC_2M_30FPS_10BIT,
    SMART_AR0130_DC_1M_30FPS_12BIT,
    SMART_SC3235_MIPI_3M_30FPS_10BIT,
    GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT,
    GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT_FORCAR,
    OMNIVISION_OS05A_MIPI_4M_30FPS_12BIT,
    OMNIVISION_OS05A_MIPI_5M_30FPS_12BIT,
    OMNIVISION_OS05A_MIPI_4M_30FPS_10BIT_WDR2TO1,
    BT1120_2M_30FPS_8BIT,
    BT656_2M_30FPS_8BIT,
    BT601_2M_30FPS_8BIT,
    SAMPLE_SNS_TYPE_BUTT,
} SAMPLE_SNS_TYPE_E;

typedef enum hiSAMPLE_RC_E
{
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_CVBR,
    SAMPLE_RC_AVBR,
    SAMPLE_RC_QPMAP,
    SAMPLE_RC_FIXQP,
    SAMPLE_RC_QVBR
} SAMPLE_RC_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
