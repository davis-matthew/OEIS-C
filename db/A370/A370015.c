/*
 * A370015  Expansion of A(x) = Sum_{n>=0} 2^(n*(n-1)/2) * (1 + 2^(2*n+1))/3 *
 *          x^(n*(n+1)/2).
 * https://oeis.org/A370015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 2^(k(k-1)/2) * (1 + 2^(2k+1)) / 3 if n = k(k+1)/2 0
 *            otherwise }
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 66,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Integer square root for unsigned __int128 arguments.
   Returns floor(sqrt(v)). */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t low = 0;
    /* High bound: sqrt(2^128-1) < 2^64. */
    idx_t high = ((idx_t)1 << 32);  /* 2^32 is safely above sqrt(1+8n) for n up to 10^4. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute d = 1 + 8n. */
    unsigned __int128 d = (unsigned __int128)1 + (unsigned __int128)8 * n;

    /* Integer sqrt of d. */
    idx_t s = isqrt_u128(d);

    /* Check whether d is a perfect square. */
    if ((unsigned __int128)s * s != d)
        return (seq_t)0;

    /* d is odd; s must be odd for k to be integer. */
    if ((s & 1) == 0)
        return (seq_t)0;

    /* k = (s - 1)/2. */
    idx_t k = (s - 1) >> 1;

    /* exp1 = k(k-1)/2. */
    idx_t exp1 = (unsigned __int128)k * (k - 1) / 2;

    /* Compute parts of the formula using pow2. */
    seq_t part1 = pow2(exp1);                     /* 2^(k(k-1)/2)   */
    seq_t part2 = (seq_t)1 + pow2(2 * k + 1);     /* 1 + 2^(2k+1)   */

    /* Final value: part1 * part2 / 3. Division by 3 is exact. */
    seq_t result = part1 * part2 / (seq_t)3;

    return result;
}
