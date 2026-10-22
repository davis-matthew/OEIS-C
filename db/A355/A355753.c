/*
 * A355753  a(n) = 3*(2*n - 1)*(3*(2*n - 1)^3 - 1) / 2 for n > 0.
 * https://oeis.org/A355753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*n - 1)*(3*(2*n - 1)^3 - 1) / 2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 88, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS sequence A355753.  */
seq_t a(idx_t n)
{
    if (n == 0)               /* not used – offset starts at 1 */
        return 0;

    /* Use 128‑bit intermediate to avoid overflow for 64‑bit seq_t. */
    __uint128_t t   = (__uint128_t)2 * ( (__uint128_t)n ) - 1;  /* t = 2*n - 1 */
    __uint128_t t2  = t * t;                                 /* t²          */
    __uint128_t t3  = t2 * t;                                /* t³          */
    __uint128_t inner = (__uint128_t)3 * t3 - 1;             /* 3*t³ - 1    */
    __uint128_t num  = (__uint128_t)3 * t * inner;           /* 3*t*(3*t³-1) */

    /* The value is even, so integer division is exact. */
    return (seq_t)(num / 2);
}
