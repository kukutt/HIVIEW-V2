#ifndef __mpp_h__
#define __mpp_h__

#include <stdint.h>
#include "sample_comm.h"

enum {
  GSF_MPP_VENC_CTL_IDR = 0,
  GSF_MPP_VENC_CTL_RST = 1,
};

typedef struct {
  
  VENC_CHN  VencChn;
  
  MOD_ID_E  srcModId;
  union {
    VPSS_GRP  VpssGrp;
    AVS_GRP   AVSGrp;
    VI_PIPE   ViPipe;
    VO_LAYER  VoLayer;
  };
  union {
    VPSS_CHN  VpssChn; // VPSS
    AVS_CHN   AVSChn;  // AVS
    VDEC_CHN  VdChn;   // VDEC
    VI_CHN    ViChn;   // VI
    VO_CHN    VoChn;   // VO
  };
  
  PAYLOAD_TYPE_E  enPayLoad;
  PIC_SIZE_E      enSize;
  HI_S32          enRcMode;
  HI_U32          u32Profile;
  HI_BOOL         bRcnRefShareBuf;
  VENC_GOP_MODE_E enGopMode;
  HI_U32          u32FrameRate;
  HI_U32          u32Gop;
  HI_U32          u32BitRate;
  hi_u32          u32LowDelay;
}gsf_mpp_venc_t;

typedef struct {
  VPSS_GRP    VpssGrp;
  
  MOD_ID_E  srcModId;
  union{
    HI_S32  ViPipe;
  };
  union{
    VI_CHN  ViChn;
  };

  HI_BOOL     enable[VPSS_MAX_PHY_CHN_NUM];
  PIC_SIZE_E  enSize[VPSS_MAX_PHY_CHN_NUM];
}gsf_mpp_vpss_t;

typedef struct {
  HI_BOOL bLowDelay;
  HI_U32 u32SupplementConfig;
  VI_STITCH_GRP_ATTR_S stStitchGrpAttr; // 3516Exxx no-support 
  // SAMPLE_VENC_SYS_Init;
  HI_BOOL vpss_en[VPSS_MAX_PHY_CHN_NUM];
  PIC_SIZE_E vpss_sz[VPSS_MAX_PHY_CHN_NUM];
}gsf_mpp_vi_t;

typedef struct {
  HI_S32 s32Cnt;
  VENC_CHN VeChn[VENC_MAX_CHN_NUM];
  void *uargs;
  int (*cb)(VENC_CHN VeChn, PAYLOAD_TYPE_E PT, VENC_STREAM_S* pstStream, void* uargs);
}gsf_mpp_recv_t;

enum {
  GSF_MPP_RGN_CREATE      = 0, // HI_MPI_RGN_Create
  GSF_MPP_RGN_DESTROY     = 1, // HI_MPI_RGN_Destroy
  GSF_MPP_RGN_SETATTR     = 2, // HI_MPI_RGN_SetAttr
  GSF_MPP_RGN_GETATTR     = 3, // HI_MPI_RGN_GetAttr
  GSF_MPP_RGN_ATTACH      = 4, // HI_MPI_RGN_AttachToChn
  GSF_MPP_RGN_DETACH      = 5, // HI_MPI_RGN_DetachFromChn
  GSF_MPP_RGN_SETDISPLAY  = 6, // HI_MPI_RGN_SetDisplayAttr
  GSF_MPP_RGN_GETDISPLAY  = 7, // HI_MPI_RGN_GetDisplayAttr
};
typedef struct {
    // create;
    RGN_ATTR_S stRegion;
    // attach;
    MPP_CHN_S   stChn;
    // modify;
    RGN_CHN_ATTR_S stChnAttr;
}gsf_mpp_rgn_t;

//api;
typedef struct {
  char  snsname[32];  // sensor imx335
  int   snscnt;       // sensor count
  int   lane;         // lane mode
  int   wdr;          // wdr mode
  int   res;          // resolution
  int   fps;          // frame rate
}gsf_mpp_cfg_t;

int gsf_mpp_cfg(char *path, gsf_mpp_cfg_t *cfg);

// new
int gsf_mpp_rgn_ctl(RGN_HANDLE Handle, int id, gsf_mpp_rgn_t *rgn);
int gsf_mpp_rgn_bitmap(RGN_HANDLE Handle, BITMAP_S *bitmap);

int gsf_mpp_venc_start(gsf_mpp_venc_t *venc);
int gsf_mpp_venc_stop(gsf_mpp_venc_t *venc);
int gsf_mpp_venc_recv(gsf_mpp_recv_t *recv);
int gsf_mpp_venc_dest();
int gsf_mpp_venc_snap(VENC_CHN VencChn, HI_U32 SnapCnt, int(*cb)(int i, VENC_STREAM_S* pstStream, void* u), void* u);
int gsf_mpp_venc_ctl(int VencChn, int id, void *args);


int gsf_mpp_vi_start(gsf_mpp_vi_t *vi);
int gsf_mpp_vi_stop();

int gsf_mpp_vpss_start(gsf_mpp_vpss_t *vpss);
int gsf_mpp_vpss_stop(gsf_mpp_vpss_t *vpss);

int gsf_mpp_ao_bind(int aodev, int ch, int aidev, int aich);
int gsf_mpp_scene_start(char *path, int scenemode);
int gsf_mpp_scene_stop();
// new end

int gsf_mpp_isp_init();
int gsf_mpp_isp_run();
int gsf_mpp_isp_ctl();
int gsf_mpp_scene_run();
int gsf_mpp_scene_stop();
int gsf_mpp_isp_stop();

int gsf_mpp_venc_init();    
int gsf_mpp_venc_run();     
int gsf_mpp_venc_ctl();     
int gsf_mpp_venc_stop();    
                        
int gsf_mpp_aenc_init();    
int gsf_mpp_aenc_run();     
int gsf_mpp_aenc_ctl();     
int gsf_mpp_aenc_stop();    

typedef struct {
  int AeChn;
  PAYLOAD_TYPE_E enPayLoad; // PT;
  int adtype; // 0:INNER, 1:I2S, 2:PCM;
  int stereo, sp, br;//stereo, sampleRate, bitRate;
  void *uargs;
  int (*cb)(int AeChn, PAYLOAD_TYPE_E PT, AUDIO_STREAM_S* pstStream, void* uargs);
}gsf_mpp_aenc_t;
int gsf_mpp_audio_start(gsf_mpp_aenc_t *aenc);
int gsf_mpp_audio_stop(gsf_mpp_aenc_t  *aenc);

extern int SENSOR_TYPE;
extern int (*sensor_set_inifile_path)(const char *pcPath);
extern int (*sensor_register_callback)(void);

#endif
