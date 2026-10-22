/*
 * A363402  a(n) = n * (4^n - 2^n) / Clausen(n, 0).
 * https://oeis.org/A363402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (4^n - 2^n) / rad(n) where rad(n) is the product of
 *            the distinct prime factors of n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

/* Return the product of distinct prime factors of n (the radical of n). */
static seq_t rad_of(idx_t n)
{
    seq_t rad = 1;
    if (n == 0) return 1;            /* defensive, not used for n=0 */

    idx_t tmp = n;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        rad *= 2;
        while ((tmp & 1) == 0)
            tmp >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            rad *= p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    /* remaining prime > sqrt(tmp) */
    if (tmp > 1)
        rad *= tmp;

    return rad;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute the radical of n.  rad(n) always divides n. */
    seq_t rad = rad_of(n);

    /* Factor to be multiplied with the difference. */
    seq_t factor = (seq_t)n / rad;          /* exact division */

    /* Compute 2^n and (4^n - 2^n). */
    seq_t pow2 = seq_pow((seq_t)2, n);      /* 2^n */
    seq_t diff = pow2 * pow2 - pow2;         /* 4^n - 2^n */

    return factor * diff;
}
