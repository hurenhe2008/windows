#ifndef _PICTURE_ZOOM_H_
#define _PICTURE_ZOOM_H_

#include <Windows.h>

/**********************************************************************************
ͼ�������㷨��    (�ۺϿ��Ǻ�ʱ��CPUռ���ʣ��ݾ���ʹ������ڲ�ֵ��˫���Բ�ֵ������)
>. ����ڲ�ֵ��
>. ˫���Բ�ֵ�� (�������Բ�ֵ��)
>. ������ֵ��   (���ξ����ֵ��)
>. MipMap��


����ڲ�ֵ����
    ��Ŀ��ͼ����������ֵ��ΪԴͼ������������ĵ㡣
    Դͼ����(w0 h0), Ŀ��ͼ����(w1 h1)�� �򣺿����fw = float(w0/w1), �߱���fh = float(h0/h1)
    Ŀ��ͼ��(x1, y1)�����ض�ӦԴͼ��(x0, y0), ��x0 = int(x1 * fw), y0 = int(y1 * fh)��(intȡ��)
    
> ˫���Բ�ֵ�� (��ά���Բ�ֵ��)
    Ŀ��ͼ�����´��������ֵ����Դͼ��λ������������2*2����4���ڽ����ص�ֵͨ����Ȩƽ������ó��ġ�
    ˫�����ڲ�ֵ�㷨�Ŵ���ͼ�������ϸߣ������������ֵ�������ĵ������
    Ȼ�����㷨���е�ͨ�˲��������ʣ�ʹ��Ƶ�����������Կ��ܻ�ʹͼ��������һ���̶��ϱ��ģ����


> ������ֵ��   (��ά���Բ�ֵ��)
> MipMap��



�����ṩ��ͼ�������㷨api�У�
windows�Դ���StretchBlt()������ȱ�������ź��λͼ�޷��洢��Ч��һ�㡣
openCV���ṩ��cvResize()�������ṩ��������ڲ�ֵ��˫���Բ�ֵ��������ֵ����
ע��
BOOL StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, 
    HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)��
void cvResize( const CvArr* src, CvArr* dst, int interpolation);

�ο��ĵ���
http://blog.csdn.net/liyuan02/article/details/6765442
***********************************************************************************/


//����ڲ�ֵ��
bool pic_zoom_near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex1(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex2(BITMAP &dst, const BITMAP &src);
bool pic_zoom_near_ex3(BITMAP &dst, const BITMAP &src);

//�������Բ�ֵ��
bool pic_zoom_2near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_2near_ex1(BITMAP &dst, const BITMAP &src);
bool pic_zoom_2near_ex2(BITMAP &dst, const BITMAP &src);

//���ξ����ֵ��
bool pic_zoom_3near(BITMAP &dst, const BITMAP &src);
bool pic_zoom_3nearex(BITMAP &dst, const BITMAP &src);


#endif //_PICTURE_ZOOM_H_
