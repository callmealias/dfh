#pragma once

#include <windows.h>

// Standalone implementation of MFC TRACE macro
// http://www.codeproject.com/Articles/154/Useful-Debugging-Macros
// http://stackoverflow.com/questions/27622/how-to-enable-the-trace-macro-in-release-mode
#ifdef _TRACE
  bool _trace(PCSTR format, ...);
  bool _trace(PCWSTR format, ...);
  #define TRACE _trace
#else
  #define TRACE __noop
#endif