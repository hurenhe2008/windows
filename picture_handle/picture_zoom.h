#ifndef _PICTURE_ZOOM_H_
#define _PICTURE_ZOOM_H_

#include <Windows.h>

/**********************************************************************************
图像缩放算法：    (综合考虑耗时、CPU占用率，暂决定使用最近邻插值、双线性插值做尝试)
>. 最近邻插值法
>. 双线性插值法 (二次线性插值法)
>. 立方插值法   (三次卷积插值法)
>. MipMap链


最近邻插值法：
    将目标图像各点的像素值设为源图像中与其最近的点。
    源图像宽高(w0 h0), 目标图像宽高(w1 h1)， 则：宽比例fw = float(w0/w1), 高比例fh = float(h0/h1)
    目标图像(x1, y1)点像素对应源图像(x0, y0), 则：x0 = int(x1 * fw), y0 = int(y1 * fh)。(int取整)
    
> 双线性插值法 (二维线性插值法)
    目标图像中新创造的象素值，由源图像位置在它附近的2*2区域4个邻近象素的值通过加权平均计算得出的。
    双线性内插值算法放大后的图像质量较高，不会出现像素值不连续的的情况。
    然而次算法具有低通滤波器的性质，使高频分量受损，所以可能会使图像轮廓在一定程度上变得模糊。


> 立方插值法   (三维线性插值法)
> MipMap链



现有提供的图像缩放算法api有：
windows自带的StretchBlt()函数，缺点是缩放后的位图无法存储、效果一般。
openCV库提供的cvResize()函数，提供上述最近邻插值、双线性插值、立方插值法。
注：
BOOL StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
    HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)；
void cvResize( const CvArr* src, CvArr* dst, int interpolation);

参考文档：
http://blog.csdn.net/liyuan02/article/details/6765442
***********************************************************************************/


//最近邻插值法
bool pic_zoom_near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex1(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex2(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex3(BITMAP &dst, const BITMAP &src);

//二次线性插值法
bool pic_zoom_2near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_2near_ex1(BITMAP &dst, const BITMAP &src);
bool pic_zoom_2near_ex2(BITMAP &dst, const BITMAP &src);

//三次卷积插值法
bool pic_zoom_3near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_3nearex(BITMAP &dst, const BITMAP &src);


#endif //_PICTURE_ZOOM_H_
