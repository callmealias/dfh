#pragma once

#include <Windows.h>
#include "nativeapi.h"

DWORDLONG GetCurrentUTCFileTime();

BOOL CreateGuid(
  _Out_  PWSTR buffer // Buffer must be have length of 37+ WCHAR's
  );

BOOL GetPathNameByHandle(
  _In_   HANDLE handle, 
  _Out_  PWSTR buffer, 
  _In_   DWORD buffer_length
  );

// Returns true if the supplied file handle references a directory
BOOL IsFileHandleDirectory(
  _In_ HANDLE handle
  );