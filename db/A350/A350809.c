/*
 * A350809  a(n) = Card({ p - (n mod p) ; p prime, p <= n }).
 * https://oeis.org/A350809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1668, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A350809: a(n) = #\{ p - (n \bmod p)\ |\ p prime,\ p <= n \}. */
/* It counts distinct least multiples > n for all primes <= n. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static inline int cmp_uint64(const void *x, const void *y)
{
    const uint64_t *vx = (const uint64_t *)x;
    const uint64_t *vy = (const uint64_t *)y;
    return (*vx < *vy) ? -1 : (*vx > *vy) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, no primes <= 1 */
    if (n < 2)
        return (seq_t)0;

    /* Prime cache: store primes up to the largest n requested so far */
    static idx_t          max_n_cached = 0;
    static uint64_t      *cached_primes = NULL;
    static size_t         cached_cnt    = 0;

    if (n > max_n_cached) {
        /* Allocate a new array of primes up to n */
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        build_primes_upto(n, &new_primes, &new_cnt);
        /* Free old cache if any */
        free(cached_primes);
        cached_primes = new_primes;
        cached_cnt    = new_cnt;
        max_n_cached  = n;
    }

    /* Allocate array for the least multiples > n */
    uint64_t *mults = (uint64_t *)malloc(cached_cnt * sizeof(uint64_t));
    if (!mults)
        return (seq_t)0; /* allocation failure – return 0 as a safe default */

    size_t mult_len = 0;
    for (size_t i = 0; i < cached_cnt; ++i) {
        uint64_t p = cached_primes[i];
        uint64_t m = ((n / p) + 1) * p;   /* least multiple of p strictly > n */
        mults[mult_len++] = m;
    }

    /* Sort and count distinct values */
    if (mult_len > 1)
        qsort(mults, mult_len, sizeof(uint64_t), cmp_uint64);

    seq_t unique = 0;
    if (mult_len > 0) {
        unique = 1;
        for (size_t i = 1; i < mult_len; ++i)
            if (mults[i] != mults[i - 1])
                ++unique;
    }

    free(mults);
    return unique;
}
