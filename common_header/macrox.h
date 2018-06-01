#ifndef _MACRO_H_
#define _MACRO_H_


//��չ��ע��: �������ʹ��()������
#define SUM(x, y)      x + y         //warning: SUM(1,2) * SUM(2,3)  -->  1 + 2 * 2 + 3 

#define SUM(x, y)     (x + y)        //warning: SUM(1 || 2, 3)  -->  1 || 2 + 3  -->  1 || (2 + 3)

#define SUM(x, y)    ((x) + (y))     //ok.



//�����ṹ���Ա����ƫ��  
#define OFFSET(type, var)      ((unsigned int)&((type *)0)->var)

//�����ṹ���Ա������С
#define OFFSIZE(type, var)     sizeof(((type *)0)->var)



//�����ṹ���ڴ���� ��Ĭ�϶��룺M�ֽڣ�
#define ALLIGN(x, M)           ((x + M - 1) & (~(M - 1)))    



// #  ##  #@    [ת���ַ�����ת���ַ���ƴ���ַ���]
#define TOSTRING(x)      #x
#define TOCHAR(x)        #@x
#define LINKSTRING(x,y)  x##y


#define TOSTR2(x)        #x
#define TOSTR(x)         TOSTR2(x)   //���κ���ת

#define SELF_MSG(msg)    message(__FILE__ "(" TOSTR(__LINE__) "):" msg)   // #msg
#pragma SELF_MSG("hello, world!")

#error "error is happened here!"
#error message



//������Ԥ�����
__FILE__       //�ļ�   [�ַ���]
__LINE__       //����   [����]
__TIME__       //ʱ��   [�ַ���]
__DATE__       //����   [�ַ���]
__FUNCTION__   //����   [�ַ���]



//C++������ ���� C
#ifdef __cplusplus
extern "C"
{
#endif 
// ... C code ...
#ifdef __cplusplus
}
#endif 



//DLL api ���뵼��
#ifdef DLLEXPORT 
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif 



#endif //_MACRO_H_