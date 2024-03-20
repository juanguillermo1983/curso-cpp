#include <iostream>
#include <Windows.h>
#include <tchar.h>

using namespace std;

int main()
{
    HANDLE hfile = CreateFileA(
        "TestSample.txt",           // nombre
        GENERIC_WRITE,              // escritura
        0,                          //no es compartido
        NULL,                       // DEFAULT SEGURIDAD
        CREATE_NEW,                 // crea archivo si no existe 
        FILE_ATTRIBUTE_NORMAL,      // archivo normal
        NULL);                      // atributo template

    // check si falla 
    if (hfile == INVALID_HANDLE_VALUE)
    {
        cout << "Fallo al crear el archivo" << endl;
    }

    char buffer[] = "Data para el buffer ...";
    DWORD dwBytesToWrite = strlen(buffer) * sizeof(char);
    DWORD dwBytesWritten = 0;


    WriteFile(hfile,        // handle del archivo
        (void*)buffer,      // start of buffer a escribir
        dwBytesToWrite,     // numero de bytes para escribir
        &dwBytesWritten,    // numero de bytes para escribir al archivo
        NULL);              // sincroniza la escritura

    if (dwBytesWritten == dwBytesToWrite)
    {
        cout << "Archivo escrito satisfactoriamente" << endl;
    }

    //close file 
    CloseHandle(hfile);

    return 0;
}


