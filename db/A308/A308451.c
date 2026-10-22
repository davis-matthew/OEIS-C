/*
 * A308451  Number of integer-sided triangles with perimeter n whose largest
 *          side length is prime.
 * https://oeis.org/A308451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n whose
 *            largest side is prime. Algorithm: iterate over all primes c <
 *            n/2, then count the integer pairs (a,b) with a ≤ b ≤ c, a + b =
 *            n – c, and a + b > c (the triangle inequality), which reduces to
 *            the bounds a ∈ [max(1, S – c), ⌊S/2⌋] with...
 * Range:     u8 n <= 218, u16 n <= 4846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                          /* offset starts at 1 */

    /* largest side c must satisfy c < n/2.  Therefore c ≤ floor((n-1)/2). */
    idx_t limit = (n - 1) / 2;
    if (limit < 2) return 0;                      /* no prime possible */

    /* static cache of primes up to the current maximum limit */
    static idx_t cached_limit = 0;
    static uint64_t *cached_primes = NULL;
    static size_t cached_cnt = 0;

    if (limit > cached_limit) {
        if (cached_primes) free(cached_primes);
        size_t cnt = 0;
        build_primes_upto(limit, &cached_primes, &cnt);
        cached_limit = limit;
        cached_cnt = cnt;
    }

    seq_t total = 0;

    for (size_t i = 0; i < cached_cnt; ++i) {
        idx_t c = cached_primes[i];       /* candidate largest side, prime */

        idx_t S = n - c;                   /* sum of the two smaller sides */

        /* a must satisfy: a ≤ b = S - a,  b ≤ c,  a ≥ 1. */
        idx_t lower = (S > c) ? (S - c) : 1;  /* from b ≤ c */
        idx_t upper = S >> 1;                /* from a ≤ b */

        if (upper < lower) continue;          /* no solutions for this c */

        seq_t count = (seq_t)(upper - lower + 1);  /* number of (a,b) pairs */
        total += count;
    }

    return total;
}
