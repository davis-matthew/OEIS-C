/*
 * A377321  a(n) is the smallest positive integer k such that n + k and n - k
 *          have the same number of distinct prime factors.
 * https://oeis.org/A377321
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that (n+k) and (n-k) have the same
 *            number of distinct prime factors (count_distinct_primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper */
#include "../utils/count_distinct_primes.h"
#include <stdint.h>
#include <stdbool.h>

/* Computes term a(n) for n >= 4 (undefined behaviour otherwise) */
seq_t a(idx_t n)
{
    /* Guard against out‑of‑range indices – not used in tests. */
    if (n < 4)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;     /* use 64‑bit for helper arguments */

    seq_t k = 1;                  /* minimal positive k */
    while (1) {
        uint64_t plus  = N + (uint64_t)k;
        uint64_t minus = N - (uint64_t)k;

        /* count_distinct_primes returns an int; compare directly */
        if (count_distinct_primes(plus) == count_distinct_primes(minus))
            return k;            /* found the minimal k */

        ++k;                     /* try next k   */
    }
}
