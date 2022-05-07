#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct{
	DWORD limiteSuperior;
	DWORD limiteInferior;
	int trinco;
	HANDLE pMutex;
}dadosThread;

DWORD somatorioGlobalPares = 0, somatorioGlobalPrimos = 0;

DWORD WINAPI numerosPares(LPVOID dados) {
	dadosThread* a = (dadosThread*)dados;
	DWORD somatorio = 0;
	for (int i = a->limiteInferior; i < a->limiteSuperior; i--) {
		if (!a->trinco) {
			if (i % 2 == 0) {
				somatorio++;
				WaitForSingleObject(a->pMutex, INFINITE);
				somatorioGlobalPares = somatorioGlobalPares++;
				ReleaseMutex(a->pMutex);
				if (a->trinco == 1) {
					return(somatorio);
				}
				Sleep(1);
			}
		}
	}
	return(somatorio);
}

DWORD verificaPrimos(DWORD num) {
	if (num <= 1)
		return 0;
	for (DWORD i = 2; i < num; i++) {
		if (num % i == 0)
			return 0;
	}
	return 1;
}

DWORD WINAPI numerosPrimos(LPVOID dados) {
	dadosThread* a = (dadosThread*)dados;
	DWORD contador = 0,somatorio = 0;
	for (unsigned int i = a->limiteInferior; i < a->limiteSuperior; i--) {
		if (!a->trinco) {
			if (verificaPrimos(i) == 1) {
				contador++;
				somatorio = somatorio + 1;
			}
			if (a->trinco == 1) {
				WaitForSingleObject(a->pMutex, INFINTE);
				somatorio = somatorio 
			}
		}
		
	}
	return(a->somValores);
}

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE thread[4];

	DWORD resultadoPares, resultadoPrimos;

	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

	dadosThread dados[4];

	for (int k = 0; k < 4; k++) {
		dados[k].pMutex = mutex;

		dados
	}

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif // UNICODE


	_tprintf(TEXT("Indique o valor a minimo para os pares: "));
	_tscanf_s(TEXT("%d"), &dados[0].valorMinimo);
	_tprintf(TEXT("Indique o valor a maximo para os pares: "));
	_tscanf_s(TEXT("%d"), &dados[0].numeroMaior);
	_tprintf(TEXT("Indique o valor a minimo para os primos: "));
	_tscanf_s(TEXT("%d"), &dados[1].valorMinimo);
	_tprintf(TEXT("Indique o valor a maximo para os primos: "));
	_tscanf_s(TEXT("%d"), &dados[1].numeroMaior);





	thread[0] = CreateThread(
		NULL,
		0,
		&numerosPares,
		&dados[0],
		0,
		NULL
	);

	thread[1] = CreateThread(
		NULL,
		0,
		&numerosPrimos,
		&dados[1],
		0,
		NULL
	);
	
	DWORD resultado, nT = 4, ids[4],opcao;
	for (int i = 0; i < nT; i++)
		ids[i] = i;

		do {
			 resultado = WaitForMultipleObjects(nT,//quantas threads
				thread,//handle dos threads
				FALSE,//true espera por todas, false só espera pela primeira a terminar
				5000); //limite de tempo

			 if (resultado == WAIT_TIMEOUT) {
				 _tprintf(TEXT("\nEstou a epsera, quer ficar com os resultados atuais?"));
				 _tcscanf_s(_T("%d"), &opcao);

				 if (opcao == 1) {
					 for (int j = 0; j < nT; j++) {
						 dados[j].trinco = 1;
					 }
				 }

				 else if (resultado >= WAIT_OBJECT_0 && resultado <= WAIT_OBJECT_0 + nT) {
					 _tprintf(TEXT("\n Terminou a thread [%d]"), ids[resultado - WAIT_OBJECT_0]);
					 CloseHandle(thread[resultado - WAIT_OBJECT_0]);
					 for (DWORD i = (resultado - WAIT_OBJECT_0); i < (nT - 1); i++) {
						 thread[i] = thread[i + 1];
						 ids[i] = ids[i + 1];
					 }
					 nT--;
				 }
				 /*GetExitCodeThread(thread[0], &resultadoPares);

				 _tprintf(_T("\n Resultado da thread Pares: %d"), resultadoPares);
				 WaitForSingleObject(thread[1], &resultadoPrimos);
				 GetExitCodeThread(thread[1], &resultadoPrimos);

				 _tprintf(_T("\n Resultado da thread Primos: %d"), resultadoPrimos);*/
			 }
		} while (nT);

		printf(TEXT("\n[LIMITE INFERIOR] - Quantidade de primos [%d] via ED\n"), dados[0].contador);
		printf(TEXT("\n[LIMITE SUPERIOR] - Quantidade de primos [%d] via ED\n"), dados[1].contador);

		
		/*else {
			GetExitCodeThread(thread[1], &resultadoPrimos);

			_tprintf(_T("\n Resultado da thread PRIMOS: %d"), resultadoPrimos);
			WaitForSingleObject(thread[0], &resultadoPares);
			GetExitCodeThread(thread[0], &resultadoPares);

			_tprintf(_T("\n Resultado da thread Pares: %d"), resultadoPares);
		}

		CloseHandle(thread[0]);
		CloseHandle(thread[1]);*/

		
			
			
			
		
	}
		

		

		




