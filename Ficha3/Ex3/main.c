#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "SO2_F3_DLL.h"

int _tmain(int argc, TCHAR* argv[]) {
	double aux;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif // UNICODE

	do {
		_tprintf(TEXT("Indique um valor a inserir na variável da DLL"));
		_tscanf_s(TEXT("%lf"), &aux);
		if (aux == -1)
			break;
		_tprintf(TEXT("\nO valor da var DLL (antes) : [%lf]"), factor);
		factor = aux;
		_tprintf(TEXT("\nO valor da var DLL (depois) : [%lf]"), factor);
		_tprintf(TEXT("\nIntroduza o valor a enviar por paramentro\n"));
		_tscanf_s(TEXT("%lf"), &aux);
		_tprintf(TEXT("\nFunção factorial da DLL é: [%lf]\n"), applyFactor(aux));
	} while (1);

	return 0;
}