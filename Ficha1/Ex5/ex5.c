#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#define MAX 256

int _tmain(int argc, LPTSTR argv[]) {
	TCHAR nome[MAX];
	unsigned int a; 

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif // UNICODE

	a = GetModuleFileName(NULL, nome, MAX);
	DWORD error = GetLastError();
	_tprintf(TEXT("[%d] - O meu file é: %s"), a, nome);

	//alinea b)
	

	TCHAR novoFicheiro[MAX];
	do {
		
		_tprintf(TEXT("\nIndique o nome de um ficheiro a executar:"));
		_fgetts(novoFicheiro, MAX, stdin);
		novoFicheiro[_tcslen(novoFicheiro) - 1] = '\0';

		

		if (_tcscmp(TEXT("#fim"), novoFicheiro)) {
			fflush(stdin);
			TCHAR parametro[MAX];
			_tprintf(TEXT("Introduzir parâmetros:"));
			_fgetts(parametro, MAX, stdin);
			parametro[_tcslen(parametro)-1] = '\0';
			_tcscat_s(novoFicheiro, MAX, TEXT(" "));
			_tcscat_s(novoFicheiro, MAX, parametro);

			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);

			if (CreateProcess(
				NULL,
				novoFicheiro,
				NULL,
				NULL,
				0,
				0,
				NULL,
				NULL,
				&si,
				&pi
			) == 0) {
				//erro
				_tprintf(TEXT("bonk"));
			}
		}
		


		
		
	} while (_tcscmp(novoFicheiro,TEXT("#fim"))!=0);
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return(0);
}