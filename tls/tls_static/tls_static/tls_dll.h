#ifndef _TLS_DLL_H_
#define _TLS_DLL_H_

#define TEST_NOTHING             0
#define TEST_STATIC_IMPORT_DLL   1
#define TEST_DYNAMIC_IMPORT_DLL  2

#define TEST_DLL  TEST_DYNAMIC_IMPORT_DLL

void test_dll_tls_api();

#endif //_TLS_DLL_H_
