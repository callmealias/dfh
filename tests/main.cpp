#include "stdafx.h"

// Simple test driver to support development

#if _WIN64
#define LIBDFH_DLL L"libdfh64.dll"
#else
#define LIBDFH_DLL L"libdfh.dll"
#endif

int wmain(int argc, wchar_t* argv[])
{
  int exitCode = 1;

  HMODULE hModule = NULL;
  HANDLE hFile = NULL;

  do {

    // Just in case it already exists
    DeleteFileW(L"test.txt");

    hModule = LoadLibraryW(LIBDFH_DLL);
    if(!hModule) {
      wprintf(L"Error loading DLL");
      break;
    }

    hFile = CreateFileW(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if(!hFile) {
      wprintf(L"Error creating file");
      break;
    }

    WCHAR buffer[] = L"\uFEFF" L"Some UTF-16LE";
    DWORD dwBytesWritten = 0;
    if(!WriteFile(hFile, buffer, (sizeof(buffer) - sizeof(WCHAR)), &dwBytesWritten, NULL)) {
      wprintf(L"Error writing to file");
      break;
    }

    if(CloseHandle(hFile)) {
      hFile = NULL;
    } else {
      wprintf(L"Error closing file handle");
      break;
    }

    if(!DeleteFileW(L"test.txt")) {
      wprintf(L"Error deleting file");
      break;
    }

    exitCode = 0;

  } while(0);

  // Cleanup
  if(hModule) {
    FreeLibrary(hModule);
    hModule = NULL;
  }

  if(hFile) {
    CloseHandle(hFile);
    hFile = NULL;
  }

	return exitCode;
}

