#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "SO2_F3_DLL.h"
typedef double (*PFUNC)(double);

int _tmain(int argc, TCHAR* argv[]) {
	HINSTANCE hLib;
	double* pVar; double aux;
	PFUNC pFunc;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE


	hLib = LoadLibrary(TEXT("SO2_F3_DLL.dll"));
	if (hLib == NULL) {
		_tprintf(TEXT("\nErro ao abrir a DLL"));
		return -1;
	}
	else {
		_tprintf(TEXT("\nAbri a DLL\n"));
		pVar = (double*)GetProcAddress(hLib, "factor");
		pFunc = (PFUNC)GetProcAddress(hLib, "applyFactor");

		do {
			_tprintf(TEXT("Indique um valor a inserir na variável da DLL"));
			_tscanf_s(TEXT("%lf"), &aux);
			if (aux == -1)
				break;
			_tprintf(TEXT("\nO valor da var DLL (antes) : [%lf]"), *pVar);
			*pVar = aux;
			_tprintf(TEXT("\nO valor da var DLL (depois) : [%lf]"), *pVar);
			_tprintf(TEXT("\nIntroduza o valor a enviar por paramentro\n"));
			_tscanf_s(TEXT("%lf"), &aux);
			_tprintf(TEXT("\nFunção factorial da DLL é: [%lf]\n"), pFunc(aux));
		} while (1);

		FreeLibrary(hLib);
		return 0;
	}
}