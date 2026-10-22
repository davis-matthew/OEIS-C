/*
 * A362160  Irregular triangle read by rows: The n-th row contains 2^n
 *          integers corresponding to the words of the n-bit Gray code with
 *          the most significant bits changing fastest.
 * https://oeis.org/A362160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 8406, u32 n <= 8406, u64 n <= 8406,
 *            u128 n <= 8406, big any n
 * Helpers:   T_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Irregular triangle A362160 – a(n) = T(r, j) where 
   r = floor(log2(n + 1)) and j = n - (2^r - 1).
   T(r, j) is defined recursively:
       T(0,0) = 0
       if j < 2^(r-1):  T(r,j) = 2 * T(r-1, j)
       else           T(r,j) = 2 * T(r-1, 2^(r-1)-k-1) + 1
       where k = j - 2^(r-1)
   This implementation follows the direct recurrence, which is O(r)
   per call and works for any n >= 0.
*/

#include <stdint.h>
#include "../utils/T_compute.h"

/* main sequence function */
seq_t a(idx_t n)
{
    /* find r = floor(log2(n + 1)) */
    idx_t n1 = n + 1;
    idx_t r;
#if defined(__GNUC__) || defined(__clang__)
    r = 63 - __builtin_clzll(n1);          /* works because n1>0 */
#else
    r = 0;
    while (n1 >>= 1) r++;                  /* portable fallback */
#endif

    /* j = n - (2^r - 1) */
    idx_t offset = ((idx_t)1 << r) - 1;
    idx_t j = n - offset;

    return T_compute(r, j);
}
