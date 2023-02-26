/*
 * sample-snap-raw.c
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */

#include <imp/imp_log.h>
#include <imp/imp_common.h>
#include <imp/imp_system.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>

#include "sample-common.h"

#define TAG "Sample-Snap-RAW"
extern struct chn_conf chn[];

int main(int argc, char *argv[])
{
	int ret;
	int i;

	IMPFrameInfo *frame_bak;
	IMPFSChnAttr fs_chn_attr[2];
	int snap_num = 2;
	FILE *fp[5];
	char path_name[128];

	for(i=0; i < snap_num; i++)
	{
		sprintf(path_name,"/tmp/snap%d.raw",i);
		fp[i] = fopen(path_name, "wb");
		if(fp == NULL) {
			IMP_LOG_ERR(TAG, "%s(%d):open fp[%d] error !\n", __func__, __LINE__, i);
			return -1;
		}
	}

	/* Step.1 System init */
	ret = sample_system_init();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_System_Init() failed\n");
		return -1;
	}
    ret = IMP_ISP_Tuning_SetSensorFPS(SENSOR_FRAME_RATE_NUM, SENSOR_FRAME_RATE_DEN);
    if (ret < 0){
        IMP_LOG_ERR(TAG, "failed to set sensor fps\n");
        return -1;
    }

	/* Step.2 FrameSource init */
	if (chn[0].enable) {
		ret = IMP_FrameSource_CreateChn(chn[0].index, &chn[0].fs_chn_attr);
		if(ret < 0){
			IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chn[0].index);
			return -1;
		}

		ret = IMP_FrameSource_SetChnAttr(chn[0].index, &chn[0].fs_chn_attr);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n",  chn[0].index);
			return -1;
		}
	}

	/* Step.3 Snap raw config */
	ret = IMP_FrameSource_GetChnAttr(0, &fs_chn_attr[0]);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetChnAttr failed\n", __func__, __LINE__);
		return -1;
	}

	ret = IMP_ISP_Tuning_SetISPBypass(IMPISP_TUNING_OPS_MODE_DISABLE);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_ISP_Tuning_SetISPBpass failed\n", __func__, __LINE__);
		return -1;
	}

	fs_chn_attr[0].pixFmt = PIX_FMT_RAW;
	ret = IMP_FrameSource_SetChnAttr(0, &fs_chn_attr[0]);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetChnAttr failed\n", __func__, __LINE__);
		return -1;
	}

	/* Step.3 config sensor reg to output colrbar raw data*/
	/* to do */

	/* Step.4 Stream On */
	if (chn[0].enable){
		ret = IMP_FrameSource_EnableChn(chn[0].index);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn(%d) error: %d\n", ret, chn[0].index);
			return -1;
		}
	}

	/* Step.4 Snap raw */
	ret = IMP_FrameSource_SetFrameDepth(0, 1);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetFrameDepth failed\n", __func__, __LINE__);
		return -1;
	}

	for(i = 0; i < snap_num; i++){
		ret = IMP_FrameSource_GetFrame(0, &frame_bak);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetFrame failed\n", __func__, __LINE__);
			return -1;
		}
		fwrite((void *)frame_bak->virAddr, frame_bak->size, 1, fp[i]);
		fclose(fp[i]);
		IMP_FrameSource_ReleaseFrame(0, frame_bak);
	}

	ret = IMP_ISP_Tuning_SetISPBypass(IMPISP_TUNING_OPS_MODE_ENABLE);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_ISP_Tuning_SetISPBpass failed\n", __func__, __LINE__);
		return -1;
	}

	ret = IMP_FrameSource_SetFrameDepth(0, 0);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetFrameDepth failed\n", __func__, __LINE__);
		return -1;
	}
	/* end */

	/* Step.5 Stream Off */
	if (chn[0].enable){
		ret = IMP_FrameSource_DisableChn(chn[0].index);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n", ret, chn[0].index);
			return -1;
		}
	}

	/* Step.6 FrameSource exit */
	if (chn[0].enable) {
		/*Destroy channel i*/
		ret = IMP_FrameSource_DestroyChn(0);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn() error: %d\n", ret);
			return -1;
		}
	}

	/* Step.7 recover sensor reg to output normal image*/
	/* to do */

	/* Step.8 System exit */
	ret = sample_system_exit();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "sample_system_exit() failed\n");
		return -1;
	}

	return 0;
}
