call "%VS110COMNTOOLS%\VsDevCmd.bat"
msbuild dfh.sln /t:Rebuild /p:Configuration=Release /p:Platform="Win32"
msbuild dfh.sln /t:Rebuild /p:Configuration=Release /p:Platform="x64"
msbuild dfh.sln /t:Build /p:Configuration=Debug /p:Platform="Win32"
msbuild dfh.sln /t:Build /p:Configuration=Debug /p:Platform="x64"
pause