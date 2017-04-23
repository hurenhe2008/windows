#include "picture_zoom.h"
#include <crtdbg.h>

#define BITS_IN_BYTE  8

typedef unsigned char uint8_t;

//32 bit color
typedef struct rgba32_s { 
    uint8_t b;       //blue
    uint8_t g;       //green
    uint8_t r;       //red
    uint8_t a;       //alpha
} rgba32_t;
#define BITMAP_BITS   (sizeof(rgba32_t) * BITS_IN_BYTE)

//pixel(x,y)'s rgba information in BITMAP.
static inline rgba32_t& pixel(const BITMAP &pic, const long x, const long y)
{
    return ((rgba32_t *)((uint8_t *)pic.bmBits + pic.bmWidthBytes * y))[x];
}


bool pic_zoom_near(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel, 
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel, 
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight, 
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight, 
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel || 
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight || 
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    long srcx = 0, srcy = 0;

    for (long x = 0; x < dst.bmWidth; ++x) {
        for (long y = 0; y < dst.bmHeight; ++y) {
            srcx = (x * src.bmWidth / dst.bmWidth);
            srcy = (y * src.bmHeight / dst.bmHeight);
            pixel(dst, x, y) = pixel(src, srcx, srcy);
        }
    }

    return true;
}

bool pic_zoom_near_ex1(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    long srcx = 0, srcy = 0;

    /* with > height, reduce for times. */
    for (long y = 0; y < dst.bmHeight; ++y) {
        for (long x = 0; x < dst.bmWidth; ++x) {
            srcx = (x * src.bmWidth / dst.bmWidth);
            srcy = (y * src.bmHeight / dst.bmHeight);
            pixel(dst, x, y) = pixel(src, srcx, srcy);
        }
    }

    return true;
}

bool pic_zoom_near_ex2(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    if (src.bmWidth > 65536 || src.bmHeight > 65536 ||
        dst.bmWidth > 65536 || dst.bmHeight > 65536) {
        return pic_zoom_near_ex1(dst, src);
    }

    //定点数 优化 除法运算  (最大图片尺寸: 65536 * 65536)
    //16.16格式定点数
    unsigned long xr_16 = (src.bmWidth << 16) / dst.bmWidth + 1; 
    unsigned long yr_16 = (src.bmHeight << 16) / dst.bmHeight + 1; 
    //可证明: (Dst.width - 1) * xr_16 < Src.width 成立

    unsigned long srcx = 0, srcy = 0;

    for (unsigned long y = 0; y < dst.bmHeight; ++y) {
        for (unsigned long x = 0; x<dst.bmWidth; ++x) {
            srcx = (x * xr_16) >> 16;
            srcy = (y * yr_16) >> 16;
            pixel(dst, x, y) = pixel(src, srcx, srcy);
        }
    }

    return true;
}

bool pic_zoom_near_ex3(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    if (src.bmWidth > 65536 || src.bmHeight > 65536 ||
        dst.bmWidth > 65536 || dst.bmHeight > 65536) {
        return pic_zoom_near_ex1(dst, src);
    }

    //定点数 优化 除法运算  (最大图片尺寸: 65536 * 65536)
    //16.16格式定点数
    unsigned long xr_16 = (src.bmWidth << 16) / dst.bmWidth + 1;
    unsigned long yr_16 = (src.bmHeight << 16) / dst.bmHeight + 1;
    //可证明: (Dst.width - 1) * xr_16 < Src.width 成立

    rgba32_t* pdstline = (rgba32_t *)dst.bmBits;
    unsigned long srcy_16 = 0, srcx_16 = 0;

    // 在x的循环中y一直不变，那么可以提前计算与y相关的值;
    // srcy的值和x变量无关，可以提前到x轴循环之前；
    // 展开Pixels函数，优化与y相关的指针计算
    for (unsigned long y = 0; y < dst.bmHeight; ++y) {

        rgba32_t* psrcline = (rgba32_t *)((uint8_t *)src.bmBits + 
            src.bmWidthBytes * (srcy_16 >> 16));

        srcx_16 = 0;
        for (unsigned long x = 0; x < dst.bmWidth; ++x) {
            pdstline[x] = psrcline[srcx_16 >> 16];
            srcx_16 += xr_16;
        }

        srcy_16 += yr_16;

        ((uint8_t *&)pdstline) += dst.bmWidthBytes;
    }

    return true;
}



//访问一个点(x,y)坐标可能超出图片边界
//边界处理模式: 边界饱和
static inline rgba32_t pixel_bound(const BITMAP& pic, long x, long y)
{
    bool binpic = true;

    if (x < 0) { 
        x = 0;  binpic = false;
    }
    else if (x >= pic.bmWidth) { 
        x = pic.bmWidth - 1;  binpic = false;
    }

    if (y < 0) { 
        y = 0;  binpic = false;
    }
    else if (y >= pic.bmHeight) {
        y = pic.bmHeight - 1;  binpic = false;
    }

    rgba32_t result = pixel(pic, x, y); 
    if (!binpic) result.a = 0; //alpha全透明

    return result;
}

static inline void calc_pixel(const BITMAP &srcpic, float fx, float fy, rgba32_t *dstpixel)
{
    long x = (long)fx; if (x > fx) --x;   //x = floor(fx);
    long y = (long)fy; if (y > fy) --y;   //y = floor(fy);

    //获取源图像在(fx, fy)所在区域的相邻4个像素点
    rgba32_t rgba0 = pixel_bound(srcpic, x, y);   
    rgba32_t rgba1 = pixel_bound(srcpic, x, y + 1);
    rgba32_t rgba2 = pixel_bound(srcpic, x + 1, y);
    rgba32_t rgba3 = pixel_bound(srcpic, x + 1, y + 1);

    float u = fx - x,  v = fy - y;
    float pm3 = u * v;
    float pm2 = u * (1 - v);
    float pm1 = v * (1 - u);
    float pm0 = (1 - u) * (1 - v);

    dstpixel->a = (pm0 * rgba0.a + pm1 * rgba1.a + pm2 * rgba2.a + pm3 * rgba3.a);
    dstpixel->r = (pm0 * rgba0.r + pm1 * rgba1.r + pm2 * rgba2.r + pm3 * rgba3.r);
    dstpixel->g = (pm0 * rgba0.g + pm1 * rgba1.g + pm2 * rgba2.g + pm3 * rgba3.g);
    dstpixel->b = (pm0 * rgba0.b + pm1 * rgba1.b + pm2 * rgba2.b + pm3 * rgba3.b);
}

bool pic_zoom_2near(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    rgba32_t *pdstline = (rgba32_t *)dst.bmBits;
    float srcy = 0.0, srcx = 0.0;

    for (unsigned long y = 0; y < dst.bmHeight; ++y) {

        srcy = (y + 0.4999999) * src.bmHeight / dst.bmHeight - 0.5;

        for (unsigned long x = 0; x < dst.bmWidth; ++x) {
            srcx = (x + 0.4999999) * src.bmWidth / dst.bmWidth - 0.5;
            calc_pixel(src, srcx, srcy, &pdstline[x]);
        }

        pdstline = (rgba32_t *)((uint8_t *)pdstline + dst.bmWidthBytes);
    }

    return true;
}

static inline void calc_pixel_ex1(const BITMAP& pic, const long x_16, const long y_16, rgba32_t *dstpixel)
{
    long x = x_16 >> 16;
    long y = y_16 >> 16;
    rgba32_t rgba0 = pixel_bound(pic, x, y);
    rgba32_t rgba1 = pixel_bound(pic, x, y + 1);
    rgba32_t rgba2 = pixel_bound(pic, x + 1, y);
    rgba32_t rgba3 = pixel_bound(pic, x + 1, y + 1);

    unsigned long u_8 = (x_16 & 0xFFFF) >> 8;
    unsigned long v_8 = (y_16 & 0xFFFF) >> 8;
    unsigned long pm3_16 = (u_8 * v_8);
    unsigned long pm2_16 = (u_8 * (unsigned long)(256 - v_8));
    unsigned long pm1_16 = (v_8 * (unsigned long)(256 - u_8));
    unsigned long pm0_16 = ((256 - u_8) * (256 - v_8));

    dstpixel->a = ((pm0_16 * rgba0.a + pm1_16 * rgba1.a + pm2_16 * rgba2.a + pm3_16 * rgba3.a) >> 16);
    dstpixel->r = ((pm0_16 * rgba0.r + pm1_16 * rgba1.r + pm2_16 * rgba2.r + pm3_16 * rgba3.r) >> 16);
    dstpixel->g = ((pm0_16 * rgba0.g + pm1_16 * rgba1.g + pm2_16 * rgba2.g + pm3_16 * rgba3.g) >> 16);
    dstpixel->b = ((pm0_16 * rgba0.b + pm1_16 * rgba1.b + pm2_16 * rgba2.b + pm3_16 * rgba3.b) >> 16);
}

bool pic_zoom_2near_ex1(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    //定点数 代替 除法
    long xr_16 = ((src.bmWidth) << 16) / dst.bmWidth + 1;
    long yr_16 = ((src.bmHeight) << 16) / dst.bmHeight + 1;

    const long csDErrorX = -(1 << 15) + (xr_16 >> 1);
    const long csDErrorY = -(1 << 15) + (yr_16 >> 1);

    rgba32_t *pdstline = (rgba32_t *)dst.bmBits;
    long srcy_16 = csDErrorY, srcx_16 = 0;

    for (long y = 0; y < dst.bmHeight; ++y) {
        srcx_16 = csDErrorX;

        for (unsigned long x = 0; x < dst.bmWidth; ++x) {
            calc_pixel_ex1(src, srcx_16, srcy_16, &pdstline[x]);  
            srcx_16 += xr_16;
        }

        srcy_16 += yr_16;
        pdstline = (rgba32_t *)((uint8_t *)pdstline + dst.bmWidthBytes);
    }

    return true;
}

//边界区域分开处理，此处处理内部区域点
static inline void calc_pixel_ex2(const BITMAP &src, const long x_16, const long y_16, rgba32_t *dstpixel)
{
    long x = (x_16 >> 16);
    long y = (y_16 >> 16);

    unsigned long u_16 = ((unsigned short)(x_16));
    unsigned long v_16 = ((unsigned short)(y_16));

    unsigned long u_8 = u_16 >> 8;
    unsigned long v_8 = v_16 >> 8;
    unsigned  long  pm3_16 = u_8 * v_8;
    unsigned  long  pm2_16 = (u_8 << 8) - pm3_16;
    unsigned  long  pm1_16 = (v_8 << 8) - pm3_16;
    unsigned  long  pm0_16 = (1 << 16) - pm1_16 - pm2_16 - pm3_16;

    rgba32_t rgba0 = pixel_bound(src, x, y);
    rgba32_t rgba1 = pixel_bound(src, x + 1, y);
    rgba32_t rgba2 = pixel_bound(src, x, y + 1);
    rgba32_t rgba3 = pixel_bound(src, x + 1, y + 1);

    dstpixel->a = ((pm0_16 * rgba0.a + pm2_16 * rgba1.a + pm1_16 * rgba2.a + pm3_16 * rgba3.a) >> 16);
    dstpixel->r = ((pm0_16 * rgba0.r + pm2_16 * rgba1.r + pm1_16 * rgba2.r + pm3_16 * rgba3.r) >> 16);
    dstpixel->g = ((pm0_16 * rgba0.g + pm2_16 * rgba1.g + pm1_16 * rgba2.g + pm3_16 * rgba3.g) >> 16);
    dstpixel->b = ((pm0_16 * rgba0.b + pm2_16 * rgba1.b + pm1_16 * rgba2.b + pm3_16 * rgba3.b) >> 16);
}

bool pic_zoom_2near_ex2(BITMAP &dst, const BITMAP &src)
{
    /* 32 = BITMAP_BITS */
    _ASSERT_EXPR(BITMAP_BITS != src.bmBitsPixel,
        "src bitmap not 32 bits(b8g8r8a8)");
    _ASSERT_EXPR(BITMAP_BITS != dst.bmBitsPixel,
        "dst bitmap not 32 bits(b8g8r8a8)");

    _ASSERT_EXPR(!src.bmWidth || !src.bmHeight,
        "src bitmap width or height 0");
    _ASSERT_EXPR(!dst.bmWidth || !dst.bmHeight,
        "dst bitmap width or height 0");

    if (BITMAP_BITS != src.bmBitsPixel ||
        BITMAP_BITS != dst.bmBitsPixel ||
        !src.bmWidth || !src.bmHeight ||
        !dst.bmWidth || !dst.bmHeight) {
        return false;
    }

    long xrIntFloat_16 = ((src.bmWidth) << 16) / dst.bmWidth + 1;
    long yrIntFloat_16 = ((src.bmHeight) << 16) / dst.bmHeight + 1;

    const long csDErrorX = -(1 << 15) + (xrIntFloat_16 >> 1);
    const long csDErrorY = -(1 << 15) + (yrIntFloat_16 >> 1);

    //计算出需要特殊处理的边界 
    //y0+y*yr>=0; y0=csDErrorY => y>=-csDErrorY/yr
    long  border_y0 = -csDErrorY / yrIntFloat_16 + 1;    
    if (border_y0 >= dst.bmHeight) border_y0 = dst.bmHeight;

    long  border_x0 = -csDErrorX / xrIntFloat_16 + 1;
    if (border_x0 >= dst.bmWidth) border_x0 = dst.bmWidth;

    //y0+y*yr<=(height-2) => y<=(height-2-csDErrorY)/yr 
    long  border_y1 = (((src.bmHeight - 2) << 16) - csDErrorY) / yrIntFloat_16 + 1; 
    if (border_y1<border_y0) border_y1 = border_y0;

    long  border_x1 = (((src.bmWidth - 2) << 16) - csDErrorX) / xrIntFloat_16 + 1;
    if (border_x1<border_x0) border_x1 = border_x0;

    rgba32_t *pdstline = (rgba32_t *)dst.bmBits;
    long  srcy_16 = csDErrorY;

    for (long y = 0; y < border_y0; ++y)
    {
        long  srcx_16 = csDErrorX;
        for (unsigned long x = 0; x < dst.bmWidth; ++x)
        {
            calc_pixel_ex2(src, srcx_16, srcy_16, &pdstline[x]);  //border 
            srcx_16 += xrIntFloat_16;
        }
        srcy_16 += yrIntFloat_16;
        ((TUInt8*&)pDstLine) += Dst.byte_width;
    }

    for (long y = border_y0; y<border_y1; ++y)
    {
        long  srcx_16 = csDErrorX;
        for (long x = 0; x<border_x0; ++x)
        {
            calc_pixel_ex2(src, srcx_16, srcy_16, &pdstline[x]); //border 
            srcx_16 += xrIntFloat_16;
        }

        {
            unsigned  long  v_8 = (srcy_16 & 0xFFFF) >> 8;
            TARGB32* PSrcLineColor = (TARGB32*)((TUInt8*)(Src.pdata) + src.bmWidthBytes * (srcy_16 >> 16));
            for (unsigned long x = border_x0; x<border_x1; ++x)
            {
                TARGB32* PColor0 = &PSrcLineColor[srcx_16 >> 16];
                TARGB32* PColor1 = (TARGB32*)((TUInt8*)(PColor0)+ src.bmWidthBytes);
                Bilinear2_Fast(PColor0, PColor1, (srcx_16 & 0xFFFF) >> 8, v_8, &pdstline[x]);
                srcx_16 += xrIntFloat_16;
            }
        }

        for (long x = border_x1; x < dst.bmWidth; ++x)
        {
            Bilinear2_Border(Src, srcx_16, srcy_16, &pdstline[x]); //border 
            srcx_16 += xrIntFloat_16;
        }
        srcy_16 += yrIntFloat_16;
        pdstline = (rgba32_t *)((uint8_t *)pdstline + dst.bmWidthBytes);
    }

    for (long y = border_y1; y < Dst.height; ++y)
    {
        long  srcx_16 = csDErrorX;
        for (unsigned long x = 0; x < dst.bmWidth; ++x)
        {
            calc_pixel_ex2(Src, srcx_16, srcy_16, &pdstline[x]);  //border 
            srcx_16 += xrIntFloat_16;
        }
        srcy_16 += yrIntFloat_16;
        pdstline = (rgba32_t *)((uint8_t *)pdstline + dst.bmWidthBytes);
    }


    return true;
}

bool pic_zoom_3near(BITMAP &dst, const BITMAP &src)
{

}

bool pic_zoom_3nearex(BITMAP &dst, const BITMAP &src)
{

}