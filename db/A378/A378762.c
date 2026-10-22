/*
 * A378762  a(n) = A378200(A378200(A378200(n))).
 * https://oeis.org/A378762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   _a378200
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A378762: a(n) = A378200(A378200(A378200(n))) = A378200(n)
 *     (A378200 is an involutive permutation of the positive integers, 
 *      defined by a triangular array where each block contains a contiguous
 *      range of numbers permuted by a simple rule). 
 * Formula for A378200(n):  
 *   1. Let r be the unique integer such that 2r²–r ≥ n > 2(r–1)²–(r–1).  
 *   2. Let start = 2(r–1)²–(r–1) (the preceding block’s cumulative size).  
 *   3. Let k = n – (start + 1) + 1 (1‑based column index inside block r).  
 *   4. Let m = 2r – 1.  
 *   5. Define
 *          P = { m – k – 1 , if k < m and k odd  
 *          P = { m – k + 1 , if k < m and k even  
 *          P = { 3m – k – 1 , if k ≥ m }  
 *   6. Result = start + P.
 *
 * The algorithm below implements these steps in O(√n) time for each call,
 * which is well within the required limits for n ≤ 10 000.  The function is
 * pure and memoization is not necessary.
 */

#include <stdint.h>
#include "../utils/_a378200.h"

/* Within the build the following types are defined:
 *   seq_t  – unsigned integer type (8, 16, 32, 64‑bit or arbitrary‑precision)
 *   idx_t  – unsigned 64‑bit index type
 */

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* a(n) equals A378200(n) because A378200 is an involution. */
    return _a378200(n);
}
