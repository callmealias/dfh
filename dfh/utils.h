#include <Windows.h>

// Detects if this is a 32bit process running on 64 bit windows
// true - x86 on x64
// false - x86 on x86
// false - x64 on x64
BOOL IsWow64Process();

// Detects if this is a 64 bit process
BOOL Is64Process();

// Sets a registry value
BOOL RegistrySetValue(
  _In_ HKEY hkey, // HKEY_LOCAL_MACHINE etc..
  _In_ DWORD view, // 0, KEY_WOW64_32KEY, or KEY_WOW64_64KEY
  _In_ LPWSTR subkey, // e.g. \Software\Microsoft\Windows\...
  _In_ LPWSTR value_name, // e.g. Foobar
  _In_ LPWSTR value, // Pointer to the data to be stored
  _In_ DWORD value_type // REG_SZ, REG_DWORD, etc..
  );

// Gets the directory containing this EXE
// e.g. C:\Foo
BOOL GetAppDirPath(
  _Out_ LPWSTR buffer, // Pointer to a buffer that receives the data
  _In_ size_t length // Size of buffer in wchar_t's
  );

// Gets appends the specified sub path to the appdir and shortens it to a Windows short path
BOOL GetAppDirRelativeShortPath(
  _Out_ LPWSTR buffer, // Pointer to a buffer that receives the data
  _In_ size_t length, // Size of buffer in wchar_t's
  _In_ LPCWSTR sub_path // Sub path to append to app path. e.g. \dfh.dll
  );

// Configures the AppInitDLLs registry settings for the specified DLL
BOOL SetAppInitDLLs(
  _In_ DWORD reg_view, // 0, KEY_WOW64_32KEY, or KEY_WOW64_64KEY
  _In_ LPWSTR dll_name // Short path of DLL e.g. C:\Users\FOOBAR~1\Desktop\dfh\dfh.dll
  );

// Clears the AppInitDLLs registry setting
BOOL ClearAppInitDLLs(
  _In_ DWORD reg_view
  );