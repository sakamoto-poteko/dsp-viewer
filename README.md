# DSP Viewer
Simple data processor & plotter. Developed on ad-hoc basis (i.e. fits only my need).

__NOTE:__
This project is not designed as a library. You are supposed to modify the code and run it directly. As it's name indicates, 
it's just a "viewer" for convenience. However, DSP (calculation) library might be extracted as an independent lib in the future.

## How to build
### Prequisities
Must have:

1. Intel IPP
2. Qt Library
3. Qwt

Better with:

1. Intel Compiler


Edit `viewer/viewer.pro`, change the the variable below to corresponding dir.

```
INCLUDEPATH += D:\Projects\Tools\qwt-6.1.2\include
QMAKE_LIBDIR += D:\Projects\Tools\qwt-6.1.2\lib
```

### Windows
In Qt shell, execute `qmake -spec win32-msvc2013 -tp vc -r`. This will generate `demo.sln`. Edit `viewer` project properties, 
make sure OpenMP and Intel IPP supports are enabled.

### Linux
Currently there's no plan to add Linux support.

### MacOS
Intel Compiler on MacOS? Laugh my ass.

## Supported Algorithms
1. Fast Fourier Transform with 2^N sample points (`class FFTCalculator`)

## Supported Plots
1. Frequency Spectrum (`class FFTSpectrumWidget`)

## Supporting Classes
1. `shared_lpp_ptr`: `std::shared_ptr` for 64-bytes aligned memory blocks.
2. `Wave`: RIFF wav file decoder class.

## License
<MIT>

Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
