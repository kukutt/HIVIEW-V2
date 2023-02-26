#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "sample_comm.h"
#include "mpp.h"
#include "mpp_t31.h"

gsf_mpp_cfg_t mpp_cfg;

typedef struct {
    pthread_t read_thread;
    const char *filename;
    VENC_CHN VeChn;
    void *uargs;
    int (*cb)(VENC_CHN VeChn, PAYLOAD_TYPE_E PT, VENC_STREAM_S* pstStream, void* uargs);
    int stopflg;
} venc_pthread_param;
venc_pthread_param pthread_param_v[4];


typedef struct {
    pthread_t read_thread;
    void *uargs;
    int (*cb)(int AeChn, PAYLOAD_TYPE_E PT, AUDIO_STREAM_S* pstStream, void* uargs);
    int stopflg;
} aenc_pthread_param;
aenc_pthread_param pthread_param_a;

int SENSOR_TYPE;
int (*sensor_set_inifile_path)(const char *pcPath);
int (*sensor_register_callback)(void);

int gsf_mpp_cfg(char *path, gsf_mpp_cfg_t *cfg){
    SENSOR_TYPE = 0;
    sensor_set_inifile_path  = NULL;
    sensor_register_callback = NULL;

    //printf("[SEEFN]%s path=%s sns=%s\n", __func__, path, cfg->snsname);fflush(stdout);
    mpp_t31_init(cfg->snsname);

    return 0;
}

int SAMPLE_COMM_ISP_Init(int  enWDRMode){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_rgn_change(RGN_HANDLE Handle, int *dev, int *layer, int *runflg){
    if (Handle == 0){
        *dev = 0;
        *layer = 0;
    }else if (Handle == 1){
        *dev = 0;
        *layer = 1;
    }else if (Handle == 2){
        *dev = 0;
        *layer = 2;
    }else if (Handle == 7){
        *dev = 0;
        *layer = 3;
    }else if (Handle == 8){
        *dev = 3;
        *layer = 0;
    }else if (Handle == 9){
        *dev = 3;
        *layer = 1;
    }else if (Handle == 10){
        *dev = 3;
        *layer = 2;
    }else if (Handle == 15){
        *dev = 3;
        *layer = 3;
    }else{
        *runflg = 0;
    }

    return 0;
}

int gsf_mpp_rgn_ctl(RGN_HANDLE Handle, int id, gsf_mpp_rgn_t *rgn){
#if 0
    printf("[SEEFN]%s handle=%d, id=%d [%d,%d]->[%d,%d]%d\n", __func__, Handle, id, 
        rgn->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X, \
        rgn->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y, \
        rgn->stRegion.unAttr.stOverlay.stSize.u32Width, \
        rgn->stRegion.unAttr.stOverlay.stSize.u32Height, \
        rgn->stChnAttr.unChnAttr.stOverlayChn.u32Layer);fflush(stdout);
#endif
    int runflg = 1;
    int dev = 0;
    int layer = 0;
    gsf_mpp_rgn_change(Handle, &dev, &layer, &runflg);
    if (runflg)mpp_t31_ctrl_osd(dev, layer, rgn->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X, rgn->stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y);
    return 0;
}

int gsf_mpp_rgn_bitmap(RGN_HANDLE Handle, BITMAP_S *bitmap){
    //printf("[SEEFN]%s handle=%d [%d %d %d]\n", __func__, Handle, bitmap->enPixelFormat, bitmap->u32Width, bitmap->u32Height);fflush(stdout);
    int runflg = 1;
    int dev = 0;
    int layer = 0;
    gsf_mpp_rgn_change(Handle, &dev, &layer, &runflg);
    if (runflg)mpp_t31_date_osd(dev, layer, bitmap->u32Width, bitmap->u32Height, bitmap->pData);
    return 0;
}

void* gsf_venc_pthread(void *u){
    venc_pthread_param *p = (venc_pthread_param *)u;

    do{
        //printf("[%d]start VeChn\r\n", p->VeChn);
        //fflush(stdout);

        mpp_t31_start_video((p->VeChn==0)?0:3);
        void *s;
        VENC_STREAM_S stream;
        memset((char *)&stream, 0, sizeof(stream));
        VENC_PACK_S pack[8];
        //struct timespec ts_new, ts_old;
        //int cost;
        //int packcount=0;
        while(0 == p->stopflg){
            s = mpp_t31_get_video((p->VeChn==0)?0:3, pack, (int*)&stream.u32PackCount);
            #if 0
            clock_gettime(CLOCK_MONOTONIC, &ts_new);
            cost = (ts_new.tv_sec*1000 + ts_new.tv_nsec/1000000) - (ts_old.tv_sec*1000 +  ts_old.tv_nsec/1000000);
            packcount++;
            if (pack[0].DataType.enH264EType != 1){
            printf("[%d]start VeChn %d[%02x%02x%02x%02x%02x%02x%02x]%d %lld %d ms %d %d\r\n", p->VeChn, \
                pack[0].DataType.enH264EType, pack[0].pu8Addr[0], pack[0].pu8Addr[1], \
                pack[0].pu8Addr[2], pack[0].pu8Addr[3], pack[0].pu8Addr[4], pack[0].pu8Addr[5], \
                pack[0].pu8Addr[6], pack[0].u32Len, pack[0].u64PTS/1000, cost, stream.u32PackCount, packcount);
            packcount=0;
            }
            ts_old = ts_new;
            #endif
            stream.pstPack = pack;
            stream.u32Seq++;
            p->cb(p->VeChn, (PAYLOAD_TYPE_E)96, &stream, NULL);
            mpp_t31_get_video_free((p->VeChn==0)?0:3, s);
        }
        mpp_t31_stop_video((p->VeChn==0)?0:3);
    }while(0);
    return NULL; 
}

int gsf_mpp_venc_start(gsf_mpp_venc_t *venc){
    printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_venc_stop(gsf_mpp_venc_t *venc){
    printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

extern int mpp_t31_init(const char *snsname);
int gsf_mpp_venc_recv(gsf_mpp_recv_t *recv){
    
    for(int i=0; i<recv->s32Cnt; i++){
        //printf("venc recv [%d]\r\n", i, recv->VeChn);
        memset((char *)&pthread_param_v[i], 0, sizeof(venc_pthread_param));
        pthread_param_v[i].VeChn = recv->VeChn[i];
        pthread_param_v[i].filename = "/app/cap.h264";
        pthread_param_v[i].uargs = recv->uargs;
        pthread_param_v[i].cb = recv->cb;
        pthread_create(&pthread_param_v[i].read_thread, NULL, gsf_venc_pthread, &pthread_param_v[i]);
    }
    return 0;
}

int gsf_mpp_venc_dest(){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_venc_snap(VENC_CHN VencChn, HI_U32 SnapCnt, int(*cb)(int i, VENC_STREAM_S* pstStream, void* u), void* u){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_venc_ctl(int VencChn, int id, void *args){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_vi_start(gsf_mpp_vi_t *vi){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_vi_stop(){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_vpss_start(gsf_mpp_vpss_t *vpss){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_vpss_stop(gsf_mpp_vpss_t *vpss){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_ao_bind(int aodev, int ch, int aidev, int aich){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_scene_start(char *path, int scenemode){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

int gsf_mpp_scene_stop(){
    //printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}

void* gsf_aenc_pthread(void *u){
    aenc_pthread_param *p = (aenc_pthread_param *)u;
    AUDIO_STREAM_S stream;

    stream.pStream = NULL;
    stream.u64PhyAddr = 0;
    stream.u32Len = 0;
    stream.u64TimeStamp = 0;
    stream.u32Seq = 0;
    do{
        while(0 == p->stopflg){
            stream.u32Len = mpp_t31_get_audio(&stream.pStream, &stream.u64TimeStamp);
            stream.u32Seq++;
            p->cb(0, PT_G711A, &stream, NULL);
        }
    }while(0);
    return NULL; 
}

int gsf_mpp_audio_start(gsf_mpp_aenc_t *aenc){
    printf("[SEEFN]%s \n", __func__);fflush(stdout);
    mpp_t31_init_audio();

    memset((char *)&pthread_param_a, 0, sizeof(venc_pthread_param));
    pthread_param_a.uargs = aenc->uargs;
    pthread_param_a.cb = aenc->cb;
    pthread_create(&pthread_param_a.read_thread, NULL, gsf_aenc_pthread, &pthread_param_a);
    return 0;
}

int gsf_mpp_audio_stop(gsf_mpp_aenc_t  *aenc){
    printf("[SEEFN]%s \n", __func__);fflush(stdout);
    return 0;
}
