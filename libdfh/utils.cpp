#include "stdafx.h"
#include <rpc.h>
#include "utils.h"
#include "nativeapi.h"

// UUID functions
#pragma comment(lib,"Rpcrt4.lib")

DWORDLONG GetCurrentUTCFileTime() {
  FILETIME ft = {};
  GetSystemTimeAsFileTime(&ft);
  DWORDLONG filetime = (((DWORDLONG) ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
  return filetime;
}

BOOL CreateGuid(PWSTR buffer) {
  BOOL success = FALSE;
  // {00000000-0000-0000-0000-000000000000}
  UUID uuid = {};
	if(UuidCreate(&uuid) == RPC_S_OK) {
    PWSTR str = NULL;
    if(UuidToStringW(&uuid, (RPC_WSTR*)&str) == RPC_S_OK) {
      wcsncpy_s(buffer, 36, (LPCWSTR)(str + sizeof(WCHAR)), 36);
      buffer[36] = L'\0';
      RpcStringFree((RPC_WSTR*)&str);
      success = TRUE;
    }
  }
  return success;
}

BOOL GetPathNameByHandle(HANDLE handle, PWSTR buffer, DWORD buffer_length)
{
  BOOL success = FALSE;

  do {

    // Cache function pointers
    static _NtQueryVolumeInformationFile pNtQueryVolumeInformationFile = NULL;
    static _NtQueryInformationFile pNtQueryInformationFile = NULL;
    if(!pNtQueryVolumeInformationFile) {
      pNtQueryVolumeInformationFile = (_NtQueryVolumeInformationFile)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQueryVolumeInformationFile");
    }    
    if(pNtQueryInformationFile == NULL) {
      pNtQueryInformationFile = (_NtQueryInformationFile)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQueryInformationFile");
    }
    if(!pNtQueryVolumeInformationFile || !pNtQueryInformationFile) {
      break;
    }

    IO_STATUS_BLOCK status = {}; 

    // Get the serial number of the volume containing the file
    BYTE vol_info_buffer[FILE_FS_VOLUME_INFORMATION_REQUIRED_SIZE] = {0};
    FILE_FS_VOLUME_INFORMATION* vol_info = (FILE_FS_VOLUME_INFORMATION*)vol_info_buffer;
    if(!NT_SUCCESS(pNtQueryVolumeInformationFile(handle, &status, vol_info, FILE_FS_VOLUME_INFORMATION_REQUIRED_SIZE, FileFsVolumeInformation))) {
      break;
    }

    // Enumerate all harddisks in order to find the corresponding serial number
    BYTE file_info_buffer[FILE_NAME_INFORMATION_REQUIRED_SIZE] = {0};
    FILE_NAME_INFORMATION* file_info = (FILE_NAME_INFORMATION*)file_info_buffer;
    DWORD vol_serial = 0;
    wcscpy_s(buffer, buffer_length, L"?:\\");
    for (buffer[0] = L'A'; buffer[0] <= L'Z'; buffer[0]++) {
      
      // Find the volume with a matching serial number 
      if(GetVolumeInformationW(buffer, NULL, 0, &vol_serial, NULL, NULL, NULL, 0) == 0 || vol_serial != vol_info->VolumeSerialNumber) {
        continue;
      }
     
      // Get the relative path for this filename
      if(NT_SUCCESS(pNtQueryInformationFile(handle, &status, file_info, FILE_NAME_INFORMATION_REQUIRED_SIZE, FileNameInformation))) {
        
        // NtQueryInformationFile omits the drive letter and colon (C:) component of the path
        DWORD length = file_info->FileNameLength / sizeof(WCHAR);
        wcsncpy_s(buffer + 2, buffer_length - 2, file_info->FileName, length);
        success = TRUE;
      }

      break;
    }

  } while(0);

  return success;
}

BOOL IsFileHandleDirectory(HANDLE handle) {
  BOOL is_dir = FALSE;
  BY_HANDLE_FILE_INFORMATION file_info = {};
  if(GetFileInformationByHandle(handle, &file_info)) {
    is_dir = (file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
  }
  return is_dir;
}