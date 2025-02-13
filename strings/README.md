# String Operations with SIMD & Assembly

## Overview
This subdirectory contains optimized implementations of two string operations using **SIMD intrinsics** and **Assembly** for maximum efficiency.

### Base64 Distance (`b64.c`)
The **Base64 distance** measures how different two strings are when only considering valid Base64 characters. The process involves:
1. **Filtering**: Removing all non-Base64 characters (`a-z, A-Z, 0-9, +, /`) while maintaining order.
2. **Encoding**: Converting the filtered strings into **Base64-encoded numbers**.
3. **Distance Calculation**: Computing the numerical difference between the two encoded values.

**Optimized with SIMD intrinsics** for faster processing, ensuring efficient string filtering and encoding.

### Hamming Distance (`hamming.s`)
The **Hamming distance** is a metric that counts the number of positions where two strings differ. It is commonly used in **error detection** and **data integrity checks**. Given two strings of equal length:
- A difference at any position increases the Hamming distance by 1.
- If the strings are identical, the distance is 0.

This implementation:
- Uses **Assembly (SSE instructions)** for efficient bitwise comparisons.
- Processes **16 bytes at a time** for improved speed.
- Efficiently counts mismatched positions using **vectorized operations**.

## Usage
1. Compile using the provided Makefile:
   ```sh
   make all
   ```
2. Run the executable:
   ```sh
   make run
   ```
3. Debug with GDB:
   ```sh
   make debug
   ```
4. Clean up compiled files:
   ```sh
   make clean
   ```


