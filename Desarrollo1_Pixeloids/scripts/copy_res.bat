@echo off 
echo %1 SolutionDir
echo %2 OutDir


xcopy "%~1res" "%~2res" /i/y/s