/*
 * A267894  Numbers whose number of odd divisors is nonprime.
 * https://oeis.org/A267894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose count of odd divisors is
 *            a nonprime number. (Computed by a linear scan and caching.)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* ------------------- static cache of found terms ------------------- */
    static seq_t *cache   = NULL;   /* array of produced terms               */
    static size_t cache_len = 0;    /* number of terms already stored       */
    static size_t cache_cap = 0;    /* current capacity of cache array       */

    static uint64_t cur = 1;        /* next integer to examine             */

    /* ------------------------------------------------------------------ */
    while (cache_len < (size_t)n) {
        /* Count odd divisors of cur */
        uint64_t odd = cur;
        while ((odd & 1ULL) == 0)          /* strip factors of 2 */
            odd >>= 1;

        seq_t d_seq = divisor_count_u64(odd); /* number of odd divisors */
        idx_t d = (idx_t)d_seq;               /* convert to idx_t for is_prime */

        if (!is_prime(d)) {
            /* Append cur to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;
        }
        ++cur;
    }
    return cache[n - 1];
}
