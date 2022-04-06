#pragma once

#include "skeleton.h"
#include "rpcriter.h"
#include "rperror.h"
#include "rphanim.h"
#include "rpmatfx.h"
#include "rpskin.h"
#include "rpuvanim.h"
#include "rpworld.h"
#include "rtanim.h"
#include "rtdict.h"
#include "rtquat.h"
#include "rwcore.h"
#include "rwplcore.h"

#define RpGetFrame(__c) ((RwFrame *)(((RwObject *)(__c))->parent))
#define RpSetFrame(__c, __f) ((((RwObject *)(__c))->parent) = (void *)(__f))

extern RwCamera* 	(*RwCameraCreate)(void);
extern RwCamera* 	(*RwCameraBeginUpdate)(RwCamera* camera);
extern RwCamera* 	(*RwCameraEndUpdate)(RwCamera* camera);
extern RwCamera* 	(*RwCameraShowRaster)(RwCamera * camera, void *pDev, RwUInt32 flags);
extern RwCamera* 	(*RwCameraClear)(RwCamera * camera, RwUInt32 flags, RwInt32 flags2);
extern RwCamera* 	(*RwCameraSetFarClipPlane)(RwCamera * camera, float value);
extern RwCamera* 	(*RwCameraSetNearClipPlane)(RwCamera * camera, float value);
extern RwCamera* 	(*RwCameraSetViewWindow)(RwCamera * camera, float *value);
extern RwCamera* 	(*RwCameraSetProjection)(RwCamera * camera, int value);

extern RwRaster* 	(*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
extern RwBool 		(*RwRasterDestroy)(RwRaster * raster);
extern RwRaster* 	(*RwRasterGetOffset)(RwRaster *raster, RwInt16 *xOffset, RwInt16 *yOffset);
extern RwInt32 	 	(*RwRasterGetNumLevels)(RwRaster * raster);
extern RwRaster* 	(*RwRasterSubRaster)(RwRaster * subRaster, RwRaster * raster, RwRect * rect);
extern RwRaster* 	(*RwRasterRenderFast)(RwRaster * raster, RwInt32 x, RwInt32 y);
extern RwRaster* 	(*RwRasterRender)(RwRaster * raster, RwInt32 x, RwInt32 y);
extern RwRaster* 	(*RwRasterRenderScaled)(RwRaster * raster, RwRect * rect);
extern RwRaster* 	(*RwRasterPushContext)(RwRaster * raster);
extern RwRaster* 	(*RwRasterPopContext)(void);
extern RwRaster* 	(*RwRasterGetCurrentContext)(void);
extern RwBool 		(*RwRasterClear)(RwInt32 pixelValue);
extern RwBool 		(*RwRasterClearRect)(RwRect * rpRect, RwInt32 pixelValue);
extern RwRaster* 	(*RwRasterShowRaster)(RwRaster * raster, void *dev, RwUInt32 flags);
extern RwUInt8* 	(*RwRasterLock)(RwRaster * raster, RwUInt8 level, RwInt32 lockMode);
extern RwRaster* 	(*RwRasterUnlock)(RwRaster * raster);
extern RwUInt8* 	(*RwRasterLockPalette)(RwRaster * raster, RwInt32 lockMode);
extern RwRaster* 	(*RwRasterUnlockPalette)(RwRaster * raster);
extern RwImage* 	(*RwImageCreate)(RwInt32 width, RwInt32 height, RwInt32 depth);
extern RwBool 		(*RwImageDestroy)(RwImage * image);
extern RwImage* 	(*RwImageAllocatePixels)(RwImage * image);
extern RwImage* 	(*RwImageFreePixels)(RwImage * image);
extern RwImage* 	(*RwImageCopy)(RwImage * destImage, const RwImage * sourceImage);
extern RwImage* 	(*RwImageResize)(RwImage * image, RwInt32 width, RwInt32 height);
extern RwImage* 	(*RwImageApplyMask)(RwImage * image, const RwImage * mask);
extern RwImage* 	(*RwImageMakeMask)(RwImage * image);
extern RwImage* 	(*RwImageReadMaskedImage)(const RwChar * imageName, const RwChar * maskname);
extern RwImage* 	(*RwImageRead)(const RwChar * imageName);
extern RwImage* 	(*RwImageWrite)(RwImage * image, const RwChar * imageName);
extern RwImage* 	(*RwImageSetFromRaster)(RwImage *image, RwRaster *raster);
extern RwRaster* 	(*RwRasterSetFromImage)(RwRaster *raster, RwImage *image);
extern RwRaster* 	(*RwRasterRead)(const RwChar *filename);
extern RwRaster* 	(*RwRasterReadMaskedRaster)(const RwChar *filename, const RwChar *maskname);
extern RwImage* 	(*RwImageFindRasterFormat)(RwImage *ipImage, RwInt32 nRasterType, RwInt32 *npWidth, RwInt32 *npHeight, RwInt32 *npDepth,RwInt32 *npFormat);

/* rwlpcore.h */
extern RwReal 		(*RwIm2DGetNearScreenZ)(void);
extern RwReal 		(*RwIm2DGetFarScreenZ)(void);
extern RwBool 		(*RwRenderStateGet)(RwRenderState state, void *value);
extern RwBool 		(*RwRenderStateSet)(RwRenderState state, void *value);
extern RwBool 		(*RwIm2DRenderLine)(RwIm2DVertex *vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);
extern RwBool 		(*RwIm2DRenderTriangle)(RwIm2DVertex *vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2, RwInt32 vert3 );
extern RwBool 		(*RwIm2DRenderPrimitive)(RwPrimitiveType primType, RwIm2DVertex *vertices, RwInt32 numVertices);
extern RwBool 		(*RwIm2DRenderIndexedPrimitive)(RwPrimitiveType primType, RwIm2DVertex *vertices, RwInt32 numVertices, RwImVertexIndex *indices, RwInt32 numIndices);

/* rtpng.h */
extern RwImage* 	(*RtPNGImageWrite)(RwImage* image, const RwChar* imageName);
extern RwImage* 	(*RtPNGImageRead)(const RwChar* imageName);

extern RwTexture* 	(*RwTextureCreate)(RwRaster *rwRaster);
extern RwTexture* 	(*RwTextureRead)(const RwChar *filename, const RwChar *imageName);
extern RwTexture* 	(*RwTextureDestroy)(RwTexture *rwTexture);

extern RwTexture* 	(*RpMaterialSetTexture)(RpMaterial *rpMat, RwTexture *rwTexture);

extern RsGlobalType* RsGlobal;