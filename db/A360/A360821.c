/*
 * A360821  Number of primes of the form k^2+1 between n^2 and 2*n^2
 *          exclusive.
 * https://oeis.org/A360821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 88, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 104 terms through n = 969 (2026-11-01)
 * Helpers:   ipow_i32, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A360821: number of primes p = k^2+1 with n^2 < p < 2*n^2 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_seq.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n >= 1 for all valid calls */
    idx_t n2   = n * n;                         /* n^2  (within seq_t limits) */
    seq_t upper = (seq_t)2 * n2 - 2;            /* 2*n^2 - 2  */
    idx_t maxk = (idx_t)floor_sqrt(upper);     /* floor( sqrt(2*n^2-2) ) */

    seq_t count = 0;
    for (idx_t k = n; k <= maxk; ++k)
    {
        seq_t p = (seq_t)k * k + 1;            /* k^2 + 1   */
        /* p is automatically < 2*n2 by choice of maxk */
        if (is_prime_seq(p))
            ++count;
    }
    return count;
}
