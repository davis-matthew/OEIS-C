/*
 * A337802  Minimum value of the cyclic self-convolution of the first n terms
 *          of the characteristic function of primes.
 * https://oeis.org/A337802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A337802 sequence.
   For the first n integers (1..n) we consider the characteristic
   function of primes f[i] (1 for prime, 0 otherwise).  We build
   the reversed version f_rev[i] = f[n-1-i].  All cyclic rotations of
   f_rev are compared to f, and the smallest dot product (number of
   overlapping prime positions) is returned.  This is equivalent to
   the cyclic self‑convolution described in the OEIS entry. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for odd callers */

    /* Allocate space for the prime indicator array (0/1). */
    uint8_t *f = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    if (!f) return 0;

    /* Initialise all numbers as prime, except 1. */
    for (idx_t i = 0; i < n; ++i) f[i] = 1;
    f[0] = 0;                       /* number 1 is not prime */

    /* Sieve of Eratosthenes up to n. */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (f[p - 1]) {            /* p is prime */
            for (idx_t j = p * p; j <= n; j += p)
                f[j - 1] = 0;
        }
    }

    /* Count primes and build the reversed array. */
    size_t prime_cnt = 0;
    uint8_t *f_rev = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    if (!f_rev) {
        free(f);
        return 0;
    }
    for (idx_t i = 0; i < n; ++i) {
        if (f[i]) ++prime_cnt;
        f_rev[i] = f[n - 1 - i];
    }

    /* If no primes, the minimum overlap is obviously 0. */
    if (prime_cnt == 0) {
        free(f);
        free(f_rev);
        return 0;
    }

    seq_t min_overlap = (seq_t)prime_cnt;

    /* For every cyclic shift of the reversed sequence compute the dot product. */
    for (idx_t shift = 0; shift < n; ++shift) {
        seq_t cnt = 0;
        for (idx_t i = 0; i < n; ++i) {
            /* use bitwise AND to avoid branching when possible */
            if (f[i] & f_rev[(i + shift) % n]) ++cnt;
        }
        if (cnt < min_overlap) {
            min_overlap = cnt;
            if (min_overlap == 0) break; /* cannot get lower */
        }
    }

    free(f);
    free(f_rev);
    return min_overlap;
}
