#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

// T31 sdk
#include <imp/imp_log.h>
#include <imp/imp_common.h>
#include <imp/imp_system.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>
#include <imp/imp_audio.h>
#include "samples/libimp-samples/sample-common.h"


#include "mpp_t31.h"
// 兼容hisi的pack
typedef struct{
    uint64_t    res1;
    uint8_t     *pu8Addr;
    uint32_t    u32Len;
    uint64_t    u64PTS;
    int         res2;
    int         DataType;
    uint32_t    u32Offset;
    uint32_t    res3;
    uint8_t     res4[8*12];
} VENC_PACK_HISI_S;

extern struct chn_conf chn[];
typedef struct{
    IMPRgnHandle *rgn;
    int w[8];
    int h[8];
    char *p[8];
} RGN_PACK_HISI_S;
RGN_PACK_HISI_S gRgn[FS_CHN_NUM];


IMPRgnHandle *mpp_t31_init_osd(int grpNum)
{
	int ret;
	IMPRgnHandle *prHander;
	IMPRgnHandle rHanderFont;
	IMPRgnHandle rHanderLogo;
	IMPRgnHandle rHanderCover;
	IMPRgnHandle rHanderRect;

	prHander = malloc(4 * sizeof(IMPRgnHandle));
	if (prHander <= 0) {
		printf("malloc() error !\n");
		return NULL;
	}

	rHanderFont = IMP_OSD_CreateRgn(NULL);
	if (rHanderFont == INVHANDLE) {
		printf("IMP_OSD_CreateRgn TimeStamp error !\n");
		return NULL;
	}

	rHanderLogo = IMP_OSD_CreateRgn(NULL);
	if (rHanderLogo == INVHANDLE) {
		printf("IMP_OSD_CreateRgn Logo error !\n");
		return NULL;
	}

	rHanderCover = IMP_OSD_CreateRgn(NULL);
	if (rHanderCover == INVHANDLE) {
		printf("IMP_OSD_CreateRgn Cover error !\n");
		return NULL;
	}

	rHanderRect = IMP_OSD_CreateRgn(NULL);
	if (rHanderRect == INVHANDLE) {
		printf("IMP_OSD_CreateRgn Rect error !\n");
		return NULL;
	}


	ret = IMP_OSD_RegisterRgn(rHanderFont, grpNum, NULL);
	if (ret < 0) {
		printf("IVS IMP_OSD_RegisterRgn failed\n");
		return NULL;
	}

	ret = IMP_OSD_RegisterRgn(rHanderLogo, grpNum, NULL);
	if (ret < 0) {
		printf("IVS IMP_OSD_RegisterRgn failed\n");
		return NULL;
	}

	ret = IMP_OSD_RegisterRgn(rHanderCover, grpNum, NULL);
	if (ret < 0) {
		printf("IVS IMP_OSD_RegisterRgn failed\n");
		return NULL;
	}

	ret = IMP_OSD_RegisterRgn(rHanderRect, grpNum, NULL);
	if (ret < 0) {
		printf("IVS IMP_OSD_RegisterRgn failed\n");
		return NULL;
	}

	ret = IMP_OSD_Start(grpNum);
	if (ret < 0) {
		printf("IMP_OSD_Start TimeStamp, Logo, Cover and Rect error !\n");
		return NULL;
	}
	prHander[0] = rHanderFont;
	prHander[1] = rHanderLogo;
	prHander[2] = rHanderCover;
	prHander[3] = rHanderRect;
	return prHander;
}


int mpp_t31_init(const char *snsname){
    int ret = -1;
    memset(gRgn, 0, sizeof(gRgn));

    do{
        if (0 == memcmp(snsname, "gc4653", 6)){
            chn[0].fs_chn_attr.picWidth=2560;
            chn[0].fs_chn_attr.picHeight=1440;
            chn[0].fs_chn_attr.crop.width=chn[0].fs_chn_attr.picWidth;
            chn[0].fs_chn_attr.crop.height=chn[0].fs_chn_attr.picHeight;

            chn[3].fs_chn_attr.crop.width=chn[0].fs_chn_attr.picWidth;
            chn[3].fs_chn_attr.crop.height=chn[0].fs_chn_attr.picHeight;
            ret = sample_system_initV2(snsname, TX_SENSOR_CONTROL_INTERFACE_I2C, 0x29);
        }else if (0 == memcmp(snsname, "sc5235", 6)){
            chn[0].fs_chn_attr.picWidth=2560;
            chn[0].fs_chn_attr.picHeight=1920;
            chn[0].fs_chn_attr.crop.width=chn[0].fs_chn_attr.picWidth;
            chn[0].fs_chn_attr.crop.height=chn[0].fs_chn_attr.picHeight;

            chn[3].fs_chn_attr.crop.width=chn[0].fs_chn_attr.picWidth;
            chn[3].fs_chn_attr.crop.height=chn[0].fs_chn_attr.picHeight;
            ret = sample_system_initV2(snsname, TX_SENSOR_CONTROL_INTERFACE_I2C, 0x30);
        }else{
            printf("error snsname=%s\r\n", snsname);
            exit(-1);
        }
        
        printf("[T31DEV]sample_system_init ret=%d\r\n", ret);
        if (ret < 0)break;
        ret = sample_framesource_init();
        printf("[T31DEV]sample_framesource_init ret=%d\r\n", ret);
        if (ret < 0)break;
        for (int i = 0; i < FS_CHN_NUM; i++) {
            if (chn[i].enable) {
                ret = IMP_Encoder_CreateGroup(chn[i].index);
                printf("IMP_Encoder_CreateGroup[%d] ret=%d\r\n", i, ret);
                if(ret < 0)break;
            }
        }
        if (ret < 0)break;

        ret = sample_encoder_init();
        printf("[T31DEV]sample_encoder_init ret=%d\r\n", ret);
        if (ret < 0)break;

        IMPCell osdcell = {DEV_ID_OSD, 0, 0};
        for (int i = 0; i < FS_CHN_NUM; i++) {
            if (chn[i].enable) {
                ret = IMP_OSD_CreateGroup(i);
                printf("[T31DEV]IMP_OSD_CreateGroup[%d] ret=%d\r\n", i, ret);
                if(ret < 0)break;

                gRgn[i].rgn = mpp_t31_init_osd(i);
                printf("[T31DEV]IMP_OSD_CreateGroup[%d] rgn=%p\r\n", i, gRgn[i].rgn);
                if(gRgn[i].rgn <= 0)break;
                osdcell.groupID = i;

                ret = IMP_System_Bind(&chn[i].framesource_chn, &osdcell);
                printf("[T31DEV]IMP_System_Bind[%d] ret=%d\r\n", i, ret);
                if(ret < 0)break;

                ret = IMP_System_Bind(&osdcell, &chn[i].imp_encoder);
                printf("[T31DEV]IMP_System_Bind[%d] ret=%d\r\n", i, ret);
                if(ret < 0)break;
            }
        }
        if (ret < 0)break;

        ret = sample_framesource_streamon();
        printf("[T31DEV]sample_framesource_streamon ret=%d\r\n", ret);
        if (ret < 0)break;
        printf("[T31DEV]ok mpp_t31_init %d\r\n", sizeof(VENC_PACK_HISI_S));
    }while(0);

    void *p = NULL;
    mpp_t31_start_video(0);
    for (int i = 0; i < 5; i++){
        p = mpp_t31_get_video(0, NULL, NULL);
        mpp_t31_get_video_free(0, p);
    }
    mpp_t31_stop_video(0);

    mpp_t31_start_video(3);
    for (int i = 0; i < 5; i++){
        p = mpp_t31_get_video(3, NULL, NULL);
        mpp_t31_get_video_free(3, p);
    }
    mpp_t31_stop_video(3);
    return ret;
}

int mpp_t31_start_video(int streamid){
    int val,chnNum,ret;
    
    val = (((chn[streamid].payloadType >> 24) << 16) | chn[streamid].index);
    chnNum = val & 0xffff;

    ret = IMP_Encoder_StartRecvPic(chnNum);
    printf("[T31DEV][%d]IMP_Encoder_StartRecvPic ret=%d\r\n", chnNum, ret);
    return ret;
}

int mpp_t31_stop_video(int streamid){
    int val,chnNum,ret;
    
    val = (((chn[streamid].payloadType >> 24) << 16) | chn[streamid].index);
    chnNum = val & 0xffff;

    ret = IMP_Encoder_StopRecvPic(chnNum);
    printf("[T31DEV][%d]IMP_Encoder_StopRecvPic ret=%d\r\n", chnNum, ret);
    return ret;
}

void *mpp_t31_get_video(int streamid, void *pack, int *pack_count){
    int val, i, j, chnNum, ret;

    val = (((chn[streamid].payloadType >> 24) << 16) | chn[streamid].index);
    chnNum = val & 0xffff;

    ret = IMP_Encoder_PollingStream(chnNum, 1000);
    if (ret < 0) {
        printf("[T31DEV][%d]IMP_Encoder_PollingStream(%d) timeout\r\n", streamid, chnNum);
        return NULL;
    }

    IMPEncoderStream *stream = malloc(sizeof(IMPEncoderStream));
    ret = IMP_Encoder_GetStream(chnNum, stream, 1);
    if (ret < 0) {
        printf("[T31DEV][%d]IMP_Encoder_GetStream(%d) failed\r\n", streamid, chnNum);
        return NULL;
    }
    int nr_pack = stream->packCount;
    VENC_PACK_HISI_S *p = (VENC_PACK_HISI_S *)pack;
    //if (streamid==0)printf("[SP][v]c=%d, s=%d, vlen=%d %lld\r\n", stream->packCount, stream->streamSize, stream->pack[0].nalType.h264NalType, stream->pack[0].timestamp);
    for (i = 0, j = 0; i < nr_pack; i++,j++) {
        IMPEncoderPack *pack = &stream->pack[i];
        if(pack->length){
            uint32_t remSize = stream->streamSize - pack->offset;
            if (p){
                p[j].u32Offset = 0;
                p[j].u64PTS = (uint64_t)pack->timestamp;
                p[j].DataType = pack->nalType.h264NalType;
            }
            if(remSize < pack->length){
                printf("[T31DEV]aa[%d] [%d/%d] packlen=%d, streamsize=%d, offset=%d\n", streamid, i, nr_pack, pack->length, stream->streamSize, pack->offset);
                if (p){
                    p[j].pu8Addr = (uint8_t *)(stream->virAddr + pack->offset);
                    p[j].u32Len = remSize;
                    p[j].pu8Addr = (uint8_t *)(stream->virAddr);
                    p[j].u32Len = pack->length - remSize;
                }
                fflush(stdout);
            }else {
                //printf("[T31DEV]bb[%d] [%d/%d] packlen=%d, streamsize=%d, offset=%d\n", streamid, i, nr_pack, pack->length, stream->streamSize, pack->offset);
                if (p){
                    p[j].pu8Addr = (uint8_t *)(stream->virAddr + pack->offset);
                    p[j].u32Len = pack->length;
                }
            }
        }
    }
    if (pack_count != NULL)*pack_count = j;
    fflush(stdout);
    return stream;
}


void mpp_t31_get_video_free(int streamid, void *p){
    IMPEncoderStream *stream = p;
    int val,chnNum;

    if (stream != NULL){
        val = (((chn[streamid].payloadType >> 24) << 16) | chn[streamid].index);
        chnNum = val & 0xffff;
        IMP_Encoder_ReleaseStream(chnNum, stream);
        free(stream);
    }
}

int mpp_t31_ctrl_osd(int grp, int id, int x, int y){
    int ret = -1;
    
    if (gRgn[grp].rgn && gRgn[grp].p[id]){
        int w,h;
        char *p = NULL;
        do{
            //printf("[OSD2.1] w=%d,h=%d,p=%p\r\n", gRgn[grp].w[id], gRgn[grp].h[id], gRgn[grp].p[id]);fflush(stdout);
            IMPOSDRgnAttr rAttrRect;
            memset(&rAttrRect, 0, sizeof(IMPOSDRgnAttr));
            w = gRgn[grp].w[id];
            h = gRgn[grp].h[id];
            //p = gRgn[grp].p[id];
            p = malloc((w-1)*h*2);
            //memset(p, 0, (w-1)*h*2);

            //rAttrRect.type = OSD_REG_RECT;
            rAttrRect.type = OSD_REG_PIC;
            rAttrRect.rect.p0.x = x;
            rAttrRect.rect.p0.y = y;
            rAttrRect.rect.p1.x = x + w - 1;
            rAttrRect.rect.p1.y = y + h - 1;
            rAttrRect.fmt = PIX_FMT_RGB555LE;
            // rAttrRect.data.picData.pData = NULL;
            // rAttrRect.fmt = PIX_FMT_MONOWHITE;
            // rAttrRect.data.lineRectData.color = OSD_RED;
            // rAttrRect.data.lineRectData.linewidth = 5;
            
            
            rAttrRect.data.picData.pData = p;
            //uint16_t *picdata = (uint16_t *)p;
            
            //printf("[OSD2.2] wh[%d,%d]\r\n", w, h);fflush(stdout);
            char *src = gRgn[grp].p[id];
            char *dst = p;
            for (int i = 0; i < h; i++){
                memcpy(dst, src, (w-1)*2);
                dst += (w-1)*2;
                src += w*2;
            }

#if 0
            static int sw = 0;
            int aa = 0;
            for (int i = 0; i < (w-1)*h; i++){
                if (sw%4 == 0){
                    picdata[i] = 0x801f;
                    aa = 1;
                }else if (sw%4 == 1){
                    picdata[i] = 0x83e0;
                    aa = 2;
                }else if (sw%4 == 2){
                    picdata[i] = 0xfc00;
                    aa = 3;
                }else{
                    picdata[i] = 0xffff;
                    aa = 4;
                }
            }
            printf("[OSD2.3] sw=%d aa=%d\r\n", sw, aa);fflush(stdout);
            if (grp == 3)sw++;
#endif
            //w -= 1;
            // for (int i = 0; i < h; i++){
            //     for (int j = 0; j < w; j++){
            //         //if (picdata[i]&0x8000)continue;
            //         if (i == 0)picdata[(i*w)+j] = 0x801f;
            //         if (j == 0)picdata[(i*w)+j] = 0x83e0;
            //         // if (x <= 100){
            //         //     picdata[i] = 0x801f;
            //         // }else if(x <= 200){
            //         //     picdata[i] = 0x83e0;
            //         // }else{
            //         //     picdata[i] = 0xfc00;
            //         // }
            //     }
            // }
            
            //char fn[64];
            //static int fnc=0;
            //snprintf(fn, sizeof(fn), "/tmp/aa%d.dat", fnc++);
            //FILE *fp= fopen(fn, "wb+");
            //fwrite(p, w*h*2, 1, fp);
            //fclose(fp);

            ret = IMP_OSD_SetRgnAttr(gRgn[grp].rgn[id], &rAttrRect);
            if (ret < 0) {
                printf("[T31DEV]IMP_OSD_SetRgnAttr ret=%d\r\n", ret);
                break;
            }
            
            ret = IMP_OSD_GetRgnAttr(gRgn[grp].rgn[id], &rAttrRect);
            if (ret < 0) {
                printf("[T31DEV]IMP_OSD_SetRgnAttr ret=%d\r\n", ret);
                break;
            }
            //printf("[OSD2.2] [%d,%d] [%d,%d]\r\n", rAttrRect.rect.p0.x, rAttrRect.rect.p0.y, rAttrRect.rect.p1.x, rAttrRect.rect.p1.y);fflush(stdout);


            IMPOSDGrpRgnAttr grAttrRect;
            // if (IMP_OSD_GetGrpRgnAttr(gRgn[grp].rgn[id], grp, &grAttrRect) < 0) {
            //     printf("[T31DEV]IMP_OSD_GetGrpRgnAttr ret=%d\r\n", ret);
            //     break;
            // }

            memset(&grAttrRect, 0, sizeof(IMPOSDGrpRgnAttr));
            grAttrRect.show = 1;
            grAttrRect.layer = id;
            grAttrRect.scalex = 1;
            grAttrRect.scaley = 1;
            if (IMP_OSD_SetGrpRgnAttr(gRgn[grp].rgn[id], grp, &grAttrRect) < 0) {
                printf("[T31DEV]IMP_OSD_SetGrpRgnAttr ret=%d\r\n", ret);
                break;
            }
            //printf("[OSD3.1] w=%d,h=%d,p=%p\r\n", gRgn[grp].w[id], gRgn[grp].h[id], gRgn[grp].p[id]);fflush(stdout);
            gRgn[grp].p[id] = NULL;
        }while(0);
        if (p != NULL)free(p);
    }
    return ret;
}

int mpp_t31_date_osd(int grp, int id, int w, int h, char *p){
    int ret = -1;

    if (gRgn[grp].rgn){
        do{
            gRgn[grp].w[id] = w;
            gRgn[grp].h[id] = h;
            gRgn[grp].p[id] = p;
            //printf("[OSD1] w=%d,h=%d,p=%p\r\n", gRgn[grp].w[id], gRgn[grp].h[id], gRgn[grp].p[id]);
        }while(0);
    }
    return ret;
}
#define    QUANT_MASK      (0xf)
#define    NSEGS           (8)
#define    SEG_SHIFT       (4)
static int G711_linear2alaw(  
long             size,                   /* In: block size             */
short           *pcm_ptr,               /* In: 16-bit PCM data        */
unsigned char   *out_ptr)               /* Out: 8-bit A-law data      */
/*EMP=================================================================*/
{
    unsigned char mask;
    long           seg;
    long           i;
    long           j;
    unsigned char aval;
    unsigned long seg_end;
    short         pcm_val;

    for (j=0; j < size; j++)
    {
        seg_end = 0x80;
        pcm_val = *pcm_ptr & 0xFFF8;
        pcm_ptr++;
       
        /*------------------------------------------------------------*/
        /* Sign (7th) bit = 1                                         */
        /*------------------------------------------------------------*/
        if (pcm_val >= 0)
            mask = 0xD5;

        /*------------------------------------------------------------*/
        /* Sign bit = 0                                               */
        /*------------------------------------------------------------*/
        else
        {
            mask = 0x55;
            pcm_val = -pcm_val - 8;
        }

        /*------------------------------------------------------------*/
        /* Convert the scaled magnitude to segment number.            */
        /*------------------------------------------------------------*/
        seg = NSEGS;

        for (i = 0; i < NSEGS; i++)
        {
            seg_end <<= 1;
            if ((unsigned long)pcm_val < seg_end)
            {
                seg = i;
                break;
            }
        }
        /*------------------------------------------------------------*/
        /* Out of range, set maximum value.                           */
        /*------------------------------------------------------------*/
        aval = 0x7F;

        /*------------------------------------------------------------*/
        /* Combine the sign, segment, and quantization bits.          */
        /*------------------------------------------------------------*/
        if (seg < NSEGS)
        {
            aval = (unsigned char)(seg << SEG_SHIFT);
            if (seg < 2)
                aval |= ((unsigned long)pcm_val >> 4) & QUANT_MASK;
            else
                aval |= ((unsigned long)pcm_val >> (seg + 3)) &
                                                            QUANT_MASK;

        }
        *out_ptr = aval ^ mask;
        out_ptr++;
    }
    return 0;
}

static int g_AudiodevID = 1;
static int g_AudiochnID = 0;
int mpp_t31_init_audio(){
    int ret;

    /* Step 1: set public attribute of AI device. */
    int devID = g_AudiodevID;
    IMPAudioIOAttr attr;
    attr.samplerate = AUDIO_SAMPLE_RATE_8000;
    attr.bitwidth = AUDIO_BIT_WIDTH_16;
    attr.soundmode = AUDIO_SOUND_MODE_MONO;
    attr.frmNum = 20;
    attr.numPerFrm = 400;
    attr.chnCnt = 1;
    ret = IMP_AI_SetPubAttr(devID, &attr);
    printf("[T31DEV]IMP_AI_SetPubAttr ret=%d\r\n", ret);
    if(ret != 0)return -1;

    memset(&attr, 0x0, sizeof(attr));
    ret = IMP_AI_GetPubAttr(devID, &attr);
    printf("[T31DEV]IMP_AI_GetPubAttr ret=%d\r\n", ret);
    if(ret != 0)return -1;

    printf("[T31DEV]Audio In GetPubAttr samplerate : %d\r\n", attr.samplerate);
    printf("[T31DEV]Audio In GetPubAttr   bitwidth : %d\r\n", attr.bitwidth);
    printf("[T31DEV]Audio In GetPubAttr  soundmode : %d\r\n", attr.soundmode);
    printf("[T31DEV]Audio In GetPubAttr     frmNum : %d\r\n", attr.frmNum);
    printf("[T31DEV]Audio In GetPubAttr  numPerFrm : %d\r\n", attr.numPerFrm);
    printf("[T31DEV]Audio In GetPubAttr     chnCnt : %d\r\n", attr.chnCnt);

    /* Step 2: enable AI device. */
    ret = IMP_AI_Enable(devID);
    printf("[T31DEV]IMP_AI_Enable ret=%d\r\n", ret);
    if(ret != 0)return -1;

    /* Step 3: set audio channel attribute of AI device. */
    int chnID = g_AudiochnID;
    IMPAudioIChnParam chnParam;
    chnParam.usrFrmDepth = 20;
    ret = IMP_AI_SetChnParam(devID, chnID, &chnParam);
    printf("[T31DEV]IMP_AI_SetChnParam ret=%d\r\n", ret);
    if(ret != 0)return -1;

    /* Step 4: enable AI channel. */
    ret = IMP_AI_EnableChn(devID, chnID);
    printf("[T31DEV]IMP_AI_EnableChn ret=%d\r\n", ret);
    if(ret != 0)return -1;

    /* Step 5: Set audio channel volume. */
    int chnVol = 60;
    ret = IMP_AI_SetVol(devID, chnID, chnVol);
    printf("[T31DEV]IMP_AI_SetVol ret=%d\r\n", ret);
    if(ret != 0)return -1;

    int aigain = 20;
    ret = IMP_AI_SetGain(devID, chnID, aigain);
    printf("[T31DEV]IMP_AI_SetGain ret=%d\r\n", ret);
    if(ret != 0)return -1;

    /* Enable Aec */
    ret = IMP_AI_EnableAec(devID, chnID, 0, 0);
    printf("[T31DEV]IMP_AI_SetGain ret=%d\r\n", ret);
    if(ret != 0)return -1;

    return 0;
}

int mpp_t31_get_audio(uint8_t **dat, uint64_t *pts){
    int ret = 0;
    int stream_len = 0;
    int devID = g_AudiodevID;
    int chnID = g_AudiochnID;
    static uint8_t buf[960];
    *dat = buf;

    do{
        ret = IMP_AI_PollingFrame(devID, chnID, 1000);
        if (ret != 0)break;

        IMPAudioFrame frm;
        ret = IMP_AI_GetFrame(devID, chnID, &frm, BLOCK);
        if (ret != 0)break;
        
        stream_len = frm.len;
        memcpy(buf, frm.virAddr, stream_len);
        G711_linear2alaw(frm.len/2, (short *)frm.virAddr, buf);
        stream_len = frm.len/2;
        *pts = frm.timeStamp;
        //printf("[SP][a]%d, alen=%d %lld\r\n", frm.len, stream_len, *pts);
        
        ret = IMP_AI_ReleaseFrame(devID, chnID, &frm);
    }while(0);

    return (ret != 0)?-1:stream_len;
}
