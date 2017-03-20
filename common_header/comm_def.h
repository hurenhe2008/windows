#ifndef _COMM_DEF_H_
#define _COMM_DEF_H_

#ifdef _WIN64
    #define WORD_LEN   8
#else /* _WIN32/other */
    #define WORD_LEN   4
#endif 

/* struct address alignment, default with system bits */
#pragma pack(WORD_LEN)

typedef struct structm_s {
    /* members */
} structm_t;

typedef struct structn_s {
    /* members */
} structn_t;

typedef struct structk_s {
    /* members */
} structk_t;

#pragma pack()


typedef enum enumm_e {
    /* member list */
} enumm_t;

typedef enum enumn_e {
    /* member list */
} enumn_t;


#endif //_COMM_DEF_H_
