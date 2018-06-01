#ifndef _MACRO_H_
#define _MACRO_H_


//宏展开注意: 将宏变量使用()包起来
#define SUM(x, y)      x + y         //warning: SUM(1,2) * SUM(2,3)  -->  1 + 2 * 2 + 3 

#define SUM(x, y)     (x + y)        //warning: SUM(1 || 2, 3)  -->  1 || 2 + 3  -->  1 || (2 + 3)

#define SUM(x, y)    ((x) + (y))     //ok.



//宏计算结构体成员变量偏移  
#define OFFSET(type, var)      ((unsigned int)&((type *)0)->var)

//宏计算结构体成员变量大小
#define OFFSIZE(type, var)     sizeof(((type *)0)->var)



//变量结构体内存对齐 （默认对齐：M字节）
#define ALLIGN(x, M)           ((x + M - 1) & (~(M - 1)))    



// #  ##  #@    [转变字符串、转变字符、拼接字符串]
#define TOSTRING(x)      #x
#define TOCHAR(x)        #@x
#define LINKSTRING(x,y)  x##y


#define TOSTR2(x)        #x
#define TOSTR(x)         TOSTR2(x)   //两次宏跳转

#define SELF_MSG(msg)    message(__FILE__ "(" TOSTR(__LINE__) "):" msg)   // #msg
#pragma SELF_MSG("hello, world!")

#error "error is happened here!"
#error message



//编译器预定义宏
__FILE__       //文件   [字符串]
__LINE__       //行数   [整数]
__TIME__       //时间   [字符串]
__DATE__       //日期   [字符串]
__FUNCTION__   //函数   [字符串]



//C++编译器 兼容 C
#ifdef __cplusplus
extern "C"
{
#endif 
// ... C code ...
#ifdef __cplusplus
}
#endif 



//DLL api 导入导出
#ifdef DLLEXPORT 
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif 



#endif //_MACRO_H_