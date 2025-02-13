# Parallelized Formula Computations

## Overview
This repo contains two optimized implementations for computing mathematical expressions efficiently using **SIMD (Single Instruction, Multiple Data) intrinsics** and **Assembly**.

### Formula1 (SIMD Optimized Computation)
This one computes:

![Formula1](https://latex.codecogs.com/png.latex?\sqrt{1+\frac{\sum_{k=1}^{n}\sqrt{x_k}}{\prod_{k=1}^{n}(x_k^2+1)}})

using **Intel AVX intrinsics** for parallel processing. Instead of looping through elements one by one, we crunch four floats at a time, making it **way faster** than the regular approach.

#### Key Features:
- **Vectorized loading**: Works on **four** floats at a time.
- **Efficient reductions**: Uses horizontal sum/product to merge results.
- **Super fast operations**: Thanks to `_mm_sqrt_ps`, `_mm_mul_ps`, and `_mm_add_ps`.

### Formula2 (Assembly Optimized Computation)
This one takes in two arrays, `x` and `y`, and calculates:

![Formula2](https://latex.codecogs.com/png.latex?\sum_{k=1}^{n}(x_k%20\cdot%20y_k)%20/%20\prod_{i=1}^{n}(x_i^2+y_i^2-2x_i%20y_i+1))

using **handwritten Assembly** with **AVX instructions**. Basically, it's squeezing out every bit of performance possible!

#### Key Features:
- **Pure Assembly**: No high-level abstractions—just raw performance.
- **Vectorized multiplications and reductions**.
- **Loop unrolling and register optimizations**.

## Usage
1. Compile the program using the provided Makefile:
   ```sh
   make all
   ```
2. Run the executable:
   ```sh
   make run
   ```
3. For debugging with GDB:
   ```sh
   make debug
   ```
4. To clean up compiled files:
   ```sh
   make clean
   ```

