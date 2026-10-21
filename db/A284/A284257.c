/*
 * A284257  a(n) = number of prime factors of n that are < the square of
 *          smallest prime factor of n (counted with multiplicity), a(1) = 0.
 * https://oeis.org/A284257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n that are < (smallest prime
 *            factor of n)^2 (counted with multiplicity)
 * Range:     u8 n <= 9994, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Work on a local copy of n as uint64_t. */
    uint64_t x = (uint64_t)n;

    /* Find the smallest prime factor (SPF). */
    uint64_t spf = 0;
    if (x % 2ULL == 0ULL) {
        spf = 2ULL;
    } else {
        for (uint64_t p = 3ULL; (unsigned __int128)p * p <= x; p += 2ULL) {
            if (x % p == 0ULL) {
                spf = p;
                break;
            }
        }
        if (spf == 0ULL)            /* x is prime */
            spf = x;
    }

    /* Threshold is spf^2, computed in 128‑bit to avoid overflow. */
    unsigned __int128 thr = (unsigned __int128)spf * spf;

    /* Count prime factors < thr (with multiplicity). */
    seq_t count = 0;

    /* Handle factor 2 separately. */
    while (x % 2ULL == 0ULL) {
        if ((unsigned __int128)2ULL < thr) count++;
        x /= 2ULL;
    }

    /* Handle odd factors. */
    for (uint64_t p = 3ULL; x > 1ULL; p += 2ULL) {
        /* Stop if p^2 > x; further p cannot divide x. */
        if ((unsigned __int128)p * p > x) break;
        while (x % p == 0ULL) {
            if ((unsigned __int128)p < thr) count++;
            x /= p;
        }
    }

    /* If a prime factor remains. */
    if (x > 1ULL) {
        if ((unsigned __int128)x < thr) count++;
    }

    return count;
}
