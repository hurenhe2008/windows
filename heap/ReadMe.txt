��������ѵĺô���
. ��������б��� (��ͬ�ṹ���ڴ�ֿ���������)
. ����Ч���ڴ���� (��ͬ�ṹ���ͷ�ʱ���µ��ڴ���Ƭ)
. �ֲ����� (���ʽṹ�����ڽڵ�ʱ���ڴ���������ֹ�ڴ治�������µ�ȱҳ�ж�)
. �����߳�ͬ���Ŀ��� (�ѷ���ʱĬ������»ụ����� ˳�����)
. �����ͷ� (�ͷ��ڴ�ʱ������һ���ͷ������ѣ�������Ҫһ�����ڵ��ͷ�)


�������windows api��

--> HANDLE GetProcessHeap();   
	��ȡ����Ĭ�϶Ѿ��
	
--> DWORD GetProcessHeaps(
		DWORD dwNumHeaps;   //�������С
		PHANDLE pheaps;     //�������ַ
	);
	��ȡ�������ж�(����Ĭ�϶�)�����ؽ�����ʵ�����жѵĸ���
	
--> HANDLE HeapCreate(
		DWORD fdwOptions;
		SIZE_T dwInitialSize;
		SIZE_T dwMaximumSize;
	);
	�����ѣ� fdwOptions: 0 ��HEAP_NO_SERIALIZE(�̲߳���ȫ��������) ��HEAP_GENERATE_EXCEPTIONS(�����׳��쳣) ��HEAP_CREATE_ENABLE_EXECUTE
			 dwInitialSize: �����ѵĳ�ʼ��С��Ĭ�϶�Ĭ��Ϊ1M.
			 dwMaximumSize: �����ѵ�����С�����Ϊ0 �������ƣ��������洢���ѷ��������޷��ٷ��䡣
			 
--> PVOID HeapAlloc(
		HANDLE hHeap,
		DWORD fdwFlags,
		SIZE_T dwBytes
	);
	������ڴ棬 hHeap: �Ѿ������HeapCreate���ء�
				 fdwFlags: HEAP_ZERO_MEMORY(�ڴ�����)��HEAP_GENERATE_EXCEPTIONS�� HEAP_NO_SERIALIZE
				 dwBytes: �����С
	ע�� ����������ڴ�(>=1MB��ʱ��Ӧ�ñ���ʹ�öѺ���������ʹ��VirtualAlloc������
	
--> PVOID HeapReAlloc(
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
		SIZE_T dwBytes
	);
	���·����ڴ棬 hHeap: �Ѿ������HeapCreate���ء�
				   fdwFlags: HEAP_ZERO_MEMORY(�ڴ�����)��HEAP_GENERATE_EXCEPTIONS�� HEAP_NO_SERIALIZE��
				             HEAP_REALLOC_IN_PLACE_ONLY(֪ͨ���ƶ��ڴ��ַ����������ƶ������µ�ַ)
				   pvMem: ��Ҫ�������ڴ���ַ��
				   dwBytes: �µ��ڴ���С��
				   
--> SIZE_T HeapSize(	
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
	);
	��ȡ�ڴ��Ĵ�С�� hHeap: �Ѿ������HeapCreate���ء�
					   fdwFlags: 0 ��HEAP_NO_SERIALIZE
					   pvMem: ��Ҫ��ȡ��С���ڴ���ַ��
					   
--> BOOL HeapFree(
		HANDLE hHeap,
		DWORD fdwFlags,
		PVOID pvMem,
	);
	�ͷ��ڴ�飬 hHeap: �Ѿ������HeapCreate���ء�
	             fdwFlags: 0 ��HEAP_NO_SERIALIZE
				 pvMem: ��Ҫ�ͷŵ��ڴ���ַ��
				 
--> BOOL HeapDestroy(
		HANDLE hHeap
	);
	���ٶѣ� hHeap: �Ѿ������HeapCreate���ء�
	ע�� ������ǲ��ڽ�����ֹǰ��ʾ�������ٶѣ� ϵͳ���ڽ�����ֹʱ�����ٶѡ�
		 �̴߳����ѣ��߳��˳�ʱ�����ɴ��ڣ������߳��޹ء�
		 ϵͳ�������������ٽ��̵�Ĭ�϶ѣ��������HeapDestroy(GetProcessHeap()), ϵͳ��ֱ�Ӻ��Բ�����FALSE.
		 
--> BOOL HeapValidate(
		HANDLE hHeap,
		DWORD dwFlags,
		LPVOID pvMem
	);
	���pvMem�ڴ��������ԣ� hHeap: �Ѿ������HeapCreate���ء�
							  dwFlags�� 0 ��HEAP_NO_SERIALIZE
							  pvMem: ��Ҫ�ͷŵ��ڴ���ַ��
	ע�����pvMemΪNULL��������������еĸ����ڴ�飬ȷ��û���κ�һ���ڴ汻�ƻ���
	    ����pvMem���������ڴ���ַ��ֻ���һ���ڴ��������ԡ�
	
--> UINT HeapCompact(
		HANDLE hHeap,
		DWORD dwFlags
	);
	���������õ��ڴ�����½��ƴ����һ�𣬲��������θ����������ڴ�������洢����
		hHeap: �Ѿ������HeapCreate���ء�
		dwFlags�� 0 ��HEAP_NO_SERIALIZE
		
--> BOOL HeapLock(HANDLE hHeap)
	BOOL HeapUnlock(HANDLE hHeap)
	�߳�ͬ�����ʡ�
	Ϊ��ȷ���Զѵķ��������ν��еģ�����HeapAlloc , HeapSize, HeapFree֮��ĺ����ڲ������HeapLock��HeapUnlock��һ�㲻��Ҫ�������á�
	
--> BOOL HeapWalk(
		HANDLE hHeap,
		PPROCESS_HEAP_ENTRY pHeapEntry
	);
	���Ա����ѵ����ݣ� ��Ҫ��ε��øú�����������ÿ�ε���ʱ������䲢��ʼ��PPROCESS_HEAP_ENTRY�ṹ�塣
	ע�� ��HeapWalk����FALSEʱ����ʾ�����Ѿ�û�и�����ڴ���Ա���(��������β��).
		 ��ѭ��HeapWalk�������ڴ��ʱ�� HeapWalkѭ��ǰ��������HeapLock��HeapUnlock��֤ͬһʱ��ֻ��һ���̷߳��ʣ���ֹ�����̷߳�����ͷ��ڴ�顣
	
	
	