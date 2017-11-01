#ifndef _SINGLETON_NEW_H_
#define _SINGLETON_NEW_H_

//-------------------------------------------------------------------------------
class Singleton
{
public:
    static Singleton* Instance()    // --> you can delete the point from Instance() outside if you want.
    {
        static Singleton sl;
        return &sl;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton &rhs);              //just declaration, not define
    Singleton& operator=(const Singleton &rhs);   //just declaration, not define
};


//-------------------------------------------------------------------------------
class SingletonX
{
public:
    static SingletonX& Instance()    // --> not always thread safe. if two or more thread call Instance under multi-cpu-core.
    {
        static SingletonX sl;   
        return sl;
    }

protected:
    SingletonX() {}
    virtual ~SingletonX() {}

private:
    SingletonX(const SingletonX &rhs) = delete;             //declaration and no warning with using delete.
    SingletonX& operator=(const SingletonX &rhs) = delete;  //declaration and no warning with using delete.
    SingletonX* operator&() = delete;     // --> forbidden to get use operator & to get SingletonX's address.
};


//-------------------------------------------------------------------------------
class SingletonT
{
public:
    static SingletonT& Instance()   
    {
        if (!m_pIns)
        {
            m_mutex.lock();
            //double check, why ???   
            //-->  one thread lock and new assign m_pIns, when other thread obtain the mutex, if not check m_pIns will memory leak.
            if (!m_pIns)    
            {
                SingletonT *tmp = new SingletonT();

                //init.

                m_pIns = tmp;
                //why not m_pIns = new SingletonT();   
                //--> think about new Class and assingn to assemble code.  1: new size, 2: call constructor, 3: assign   or  2: assingn  3: call constructor.
            }
            m_mutex.unlock;
        }
        return *m_pIns;
    }

protected:
    SingletonT() {}
    virtual ~SingletonT() {}

private:
    SingletonT(const SingletonT &rhs) = delete;             
    SingletonT& operator=(const SingletonT &rhs) = delete;  
    SingletonT* operator&() = delete;    

private:
    static SingletonT *m_pIns;
    static Mutex m_mutex;   
};
SingletonT* SingletonT::m_pIns = nullptr;
Mutex SingletonT::m_mutex;


//-------------------------------------------------------------------------------
//use method:  class Test : public SingletonTmpl<Test>
template <typename TT>
class SingletonTmpl
{
public:
    static TT& Instance()
    {
        if (!m_pIns)
        {
            m_mutex.lock();
            if (!m_pIns)
            {
                TT *tmp = new TT();
                //something init.
                m_pIns = tmp;
            }
            m_mutex.unlock;
        }
        return *m_pIns;
    }

protected:
    SingletonT() {}
    virtual ~SingletonT() {}

private:
    SingletonT(const SingletonT &rhs) = delete;
    SingletonT& operator=(const SingletonT &rhs) = delete;
    SingletonT* operator&() = delete;

private:
    static TT *m_pIns;
    static Mutex m_mutex;
};
template <typename TT>
TT* SingletonT<TT>::m_pIns = nullptr;

template <typename TT>
Mutex SingletonT<TT>::m_mutex;


//-------------------------------------------------------------------------------
//use method:  SingletonTmpl<Test>   (class Test)
template <typename TT>
class SingletonTmpl : public TT
{
public:
    static TT& Instance()
    {
        if (!m_pIns)
        {
            m_mutex.lock();
            if (!m_pIns)
            {
                TT *tmp = new TT();
                //something init.
                m_pIns = tmp;
            }
            m_mutex.unlock;
        }
        return *m_pIns;
    }

protected:
    SingletonT() {}
    virtual ~SingletonT() {}

private:
    SingletonT(const SingletonT &rhs) = delete;
    SingletonT& operator=(const SingletonT &rhs) = delete;
    SingletonT* operator&() = delete;

private:
    static TT *m_pIns;
    static Mutex m_mutex;
};
template <typename TT>
TT* SingletonT<TT>::m_pIns = nullptr;

template <typename TT>
Mutex SingletonT<TT>::m_mutex;

#endif //_SINGLETON_NEW_H_