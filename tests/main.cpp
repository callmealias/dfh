#include "stdafx.h"

// Simple test driver to support development

#if _WIN64
#define LIBDFH_DLL L"libdfh64.dll"
#else
#define LIBDFH_DLL L"libdfh.dll"
#endif


// Simulates a simple file delete
BOOL TestFileDelete() {
  
  BOOL success = FALSE;

  HANDLE hFile = NULL;

  do {

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

    success = TRUE;

  } while(0);

  if(hFile) {
    CloseHandle(hFile);
    hFile = NULL;
  }

  return success;

}


// Simulates a directory delete
BOOL TestDirectoryDelete() {
  BOOL success = false;

  do {

    if(!CreateDirectoryW(L"test",NULL)) {
      wprintf(L"Error creating directory");
      break;
    }

    if(!RemoveDirectoryW(L"test")) {
      wprintf(L"Error deleting directory");
      break;
    }

    success = TRUE;

  } while(0);

  return success;
}

int wmain(int argc, wchar_t* argv[])
{
  int exit_code = 1;

  HMODULE hModule = NULL;

  do {
    
    // Cleanup from prior test run
    DeleteFileW(L"test.txt");
    RemoveDirectoryW(L"test");

    // Inject DLL
    hModule = LoadLibraryW(LIBDFH_DLL);
    if(!hModule) {
      wprintf(L"Error loading DLL");
      break;
    }

    if(!TestFileDelete()) {
       wprintf(L"Test file delete failed");
       break;
    }

    if(!TestDirectoryDelete()) {
       wprintf(L"Test directory delete failed");
       break;
    }

    exit_code = 0;

  } while(0);

  // Cleanup
  if(hModule) {
    FreeLibrary(hModule);
    hModule = NULL;
  }

	return exit_code;
}

