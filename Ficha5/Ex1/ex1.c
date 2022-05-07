#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <stdio.h>
#include <fcntl.h> 
#include <io.h>


// funcionalidade relacionada com temporização

static double PerfCounterFreq; // n ticks por seg.

void initClock() {
	LARGE_INTEGER aux;
	if (!QueryPerformanceFrequency(&aux))
		_tprintf(TEXT("\nSorry - No can do em QueryPerfFreq\n"));
	PerfCounterFreq = (double)(aux.QuadPart); // / 1000.0;
	_tprintf(TEXT("\nTicks por sec.%f\n"), PerfCounterFreq);
}

__int64 startClock() {
	LARGE_INTEGER aux;
	QueryPerformanceCounter(&aux);
	return aux.QuadPart;
}

double stopClock(__int64 from) {
	LARGE_INTEGER aux;
	QueryPerformanceCounter(&aux);
	return (double)(aux.QuadPart - from) / PerfCounterFreq;
}

// estrutura de dados para controlar as threads

typedef struct {
	unsigned int limitesuperior;
	unsigned int limiteinferior;
	unsigned int* contadormultiplos;
	//HANDLE mutexDoPovo;
	CRITICAL_SECTION* criticalSectionPartilhada;
} TDados;

// função da(s) thread(s)
// ...

DWORD WINAPI MultiplosdeTres(LPVOID dados) {
	TDados* a = (TDados*)dados;
	for (unsigned int i = a->limiteinferior; i <= a->limitesuperior; i++) {
		if (i % 3 == 0) {
			//inicio da zona critica
			//WaitForSingleObject(a->mutexDoPovo, INFINITE);
			EnterCriticalSection(a->criticalSectionPartilhada);
			(*(a->contadormultiplos))++;
			//ReleaseMutex(a->mutexDoPovo);
			LeaveCriticalSection(a->criticalSectionPartilhada);
		
		}
	}
	return 0;
}

// número * máximo * de threads
// podem (e devem) ser menos
#define MAX_THREADS 20





int _tmain(int argc, TCHAR* argv[]) {

	// matriz de handles das threads
	HANDLE hThreads[MAX_THREADS];

	// Matriz de dados para as threads;
	TDados tdados[MAX_THREADS];

	// número efectivo de threads
	int numthreads;

	// limite superior
	unsigned int limsup;

	// variáveis para cronómetro
	__int64 clockticks;
	double duracao;

	unsigned int range;
	unsigned int inter;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	initClock();
	_tprintf(TEXT("\nLimite sup. -> "));
	_tscanf_s(TEXT("%u"), &limsup);
	_tprintf(TEXT("\nNum threads -> "));
	_tscanf_s(TEXT("%u"), &numthreads);
	if (numthreads > MAX_THREADS)
		numthreads = MAX_THREADS;

	// FAZER prepara e cria threads
	//       manda as threads começar

	//contador de multiplos
	unsigned int contadorMultiplos = 0;

	//HANDLE mutexPartilhado = CreateMutex(NULL,FALSE,NULL);
	CRITICAL_SECTION criticalSectionPartilhada;
	BOOL ii = InitializeCriticalSectionAndSpinCount(&criticalSectionPartilhada, 200);

	for (int i = 0; i < numthreads; i++) {
		tdados[i].contadormultiplos = &contadorMultiplos;
		tdados[i].limiteinferior =(int)( 1 + (limsup / numthreads) * i);
		tdados[i].limitesuperior =(int)(  (limsup / numthreads) * (i+1));
		//tdados[i].mutexDoPovo = mutexPartilhado;
		tdados[i].criticalSectionPartilhada = &criticalSectionPartilhada;
		hThreads[i] = CreateThread(NULL, 0, MultiplosdeTres, &tdados[i], CREATE_SUSPENDED, NULL);
	}

	//

	clockticks = startClock();
	for (int i = 0; i < numthreads; i++) {
		ResumeThread(hThreads[i]);
	}

	// FAZER aguarda / controla as threads 
	//       manda as threads parar

	WaitForMultipleObjects(numthreads, hThreads, TRUE, INFINITE);
	//CloseHandle(mutexPartilhado);
	DeleteCriticalSection(&criticalSectionPartilhada);

	duracao = stopClock(clockticks);
	_tprintf(TEXT("\nSegundos=%f\n"), duracao);
	_tprintf(TEXT("\nNumero de multiplos de 3 [%d]"), contadorMultiplos);

	// FAZER apresenta resultados

	// Cód. ref. para aguardar por uma tecla – caso faça falta
	// _tprintf(TEXT("\nCarregue numa tecla"));
	// _gettch();

	return 0;
}
// Este código é apenas uma ajuda para o exercício. Se quiser, mude-o
