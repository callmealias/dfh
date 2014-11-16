#pragma once

// Target Windows XP SP3 with IE8
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745.aspx

#define NTDDI_VERSION NTDDI_WINXPSP3
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WINVER _WIN32_WINNT_WS03
#define _WIN32_IE _WIN32_IE_IE80
#include <SDKDDKVer.h>