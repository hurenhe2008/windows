#ifndef __MEMORY_TRACE_H__
#define __MEMORY_TRACE_H__

//#define MEMORY_CRTDBG_DETECT   /* (crtdbg/vld) detect */

/* DEBUG */
#ifdef _DEBUG
	/*crtdbg*/
	#ifdef MEMORY_CRTDBG_DETECT 
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		
		#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#define new DEBUG_NEW
		
		#define MEMORY_TRACE_START   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
		#define MEMORY_TRACE_END   
	/*vld*/
	#else                
		#define VLD_ENABLE_FORCE
		#include "vld.h"
		#define MEMORY_TRACE_START  
		#define MEMORY_TRACE_END     VLDReportLeaks()
	#endif
/* RELEASE */
#else 
	#define MEMORY_TRACE_START
	#define MEMORY_TRACE_END
#endif

#endif   //__MEMORY_TRACE_H__