#pragma once

#include <windows.h>

#ifdef FICHA3_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

DLL_IMP_API int b;

DLL_IMP_API int soma(int a);