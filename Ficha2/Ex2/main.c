#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define TAM 200
#define MAX 256

int _tmain(int argc, TCHAR* argv[]) {
    HKEY chave;
    TCHAR chave_nome[TAM];
    DWORD resposta;

    /* ... Mais variáveis ... */

    DWORD versao = 1, opcao = 0, sizeVal = 257 * sizeof(DWORD), sizeNome = 257 * sizeof(DWORD);

#ifdef UNICODE 
    _setmode(_fileno(stdin), _O_WTEXT);
    _setmode(_fileno(stdout), _O_WTEXT);
    _setmode(_fileno(stderr), _O_WTEXT);
#endif

    /* ... as várias alíneas ...*/

      /*alinea b*/
    TCHAR chave_final[TAM] = TEXT("Software\\aula\\");


    _tprintf(TEXT("Indique o nome de uma chave:"));
    _fgetts(chave_nome, TAM, stdin);
    chave_nome[_tcslen(chave_nome) - 1] = '\0';


    _tcscat_s(chave_final, TAM, chave_nome);

    _tprintf(TEXT("Caminho completo: [%s]"), chave_final);

    //verificar se a chave ja existe

   /*if (RegOpenKeyEx(HKEY_CURRENT_USER, chave_final, 0, KEY_ALL_ACCESS, &chave) != ERROR_SUCCESS) {
        _tprintf(TEXT("A chave ainda não existe e irá ser criada"));
    }
    else {
        _tprintf(TEXT("Chave já existe"));
    }*/ 

    if (RegCreateKeyEx(HKEY_CURRENT_USER,
        chave_final,
        0,
        NULL,
        REG_OPTION_VOLATILE,
        KEY_ALL_ACCESS,
        NULL,
        &chave,
        &resposta) != ERROR_SUCCESS) {
            DWORD error = GetLastError();
            _tprintf(TEXT("Erro ao abrir a chave: [%d]"), error);
    }
    else {
        if (resposta == REG_OPENED_EXISTING_KEY) {
            _tprintf(_T("\nA chave já existia, foi apenas aberta"));
        }
        else {
            _tprintf(TEXT("Chave criada"));
        }

        DWORD opcao;

        //alinea d)

       _tprintf(TEXT("Escolha uma opção:\n1 - Criar valor\n2 - Consular valor\n3 - Eliminar valor"));
       _tscanf_s(_T("%29d"), &opcao);
       _fgettc(opcao, TAM, stdin);


       switch (opcao)
       {
       case 1:
           //opcao 1
           TCHAR nome[TAM];
           _tprintf(_T("Indique o nome do par chave-valor:"));
           _fgetts(nome, TAM, stdin);
           nome[_tcslen(nome) - 1] = '\0';

           TCHAR val[TAM];
           _tprintf(_T("Indique o valor:"));
           _fgetts(val, TAM, stdin);
           val[_tcslen(val) - 1] = '\0';


           RegSetValueEx(chave, nome, 0, REG_SZ, (LPBYTE)val, (DWORD)(_tcslen(val) + 1) * sizeof(TCHAR));
           break;
       case 2:
           //opcao2
           _tprintf(_T("Consultar todos os atributos"));

           DWORD i = 1;
           while (RegEnumValue(chave, i++, nome, &sizeNome, NULL, NULL, (LPBYTE)val, &sizeVal) != ERROR_NO_MORE_ITEMS) {
               _tprintf(TEXT("\n[%d] - atributo [%s] - chave [%s]\n"), i, nome, val);
               sizeNome = 257 * sizeof(TCHAR);
               sizeVal = 257 * sizeof(TCHAR);
           }
           break;
       case 3:
           _tprintf(TEXT("\nIntroduza o nome do atributo que pretende apagar"));
           _fgetts(nome, MAX, stdin); nome[_tcslen(nome)] = '\0';
           RegDeleteValue(chave, nome);
           break;
       case 4:
           _tprintf(TEXT("\nIntroduza o nome do atributo que pretende procurar:"));
           _fgetts(nome, MAX, stdin); nome[_tcslen(nome)] = '\0';
           RegQueryValueEx(chave, nome, NULL, NULL, (LPBYTE)val, &sizeVal);
           _tprintf(TEXT("\n Atributo [%s] valor [%s] "), nome, val);
           break;
       default:
           break;
       }
       
    }

    RegCloseKey(chave);

    return 0;
}