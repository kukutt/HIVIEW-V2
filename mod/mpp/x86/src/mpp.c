#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "sample_comm.h"
#include "mpp.h"

gsf_mpp_cfg_t mpp_cfg;

typedef struct {
    pthread_t read_thread;
    const char *filename;
    VENC_CHN VeChn;
    void *uargs;
    int (*cb)(VENC_CHN VeChn, PAYLOAD_TYPE_E PT, VENC_STREAM_S* pstStream, void* uargs);
    int stopflg;
} venc_pthread_param;
venc_pthread_param pthread_param[4];

int SENSOR_TYPE;
int (*sensor_set_inifile_path)(const char *pcPath);
int (*sensor_register_callback)(void);

int gsf_mpp_cfg(char *path, gsf_mpp_cfg_t *cfg){
    SENSOR_TYPE = 0;
    sensor_set_inifile_path  = NULL;
    sensor_register_callback = NULL;

    printf("%s => load libsns.so sensor_type:%d\n", __func__, SENSOR_TYPE);

    return 0;
}

int gsf_mpp_rgn_ctl(RGN_HANDLE Handle, int id, gsf_mpp_rgn_t *rgn){
    return 0;
}

int SAMPLE_COMM_ISP_Init(int  enWDRMode){
    printf("%s => enWDRMode:%d\n", __func__, enWDRMode);
}

int gsf_mpp_rgn_bitmap(RGN_HANDLE Handle, BITMAP_S *bitmap){
    return 0;
}

extern int simplest_h264_parser_one(FILE *fp, const char *buf, int buf_len_max, int *buf_len, int *type);
void* gsf_venc_pthread(void *u){
    venc_pthread_param *p = (venc_pthread_param *)u;
    FILE *fp = NULL;
    int max_len = 1000000, len;
    char *buf = NULL;
    int type;
    int ret = 0;
    VENC_STREAM_S stream;


    do{
        printf("start VeChn[%d]\r\n", p->VeChn);
        FILE *fp = fopen(p->filename, "r");
        if (fp == NULL)break;
        uint8_t *buf = (uint8_t *)malloc(1000000);
        if (buf == NULL)break;
        printf("[%d]start VeChn\r\n", p->VeChn);

        struct timespec ts;
        int offset = 0;
        VENC_STREAM_S stream;
        memset((char *)&stream, 0, sizeof(stream));
        VENC_PACK_S pack[8];
        int totallen = 0;
        while(0 == p->stopflg){
            usleep(40000);

            offset = 0;
            int i = 0;
            while(i < 8){
                ret = simplest_h264_parser_one(fp, (char *)&buf[offset], max_len, &len, &type);
                // fprintf(stdout,"|%8d|%8d|%02x%02x%02x%02x%02x%02x%02x|%8d|%d\r\n",len, type, \
                //     buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], totallen, ret);
                if (ret == 0)totallen += len;
                if ((feof(fp) != 0) || (ret != 0)){
                    printf("[%d]SEEK_SET totallen=%d, type=%d\r\n", p->VeChn, totallen, type);
                    fseek(fp, 0, SEEK_SET);
                    totallen = 0;
                    break;
                }
                pack[i].u32Len = len;
                pack[i].u32Offset = 0;
                pack[i].pu8Addr = &buf[offset];
                clock_gettime(CLOCK_MONOTONIC, &ts);
                pack[i].u64PTS = (ts.tv_sec*1000000 + ts.tv_nsec/1000);
                pack[i].DataType.enH264EType = (H264E_NALU_TYPE_E)type;
                offset += len;
                i++;
                if ((type == 1) || (type == 5)){
                    break;
                }
            }
            stream.pstPack = pack;
            stream.u32PackCount = i;
            stream.u32Seq++;
            p->cb(p->VeChn, (PAYLOAD_TYPE_E)96, &stream, NULL);
        }
    }while(0);  
}

int gsf_mpp_venc_start(gsf_mpp_venc_t *venc){
    //printf("venc start %d %d\r\n", venc->VencChn, venc->enSize);
    return 0;
}

int gsf_mpp_venc_stop(gsf_mpp_venc_t *venc){
    //printf("venc stop\r\n");
    return 0;
}

int gsf_mpp_venc_recv(gsf_mpp_recv_t *recv){
    for(int i=0; i<recv->s32Cnt; i++){
        //printf("venc recv [%d]\r\n", i, recv->VeChn);
        memset((char *)&pthread_param[i], 0, sizeof(venc_pthread_param));
        pthread_param[i].VeChn = recv->VeChn[i];
        pthread_param[i].filename = "/tmp/cap.h264";
        pthread_param[i].uargs = recv->uargs;
        pthread_param[i].cb = recv->cb;
        pthread_create(&pthread_param[i].read_thread, NULL, gsf_venc_pthread, &pthread_param[i]);
    }
    return 0;
}

int gsf_mpp_venc_dest(){
    return 0;
}

int gsf_mpp_venc_snap(VENC_CHN VencChn, HI_U32 SnapCnt, int(*cb)(int i, VENC_STREAM_S* pstStream, void* u), void* u){
    return 0;
}

int gsf_mpp_venc_ctl(int VencChn, int id, void *args){
    return 0;
}

int gsf_mpp_vi_start(gsf_mpp_vi_t *vi){
    return 0;
}

int gsf_mpp_vi_stop(){
    return 0;
}

int gsf_mpp_vpss_start(gsf_mpp_vpss_t *vpss){
    return 0;
}

int gsf_mpp_vpss_stop(gsf_mpp_vpss_t *vpss){
    return 0;
}

int gsf_mpp_ao_bind(int aodev, int ch, int aidev, int aich){
    return 0;
}

int gsf_mpp_scene_start(char *path, int scenemode){
    return 0;
}

int gsf_mpp_scene_stop(){
    return 0;
}

int gsf_mpp_audio_start(gsf_mpp_aenc_t *aenc){
    return 0;
}

int gsf_mpp_audio_stop(gsf_mpp_aenc_t  *aenc){
    return 0;
}
