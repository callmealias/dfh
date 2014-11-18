#include "stdafx.h"
#include "utils.h"
#include "mhook-lib\mhook.h"
#include "trace-lib\trace.h"

// Directory that deleted files and metadata will be saved to
// NOTE: This setting needs to be identical across the dfh.exe and libdfh.dll projects
#define DELETED_FILES_DIRECTORY L"C:\\dfh"

// Function pointer to original NtSetInformationFile
static _NtSetInformationFile Original_NtSetInformationFile = NULL; 

// Hooked version of NtSetInformationFile
// Makes a backup copy of the deleted file and saves associated metadata before 
// calling the original version of NtSetInformationFile
NTSTATUS WINAPI Hooked_NtSetInformationFile(
  HANDLE FileHandle, 
  PIO_STATUS_BLOCK IoStatusBlock, 
  PVOID FileInformation, 
  ULONG Length, 
  FILE_INFORMATION_CLASS FileInformationClass
  ) {

    // Is this a delete operation?
    if(FileInformation != NULL && Length == sizeof(BOOLEAN) && FileInformationClass == FileDispositionInformation && *(BOOLEAN*) FileInformation != FALSE && !IsFileHandleDirectory(FileHandle)) {
      
      FILE* metadata_file = NULL;
      WCHAR path[MAX_PATH] = { 0 };
      WCHAR path_metadata[MAX_PATH] = { 0 };

      do {
        
        // Get full file path from handle  
        if(!GetPathNameByHandle(FileHandle, path, MAX_PATH)) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error getting path from handle\n");
          break;
        }

        // Ignore deletes originating from the DELETED_FILES_DIRECTORY
        if(_wcsnicmp(path, DELETED_FILES_DIRECTORY, (_countof(DELETED_FILES_DIRECTORY) - 1)) == 0) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Ignoring delete: (%s)\n", path);
          break;
        }

        TRACE(L"dfh: Hooked_NtSetInformationFile: Intercepted delete: (%s)\n", path);

        // Generate a GUID
        WCHAR guid[37] = { 0 };
        if(!CreateGuid(guid)) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error creating GUID\n");
          break;
        }

        // Generate new file name
        WCHAR path_new[MAX_PATH] = { 0 };
        swprintf_s(path_new, MAX_PATH, L"%s\\%s", DELETED_FILES_DIRECTORY, guid);

        // Generate metadata file name
        swprintf_s(path_metadata, MAX_PATH, L"%s\\%s.txt", DELETED_FILES_DIRECTORY, guid);

        // Copy file
        if(!CopyFileW(path,path_new,TRUE)) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error copying file: (%s)\n", path);
          break;
        }

        // Write metadata file
        // Format: TimeStamp,ProcessId,OrigFilename,NewFileName
        if(_wfopen_s(&metadata_file, path_metadata, L"w")) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error opening metadata file for writing: (%s)\n", path_metadata);
          break;
        }
        DWORDLONG filetime = GetCurrentUTCFileTime();
        DWORD process_id = GetCurrentProcessId();
        if(fwprintf_s(metadata_file,L"%llu,%d,%s,%s", filetime, process_id, path, path_new) < 0) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error writing to metadata file: (%s)\n", path_metadata);
          break;
        }

      } while(0);

      // Cleanup
      if(metadata_file) {
        if(fclose(metadata_file)) {
          TRACE(L"dfh: Hooked_NtSetInformationFile: Error closing metadata file: (%s)\n", path_metadata);
        }
      }

    } 
    
    // Call original
    return Original_NtSetInformationFile(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);    
}

void SetHooks()
{
  if(!Original_NtSetInformationFile) {
    Original_NtSetInformationFile = (_NtSetInformationFile)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtSetInformationFile");
  }

	if (Original_NtSetInformationFile && Mhook_SetHook((PVOID*)&Original_NtSetInformationFile, Hooked_NtSetInformationFile)) {
    TRACE(L"dfh: SetHooks: Hooked NtSetInformationFile\n");
	} else {
    TRACE(L"dfh: SetHooks: Error hooking NtSetInformationFile\n");
  } 
}

void ClearHooks()
{
	if (Mhook_Unhook((PVOID*)&Hooked_NtSetInformationFile)) {
    Original_NtSetInformationFile = NULL;
    TRACE(L"dfh: ClearHooks: Unhooked NtSetInformationFile\n");
	} else {
    TRACE(L"dfh: ClearHooks: Error unhooking NtSetInformationFile\n");
  }
}

// Entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    SetHooks();
    break;
	case DLL_THREAD_ATTACH:
    break;
	case DLL_THREAD_DETACH:
    break;
	case DLL_PROCESS_DETACH:
    ClearHooks();
		break;
	}
	return TRUE;
}

