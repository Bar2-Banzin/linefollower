:: Build C++
msbuild "Image Processing C++ Original.sln" 

:: Run Test
python test.py %1 %2% %3%