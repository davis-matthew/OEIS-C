/*
 * A329522  Composite numbers whose sum of digits is not composite.
 * https://oeis.org/A329522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th composite integer m > 1 such that digit_sum(m)
 *            is 1 or a prime number. The sequence starts 10,12,…
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of already generated terms and
 * grows it lazily when a larger n is requested.  It relies on the
 * helper function `is_prime(idx_t)` from the build utilities.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* behaviour for n < 1 is irrelevant */

    static seq_t *cache = NULL;   /* array of found terms           */
    static size_t cache_len = 0;   /* number of terms stored        */
    static size_t cache_cap = 0;   /* allocated capacity             */
    static idx_t next_candidate = 2;   /* integer to test next          */

    /* Initialise on first call. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Advance to next candidate. */
        next_candidate++;

        /* Skip 1 and primes; only composite numbers are considered. */
        if (next_candidate <= 1 || is_prime(next_candidate))
            continue;

        /* Compute decimal digit sum. */
        unsigned sum = 0;
        idx_t tmp = next_candidate;
        while (tmp > 0) {
            sum += (unsigned)(tmp % 10);
            tmp /= 10;
        }

        /* Accept if digit sum is 1 or prime (i.e., not composite). */
        if (sum == 1 || is_prime((idx_t)sum)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }

    return cache[n - 1];
}
