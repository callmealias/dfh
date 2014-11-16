#pragma once

#include <Windows.h>
#include "nativeapi.h"

DWORDLONG GetCurrentUTCFileTime();

BOOL CreateGuid(
  _Out_  PWSTR buffer // Buffer must be have length of 37+ WCHAR's
  );

BOOL GetPathNameByHandleW(
  _In_   HANDLE handle, 
  _Out_  PWSTR buffer, 
  _In_   DWORD buffer_length
  );