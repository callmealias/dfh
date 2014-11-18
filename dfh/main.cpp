#include "stdafx.h"
#include "utils.h"

// Directory that deleted files and metadata will be saved to
// NOTE: This setting needs to be identical across the dfh.exe and libdfh.dll projects
#define DELETED_FILES_DIRECTORY L"C:\\dfh"

// Makes all system changes required to commence hooking file deletes originating
// in both 32 and 64 bit processes. All future processes created will be hooked.
// Hooks are not added to existing processes.
BOOL InstallHook() {
  
  BOOL success = FALSE;

  do {

    if(!CreateAllAccessDirectory(DELETED_FILES_DIRECTORY)) {
      break;
    }
  
    if(!SetAppInitDLLs(KEY_WOW64_32KEY, L"\\libdfh.dll")) {
      break;
    }

    if(IsWow64Process() || Is64Process()) {
      if(!SetAppInitDLLs(KEY_WOW64_64KEY, L"\\libdfh64.dll")) {
        break;
      }
    }

    success = TRUE;

  } while(0);

  return success;
}

// Makes all system changes required to stop hooking file deletes originating
// in both 32 and 64 bit processes. All future processes created will not be hooked.
// Hooks are not removed from existing processes.
BOOL UninstallHook() {
  
  BOOL success = FALSE;

  do {

    if(!ClearAppInitDLLs(KEY_WOW64_32KEY)) {
      break;
    }
    if(IsWow64Process() || Is64Process()) {
      if(!ClearAppInitDLLs(KEY_WOW64_64KEY)) {
        break;
      }
    }
    success = TRUE;

  } while(0);

  return success;
}

// Entry point
int wmain(int argc, wchar_t* argv[])
{

  //DebugBreak();

  int exit_code = 1;

  if(argc == 2 && _wcsicmp(L"/i", argv[1]) == 0) {
    if(InstallHook()) {
      wprintf(L"Successfully installed hook \r\n");
      exit_code = 0;
    } else {
      wprintf(L"Error installing hook \r\n");
    }

  }
  else if(argc == 2 && _wcsicmp(L"/u", argv[1]) == 0) {
    if(UninstallHook()) {
      wprintf(L"Successfully uninstalled hook \r\n");
      exit_code = 0;
    } else {
      wprintf(L"Error uninstalling hook \r\n");
    }
  } else {
    wprintf(L"Delete File Hook 1.0a - Intercept file deletes at Windows Native API level \r\n");
    wprintf(L"https://github.com/pmolchanov/dfh \r\n");
    wprintf(L"\r\n");
    wprintf(L"usage: dfh.exe [/i] [/u] \r\n");
    wprintf(L"\r\n");
    wprintf(L"  /i  Installs the hook. Deleted files are saved to " DELETED_FILES_DIRECTORY L". \r\n");
    wprintf(L"  /u  Uninstalls the hook. \r\n");
  }

	return exit_code;
}

