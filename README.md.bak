# Finite field arithmetic
A C and ARM64 finite field arithmetic library

Implemented random number generation, modular reduction, addition, subtraction, negation, multiplication, multiplicative inverse, Legendre symbol, square root in some prime fields of sizes from 64 bits to 512 bits.

Implementation is constant-time, constant memory access with no branching and input independent.


**EASIEST WAY TO RUN**

You can do all benchmarks and tests by giving execution priviledges to the script `go.sh` 
`chmod +x go.sh`

This script cleans, compiles and runs the program. Make sure execution priviledges are provided for the directory. 

`./go.sh` Makes all prime sizes and runs arithmetic tests and benchmarks

`./go.sh FAST` Makes all prime sizes with ARM64 assebly, and runs all tests and benchmark 

`./go.sh X` Makes X bit prime, runs all tests and benchmark

`./go.sh X FAST` Makes X bit prime with ARM64 assembly, runs all tests and benchmarks

`./go.sh ALT` Makes all prime sizes and runs arithmetic tests and benchmarks with an alternative prime for each prime size

`./go.sh ALT X` `./go.sh ALT FAST` `./go.sh ALT X FAST` As above with appropriate modifications.

---


Run `make` which will compile arithmetic tests and benchmarks for 64,128,192,256,512 bit sized prime fields. The executables are arith64, arith128, arith192, arith256, arith512..

By running `make arith64` you will only compile the 64 bit prime arithmetic (and respectively for 128, 192, 256, 512)

By running `make arith64 OPT_LEVEL=FAST` you will compile ARM64 assembly code which is optimised. It speeds up the multiplications, modular reduction, etc. For other prime sizes use `arith128`, etc. respectively. 

By running `make arith64 ALT_PRIMES=ALT` the compilation takes place by using alternative prime values. Different prime sizes and assembly can be made with corresponding flags.



---

Primes:

### 64 bits
ORIGINAL

`p = 2^61 - 1 = 0x1FFFFFFFFFFFFFFF`

ALT

`p = 2^64 - 59 = 0xFFFFFFFFFFFFFFC5` 
### 128 bits
ORIGINAL

`p = 2^127 - 1 = 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF`

ALT

`p = 2^128 - 173 = 0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFF53`

### 192 bits
ORIGINAL

`p = 2^192 - 237 = 0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFF13`

ALT

`p = 2^191 - 19 = 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFED`

### 256 bits
ORIGINAL

`p = 2^255 - 19 = 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFED`

ALT

`p = 2^256 - 2^224 + 2^192 + 2^96 - 1 = 0xFFFFFFFF00000001 0000000000000000 00000000FFFFFFFF FFFFFFFFFFFFFFFF`

### 512 bits
ORIGINAL

`p = 2^512 - 2^256 + 2^192 - 2^128 - 1 = 0xFFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF 0000000000000000 FFFFFFFFFFFFFFFE FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF`


ALT
`p = 2^511 - 2^320 + 1 = 0x7FFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF FFFFFFFFFFFFFFFF 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000001`




---

# Copyright 2024 Novak Kaluđerović

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


