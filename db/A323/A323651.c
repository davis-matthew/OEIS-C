/*
 * A323651  Number of elements added at n-th stage to the toothpick structure
 *          of A323650.
 * https://oeis.org/A323651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A323651
 *
 * For n ≥ 1:
 *   if n is odd:  a(n) = A147582( (n+1)/2 )
 *   if n is even: a(n) = 2 * A147582( n/2 )
 *
 * A147582(k) is 1 for k = 1 and for k ≥ 2
 *   A147582(k) = 4 * 3^( popcount(k-1) – 1 )
 *   where popcount(x) is the number of 1‑bits in x.
 *
 * The implementation uses a small loop to compute 3^e; e ≤ 63
 * (since k ≤ 2^64–1).  The function works for the built‑in
 * integer types as well as for arbitrary‑precision integer
 * classes supported by the evaluation harness.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3_small.h"

/* Portable popcount for 64‑bit unsigned integers */
static inline idx_t popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    idx_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* k = (n+1)/2 works for both odd and even n */
    idx_t k = (n + 1) >> 1;

    /* Compute A147582(k) */
    seq_t val;
    if (k == 1)
        val = 1;
    else
    {
        idx_t e = popcount_u64(k - 1) - 1;   /* e ≥ 0 for k ≥ 2 */
        seq_t pow3 = pow3_small(e);
        val = 4 * pow3;                      /* multiply by 4 */
    }

    /* Return according to parity of n */
    return (n & 1) ? val : val * 2;
}
