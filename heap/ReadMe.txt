创建额外堆的好处：
. 对组件进行保护 (不同结构体内存分开，不互踩)
. 更有效的内存管理 (不同结构体释放时导致的内存碎片)
. 局部访问 (访问结构体相邻节点时，内存相连，防止内存不相连导致的缺页中断)
. 避免线程同步的开销 (堆分配时默认情况下会互斥加锁 顺序访问)
. 快速释放 (释放内存时，可以一次释放整个堆，而不需要一个个节点释放)


【堆相关windows api】

--> HANDLE GetProcessHeap();   
	获取进程默认堆句柄
	
--> DWORD GetProcessHeaps(
		DWORD dwNumHeaps;   //堆数组大小
		PHANDLE pheaps;     //堆数组地址
	);
	获取进程所有堆(包括默认堆)，返回进程中实际所有堆的个数
	
--> HANDLE HeapCreate(
		DWORD fdwOptions;
		SIZE_T dwInitialSize;
		SIZE_T dwMaximumSize;
	);
	创建堆， fdwOptions: 0 、HEAP_NO_SERIALIZE(线程不安全，不建议) 、HEAP_GENERATE_EXCEPTIONS(错误抛出异常) 、HEAP_CREATE_ENABLE_EXECUTE
			 dwInitialSize: 创建堆的初始大小，默认堆默认为1M.
			 dwMaximumSize: 创建堆的最大大小，如果为0 则不做限制，如果物理存储器已分配完则无法再分配。
			 
--> PVOID HeapAlloc(
		HANDLE hHeap,
		DWORD fdwFlags,
		SIZE_T dwBytes
	);
	分配堆内存， hHeap: 堆句柄，由HeapCreate返回。
				 fdwFlags: HEAP_ZERO_MEMORY(内存清零)、HEAP_GENERATE_EXCEPTIONS、 HEAP_NO_SERIALIZE
				 dwBytes: 分配大小
	注： 如果分配大块内存(>=1MB）时，应该避免使用堆函数，建议使用VirtualAlloc函数。
	
--> PVOID HeapReAlloc(
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
		SIZE_T dwBytes
	);
	重新分配内存， hHeap: 堆句柄，由HeapCreate返回。
				   fdwFlags: HEAP_ZERO_MEMORY(内存清零)、HEAP_GENERATE_EXCEPTIONS、 HEAP_NO_SERIALIZE、
				             HEAP_REALLOC_IN_PLACE_ONLY(通知不移动内存地址，如果必须移动返回新地址)
				   pvMem: 想要调整的内存块地址。
				   dwBytes: 新的内存块大小。
				   
--> SIZE_T HeapSize(	
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
	);
	获取内存块的大小， hHeap: 堆句柄，由HeapCreate返回。
					   fdwFlags: 0 、HEAP_NO_SERIALIZE
					   pvMem: 想要获取大小的内存块地址。
					   
--> BOOL HeapFree(
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
	);
	释放内存块， hHeap: 堆句柄，由HeapCreate返回。
	             fdwFlags: 0 、HEAP_NO_SERIALIZE
				 pvMem: 需要释放的内存块地址。
				 
--> BOOL HeapDestroy(
		HANDLE hHeap
	);
	销毁堆， hHeap: 堆句柄，由HeapCreate返回。
	注： 如果我们不在进程终止前显示调用销毁堆， 系统会在进程终止时替销毁堆。
		 线程创建堆，线程退出时堆依旧存在，堆与线程无关。
		 系统不允许主动销毁进程的默认堆，如果调用HeapDestroy(GetProcessHeap()), 系统会直接忽略并返回FALSE.
		 
--> BOOL HeapValidate(
		HANDLE hHeap,
		DWORD dwFlags,
		LPVOID pvMem
	);
	检查pvMem内存块的完整性， hHeap: 堆句柄，由HeapCreate返回。
							  dwFlags： 0 、HEAP_NO_SERIALIZE
							  pvMem: 需要释放的内存块地址。
	注：如果pvMem为NULL，则函数会遍历堆中的各个内存块，确保没有任何一块内存被破坏。
	    建议pvMem传入具体的内存块地址，只检查一块内存块的完整性。
	
--> UINT HeapCompact(
		HANDLE hHeap,
		DWORD dwFlags
	);
	将堆中闲置的内存块重新结合拼接在一起，并撤销调拔给堆中闲置内存块的物理存储器。
		hHeap: 堆句柄，由HeapCreate返回。
		dwFlags： 0 、HEAP_NO_SERIALIZE
		
--> BOOL HeapLock(HANDLE hHeap)
	BOOL HeapUnlock(HANDLE hHeap)
	线程同步访问。
	为了确保对堆的访问是依次进行的，诸如HeapAlloc , HeapSize, HeapFree之类的函数内部会调用HeapLock、HeapUnlock，一般不需要主动调用。
	
--> BOOL HeapWalk(
		HANDLE hHeap,
		PPROCESS_HEAP_ENTRY pHeapEntry
	);
	调试遍历堆的内容， 需要多次调用该函数来遍历。每次调用时必须分配并初始化PPROCESS_HEAP_ENTRY结构体。
	注： 当HeapWalk返回FALSE时，表示堆中已经没有更多的内存可以遍历(遍历到结尾处).
		 当循环HeapWalk遍历堆内存块时， HeapWalk循环前后必须调用HeapLock、HeapUnlock保证同一时刻只有一个线程访问，防止其他线程分配或释放内存块。
	
	
	