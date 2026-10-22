/*
 * A397705  Odd numbers m > 5, with no representation m = p + 2q with p, q
 *          prime and p <= q.
 * https://oeis.org/A397705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A397705: odd numbers >5 with no representation m = p + 2q
   where p, q are primes and p <= q.  This implementation performs a
   simple search, testing each odd candidate in turn and storing
   the exceptional values in a cached array. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* current odd candidate to test */
    static uint64_t next_m = 7;   /* first odd >5 */

    /* handle cache allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have enough */
    while (cache_len < n) {
        bool has_rep = false;
        uint64_t m = next_m;                     /* candidate odd >5   */
        uint64_t p_max = m / 3;                 /* maximum p to check*/

        /* iterate over odd primes p <= m/3 */
        for (uint64_t p = 3; p <= p_max; p += 2) {
            if (!is_prime((idx_t)p))
                continue;
            uint64_t q = (m - p) / 2;           /* (m - p) is even for odd m,p */
            if (is_prime((idx_t)q)) {
                has_rep = true;
                break;
            }
        }

        if (!has_rep) {
            /* cache the exceptional value */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = m;
        }

        next_m += 2;   /* move to next odd */
    }

    return cache[n - 1];
}
