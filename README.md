Delete File Hook (dfh): A Windows tool for capturing file deletes originating from almost any process

Overview:
==

The dfh tool intercepts file deletes originating from most windows processes (any executable linked with User32.dll) and makes a backup copy of the deleted file in the ``C:\dfh`` directory before the file is deleted. It also saves a metadata file for each deleted file that includes a timestamp, the process id from which the delete originated, the original filename, and the backup copy filename. 

Usage:
==

1. Install the [Microsoft Visual C++ 2012 Redistributables](http://www.microsoft.com/en-us/download/details.aspx?id=30679) for both x86 and x64. 
2. Visit [GitHub releases](https://github.com/pmolchanov/dfh/releases) and download precompiled binaries for Microsoft Windows.
3. Open a command prompt as *Administrator* and run the following command to get help: ``dfh.exe /?``

Compile from source
== 

Visual Studio 2012 Update 4 is required. To compile the source code run the following command:

```
build.bat
```





