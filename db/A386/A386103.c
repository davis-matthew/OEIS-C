/*
 * A386103  Primes having only {1, 2, 7, 9} as digits.
 * https://oeis.org/A386103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime whose decimal digits belong to {1,2,7,9}. */
/* The sequence starts a(1)=2, a(2)=7, a(3)=11, ...   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static bool allowed_digits(idx_t v) {
    if (v == 0) return false;
    while (v > 0) {
        unsigned d = v % 10;
        if (d != 1 && d != 2 && d != 7 && d != 9)
            return false;
        v /= 10;
    }
    return true;
}

seq_t a(idx_t n) /* Sequences return 0 for the undefined a(0). */
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;      /* cached found primes        */
    static idx_t cache_cap = 0;      /* allocated capacity        */
    static idx_t cached_cnt = 0;     /* number of cached primes   */
    static idx_t next_candidate = 2; /* next integer to test      */

    while (cached_cnt < n) {
        idx_t cand = next_candidate++;
        /* skip even numbers >2 early */
        if (cand != 2 && (cand & 1) == 0) continue;

        if (!allowed_digits(cand)) continue;
        /* primality test */
        if (is_prime(cand)) {
            /* grow cache if needed */
            if (cached_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cached_cnt++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
