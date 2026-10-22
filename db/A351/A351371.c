/*
 * A351371  a(n) = Sum_{p|n, p prime} (p + n/p).
 * https://oeis.org/A351371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} (p + n/p)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A351371 for any n ≥ 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t orig = (uint64_t)n;   /* original n, stays constant          */
    uint64_t m    = orig;          /* remaining part while factoring      */
    seq_t    sum  = (seq_t)0;      /* result accumulated as seq_t          */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {          /* p is a distinct prime factor       */
            /* add p + n/p  (n is orig) */
            sum += (seq_t)p + (seq_t)(orig / p);

            /* remove all powers of p from m      */
            while (m % p == 0) m /= p;
        }
    }

    /* If m > 1, it is the remaining large prime factor. */
    if (m > 1) {
        sum += (seq_t)m + (seq_t)(orig / m);
    }

    return sum;
}
