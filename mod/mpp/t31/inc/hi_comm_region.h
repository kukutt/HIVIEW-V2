/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: hi_comm_region.h
 * Author:
 * Create: 2010-12-13
 */
#ifndef __HI_COMM_REGION_H__
#define __HI_COMM_REGION_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef HI_U32 RGN_HANDLE;

/* type of video regions */
typedef enum hiRGN_TYPE_E {
    OVERLAY_RGN = 0, /* video overlay region */
    COVER_RGN,
    COVEREX_RGN,
    OVERLAYEX_RGN,
    MOSAIC_RGN,
    RGN_BUTT
} RGN_TYPE_E;

typedef struct hiOVERLAY_ATTR_S {
    PIXEL_FORMAT_E enPixelFmt;
    HI_U32 u32BgColor;
    SIZE_S stSize;
    HI_U32 u32CanvasNum;
} OVERLAY_ATTR_S;

typedef struct hiOVERLAY_CHN_ATTR_S {
    POINT_S stPoint;
    HI_U32 u32FgAlpha;
    HI_U32 u32BgAlpha;
    HI_U32 u32Layer; /* OVERLAY region layer range:[0,7] */
} OVERLAY_CHN_ATTR_S;

typedef enum hiRGN_AREA_TYPE_E {
    AREA_RECT = 0,
    AREA_QUAD_RANGLE,
    AREA_BUTT
} RGN_AREA_TYPE_E;


typedef struct hiRGN_QUADRANGLE_S {
    HI_BOOL bSolid;     /* whether solid or dashed quadrangle */
    HI_U32 u32Thick;    /* Line Width of quadrangle, valid when dashed quadrangle */
    POINT_S stPoint[4]; /* points of quadrilateral */
} RGN_QUADRANGLE_S;

typedef struct hiCOVER_CHN_ATTR_S {
    RGN_AREA_TYPE_E enCoverType; /* rect or arbitary quadrilateral COVER */
    union {
        RECT_S stRect;                 /* config of rect */
        RGN_QUADRANGLE_S stQuadRangle; /* config of arbitary quadrilateral COVER */
    };
    HI_U32 u32Color;
    HI_U32 u32Layer;               /* COVER region layer */
    //RGN_COORDINATE_E enCoordinate; /* ratio coordiante or abs coordinate */
} COVER_CHN_ATTR_S;

typedef union hiRGN_ATTR_U {
    OVERLAY_ATTR_S stOverlay;     /* attribute of overlay region */
} RGN_ATTR_U;

typedef union hiRGN_CHN_ATTR_U {
    OVERLAY_CHN_ATTR_S stOverlayChn;     /* attribute of overlay region */
    COVER_CHN_ATTR_S stCoverChn;         /* attribute of cover region */
} RGN_CHN_ATTR_U;

/* attribute of a region */
typedef struct hiRGN_ATTR_S {
    RGN_TYPE_E enType; /* region type */
    RGN_ATTR_U unAttr; /* region attribute */
} RGN_ATTR_S;

/* attribute of a region */
typedef struct hiRGN_CHN_ATTR_S {
    HI_BOOL bShow;
    RGN_TYPE_E enType;        /* region type */
    RGN_CHN_ATTR_U unChnAttr; /* region attribute */
} RGN_CHN_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __HI_COMM_REGION_H__ */
