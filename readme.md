# Math

![GitHub issues](https://img.shields.io/github/issues/bluewingtan/math?style=flat-square) ![GitHub closed issues](https://img.shields.io/github/issues-closed/bluewingtan/math?style=flat-square) ![Language](https://img.shields.io/badge/language-C%2B%2B-blue?style=flat-square) ![Author](https://img.shields.io/badge/powered_by-BlueWingTan-blue?style=flat-square)

> The NUDTTK math library encapsulates matrix operations and partial fitting basic algorithms, laying the algorithm foundation for further space orbit simulation.

This project is a modern reconstruction of the NUDTTK math library. While keeping the original API unchanged, the computing performance is greatly optimized, and the problems of buffer overflow and memory leakage are eliminated.

Issues or P/Rs are welcome.

## Table of Contents

[toc]

## Features

**CALMO**:

- **Cross-platform** with high-performance code
- **Accelerated** by Intel® MKL
- **Lazy evaluation** of high-overhead matrix operations
- **Modern** C++ language (C++ 11/14/17/20)
- **Optimized** code

## Dependences

- **eigen** (minimum 3.1, 3.3.7 and above are recommended)
We use Eigen as a matrix computing support background.

- **boost** (minimum 1.70.0, 1.72.0 and above are recommended), *optional*
Since some compilers do not support modern C++, it is necessary to degenerate the code to boost when using advanced features if needed.

- **Intel® MKL** (minimum 2018, 2020 and above are recommended), *optional*
If you want to use MKL acceleration, you need to install Intel® MKL acceleration library.

## Compilers and `__cplusplus` Macro

The following are the supported compilers and versions:

|Compiler|Version|`__cplusplus` Macro|Note|
|:-:|:-:|:-:|:-:|
|MSVC|1500|201103L|Minimal (Untested)|
|MSVC|1925|201703L|Tested|
|GCC|4.8|201103L|Minimal (Untested)|
|GCC|9.3|201703L|Untested|
|clang|3.4|201103L|Minimal (Untested)|
|clang|10.0|201703L|Tested without MKL|
|ICC|19.4|201703L|Tested|