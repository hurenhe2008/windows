
Windows 线程本地存储(TLS):  
	大致分为两种模式：  【静态TLS】 、 【动态TLS】     （强烈简易使用动态TLS）


【静态TLS】 

	格式： 
			__declspec(thread) + 类型 + 全局变量/静态全局变量/静态局部变量

	概念： 
			 __declspec(thread)的前缀 为 Microsoft给Visual C++编译器的一个修改符，
			告诉编译器对应的变量应该放入可执行文件或Dll文件的.tls段中。 
			__declspec(thread)后面修饰的变量必须声明为函数外(或函数中)的全局变量或
			静态变量，不能修饰局部变量，局部变量总是属于特定线程线程栈上变量。

	原理：
			编译器在编译时检测到变量为__declspec(thread)修饰，则会把该变量放入每一个
			obj文件的“.tls”节中，链接器连接所有obj对象文件时会将所有.tls节组合为一个
			大的完整的.tls节存入可执行文件或DLL文件。 为使含有静态TLS的程序能够运行，
			操作系统加载器在加载exe或dll时检测.tls节，并且为.tls所保存的变量动态分配
			足够的内存块来存放.tls节中的静态tls变量。 

			[exe运行中静态tls如何操作？]
			程序运行引用静态tls变量时， 都必须转换为已分配内存块中的相对内存位置。为此，
			编译器需要额外的生成一些辅助代码来引用静态tls变量，这将使你的程序变大和运行较慢。
		    在X86 CPU上，将为每次引用的静态tls变量生成3条辅助机器指令。
			另外，如果exe运行期间创建了另外的线程，访问静态tls变量时，操作系统就要将它捕获
			并自动分配另一个内存块，以便存放新线程的静态tls变量。新线程只拥有对自己的静态
			tls变量的访问权，无法访问其他线程的静态tls变量。

			[dll隐式链接时，dll中包含的静态tls如何操作？]
			操作系统加载器加载exe和相应依赖Module（dll）时，会检测exe和dll的.tls节是否存在变量，
			如果有会将exe和所有dll中.tls节中变量大小相加，分配足够到的内存块存放。
			如果进程中创建另一个线程，系统会自动分配足够大的内存块来存放exe和所有隐式链接dll中
			包含的静态tls变量。

			[dll显示链接时，dll中包含的静态tls如何操作？]
			通过LoadLibray(ex)加载的dll, 线程如何访问显示链接DLL中的静态tls变量呢？ 
			操作系统会查看进程中已经存在的所有线程，并扩大线程访问的TLS内存块，将DLL的静态tls
			变量存入扩大的TLS内存块中。另外，调用FreeLibrary释放Dll时，相应Dll的静态tls变量也
			会在所有线程的TLS内存块中被释放，TLS内存块被压缩。
			注意： 虽然系统允许包含静态tls变量的DLL被显示加载，但其包含的静态tls变量没有被相应
			的初始化，如果试图访问这些静态tls变量，有可能导致访问违规！！！
			请记住： 如果某个DLL包含静态TLS数据，请不要对这个DLL采用显示链接的方式，否则可能会
			出错！！！ 

【动态TLS】

	调用函数：   
			DWORD  TlsAlloc();   
			            //创建所有线程的tls数组可用索引。
			
			BOOL   TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);   
			            //设置tls数组索引dwTlsIndex处保存的值lpTlsValue。

			LPVOID TlsGetValue(DWORD dwTlsIndex);  
			            //返回调用线程TLS数组索引dwTlsIndex处保存的值。

			BOOL   TlsFree(DWORD dwTlsIndex);  
			            //释放所有线程的TLS数组索引dwTlsIndex,并将该位置标记为未使用。  

    原理：
			线程本地存储器位标志范围[0 - (TLS_MINIMUM_AVAILABLE-1)],window会根绝需要动态调整范围。
			默认情况下为TLS_MINIMUM_AVAILABLE(64)个tls位标志。

			#define TLS_MINIMUM_AVAILABLE   64              //默认最小线程本地存储个数
			#define TLS_OUT_OF_INDEXES      0xFFFFFFFF      //异常标志
			                
							           线程TLS内部数据结构：
    		-------------线程1--------------            -------------线程2-------------- 
		   |          TLS slot数组          |          |          TLS slot数组          |
		   |         value   index          |          |         value   index          |
		   |          [0]      0            |          |          [0]      0            |
		   |          [0]      1            |          |          [0]      1            |
		   |          [0]      2            |          |          [0]      2            |
		   |          [0]      3            |          |          [0]      3            |
		   |          [0]      4            |          |          [0]      4            |
		   |                .               |          |                .               |
		   |                .               |          |                .               |
		   |                .               |          |                .               |
		   |                                |          |                                |
		   |          [0]      TLS_MIN-2    |          |          [0]      TLS_MIN-2    |
		   |          [0]      TLS_MIN-1    |          |          [0]      TLS_MIN-1    |
		    --------------------------------            --------------------------------

			线程本地存储器的位标志显示了该进程中所有运行的线程正在使用的一组标志。每个标志均可设置
			为 FREE 或 INUSE, 表示TLS插槽(slot)是否正在使用。Microsoft保证至少
			TLS_MINIMUM_AVAILABLE64位标志是可供使用的。

			每一个线程都拥有一个属于自己独立的TLS slot数组， 用于存储TLS数据。

			--》 使用动态TLS，首先调用TlsAlloc()来命令系统对进程的线程本地存储器的位标志进行扫描，
			     找到一个可用的位置，并返回该索引；如果找不到，则返回TLS_OUT_OF_INDEXES。
			     事实上,TlsAlloc还会自动清空所有线程的TLS数组对应索引的值，防止遗留值引起数据异常。
			--》 调用TlsSetValue设置特定索引位置需要保存的值/调用TlsGetValue获取特定索引保存的值。
			     可以通过TlsGetValue()来检测特定索引位置是否保存有效值。
				 在TlsAlloc后直接调用TlsGetValue应该返回NULL.
			--》 使用TlsFree()来释放所有线程在特定索引位置的值，并将进程的位标志数组的index位置
			     设置为FREE状态。 如果成功则返回TRUE，注意，如果试图释放一个没有分配的索引位，
				 将产生一个错误。


【总结】
	动态TLS的使用相对静态TLS稍微麻烦一点，但是无论是将其用在可执行文件中还是DLL中，都还是很简单的。
	而且当用在DLL中时，没有由于DLL显示链接方式而可能产生的访问违规问题。
	所以，如果要在DLL中用TLS，又不能保证客户始终采用隐式链接方式，那么请采用动态TLS的实现。






文章借鉴：
http://blog.csdn.net/xiaoliangsky/article/details/43158713
