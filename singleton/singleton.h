#ifndef _SINGLETON_H_
#define _SINGLETON_H_


class Singleton
{
public:
    static Singleton& instance();

    /* not safe-thread, advise not call it if you can't control */
    static void destroy(); 

private:
    Singleton();

    ~Singleton();

    bool init();

private:
    Singleton(const Singleton &) = delete;
    Singleton& operator=(const Singleton &) = delete;
    Singleton* operator&() = delete; /* avoid delete outer */

private:
    static Singleton *mp_self;
};


/* some front endpoint use this singleton */
class SingletonF 
{
public:
    static SingletonF& instance();

    /* must know when and where to call before other threads call instance */
    bool init();

private:
    SingletonF();

    ~SingletonF();

    SingletonF(const SingletonF &) = delete;
    SingletonF& operator=(const SingletonF &) = delete;
    /* avoid delete outer and address access error */
    Singleton* operator&() = delete; 
};


#endif //_SINGLETON_H_
