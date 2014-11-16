#include "stdafx.h"
#include "utils.h"

#include "Shlwapi.h"
#pragma comment(lib,"Shlwapi.lib")

BOOL IsWow64Process() {
  BOOL is_wow64 = false;
  IsWow64Process(GetCurrentProcess(), &is_wow64);
  return is_wow64;
}

BOOL Is64Process() {
#if _WIN64
  return TRUE;
#else
  return FALSE;
#endif
}

BOOL RegistrySetValue(HKEY hkey, DWORD view, LPWSTR subkey, LPWSTR value_name, void* value, DWORD value_type) {
  BOOL success = FALSE;
  HKEY hkey_result = NULL;  
  DWORD options = KEY_WRITE | view;
  if(RegOpenKeyExW(hkey,subkey,0,options,&hkey_result) == ERROR_SUCCESS) {
    size_t size = 0;
    switch(value_type) {
    case REG_SZ:
      size = sizeof(WCHAR) * (wcslen((LPWSTR)value) + 1);
      break;
    case REG_DWORD:
      size = sizeof(DWORD);
    // TODO: add other cases here as needed
    default:
      break;
    }
    if(size > 0 && RegSetValueExW(hkey_result, value_name, 0, value_type, (const BYTE*)value, (DWORD)size) == ERROR_SUCCESS) {
      success = TRUE;
    }
    if(hkey != hkey_result) {
      RegCloseKey(hkey_result);
    }
  }
  return success;
}

BOOL GetAppDirPath(LPWSTR buffer, size_t length) {
  BOOL success = FALSE;
  WCHAR path[MAX_PATH] = {0};
  if(GetModuleFileNameW(NULL,path, _countof(path)) > 0) {
    wchar_t* filename = PathFindFileNameW(path);
    if(filename > path) {
      filename[-1] = L'\0';
    }
    size_t path_length = wcsnlen_s(path, _countof(path));
    wcscpy_s(buffer, length, path);
    success = TRUE;
  }
  return success;
}

BOOL GetAppDirRelativeShortPath(LPWSTR buffer, size_t length, LPWSTR sub_path) {
  BOOL success = FALSE;
  WCHAR path[MAX_PATH] = {0};
  if(GetAppDirPath(path, _countof(path))) {
    size_t path_length = wcsnlen_s(path, _countof(path));
    size_t sub_path_len = wcslen(sub_path);
    if((path_length + sub_path_len) < _countof(path)) {
      wcsncpy_s( (path + path_length), (_countof(path) - path_length), sub_path, sub_path_len);
      success = (GetShortPathNameW(path, buffer, (DWORD)length) > 0);
    }
  }
  return success;
}

BOOL SetAppInitDLLs(DWORD reg_view, LPWSTR dll_name) {
  
  BOOL success = FALSE;
  WCHAR buffer[MAX_PATH] = {0};
  DWORD flag = 0;
  
  do {

    if(!GetAppDirRelativeShortPath(buffer, _countof(buffer), dll_name)) {
      break;
    }

    if(!RegistrySetValue(HKEY_LOCAL_MACHINE, reg_view, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"AppInit_DLLs", (void*)buffer, REG_SZ)) {
      break;
    }

    flag = 1;
    if(!RegistrySetValue(HKEY_LOCAL_MACHINE, reg_view, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"LoadAppInit_DLLs", (void*)&flag, REG_DWORD)) {
      break;
    }

    flag = 0;
    if(!RegistrySetValue(HKEY_LOCAL_MACHINE, reg_view, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"RequireSignedAppInit_DLLs", (void*)&flag, REG_DWORD)) {
      break;
    }

    success = TRUE;

  } while(0);

  return success;
}

BOOL ClearAppInitDLLs(DWORD reg_view) {

  BOOL success = RegistrySetValue(HKEY_LOCAL_MACHINE, reg_view, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows", L"AppInit_DLLs", (void*)L"\0", REG_SZ);
  return success;

}
